//
//  FireBullet.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef FIREBULLET_HPP
#define FIREBULLET_HPP
#include "Bullet.hpp"
#include <string>

#include "Sprite.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class FireBullet : public Bullet {
public:
    explicit FireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
    virtual void Update(float deltaTime) override;
};
#endif // FIREBULLET_HPP
