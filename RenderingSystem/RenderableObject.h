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
	MaterialData materialData;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	unsigned int shaderType = 0;
public:
	void setPosition(glm::vec3 position) {
		modelMatrix = glm::mat4(1.f);
		modelMatrix = glm::translate(modelMatrix, position);
	}
	void translate(glm::vec3 position) {
		modelMatrix = glm::translate(modelMatrix, position);
	}
	void scale(float scalar) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
	}
	void rotateAround(glm::vec3 axis, float howMutch) {
		modelMatrix = glm::rotate(modelMatrix, howMutch, axis);
	}
	RenderableObject(std::string modelPath);
	RenderableObject(float* vertices, unsigned int verticesCount, unsigned int diffuseTextureMap, unsigned int specularTextureMap);
	RenderableObject(float* vertices, unsigned int verticesCount);
	~RenderableObject();
	unsigned int getShaderType() { return shaderType; }
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void RenderObjects(GLFWwindow* window, Camera* camera);
};
