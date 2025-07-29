#include <stdexcept>
#include <engine/core/Context.h>
#include <engine/core/Engine.h>

namespace hnh::engine::context {
    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        Engine::get().getObjectManager()->updateFBOs();
    }

    Context::Context(int targetWidth, int targetHeight, const std::string &title) {
        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize glfw");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);




        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // remove window borders
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        int screenWidth = mode->width;
        int screenHeight = mode->height;

        float scalar = (float)targetWidth/(float)screenWidth;
        int fixedHeight = scalar * screenHeight;

        renderResolution = {targetWidth, fixedHeight};
        // This creates a fullscreen window at the monitor's native resolution
        window = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), nullptr, nullptr);
        glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
        if(window == nullptr) {
            throw std::runtime_error("Failed to create window");
        }
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glEnable(GL_DEPTH_TEST);
        if(glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("gl error");
        }
    }

    Context::~Context() {
        glfwTerminate();
    }
}