//
// Created by s453388 on 27.05.2025.
//
#pragma once
#include <algorithm>
#include <iostream>
#include <engine/core/ObjectManager.h>

#include <engine/core/Engine.h>
#include <engine/rendering/Shader.h>


namespace hnh::engine {
    ObjectManager::ObjectManager(): fullScreenShader(Shader("shaders/fullscreen.vert", "shaders/fullscreen.frag")) {
        float quadVertices[] = {
            // positions   // texCoords
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &fullscreenQuadVAO);
        glGenBuffers(1, &fullscreenQuadVBO);
        glBindVertexArray(fullscreenQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        createFBO(fbo1, tex1);
        createFBO(fbo2, tex2);
    }

    void ObjectManager::draw() {
        // Render into FBO 1
        glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
        auto backgroundColor = Engine::get().backgroundColor;
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
        auto renderResolution = Engine::get().getRenderResolution();
        glViewport(0, 0, renderResolution.x, renderResolution.y);
        glClear(GL_COLOR_BUFFER_BIT);
        sortObjects();
        for (auto &gameObject: objects) {
            auto rootTransform = glm::mat3({
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
            });
            gameObject->draw(rootTransform);
        }
        // Render into FBO 2
        glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
        glClearColor(0.0, 0.0, 0.0, 0.0); // Blue
        glClear(GL_COLOR_BUFFER_BIT);
        for (auto &uiObject: ui) {
            auto rootTransform = glm::mat3({
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
            });
            uiObject->draw(rootTransform);
        }
        //Set Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Composite to screen
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(Engine::get().getWindow(), &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        fullScreenShader.use();
        fullScreenShader.setInteger("tex1", 0);
        fullScreenShader.setInteger("tex2", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);

        glBindVertexArray(fullscreenQuadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void ObjectManager::sortObjects() {
        std::sort(objects.begin(), objects.end(),
                  [](const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b) {
                      return a->getZIndex() < b->getZIndex();
                  });
    }

    void ObjectManager::reparentUIObject(engine::ui::UIObject *newParent, engine::ui::UIObject *child) {
        // Find the unique_ptr for the child in the top-level list
        auto it = std::find_if(ui.begin(), ui.end(),
            [&](const std::unique_ptr<engine::ui::UIObject>& ptr) {
                return ptr.get() == child;
            });

        if (it != ui.end()) {
            // Transfer ownership to the new parent's children
            newParent->addChild(std::move(*it));
            ui.erase(it);
        }
    }

    bool ObjectManager::checkUIInput(engine::InputEvent* inputEvent, glm::vec2 &mousePos) {
        bool intercepted = false;
        for (auto &uiObject: ui) {
            auto rootTransform = glm::mat3({
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
            });
            if (uiObject->checkForUIInput(rootTransform, inputEvent, mousePos)) {
                intercepted = true;
            }
        }
        return intercepted;

    }

    /// Changes the textures size for one fbo
    /// \param fbo: The FBO to update
    void ObjectManager::updateFBO(GLuint &fbo, GLuint &tex) const{
        auto renderResolution = Engine::get().getRenderResolution();
        int width= renderResolution.x;
        int height = renderResolution.y;

        // Delete the old texture
        if (glIsTexture(tex)) {
            glDeleteTextures(1, &tex);
        }

        // Create a new texture
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Bind the FBO and attach the new texture
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

        // Check FBO completeness again
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Updated framebuffer not complete!" << std::endl;
        }

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ObjectManager::createFBO(GLuint &fbo, GLuint &tex) const {
        {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            auto renderResolution = Engine::get().getRenderResolution();
            int width= renderResolution.x;
            int height = renderResolution.y;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            //Set Filtering while scaling to nearest. Pixelate
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "Framebuffer not complete!" << std::endl;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }


    void ObjectManager::updateFBOs(){
        updateFBO(fbo1, tex1);
        updateFBO(fbo2, tex2);
    }
}
