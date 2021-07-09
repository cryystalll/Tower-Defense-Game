#include <string>

//#include <string>


#include "AudioHelper.hpp"
//#include "StrongEnemy.hpp"


#include "NewBullet.hpp"
#include "Group.hpp"
//#include "WBCellTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Enemy.hpp"
#include "Turret.hpp"

#include "SofaEnemy.hpp"

SofaEnemy::SofaEnemy(int x, int y) : Enemy("play/enemy-2.png", x, y, 16, 100, 10, 10) {
	// Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
	// and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
}
//void SofaEnemy::freeze() {
////hp -= damage;
//    Velocity.x=0;
//    
//
//}
//void SofaEnemy::CreateBullet() {
////    Engine::Point diff = Engine::Point(1,0);
////    float rotation = ALLEGRO_PI / 2;
////    getPlayScene()->BulletGroup->AddNewObject(new NewBullet(Position , diff, rotation, this));
////    AudioHelper::PlayAudio("gun.wav");
//}
