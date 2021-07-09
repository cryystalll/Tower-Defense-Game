//
//  hero.cpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/6/4.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#include "hero.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "IceCreamBullet.hpp"
#include "FireBullet.hpp"
#include "Group.hpp"
#include "PockyBullet.hpp"
#include "WBCellTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"
const int hero::Price = 0;
hero::hero(float x, float y) :
    // TODO 2 (2/8): You can imitate the 2 files: 'FreezeTurret.hpp', 'FreezeTurret.cpp' to create a new turret.
    Turret("play/turret-3.png", x, y, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
        hp = 100000;
        id =7;
        CollisionRadius = 10;
    Anchor.y += 8.0f / GetBitmapHeight();
}
void hero::CreateBullet() {
//Engine::Point diff = Engine::Point(1,0);
//float rotation = ALLEGRO_PI / 2;
//getPlayScene()->BulletGroup->AddNewObject(new PockyBullet(Position , diff, rotation, this));
//AudioHelper::PlayAudio("gun.wav");
}
void hero::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    if (!Enabled)
        return;
    if (Target) {
        if (Target->Position.x < Position.x&& Target->Position.y >= Position.y  && Target->Position.y < Position.y+scene->BlockSize) {
            Target->lockedTurrets.erase(lockedTurretIterator);
            Target = nullptr;
            lockedTurretIterator = std::list<Turret*>::iterator();
        }
        // Shoot reload.
//        reload -= deltaTime;
//        if (reload <= 0) {
//            // shoot.
//            reload = coolDown;
//            CreateBullet();
//        }
    }
    if (!Target) {
        // Lock first seen target.
        // Can be improved by Spatial Hash, Quad Tree, ...
        // However simply loop through all enemies is enough for this program.
        for (auto& it : scene->EnemyGroup->GetObjects()) {
            if (it->Position.x > Position.x && it->Position.y >= Position.y  && it->Position.y < Position.y+scene->BlockSize) {
                Target = dynamic_cast<Enemy*>(it);
                Target->lockedTurrets.push_back(this);
                lockedTurretIterator = std::prev(Target->lockedTurrets.end());
                break;
            }
        }
    }
    for (auto& it : scene->EnemyGroup->GetObjects()) {
    if(it->Position.x >= PlayScene::EndGridPointx * PlayScene::BlockSize && it->Position.x <= PlayScene::EndGridPointx * PlayScene::BlockSize+PlayScene:: BlockSize*2 &&it->Position.y >= Position.y  && it->Position.y < Position.y+scene->BlockSize){
        clear = dynamic_cast<Enemy*>(it);
        clear->Blow();
    }
    
        
    }
//    for (auto& it: lockedEnemy){
//        it->Target = nullptr;
//    }
    Velocity = Engine::Point(50 , 0);
    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
        
    //    Position.y += Velocity.y * deltaTime;
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        
        if (it->Position.x >= Position.x-PlayScene::BlockSize&&it->Position.x<=Position.x+PlayScene::BlockSize/2 &&it->Position.y >= Position.y  && it->Position.y < Position.y+scene->BlockSize) {
            killall= dynamic_cast<Enemy*>(it);
            killall->Blow();
        }
        
        }
    if(Position.x >= PlayScene::SpawnGridPointx * PlayScene::BlockSize ){
        Hit(hp);
    }
//    
//
}

