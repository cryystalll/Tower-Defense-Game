#include <string>


#include "AudioHelper.hpp"
//#include "StrongEnemy.hpp"


#include "NewBullet.hpp"
#include "Group.hpp"
//#include "WBCellTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"
#include "Turret.hpp"

#include "NormalEnemy.hpp"

NormalEnemy::NormalEnemy(int x, int y) : Enemy("play/enemy-1.png", x, y, 10, 50, 5, 5) {
    // TODO 2 (6/8): You can imitate the 2 files: 'NormalEnemy.hpp', 'NormalEnemy.cpp' to create a new enemy.
}
//void NormalEnemy::freeze() {
////hp -= damage;
//    Velocity.x=0;
//    
//
//}
//void NormalEnemy::CreateBullet() {
////    Engine::Point diff = Engine::Point(1,0);
////    float rotation = ALLEGRO_PI / 2;
////    getPlayScene()->BulletGroup->AddNewObject(new NewBullet(Position , diff, rotation, this));
////    AudioHelper::PlayAudio("gun.wav");
//}
