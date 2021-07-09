#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <vector>
#include <string>
#include "Point.hpp"
#include "Sprite.hpp"

class Enemy;
class PlayScene;
class Bullet;

class Turret: public Engine::Sprite {
protected:
    int price;
    float coolDown;
    float reload = 0;
    Engine::Point target;
    float rotateRadian = 2 * ALLEGRO_PI;
    std::list<Turret*>::iterator lockedTurretIterator;
    PlayScene* getPlayScene();
    virtual void OnExplode();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;

public:
    int id;
    int hp ;
    float reachEndTime;
    int CollisionRadius;
    std::list<Enemy*> lockedEnemy;//鎖定我的砲台
    std::list<Bullet*> lockedBullets;
    bool Enabled = true;
    bool Preview = false;
    Enemy* Target = nullptr;
    Turret(/*std::string imgBase,*/std::string imgTurret, float x, float y,/* float radius,*/ int price, float coolDown);
    virtual void Update(float deltaTime) override;
    virtual void Hit(float damage);
    virtual void Draw() const override;
	int GetPrice() const;
    void trash();
//    void blow();
};
#endif // TURRET_HPP
