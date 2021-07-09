//
//  EnemyBullet.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/29.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef EnemyBullet_hpp
#define EnemyBullet_hpp

#include <stdio.h>
#include <string>

#include "Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class EnemyBullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    Enemy* parent;
    PlayScene* getPlayScene();
    virtual void OnExplode(Turret* turret);
public:
    Turret* Target = nullptr;
    explicit EnemyBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent);
//    explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy* parent);
    virtual void Update(float deltaTime) override;
};

#endif /* EnemyBullet_hpp */
