#pragma once

#include <iostream>
#include <engine/ui/UIButton.h>

#include <utility>

#include <engine/Assets.h>

#include "engine/core/Engine.h"

namespace hnh::engine::ui {
    UIButton::UIButton(const std::shared_ptr<UITexture>& uiTexture, const glm::vec2 &position, const glm::ivec2 &size,
                       float scale, Shader shader): UIBox(uiTexture, position, size, scale,
                                                          std::move(shader)),
                                                    hoveredTexture(uiTexture),
                                                    normalTexture(uiTexture),
                                                    disabledTexture(uiTexture) {
    }

    void UIButton::setHoveredTexture(std::shared_ptr<UITexture> uiTexture) {
        hoveredTexture = std::move(uiTexture);
    }

    void UIButton::setDisabledTexture(std::shared_ptr<UITexture> uiTexture) {
        disabledTexture = std::move(uiTexture);
    }

    void UIButton::setOnclickSound(const std::filesystem::path &audioPath) {
        int result = onClickSound.load(resolveAssetPath(audioPath).string().c_str());
        if (result != 0) {
            std::cerr << "Failed to load onClick sound " << result << std::endl;
        }else {
            hasClickSound = true;
        }
    }

    void UIButton::setDisabled(bool disabled) {
        UIBox::setDisabled(disabled);
        if(disabled) texture = disabledTexture;
        else texture = normalTexture;
    }

    void UIButton::onEnter() {
        texture = hoveredTexture;
    }

    void UIButton::onClick() {
        if(hasClickSound) Engine::get().audioManager.play(onClickSound);
    }

    void UIButton::onExit() {
        texture = normalTexture;
    }
}
