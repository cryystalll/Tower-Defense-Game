//
//  FastEnemy.cpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//
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
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"
#include "FastEnemy.hpp"
#include "Enemy.hpp"
#include <list>
#include <vector>
#include <string>

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

//#include "NormalEnemy.hpp"

#include "Point.hpp"
#include "Sprite.hpp"

class Enemy;
FastEnemy::FastEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, 50, 5, 5) {
    // TODO 2 (6/8): You can imitate the 2 files: 'NormalEnemy.hpp', 'NormalEnemy.cpp' to create a new enemy.
    
}
//void FastEnemy::Update(float deltaTime) {
//
//    
//    PlayScene* scene = getPlayScene();
//        if(Target){
//            if (Target->Position.x > Position.x || Target->Position.y != Position.y  ) {
//                Target->lockedEnemy.erase(lockedEnemyIterator);
//                Target = nullptr;
//                lockedEnemyIterator = std::list<Enemy*>::iterator();
//            }
//            // Shoot reload.
//            else if(Position.x - Target->Position.x < 100){
//                 Velocity.x = 0;
//            }
//
//            reload -= deltaTime;
//            if (reload <= 0) {
//                // shoot.
//                reload = coolDown;
//                CreateBullet();
//            }
//        }
//        if(!Target){
//            Velocity = Engine::Point(speed , 0);
//        for (auto& it : scene->TowerGroup->GetObjects()) {
//                    if (it->Position.x < Position.x && it->Position.y==Position.y ) {
//                        Target = dynamic_cast<Turret*>(it);//拿到目標砲台
//                        Target->lockedEnemy.push_back(this);//敵人的鎖定砲台推入自己
//                        lockedEnemyIterator = std::prev(Target->lockedEnemy.end());//指向前一個砲台
//                        break;
//                    }
//                }
//        }
//    float remainSpeed = speed * deltaTime;
//
//    Position.x -= Velocity.x * deltaTime;
//    Position.y += Velocity.y * deltaTime;
//    if(Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize + PlayScene::BlockSize / 2){
//        Hit(hp);
//        getPlayScene()->Hit();
//        reachEndTime = 0;
//        return;
//    }
//    Engine::Point vec = target - Position;
//    reachEndTime = (vec.Magnitude() - remainSpeed) / speed;
//}
void FastEnemy::Hit(float damage) {
    hp -= damage;
    Velocity.x+=50;
    
if (hp <= 0) {
    OnExplode();
    // Remove all turret's reference to target.
    for (auto& it: lockedTurrets)
        it->Target = nullptr;
    for (auto& it: lockedBullets)
        it->Target = nullptr;
    getPlayScene()->EarnMoney(money);
    getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
    AudioHelper::PlayAudio("explosion.wav");
}
}
//void FastEnemy::CreateBullet() {
////    Engine::Point diff = Engine::Point(1,0);
////    float rotation = ALLEGRO_PI / 2;
////    getPlayScene()->BulletGroup->AddNewObject(new NewBullet(Position , diff, rotation, this));
////    AudioHelper::PlayAudio("gun.wav");
//}
//void FastEnemy::freeze() {
////hp -= damage;
//    Velocity.x-=10;
//
//
//}
//Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money) :
//    Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money) {
//    CollisionRadius = radius;
//    reachEndTime = 0;
//    Velocity = Engine::Point(speed , 0);
//    target = Engine::Point(PlayScene::EndGridPointx , static_cast<int>(floor(Position.y / PlayScene::BlockSize))) * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
//}
//
//}

