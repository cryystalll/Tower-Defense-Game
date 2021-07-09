//
//  FreezeTurret.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef FreezeTurret_hpp
#define FreezeTurret_hpp

#include "Turret.hpp"

class FreezeTurret: public Turret {
public:
//    int id=2;
    static const int Price;
    FreezeTurret(float x, float y);
    void CreateBullet() override;
};


#endif /* FreezeTurret_hpp */
