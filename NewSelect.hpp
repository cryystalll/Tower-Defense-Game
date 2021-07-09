//
//  NewSelect.hpp
//  project 2
//
//  Created by 黃鈺舒 on 2021/5/23.
//  Copyright © 2021 黃鈺舒. All rights reserved.
//

#ifndef NEWSELECT_HPP
#define NEWSELECT_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "IScene.hpp"

class NewSelect final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit NewSelect() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif // NEWSELECT_HPP
