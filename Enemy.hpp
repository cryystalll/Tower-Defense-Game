#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;

class Enemy : public Engine::Sprite {
protected:
    int price;
    float coolDown = 3;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    Engine::Point target;
	float speed;
	int money;
    std::list<Enemy*>::iterator lockedEnemyIterator;
	PlayScene* getPlayScene();
//	virtual void OnExplode();
    
    
public:
//    bool Enabled = true;
    float hp;
	float reachEndTime;
	std::list<Turret*> lockedTurrets;//鎖定我的砲台
	std::list<Bullet*> lockedBullets;
    Turret* Target = nullptr;
    bool Enabled = true;
	Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money );
 	
//	void Update(float deltaTime) override;
    virtual void OnExplode();
    virtual void CreateBullet() ;
    void Blow();
    virtual void Update(float deltaTime) override;
    void Attack(float deltaTime) ;
    virtual void Hit(float damage);
	void Draw() const override;
    virtual void freeze(float deltaTime);
};
#endif // ENEMY_HPP
