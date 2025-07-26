#pragma once
#include <engine/ui/UIObject.h>

#include "QueueVisualizationItem.h"

namespace hnh::game {
    ///Will create \ref QueueVisualizationItem s for each character in the queue and pass on draw and input calls to them.
    ///It manages the positions of those objects as well.
    class QueueVisualization : public engine::ui::UIObject {
    public:
        QueueVisualization();
        void updateUI();
        void draw(glm::mat3 &parentTransform) const override;
        bool checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) override;

    private:
        int itemOffset = 20;
        int maxWidth = 100;
        void populateQueue();
        std::vector<QueueVisualizationItem> queueItems;
    };
}
