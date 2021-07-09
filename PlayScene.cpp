#include <allegro5/allegro.h>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <numeric> 

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "NormalEnemy.hpp"
#include "PlateletTurret.hpp"
#include "WBCellTurret.hpp"
#include "FreezeTurret.hpp"
#include "ZeroTurret.hpp"
#include "BombTurret.hpp"
#include "Bomb.hpp"
#include "MoveTurret.hpp"
#include "hero.hpp"
#include "Plane.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "SofaEnemy.hpp"
#include "StrongEnemy.hpp"
#include "FastEnemy.hpp"
#include "Sprite.hpp"
#include "Turret.hpp"
#include "TurretButton.hpp"
#include "LOG.hpp"


bool PlayScene::DebugMode = false;
bool PlayScene::repeat = false ;
static int flag[1000]={};
static int specialturrent = 0;
//bool PlayScene::move = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 12, PlayScene::MapHeight = 6;//50;//13;
const int PlayScene::BlockSize = 128;
const float PlayScene::DangerTime = 7.61;
const int PlayScene::SpawnGridPointx = 12;
const int PlayScene::EndGridPointx = -1;
// TODO 4 (1/3): Set a cheat sequence here.
//int cheat[]
//={ALLEGRO_KEY_UP,ALLEGRO_KEY_UP,ALLEGRO_KEY_DOWN,ALLEGRO_KEY_DOWN,ALLEGRO_KEY_LEFT,ALLEGRO_KEY_RIGHT,ALLEGRO_KEY_ENTER};

