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

//usefull when creating multiple light objects with same settings
struct Attenuation {
	float constantMultiplier;
	float linearMultiplier;
	float quadraticMultiplier;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

static float getSpotLightCutOff(float degrees) {
	return glm::cos(glm::radians(degrees));
}

struct SpotLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const Attenuation& attenuation;
	float innerCutOff = getSpotLightCutOff(10.f);
	float outerCutOff = getSpotLightCutOff(15.f);
};

class RenderableObject
{
	unsigned int id;
	Model* model = nullptr;
	static unsigned int IdAdder;
	static std::vector<RenderableObject*> renderableObjects;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	unsigned int shaderType = 0;
	glm::vec3 position;
	bool isLuminous = false;
	unsigned int luminousObjectId = 0;
	static bool areLightsSet;
public:
	MaterialData materialData;
	static unsigned int luminousObjectsCount;
	static void SetLightProperties(const DirectionalLight& dirLight, const SpotLight& spotLight);

	//we use this function also to reset model matrix values
	//example for non constant rotation in game loop...
	//backpack.setPosition(); --> without this the object will rotate non stop
	//backpack.rotateAround(glm::normalize(glm::vec3(0.1f, 0.1f, 0.f)), glm::radians(50.f));
	void setPosition(glm::vec3 position);

	void translate(glm::vec3 position);
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
	void turnToLamp(const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation);
	unsigned int getShaderType() { return shaderType; }
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void RenderObjects(GLFWwindow* window, Camera* camera);
};
