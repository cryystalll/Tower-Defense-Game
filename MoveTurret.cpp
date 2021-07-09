//
//  MoveTurret.cpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/6/2.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#include "MoveTurret.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "PockyBullet.hpp"
#include "Group.hpp"
#include "WBCellTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"
const int MoveTurret::Price = 30;
MoveTurret::MoveTurret(float x, float y) :
    // TODO 2 (2/8): You can imitate the 2 files: 'FreezeTurret.hpp', 'FreezeTurret.cpp' to create a new turret.
    Turret("play/killer.png", x, y, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
//        hp  = 5000;
        id = 6;
        CollisionRadius = 10;
    Anchor.y += 8.0f / GetBitmapHeight();
}
void MoveTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(1,0);
    float rotation = ALLEGRO_PI / 2;
    getPlayScene()->BulletGroup->AddNewObject(new PockyBullet(Position , diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
