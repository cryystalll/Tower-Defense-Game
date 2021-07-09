//
//  NewBullet.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/29.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef NewBullet_hpp
#define NewBullet_hpp

#include <stdio.h>

#include "EnemyBullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class NewBullet : public EnemyBullet {
public:
    explicit NewBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent);
    
    void OnExplode(Turret* turret) override;
};


#endif /* NewBullet_hpp */
