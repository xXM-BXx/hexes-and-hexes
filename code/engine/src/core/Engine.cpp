#include <engine/core/Engine.h>

#include <glm/ext/matrix_clip_space.hpp>

namespace hnh::engine {
    Engine & Engine::get() {
        static Engine instance;
        return instance;
    }

    void Engine::init(int width, int height, const std::string &title) {
        context = std::make_unique<context::Context>(width, height, title);
        inputHandler = std::make_unique<InputHandler>(context.get());
        objectManager = std::make_unique<ObjectManager>();
    }

    Engine::Engine(){
        audioManager.setGlobalVolume(1.0f);
    }

    ObjectManager * Engine::getObjectManager() {
        return objectManager.get();
    }

    ResourceManager * Engine::getResourceManager() {
        return &resourceManager;
    }

    InputHandler * Engine::getInputHandler() {
        return inputHandler.get();
    }

    void Engine::run() {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glfwSetTime(1.0 / 60);

        //start.invoke();
        while (!glfwWindowShouldClose(getWindow())) {
            //update.invoke(deltaTime);
            //draw();
            //updateDeltaTime();
            //glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
    }

    glm::ivec2
    Engine::screenToWorld(glm::vec2 mousePos) const {
        //Convert mousePos to Normalized Device Coordinates (NDC)
        glm::vec2 ndc;
        ndc.x = (mousePos.x / getRenderResolution().x) * 2.0f - 1.0f;
        ndc.y = 1.0f - (mousePos.y / getRenderResolution().y) * 2.0f; // Flip Y axis

        //NDC to clip space (z = 0 for 2D plane)
        auto clipSpace = glm::vec4(ndc.x, ndc.y, 0.0f, 1.0f);

        //Inverse project to world space
        auto inverseProj = inverse(getSceneProjectionMatrix());
        auto world = inverseProj * clipSpace;


        return glm::vec2(world);
    }

    glm::mat4 Engine::getSceneProjectionMatrix() const {
        //Calculate Projection matrix
        auto renderResolution = getRenderResolution();
        int windowWidth= renderResolution.x;
        int windowHeight = renderResolution.y;

        return  glm::ortho(-((float)windowWidth / 2), ((float)windowWidth / 2), ((float)windowHeight / 2), -((float)windowHeight / 2));
    }

    glm::mat4 Engine::getUIProjectionMatrix() const {
        //Calculate Projection matrix
        auto renderResolution = getRenderResolution();
        int windowWidth= renderResolution.x;
        int windowHeight = renderResolution.y;

        return glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);
    }

    void Engine::setVolume(float volume) {
        audioManager.setGlobalVolume(volume);
    }


    void Engine::draw() {
        glClearColor(.5, 0, .5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        objectManager->draw();
        glfwSwapBuffers(getWindow());
    }


    void Engine::updateDeltaTime() {
        float frameTime = glfwGetTime();
        deltaTime = frameTime - lastFrameTime;
        lastFrameTime = frameTime;
    }


    Engine::~Engine() {
        glfwTerminate();
    }
}
