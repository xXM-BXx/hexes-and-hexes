#include "ActionListItem.h"

#include "../../Game.h"
#include "engine/core/Engine.h"

namespace hnh::game {
    ActionListItem::ActionListItem(hnh::game::Action *action, const std::string &onClickName, glm::ivec2 size)
        : action(action),
          button(engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>("sprites/UIBox.png"),
                 {0, 0}, {size.x, size.y * 3}),
          description(
              engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>("sprites/UIBox.png"),
              {(size.x / 4) + 2, 0}, {350, 300}),
          actionCooldownText(
              engine::Engine::get().getResourceManager()->loadResource<engine::ui::BitmapFont>(
                  "sprites/minogram_6x10.png", "json/minogram_6x10.json"), U"", {(size.x / 4) - 18, 1}),
          actionNameText(
              engine::Engine::get().getResourceManager()->loadResource<engine::ui::BitmapFont>(
                  "sprites/minogram_6x10.png", "json/minogram_6x10.json"), U"", {4, 1}) {
        this->onClickName = onClickName;
        setupCallbacks();
        description.visible = false;
        addDescriptionText();
    }

    void ActionListItem::addDescriptionText() {
        auto font = engine::Engine::get().getResourceManager()->loadResource<engine::ui::BitmapFont>(
                  "sprites/minogram_6x10.png", "json/minogram_6x10.json");
        std::unique_ptr<engine::ui::UIText> text = std::make_unique<engine::ui::UIText>(font);
        text->position = {4, 2};
        text->scale = 0.5f;
        descriptionText = dynamic_cast<engine::ui::UIText*>(description.addChild(std::move(text)));
    }

    void ActionListItem::draw(glm::mat3 &parentTransform) const {
        UIObject::draw(parentTransform);
        auto transform = parentTransform * calculateLocalMatrix();
        button.draw(transform);
        actionNameText.draw(transform);
        actionCooldownText.draw(transform);
        description.draw(transform);
    }

    bool ActionListItem::checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent,
                                         glm::vec2 &mousePos) {
        setupCallbacks();
        auto transform = parentTransform * calculateLocalMatrix();
        bool intercepted = false;
        if (button.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        return intercepted;
    }

    void ActionListItem::updateUI() {
        button.setOnclickSound(Game::instance->defaultButtonClickSound);

        if(action->currentCooldown > 0) {
            button.setDisabled(true);
        }else if (action->currentCooldown == 0) {
            button.setDisabled(false);
        }


        int currentCooldown = action->currentCooldown;
        if (currentCooldown == 0) currentCooldown = action->maxCooldown;
        std::wstring wide = std::to_wstring(currentCooldown);
        std::u32string cooldownText = std::u32string(wide.begin(), wide.end()) + U"\u231A";
        int cooldownTextLength = wide.length() * 6 + 10;
        actionCooldownText.position = {(button.getSize().x / 4) - cooldownTextLength, 1};

        auto displayName = action->displayName;
        actionNameText.setText(std::u32string(displayName.begin(), displayName.end()));
        actionCooldownText.setText(cooldownText);

        auto descriptionString = action->displayDescription;
        descriptionText->setText(std::u32string(descriptionString.begin(), descriptionString.end()));
    }

    void ActionListItem::setupCallbacks() {
        button.setHoveredTexture(
            engine::Engine::get().getResourceManager()->loadResource<
                engine::ui::UITexture>("sprites/UIButtonHover.png"));
        button.setDisabledTexture(engine::Engine::get().getResourceManager()->loadResource<
                engine::ui::UITexture>("sprites/UIButtonDisabled.png"));
        button.setOnClickCallback([name = onClickName]() {
            if(Game::instance->getPlayerCharacter())
            Game::instance->getPlayerCharacter()->setSelectedAction(name);
        });
        button.setOnEnterCallback([desc = &description]() {
            desc->visible = true;
        });
        button.setOnExitCallback([desc = &description]() {
            desc->visible = false;
        });
    }
}
