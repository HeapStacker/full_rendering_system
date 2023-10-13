#include "RenderableObject.h"
#include <algorithm>
#include "stb_image.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
unsigned int RenderableObject::IdAdder = 0;
std::vector<RenderableObject*> RenderableObject::renderableObjects;
static Shader* lightedObjectShader = nullptr;
static Shader* lampShader = nullptr;
static bool areShadersSet = false;

static void DeleteShaders() {
	if (lightedObjectShader) delete lightedObjectShader;
	if (lampShader) delete lampShader;
	lightedObjectShader = nullptr;
	lampShader = nullptr;
}

static void SetShaders() {
	if (!areShadersSet) {
		lightedObjectShader = new Shader("lighting.vs", "multyLight.fs");
		lampShader = new Shader("lightObj.vs", "lightObj.fs");
		
		lightedObjectShader->use();
		lightedObjectShader->setInt("material.texture_diffuse", 0);
		lightedObjectShader->setInt("material.texture_specular", 1); //create a better solution for more objects and different specular and diffuse maps
		lightedObjectShader->setFloat("material.shininess", 32.0f);

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};
		glm::vec3 pointLightColors[] = {
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.3f, 0.1f, 0.1f)
		};
		// Directional light
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "dirLight.diffuse"), 0.05f, 0.05f, 0.05);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
		// Point light 1                 					->
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[0].quadratic"), 0.032);
		// Point light 2               						->
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[1].quadratic"), 0.032);
		// Point light 3               						->
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[2].quadratic"), 0.032);
		// Point light 4            						->
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "pointLights[3].quadratic"), 0.032);
		// spot light										->
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightedObjectShader->ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		areShadersSet = true;
	}
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

RenderableObject* RenderableObject::FindById(unsigned int id) {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		if (RenderableObject::renderableObjects[i]->id == id) return RenderableObject::renderableObjects[i];
	}
	return nullptr;
}

RenderableObject::RenderableObject(std::string modelPath) {
	id = ++RenderableObject::IdAdder;
	shaderType = 0;
	SetShaders();
	model = new Model(modelPath);
	RenderableObject::renderableObjects.push_back(this);
}

RenderableObject::RenderableObject(float* vertices, unsigned int verticesSize, const char* diffuseTextureMap, const char* specularTextureMap) {
	id = ++RenderableObject::IdAdder;
	shaderType = 0;
	SetShaders();
	materialData.diffuseMap = loadTexture(diffuseTextureMap);
	materialData.specularMap = loadTexture(specularTextureMap);
	materialData.verticesCount = verticesSize / (sizeof(float) * 8);
	glGenVertexArrays(1, &materialData.VAO);
	glGenBuffers(1, &materialData.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, materialData.VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(materialData.VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	RenderableObject::renderableObjects.push_back(this);
}

RenderableObject::RenderableObject(float* vertices, unsigned int verticesSize) {
	id = ++RenderableObject::IdAdder;
	shaderType = 1;
	SetShaders();
	materialData.verticesCount = verticesSize / (sizeof(float) * 8);
	glGenVertexArrays(1, &materialData.VAO); //it's bad to create so many VAOs and VBOs
	glGenBuffers(1, &materialData.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, materialData.VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(materialData.VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	this->setPosition();
	RenderableObject::renderableObjects.push_back(this);
}

RenderableObject::~RenderableObject() {
	if (model) delete model;
	glDeleteVertexArrays(1, &materialData.VAO);
	glDeleteBuffers(1, &materialData.VBO);
	if (renderableObjects.size() == 0) { DeleteShaders(); } //this would lead to a major bug if we wanted to run empty content without any objects
	Erase(id);
}

//object isn't fully removed (only at the end of the program)
//UNSTABLE - doesn't crash the content but stops the camera from moving
void RenderableObject::Erase(unsigned int id) { 
	RenderableObject* object = RenderableObject::FindById(id);
	RenderableObject::renderableObjects.erase(std::remove(RenderableObject::renderableObjects.begin(), RenderableObject::renderableObjects.end(), object), RenderableObject::renderableObjects.end());
}

void RenderableObject::ReadActiveIDs() {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		std::cout << RenderableObject::renderableObjects[i]->id << ". id is active\n";
	}
}

void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
	shader->use();
	shader->setVec3("viewPos", camera->Position);
	shader->setVec3("spotLight.position", camera->Position);
	shader->setVec3("spotLight.direction", camera->Front);
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
}


//refactor to gain more optimised results
void RenderableObject::RenderObjects(GLFWwindow* window, Camera* camera) {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //make that the clear color can be changed somewhere
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //change the way of passing SCR_WIDTH & SCR_HEIGHT
	glm::mat4 view = camera->GetViewMatrix();
	static Shader testLampShader("lightObj.vs", "lightObj.fs");
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		if (renderableObjects[i]->shaderType == 0) {
			setShaderDrawProperties(lightedObjectShader, camera, renderableObjects[i]->modelMatrix, view, projection);
			if (renderableObjects[i]->model) {
				renderableObjects[i]->model->Draw(*lightedObjectShader);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderableObjects[i]->materialData.diffuseMap);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, renderableObjects[i]->materialData.diffuseMap);
				glBindVertexArray(renderableObjects[i]->materialData.VAO);
				glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
			}
		}
		else if (renderableObjects[i]->shaderType == 1) {
			setShaderDrawProperties(&testLampShader, camera, renderableObjects[i]->modelMatrix, view, projection);
			testLampShader.setMat4("model", renderableObjects[i]->modelMatrix);
			glBindVertexArray(renderableObjects[i]->materialData.VAO);
			glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
		}
		else {
			std::cout << "Shader type not implemented yet\n";
			glfwTerminate();
			DeleteShaders();
			exit(3);
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}
