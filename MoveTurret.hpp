//
//  MoveTurret.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/6/2.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef MoveTurret_hpp
#define MoveTurret_hpp

#include <stdio.h>
#include "Turret.hpp"

class MoveTurret: public Turret {
public:
    
    static const int Price;
    MoveTurret(float x, float y);
    void CreateBullet() override;
};

#endif /* MoveTurret_hpp */
