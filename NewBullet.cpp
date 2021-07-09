//
//  NewBullet.cpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/29.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#include "NewBullet.hpp"
#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "Turret.hpp"
//#include "IceCreamBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"

class Turret;
class Enemy;

NewBullet::NewBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent) :
    EnemyBullet("play/bullet-7.png", -500, 1, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
    // TODO 2 (1/8): You can imitate the 2 files: 'FireBullet.hpp', 'FireBullet.cpp' to create a new bullet.
}

void NewBullet::OnExplode(Turret* turret) {
   std::random_device dev;
   std::mt19937 rng(dev());
   std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
   getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), turret->Position.x, turret->Position.y));
}


