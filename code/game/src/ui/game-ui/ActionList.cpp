#pragma once
#include "ActionList.h"
#include "../../Game.h"


namespace hnh::game {
    ActionList::ActionList(): pageDownButton(
                                  engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
                                      "sprites/UIBox.png"), {0, 0}, {1, 50}),
                              pageUpButton(
                                  engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
                                      "sprites/UIBox.png"), {0, 0}, {1, 50}) {
        scale = 4;
        setupPageSwitching();
    }

    void ActionList::setupPageSwitching() {
        pageDownButton.position = {(width / scale / 2) - 15, 0};
        pageDownButton.setHoveredTexture(
            engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
                "sprites/UIButtonHover.png"));
        pageDownButton.setDisabledTexture(
            engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
                "sprites/UIButtonDisabled.png"));
        pageDownButton.setOnclickSound("audio/ButtonPress.wav");


        pageUpButton.position = {(width / scale / 2) + 5, 0};
        pageUpButton.setHoveredTexture(engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
            "sprites/UIButtonHover.png"));
        pageUpButton.setDisabledTexture(engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
            "sprites/UIButtonDisabled.png"));
        pageUpButton.setOnclickSound("audio/ButtonPress.wav");

        auto font = engine::Engine::get().getResourceManager()->loadResource<engine::ui::BitmapFont>(
            "sprites/minogram_6x10.png", "json/minogram_6x10.json");
        std::unique_ptr<engine::ui::UIText> textDown = std::make_unique<engine::ui::UIText>(font, U"<");
        textDown->position = {4, 1};
        pageDownButton.addChild(std::move(textDown));
        std::unique_ptr<engine::ui::UIText> textUp = std::make_unique<engine::ui::UIText>(font, U">");
        textUp->position = {3, 1};
        pageUpButton.addChild(std::move(textUp));
        setPageSwitchCallback();
    }

    void ActionList::setPageSwitchCallback() {
        pageUpButton.setOnClickCallback([&]() {
            if (selectedPage < actionListItems.size() - 1 && !pageUpButton.getDisabled()) {
                selectedPage++;
                pageDownButton.setDisabled(false);
            }
            if (selectedPage == actionListItems.size() - 1) pageUpButton.setDisabled(true);
        });
        pageDownButton.setOnClickCallback([&]() {
            if (selectedPage > 0 && !pageDownButton.getDisabled()) {
                selectedPage--;
                pageUpButton.setDisabled(false);
            }
            if (selectedPage == 0) pageDownButton.setDisabled(true);
        });
    }

    void ActionList::startUpPageSwitchFix() {
        if (selectedPage == actionListItems.size() - 1) pageUpButton.setDisabled(true);
        if (selectedPage == 0) pageDownButton.setDisabled(true);
    }

    void ActionList::updateUI() {
        if(!Game::instance->getPlayerCharacter()) return;
        getActions();
        for (auto &actionListPage: actionListItems) {
            for (ActionListItem &actionListItem: actionListPage) {
                actionListItem.updateUI();
            }
        }
        int approxHeight = (listVerticalOffset * pageSize) * 4;
        int yPos = engine::Engine::get().getRenderResolution().y / 2 - approxHeight / 2;
        position = {10, yPos};
        startUpPageSwitchFix();
    }

    void ActionList::draw(glm::mat3 &parentTransform) const {
        if(!visible) return;
        UIObject::draw(parentTransform);

        auto transform = parentTransform * calculateLocalMatrix();

        if (actionListItems.empty()) return;

        for (int i = 0; i < actionListItems[selectedPage].size(); ++i) {
            auto translation = glm::mat3{
                1, 0, 0,
                0, 1, 0,
                0, listVerticalOffset * i, 1
            };
            auto itemTransform = transform * translation;
            actionListItems[selectedPage][i].draw(itemTransform);
        }

        auto pageSwitchTranslation = glm::mat3{
            1, 0, 0,
            0, 1, 0,
            0, listVerticalOffset * actionListItems[selectedPage].size(), 1
        };
        auto pageSwitchTransform = transform * pageSwitchTranslation;
        pageDownButton.draw(pageSwitchTransform);
        pageUpButton.draw(pageSwitchTransform);
    }

    bool ActionList::checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) {
        auto transform = parentTransform * calculateLocalMatrix();
        bool intercepted = false;
        if (actionListItems.empty()) return intercepted;
        for (int i = 0; i < actionListItems[selectedPage].size(); ++i) {
            auto translation = glm::mat3{
                1, 0, 0,
                0, 1, 0,
                0, listVerticalOffset * i, 1
            };
            auto itemTransform = transform * translation;
            if (actionListItems[selectedPage][i].checkForUIInput(itemTransform, inputEvent, mousePos))
                intercepted = true;
        }
        auto pageSwitchTranslation = glm::mat3{
            1, 0, 0,
            0, 1, 0,
            0, listVerticalOffset * actionListItems[selectedPage].size(), 1
        };
        auto pageSwitchTransform = transform * pageSwitchTranslation;
        if (pageDownButton.checkForUIInput(pageSwitchTransform, inputEvent, mousePos)) intercepted = true;
        if (pageUpButton.checkForUIInput(pageSwitchTransform, inputEvent, mousePos)) intercepted = true;
        return intercepted;
    }

    void ActionList::getActions() {
        auto player = Game::instance->getPlayerCharacter();
        if (player) {
            actionListItems.clear();
            unsigned int currentPage = 0;
            unsigned int currentPageItem = 0;
            actionListItems.emplace_back();
            for (auto &pair: player->actions) {
                if (currentPageItem >= pageSize) {
                    currentPage++;
                    actionListItems.emplace_back();
                    currentPageItem = 0;
                }
                actionListItems[currentPage].emplace_back(pair.second.get(), pair.first,
                                                          glm::ivec2{width, listVerticalOffset});
                currentPageItem++;
            }
        }
    }
}
