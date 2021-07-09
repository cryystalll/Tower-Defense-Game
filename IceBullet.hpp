//
//  IceBullet.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/6/1.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef IceBullet_hpp
#define IceBullet_hpp

#include <stdio.h>
#include "EnemyBullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class IceBullet : public EnemyBullet {
public:
    explicit IceBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent);
    
    void OnExplode(Turret* turret) override;
};


#endif /* IceBullet_hpp */
