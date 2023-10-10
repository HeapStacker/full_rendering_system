#ifndef CONTENT_INIT
#define CONTENT_INIT

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "camera.h"
#include "RenderableObject.h"

class ContentInitializer
{
    static ContentInitializer* contentInitializer_;
    void initGLFW() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }
    GLFWwindow* createWindow(const char* windowName, int windowWidth, int windowHeight) {
        GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(1);
        }
        return window;
    }
    void setCoreSettings();
    void loadGlad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(2);
        }
        stbi_set_flip_vertically_on_load(true);
        glEnable(GL_DEPTH_TEST);
    }
    void setLastXY(unsigned int windowWidth, unsigned int windowHeight) {
        lastX = windowWidth; lastY = windowHeight;
    }
    ContentInitializer() {}
public:
    Camera* camera = nullptr; //refactor
    bool firstMouse = true;
    float lastX = 0.f;
    float lastY = 0.f;
	GLFWwindow* window = nullptr;
    ContentInitializer(ContentInitializer& other) = delete;
    void operator=(const ContentInitializer&) = delete;
    static ContentInitializer* GetInstance();
    ~ContentInitializer() { delete camera; }

    void setUp(const char* contentName = "OpenGl Context", int windowWidth = 800, int windowHeight = 600, glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), float cameraFov = 45.f, float cameraSensitivity = 0.1f, int fpsCap = 60) {
        initGLFW();
        setLastXY(windowWidth, windowHeight); //fix maby
        window = createWindow(contentName, windowWidth, windowHeight);
        setCoreSettings();
        loadGlad();
        camera = new Camera(cameraPos, cameraFov, cameraSensitivity);
    }
    void changeCamera(Camera* camera) {
        delete(this->camera);
        this->camera = nullptr;
        this->camera = camera;
    }
};


#endif

