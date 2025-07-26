#pragma once
#include "QueueVisualization.h"

#include <queue>

#include "../../Game.h"

namespace hnh::game {
    QueueVisualization::QueueVisualization() {
        scale = 4;
        position = {150 * scale, 4};
    }

    void QueueVisualization::updateUI() {
        populateQueue();
        for (QueueVisualizationItem& item: queueItems) {
            item.updateUI();
        }
    }

    void QueueVisualization::draw(glm::mat3 &parentTransform) const {
        if(!visible) return;
        UIObject::draw(parentTransform);
        auto transform = parentTransform * calculateLocalMatrix();
        for(int i = 0; i < queueItems.size(); i++) {
            auto realOffset = itemOffset * i;
            if(realOffset > maxWidth) realOffset = maxWidth;
            glm::mat3 translation = {1, 0, 0,
                0, 1, 0,
                realOffset, 0 ,1.0f
            };
            auto itemTransform = transform * translation;
            queueItems[i].draw(itemTransform);
        }
    }

    bool QueueVisualization::checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent,
        glm::vec2 &mousePos) {
        if(!visible) return false;
        bool intercepted = false;
        if(UIObject::checkForUIInput(parentTransform, inputEvent, mousePos)) intercepted = true;
        auto transform = parentTransform * calculateLocalMatrix();
        for(int i = 0; i < queueItems.size(); i++) {
            auto realOffset = itemOffset * i;
            if(realOffset > maxWidth) realOffset = maxWidth;
            glm::mat3 translation = {1, 0, 0,
                0, 1, 0,
                realOffset, 0 ,1.0f
            };
            auto itemTransform = transform * translation;
            if(queueItems[i].checkForUIInput(itemTransform, inputEvent, mousePos)) intercepted = true;
        }

        return intercepted;
    }

    void QueueVisualization::populateQueue() {
        queueItems.clear();
        auto queue = Game::instance->queue;
        for (Character* character: queue) {
            if(!character->getIsDead()) {
                queueItems.emplace_back(character);
            }
        }
    }
}
