//
//  hero.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/6/4.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef hero_hpp
#define hero_hpp

#include <stdio.h>
#include "Turret.hpp"
class Turret;
class Enemy;

class hero: public Turret {
public:
//    Enemy* blowout = nullptr;
//    int id=5;
    Enemy* clear;
    Enemy* killall;
    Turret* removeturret = nullptr;
    static const int Price;
    hero(float x, float y);
    void CreateBullet() override;
    
//    void Remove();
    void Update(float deltaTime) override;
};

#endif /* hero_hpp */
