//
//  BombTurret.cpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/31.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#include "BombTurret.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/base.h>
#include <cmath>
#include <string>
#include <math.h>
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
#include "AudioHelper.hpp"
#include "IceCreamBullet.hpp"
#include "FireBullet.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "WBCellTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"
class Enemy;
const int BombTurret::Price = 30;
int flag = 0;
BombTurret::BombTurret(float x, float y) :
    
    // TODO 2 (2/8): You can imitate the 2 files: 'FreezeTurret.hpp', 'FreezeTurret.cpp' to create a new turret.
    Turret("play/turret-6.png", x, y, Price, 0.5) {
        hp = 1;
        id =4;
        CollisionRadius = 10;
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void BombTurret::CreateBullet() {
//    Engine::Point diff = Engine::Point(1,0);
//    float rotation = ALLEGRO_PI / 2;
//    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position , diff, rotation, this));
//    AudioHelper::PlayAudio("gun.wav");
}
void BombTurret::Hit(float damage){
    hp -= damage;
        if (hp <= 0) {
            OnExplode();
//            Draw();
            
            PlayScene* scene = getPlayScene();
            for (auto& it : scene->EnemyGroup->GetObjects()) {
                if((it->Position-Position).Magnitude()<=500){
                    blowout = dynamic_cast<Enemy*>(it);
                    blowout->Blow();
                }
            }
            
            // Remove all turret's reference to target.
            for (auto& it: lockedEnemy){
                it->Target = nullptr;
            }
    //        for (auto& it: lockedBullets)
    //            it->Target = nullptr;
    //        getPlayScene()->EarnMoney(money);
    //        getPlayScene()->life(lives);
            getPlayScene()->TowerGroup->RemoveObject(objectIterator);
            AudioHelper::PlayAudio("explosion.wav");
            return;
        }
}
void BombTurret::Draw() const{
    
    Sprite::Draw();
    
        al_draw_circle(Position.x, Position.y, 500, al_map_rgb(255, 97, 0), 2);
    
//    if (flag==1) {
//         Draw target radius.
//        al_draw_circle(Position.x, Position.y, 100, al_map_rgb(0, 0, 255), 2);
//    }
}
