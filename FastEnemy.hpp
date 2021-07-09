//
//  FastEnemy.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef FastEnemy_hpp
#define FastEnemy_hpp

#include "Enemy.hpp"

class FastEnemy : public Enemy {
public:
    FastEnemy(int x, int y);
    void Hit(float damage) override;
//    void Update(float deltaTime) override;
//    void freeze();
//    void CreateBullet()override;
};

#endif /* FastEnemy_hpp */
