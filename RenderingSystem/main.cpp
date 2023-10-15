#define STB_IMAGE_IMPLEMENTATION //must only be here (on the start)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include <iostream>
#include "contentInitializer.h"
#include "RenderableObject.h"

using namespace std::chrono;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLFWwindow* window = nullptr;
Camera* camera = nullptr;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
		RenderableObject::Erase(2);
	}
}

static glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
};

static glm::vec3 pointLightColors[] = {
	glm::vec3(1.f, 1.f, 1.f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.3f, 0.1f, 0.1f)
};

static int fps = 120;

static float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

Attenuation attenuation = { 1.f, 0.09f, 0.032f };
DirectionalLight dirLight = { {-0.2f, -1.0f, -0.3f}, {0.2f, 0.2f, 0.2f}, {0.05f, 0.05f, 0.05f}, {0.2f, 0.2f, 0.2f} };
SpotLight spotLight = { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, attenuation, getSpotLightCutOff(10.f), getSpotLightCutOff(15.f) };

int main()
{
	ContentInitializer* initializer = ContentInitializer::GetInstance();
	initializer->setUp("OpenGL Tut", SCR_WIDTH, SCR_HEIGHT);
	window = initializer->window;
	camera = initializer->camera;
	RenderableObject::SetLightProperties(dirLight, spotLight);
	RenderableObject backpack("models/backpack/backpack.obj");
	RenderableObject cube(vertices, sizeof(vertices), "textures/container2.png", "textures/container2_specular.png");
	RenderableObject cube1(vertices, sizeof(vertices), "textures/container2.png", "textures/container2_specular.png");
	RenderableObject cube2(vertices, sizeof(vertices), "textures/container2.png", "textures/container2_specular.png");
	RenderableObject cube3(vertices, sizeof(vertices), "textures/container2.png", "textures/container2_specular.png");
	//cube3.turnToLamp(pointLightColors[0], 0.4f, { 1.f, 0.09f, 0.032f });
	cube3.turnToLamp(pointLightColors[0], 0.4f, { 1.f, 1.f, 1.f });
	cube1.setPosition({ 5, 5, 5 });
	cube2.setPosition({ -1, 5, 5 });
	cube3.setPosition({ 5, -1, 10 });
	system_clock::time_point tp1, tp2;
	duration frameDuration = milliseconds(1000) / fps;
	while (!glfwWindowShouldClose(window))
	{
		tp1 = system_clock::now();
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		backpack.setPosition({2, 2, 2});
		backpack.rotateAround({ 0.1f, 0.1f, 0.f }, (float)glfwGetTime() * 100);
		cube.setPosition({0, 0, 0});
		cube.rotateAround({ 0, 1, 0 }, 45);
		RenderableObject::RenderObjects(window, camera);
		tp2 = system_clock::now();
		if (frameDuration > tp2 - tp1) {
			std::this_thread::sleep_for(frameDuration - (tp2 - tp1));
		}
	}
	glfwTerminate();
	return 0;
}