const std::vector<int> PlayScene::code={ALLEGRO_KEY_UP,ALLEGRO_KEY_UP,ALLEGRO_KEY_DOWN,ALLEGRO_KEY_DOWN,ALLEGRO_KEY_LEFT,ALLEGRO_KEY_RIGHT,ALLEGRO_KEY_ENTER};
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
	mapState.clear();
	keyStrokes.clear();
	ticks = 0;
	deathCountDown = -1;
	lives = 10;
	money = 150;
	SpeedMult = 1;
	laneNum = std::vector<int>(MapHeight);
	std::iota(laneNum.begin(), laneNum.end(), 0);
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
//    AddNewObject(TurrentGroup = new Group());
	AddNewObject(BulletGroup = new Group());
    AddNewObject(NewBulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	ReadEnemyWave();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	preview = nullptr;
	UIGroup->AddNewObject(imgTarget);
	// Preload Lose Scene
	deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
	Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
	// Start BGM.
	bgmId = AudioHelper::PlayBGM("play.ogg");
}
void PlayScene::Terminate() {
	AudioHelper::StopBGM(bgmId);
	AudioHelper::StopSample(deathBGMInstance);
	deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	if (SpeedMult == 0)
		deathCountDown = -1;
	else if (deathCountDown != -1)
		SpeedMult = 1;
	// Calculate danger zone.
	std::vector<float> reachEndTimes;
	for (auto& it : EnemyGroup->GetObjects()) {//每隔enemy的reach end time
		reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
	}
	// Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
	std::sort(reachEndTimes.begin(), reachEndTimes.end());
	float newDeathCountDown = -1;
	int danger = lives;
	for (auto& it : reachEndTimes) {//每個enemy離到達終點的時間有多久
		if (it <= DangerTime) {//在7秒內會到達
			danger--;//lives--
			if (danger <= 0) {//七秒內命被扣光死掉
				// Death Countdown
				float pos = DangerTime - it;
				if (it > deathCountDown) {
					// Restart Death Count Down BGM.
					AudioHelper::StopSample(deathBGMInstance);
					if (SpeedMult != 0)
						deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
				}
				float alpha = pos / DangerTime;
				alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
				dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
				newDeathCountDown = it;
				break;
			}
		}
	}
	deathCountDown = newDeathCountDown;
	if (SpeedMult == 0)
		AudioHelper::StopSample(deathBGMInstance);
	if (deathCountDown == -1 && lives > 0) {
		AudioHelper::StopSample(deathBGMInstance);
		dangerIndicator->Tint.a = 0;
	}
	if (SpeedMult == 0)
		deathCountDown = -1;
	for (int i = 0; i < SpeedMult; i++) {
	    IScene::Update(deltaTime);
		// Check if we should create new enemy.
		ticks += deltaTime;
		if (enemyWaveData.empty()) {
			if (EnemyGroup->GetObjects().empty()) {
				// Win.
				Engine::GameEngine::GetInstance().ChangeScene("win");
			}
			continue;
		}
		auto current = enemyWaveData.front();
		if (ticks < std::get<1>(current))
			continue;
		ticks -= std::get<1>(current);
		enemyWaveData.pop_front();
        std::random_device dev;
	    std::mt19937 rng(dev());
		std::shuffle(laneNum.begin(), laneNum.end(), rng);
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, 3);
        Enemy* enemy;
        for(int j = 0 ; j < std::get<2>(current) ; j++){
            const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPointx * BlockSize + BlockSize / 2 + dist(rng) * 15, laneNum[j] * BlockSize + BlockSize / 2);
            switch (std::get<0>(current)) {
				case 1:
					EnemyGroup->AddNewObject(enemy = new NormalEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
					break;
				case 2:
					EnemyGroup->AddNewObject(enemy = new SofaEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
					break;
				case 3:
					EnemyGroup->AddNewObject(enemy = new StrongEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
					break;
					// TODO 2 (7/8): You need to modify 'resources/enemy1.txt', or 'resources/enemy2.txt' to spawn the 4th enemy.
					//         The format is "[EnemyId] [TimeDelay] [LaneNum] [Repeat]".
					// TODO 2 (8/8): Enable the creation of the 4th enemy.
                case 4:
                    EnemyGroup->AddNewObject(enemy = new FastEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
				default:
					continue;
			}
		    // Compensate the time lost.
		    enemy->Update(ticks);
		}
	}
	if (preview) {
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		// To keep responding when paused.
		preview->Update(deltaTime);
	}
}
void PlayScene::Draw() const {
	IScene::Draw();
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {//here
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
        if(preview->id==5){
//            move=false;
            EarnMoney(-preview->GetPrice());
            // Remove Preview.
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            // Construct real turret.
//            PlayScene* scene = getPlayScene();
               for (auto& it : TowerGroup->GetObjects()) {
                   if(it->Position.x==x * BlockSize + BlockSize / 2&&it->Position.y==y * BlockSize + BlockSize / 2){
                       remove = dynamic_cast<Turret*>(it);
                       remove->trash();
                   }
               }
            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
//            TowerGroup->AddNewObject(preview);
            // To keep responding when paused.
            preview->Update(0);
            // Remove Preview.
            preview = nullptr;

            mapState[y][x] = TILE_FLOOR;
            OnMouseMove(mx, my);
        }
//        else if(preview->id==6){
//
//        }
		else if (mapState[y][x] != TILE_OCCUPIED) {
			if (!preview)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			EarnMoney(-preview->GetPrice());
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			TowerGroup->AddNewObject(preview);
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;

			mapState[y][x] = TILE_OCCUPIED;
			OnMouseMove(mx, my);
		}
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	// TODO 4 (2/3): Set Tab as a code to active or de-active debug mode
//    if (keyCode == ALLEGRO_KEY_TAB) {
//        DebugMode = true;
//    }
    int cnt=0;
	if (keyCode==ALLEGRO_KEY_TAB) {
		DebugMode = !DebugMode;
	}else {
		keyStrokes.push_back(keyCode);
		if (keyStrokes.size() > code.size())
			keyStrokes.pop_front();
        // TODO 4 (3/3): Check whether the input sequence is correct
        std::list<int>::iterator it;
        std::vector<int>::iterator tmp;
        int i;
        for (it=keyStrokes.begin(),i=0 ; it != keyStrokes.end(),i<code.size(); ++it,++i){
               
                if(*it==code[i]){
                    cnt++;
                
            }
        }
        if(cnt==7){
            EffectGroup->AddNewObject(new Plane());
            money += 10000;
            lives += 10;
            UILives->Text = std::string("Life") +std::to_string(lives);
            
        }
		
	}
	if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for WBCellTurret.
		UIBtnClicked(0);
	}else if (keyCode == ALLEGRO_KEY_I) {
        // Hotkey for PlateletTurret.
        for (auto& it : TowerGroup->GetObjects()) {
            move = dynamic_cast<Turret*>(it);
            if(move->id==6){
                move->Position.y-=BlockSize;
            }
            
        }
    }else if (keyCode == ALLEGRO_KEY_K) {
         for (auto& it : TowerGroup->GetObjects()) {
                   move = dynamic_cast<Turret*>(it);
                   if(move->id==6){
                       move->Position.y+=BlockSize;
                   }
                   
               }
    }else if (keyCode == ALLEGRO_KEY_J) {
         for (auto& it : TowerGroup->GetObjects()) {
                   move = dynamic_cast<Turret*>(it);
                   if(move->id==6){
                       move->Position.x-=BlockSize;
                   }
                   
               }
    }else if (keyCode == ALLEGRO_KEY_L) {
         for (auto& it : TowerGroup->GetObjects()) {
                   move = dynamic_cast<Turret*>(it);
                   if(move->id==6){
                       move->Position.x+=BlockSize;
                   }
                   
               }
    }
	else if (keyCode == ALLEGRO_KEY_W) {
		// Hotkey for PlateletTurret.
		UIBtnClicked(1);
	}
	// TODO 2 (5/8): Make the E key to create the 3th turret.
    else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for PlateletTurret.
        UIBtnClicked(2);
    }
    else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for PlateletTurret.
        UIBtnClicked(3);
    }
    else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for PlateletTurret.
        UIBtnClicked(4);
    }
    else if (keyCode == ALLEGRO_KEY_Y) {
        // Hotkey for PlateletTurret.
        UIBtnClicked(5);
    }
    else if (keyCode == ALLEGRO_KEY_U) {
        // Hotkey for PlateletTurret.
        UIBtnClicked(6);
    }
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
    
    
}
void PlayScene::Kill(int y){
    if(flag[y]==0){
    int x = EndGridPointx * BlockSize+BlockSize/2;
    killer = new hero(0, 0);
//    killer->GetObjectIterator()->first = false;
//    UIGroup->RemoveObject(killer->GetObjectIterator());
    // Construct real turret.
    killer->Position.x = x;
    killer->Position.y = y;
    killer->Enabled = true;
    killer->Preview = false;
    killer->Tint = al_map_rgba(255, 255, 255, 255);
    TowerGroup->AddNewObject(killer);
    // To keep responding when paused.
    killer->Update(ticks);
    // Remove Preview.
    flag[y] = 1;
    killer = nullptr;
    }

//    mapState[y][x] = TILE_OCCUPIED;
}
void PlayScene::Hit() {
    lives--;
    UILives->Text = std::string("Life") +std::to_string(lives);
	if (lives<=0) {//live要-- 
////
////		// Free resources.
//				delete TileMapGroup;
//				delete GroundEffectGroup;
//				delete DebugIndicatorGroup;
//				delete TowerGroup;
//				delete EnemyGroup;
//                delete NewBulletGroup;
//				delete BulletGroup;
//				delete EffectGroup;
//				delete UIGroup;
//				delete imgTarget;//
//		//lose
		Engine::GameEngine::GetInstance().ChangeScene("lose");
	}
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	UIMoney->Text = std::string("$") + std::to_string(this->money);
}
//void PlayScene::EarnMoney(int money) {
//    this->lives += lives;
//    UIMoney->Text = std::string("$") + std::to_string(this->money);
//}
void PlayScene::ReadMap() {//讀場景
	std::string filename = std::string("resources/map") + std::to_string(MapId) + ".txt";
	// Read map file.
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(false); break;
		case '1': mapData.push_back(true); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("play/deep-pink.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/pink.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}
void PlayScene::ReadEnemyWave() {//讀enemy位置
	std::string filename = std::string("resources/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type, wait, totallane,repeat; 
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while (fin >> type && fin >> wait && fin >> totallane && fin >> repeat) {
		for (int i = 0; i < repeat; i++)
			enemyWaveData.emplace_back(type, wait, totallane);
	}
	fin.close();
}
void PlayScene::ConstructUI() {//砲台選項
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 0, 128*MapHeight, 1536, 128));
	// Text
	//UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 128*MapHeight));
	UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 128*MapHeight+30));


    TurretButton* btn;
	// Button 1
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/turret-1.png", 180, BlockSize*MapHeight, 0, 0, 0, 0)
		, 170, 128*MapHeight, WBCellTurret::Price);
	// Reference: Class Member Function Pointer and std::bind.
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
	UIGroup->AddNewControlObject(btn);

	// Button 2
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/turret-2.png", 300, BlockSize*MapHeight, 0, 0, 0, 0)
		, 290, 128*MapHeight, PlateletTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
	UIGroup->AddNewControlObject(btn);
    // TODO 2 (3/8): Create a button to support constructing the 3th tower.
    
    // Button 3
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/turret-5.png", 420, BlockSize*MapHeight, 0, 0, 0, 0)
        , 410, 128*MapHeight, FreezeTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
    UIGroup->AddNewControlObject(btn);
    
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/turret-4.png", 540, BlockSize*MapHeight, 0, 0, 0, 0)
        , 530, 128*MapHeight, ZeroTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
    UIGroup->AddNewControlObject(btn);
    
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/turret-6.png", 660, BlockSize*MapHeight, 0, 0, 0, 0)
        , 650, 128*MapHeight, BombTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
    UIGroup->AddNewControlObject(btn);
    
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/bomb.png", 780, BlockSize*MapHeight, 0, 0, 0, 0)
        , 770, 128*MapHeight, Bomb::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 5));
    UIGroup->AddNewControlObject(btn);
    
    once = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/killer.png", 900, BlockSize*MapHeight, 0, 0, 0, 0)
        , 890, 128*MapHeight, MoveTurret::Price);
    once->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 6));
    UIGroup->AddNewControlObject(once);

	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int shift = 135 + 25;
	dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
	dangerIndicator->Tint.a = 0;
	UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::UIBtnClicked(int id) {//選砲台 新增砲台
	if (preview) {
		UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
	if (id == 0 && money >= WBCellTurret::Price)
		preview = new WBCellTurret(0, 0);
	else if (id == 1 && money >= PlateletTurret::Price)
		preview = new PlateletTurret(0, 0);
	// TODO 2 (4/8): On callback, create the 3th tower.
    else if (id == 2 && money >= FreezeTurret::Price)
        preview = new FreezeTurret(0, 0);
    else if (id == 3 && money >= ZeroTurret::Price)
        preview = new ZeroTurret(0, 0);
    
    else if (id == 4 && money >= BombTurret::Price)
        preview = new BombTurret(0, 0);
        
    else if (id == 5 && money >= Bomb::Price)
        preview = new Bomb(0, 0);
//        move = true;
    else if (id == 6 && money >= MoveTurret::Price && specialturrent==0){
        preview = new MoveTurret(0, 0);
        specialturrent = 1;
    }
//        UIGroup->RemoveObject(once->GetObjectIterator());
//        repeat = !repeat;
	if (!preview)
		return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	UIGroup->AddNewObject(preview);
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth  || y < 0 || y >= MapHeight  )
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		if (pnt.x == x && pnt.y == y){
			return false;
		}
	}
	return true;
}
