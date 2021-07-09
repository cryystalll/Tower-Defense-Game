//
//  ZeroTurret.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/31.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef ZeroTurret_hpp
#define ZeroTurret_hpp

#include <stdio.h>
#include "Turret.hpp"

class ZeroTurret: public Turret {
public:
//    int id=3;
    static const int Price;
    ZeroTurret(float x, float y);
    void CreateBullet() override;
};

#endif /* ZeroTurret_hpp */
