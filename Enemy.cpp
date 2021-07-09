#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "ExplosionEffect.hpp"
#include "IceBullet.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"

PlayScene* Enemy::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {//爆炸效果
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}

void Enemy::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        OnExplode();
        // Remove all turret's reference to target.
        for (auto& it: lockedTurrets)
            it->Target = nullptr;
        for (auto& it: lockedBullets)
            it->Target = nullptr;
        getPlayScene()->EarnMoney(money);
//        getPlayScene()->life(lives);
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
void Enemy::Blow(){
     OnExplode();
    for (auto& it: lockedTurrets)
        it->Target = nullptr;
    for (auto& it: lockedBullets)
        it->Target = nullptr;
    getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
    AudioHelper::PlayAudio("explosion.wav");
    
}
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money) :
    Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money) {
    CollisionRadius = radius;
    reachEndTime = 0;
    Velocity = Engine::Point(speed , 0);
    target = Engine::Point(PlayScene::EndGridPointx , static_cast<int>(floor(Position.y / PlayScene::BlockSize))) * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
}
void Enemy::CreateBullet(){
    Engine::Point diff = Engine::Point(1,0);
    float rotation = ALLEGRO_PI / 2;
    // Change bullet position to the front of the gun barrel.
    // TODO 3 (1/2): Add a Shoot Effect here.
//    getPlayScene()->BulletGroup->AddNewObject(new ShootingEffect(Position.x+50, Position.y));
    getPlayScene()->BulletGroup->AddNewObject(new IceBullet(Position , diff, rotation, this));
//    getPlayScene()->BulletGroup->AddNewObject(new PockyBullet(Position + normalized * 36 + normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("laser.wav");
    
}

void Enemy::Update(float deltaTime) {
     PlayScene* scene = getPlayScene();
                if(Target){
                    if (Target->Position.x > Position.x || Target->Position.y != Position.y  ) {
    //                    Target->lockedEnemy.erase(lockedEnemyIterator);
                        Target = nullptr;
    //                    lockedEnemyIterator = std::list<Enemy*>::iterator();
                    }
                    
//                     Shoot reload.
                    else if(Position.x - Target->Position.x < 100){
                         Velocity.x = 0;
                    }
                        
                    for (auto& it : scene->TowerGroup->GetObjects()) {
                                if (Position.x - it->Position.x < 100 && it->Position.y==Position.y ) {
                                               
                                            Velocity.x = 0;
                                               
                                           }
                    }
                   
                    
                    reload -= deltaTime;
                    if (reload <= 0) {
                        // shoot.
                        reload = coolDown;
                        CreateBullet();//發射子彈
                    }
                }
                if(!Target){
                    Velocity = Engine::Point(speed , 0);
                for (auto& it : scene->TowerGroup->GetObjects()) {
                            if (it->Position.x < Position.x && it->Position.y==Position.y ) {
                                Target = dynamic_cast<Turret*>(it);//拿到目標砲台
                                Target->lockedEnemy.push_back(this);//敵人的鎖定砲台推入自己
                                lockedEnemyIterator = std::prev(Target->lockedEnemy.end());//指向前一個砲台
                                break;
                            }
                        }
                }
            float remainSpeed = speed * deltaTime;

            Position.x -= Velocity.x * deltaTime;
            Position.y += Velocity.y * deltaTime;
            if(Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize +  PlayScene::BlockSize *2){
                getPlayScene()->Kill(Position.y);
            }
            if(Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2){
                Hit(hp);
                getPlayScene()->Hit();
                reachEndTime = 0;
                return;
            }
            Engine::Point vec = target - Position;
            reachEndTime = (vec.Magnitude() - remainSpeed) / speed;
}
void Enemy::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}
void Enemy::freeze(float deltaTime) {
//    float remainSpeed = speed * deltaTime;
    Velocity.x = 0;
    Position.x -= Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
    if(Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2){
    Hit(hp);
    getPlayScene()->Hit();
    reachEndTime = 0;
    return;
}
Engine::Point vec = target - Position;
//reachEndTime = (vec.Magnitude() - remainSpeed) / speed;


}

