#include "manualVertices.h"

void setLightOptions(Shader& lightingShader)
{
    lightingShader.use();
    lightingShader.setFloat("material.shininess", 32.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.3f, 0.1f, 0.1f)
    };
    // Directional light
    glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.diffuse"), 0.05f, 0.05f, 0.05);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
    // Point light 1                             
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].linear"), 0.09f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].quadratic"), 0.032);
    // Point light 2                             
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].quadratic"), 0.032);
    // Point light 3                            
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].quadratic"), 0.032);
    // Point light 4                          
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].quadratic"), 0.032);
    // spot light
    glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.quadratic"), 0.032);
    glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
    glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
}
