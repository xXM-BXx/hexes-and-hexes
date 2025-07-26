//
// Created by mburc on 23.06.2025.
//

#pragma once
#include <soloud_wav.h>
#include <engine/ui/UIBox.h>

namespace gl3::engine::ui {
    ///A box with a few more functions.
    class UIButton : public UIBox {
    public:
        /// Create a button the same way as a \ref UIBox.
        UIButton(const std::shared_ptr<UITexture>& uiTexture, const glm::vec2 &position = glm::vec2(0.0f, 0.0f),
                 const glm::ivec2 &size = glm::ivec2(1, 1), float scale = 1.0f,
                 Shader shader = Shader("shaders/uiBox.vert", "shaders/uiBox.frag"));

        ///Set another texture to be displayed when the mouse hovers the button.
        void setHoveredTexture(std::shared_ptr<UITexture> uiTexture);
        ///Set another texture to be displayed when the button is disabled.
        void setDisabledTexture(std::shared_ptr<UITexture> uiTexture);

        /// Set a sound to be played when clicking the button.
        /// @param audioPath The path to the .wav file with the root /asset/.
        void setOnclickSound(const std::filesystem::path &audioPath);

        void setDisabled(bool disabled) override;

    protected:
        void onEnter() override;
        void onClick() override;
        void onExit() override;

        SoLoud::Wav onClickSound;
        bool hasClickSound = false;
        std::shared_ptr<UITexture> normalTexture;
        std::shared_ptr<UITexture> hoveredTexture;
        std::shared_ptr<UITexture> disabledTexture;
    };
}
