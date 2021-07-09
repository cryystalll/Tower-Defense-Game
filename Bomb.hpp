//
//  Bomb.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/31.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef Bomb_hpp
#define Bomb_hpp

#include <stdio.h>
#include "Turret.hpp"
class Turret;
class Enemy;

class Bomb: public Turret {
public:
//    Enemy* blowout = nullptr;
//    int id=5;
    Turret* removeturret = nullptr;
    static const int Price;
    Bomb(float x, float y);
    void CreateBullet() override;
//    void Remove();
//    void Update(float deltaTime) override;
};

#endif /* Bomb_hpp */
