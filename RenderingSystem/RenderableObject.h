#pragma once
#include "Camera.h"
#include "model.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct MaterialData {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int diffuseMap;
	unsigned int specularMap;
	unsigned int verticesCount;
};

class RenderableObject
{
	unsigned int id;
	Model* model = nullptr;
	static unsigned int IdAdder;
	static std::vector<RenderableObject*> renderableObjects;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	unsigned int shaderType = 0;
public:
	MaterialData materialData;

	//we use this function also to reset model matrix values
	//example for non constant rotation in game loop...
	//backpack.setPosition(); --> without this the object will rotate non stop
	//backpack.rotateAround(glm::normalize(glm::vec3(0.1f, 0.1f, 0.f)), glm::radians(50.f));
	void setPosition(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f)) {
		modelMatrix = glm::mat4(1.f);
		modelMatrix = glm::translate(modelMatrix, position);
	}

	void translate(glm::vec3 position) {
		modelMatrix = glm::translate(modelMatrix, position);
	}
	void scale(float scalar) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
	}
	void rotateAround(glm::vec3 axis, float degrees) {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
	}
	RenderableObject(std::string modelPath);
	RenderableObject(float* vertices, unsigned int verticesCount, const char* diffuseTexturePath, const char* specularTexturePath);
	RenderableObject(float* vertices, unsigned int verticesCount);
	~RenderableObject();
	unsigned int getShaderType() { return shaderType; }
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void RenderObjects(GLFWwindow* window, Camera* camera);
};
