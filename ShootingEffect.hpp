//
//  ShootingEffect.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef ShootingEffect_hpp
#define ShootingEffect_hpp

#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class ShootingEffect : public Engine::Sprite {
protected:
    PlayScene* getPlayScene();
    float timeTicks;
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
    float timeSpan = 0.5;
public:
    ShootingEffect(float x, float y);
    void Update(float deltaTime) override;
};

#endif /* ShootingEffect_hpp */
