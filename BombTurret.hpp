//
//  BombTurret.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/31.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef BombTurret_hpp
#define BombTurret_hpp

#include <stdio.h>
#include "Turret.hpp"
class Enemy;

class BombTurret: public Turret {
public:
//    int id=4;
    Enemy* blowout = nullptr;
    static const int Price;
    BombTurret(float x, float y);
    void CreateBullet() override;
    void Hit(float damage)override;
    void Draw() const override;
//    void Update(float deltaTime) override;
};

#endif /* BombTurret_hpp */
