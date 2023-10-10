#include "contentInitializer.h"
#include "stb_image.h"

ContentInitializer* ContentInitializer::contentInitializer_ = nullptr;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void ContentInitializer::setCoreSettings() {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

ContentInitializer* ContentInitializer::GetInstance()
{
    if (!ContentInitializer::contentInitializer_) {
        ContentInitializer::contentInitializer_ = new ContentInitializer();
    }
    return ContentInitializer::contentInitializer_;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (ContentInitializer::GetInstance()->firstMouse)
    {
        ContentInitializer::GetInstance()->lastX = xpos;
        ContentInitializer::GetInstance()->lastY = ypos;
        ContentInitializer::GetInstance()->firstMouse = false;
    }
    float xoffset = xpos - ContentInitializer::GetInstance()->lastX;
    float yoffset = ContentInitializer::GetInstance()->lastY - ypos;
    ContentInitializer::GetInstance()->lastX = xpos;
    ContentInitializer::GetInstance()->lastY = ypos;
    ContentInitializer::GetInstance()->camera->ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ContentInitializer::GetInstance()->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}