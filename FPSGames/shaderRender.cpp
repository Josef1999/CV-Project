#include"include/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/learnopengl/camera.h"
#include "include/learnopengl/filesystem.h"
#include "include/learnopengl/model.h"
#include "include/learnopengl/shader_m.h"
#include "include/stb_image.h"
#include <iostream>

/**bullet*/
#include <stdio.h>
#include "btBulletDynamicsCommon.h"
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>


#include "GLFW/glfw3.h"
#include "gun.h"
#include "utils.h"
#include "Font.h"
#include "Shape.h"
#include "control.h"



void renderCube(Shader& shader, unsigned int cubeVAO) {
	// 视图转换
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("view", viewMatrix);
	// 模型转换
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 1.5f, -4.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(120.0f), WORLD_UP);
	shader.setMat4("model", modelMatrix);
	// 投影转换
	glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader.setMat4("projection", projMatrix);

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderSphere(btRigidBody* sphere, Shader& shader, unsigned int cubeVAO)
{
	//if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE) {
	//	//        std::cout << RED << "sphere error" << RESET << std::endl;
	//	return;
	//}
	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);

	float mat[16];
	t.getOpenGLMatrix(mat);

	glm::mat4 positionTrans = glm::make_mat4(mat);

	glm::mat4 model;
	//    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = positionTrans * model;
	model = glm::scale(model, glm::vec3(r));
	//glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	shader.setMat4("model", model);

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}




// 设置光照相关属性
void renderLight(Shader& shader, glm::mat4 lightSpaceMatrix, unsigned int depthMap)
{
	shader.setVec3("viewPos", camera.getPosition());
	shader.setVec3("lightDirection", lightDirection);
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, depthMap);
}
void renderGUI(Shader& textShader, Shader& quadsShader, std::vector<Quads> quadsObjects, std::vector<Text> textObjects, std::vector<glm::vec2> textObjectsPos)
{
	for (unsigned int i = 0; i < quadsObjects.size(); i++) {
		quadsShader.use();
		quadsObjects[i].activateTexture();
		quadsObjects[i].draw();
	}

	for (unsigned int i = 0; i < textObjects.size(); i++) {
		textObjects[i].SetPosition(textObjectsPos[i]);
		textObjects[i].Render(textShader);
	}
}
void renderGunAndCamera(Gun& curGun, Model& cameraModel, Shader& shader)
{
	// 视图转换
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("view", viewMatrix);
	// 投影转换
	glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader.setMat4("projection", projMatrix);

	// -------
	// 层级建模

	// 模型转换
	glm::mat4 modelMatrix = glm::mat4(1.0f);


	// 渲染枪支
	// curGun.Display_HoldGun(camera, shader, modelMatrix);

}
void renderCamera(Model& model, glm::mat4 modelMatrix, Shader& shader)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw() / 2), WORLD_UP);
	modelMatrix = glm::translate(modelMatrix, cameraPos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

	// 应用变换矩阵
	shader.setMat4("model", modelMatrix);

	model.Draw(shader);
}
void renderEnemy(Model& model, Shader& shader)
{
	// 视图转换
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("view", viewMatrix);
	// 模型转换
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 1.5f, -4.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-120.0f), WORLD_UP);
	shader.setMat4("model", modelMatrix);
	// 投影转换
	glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader.setMat4("projection", projMatrix);

	model.Draw(shader);
}
void renderMap(Model& model, Shader& shader)
{
	// 视图转换
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	shader.setMat4("view", viewMatrix);
	// 模型转换
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	shader.setMat4("model", modelMatrix);
	// 投影转换
	glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader.setMat4("projection", projMatrix);

	model.Draw(shader);
}



void renderSkyBox(Shader& shader, unsigned int cubemapTexture, unsigned int skyboxVAO)
{
	// viewMatrix 通过构造，移除相机的移动
	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	// 投影
	glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);

	shader.setMat4("view", viewMatrix);
	shader.setMat4("projection", projMatrix);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


//  Text Rendering
void textTextureInit(Shader& shader, std::vector<glm::vec2> textObjectsPos, std::vector<Text>& textObjects)
{
	Text ammoText, healthText;

	ammoText.Configure(shader, "asset/Fonts/Roboto-BoldItalic.ttf");
	ammoText.SetText("35");
	ammoText.SetScale(0.8f);
	ammoText.SetPosition(textObjectsPos[0]);
	ammoText.SetColor(glm::vec3(0.5, 0.8f, 0.2f));
	textObjects.push_back(ammoText);

	healthText.Configure(shader, "asset/Fonts/Roboto-BoldItalic.ttf");
	healthText.SetText("100");
	healthText.SetScale(0.8f);
	healthText.SetPosition(textObjectsPos[1]);
	healthText.SetColor(glm::vec3(0.3, 0.7f, 0.9f));
	textObjects.push_back(healthText);
}

//  Quad Rendering: Ammo, Health, crossHair
void quadsTextureInit(std::vector<Quads>& quadsObjects, std::vector<glm::vec2> quadsObjectsPos, std::vector<float> quadsSize)
{
	Quads ammoIcon(quadsSize[0], quadsSize[0], quadsObjectsPos[0].x, quadsObjectsPos[0].y),
		healthIcon(quadsSize[1], quadsSize[1], quadsObjectsPos[1].x, quadsObjectsPos[1].y),
		//	we adjust crossHair position here, 
		//	bcz posx and posy in Quads are left-down positions
		crossHair(quadsSize[2], quadsSize[2], SCR_WIDTH / 2 - quadsSize[2] * 0.5 * SCR_WIDTH,
			SCR_HEIGHT / 2 - quadsSize[2] * 0.5 * SCR_HEIGHT);

	ammoIcon.loadTextures("asset/textures/Ammo.png");
	quadsObjects.push_back(ammoIcon);
	healthIcon.loadTextures("asset/textures/HealthIcon.png");
	quadsObjects.push_back(healthIcon);
	crossHair.loadTextures("asset/textures/crossHair.png");
	quadsObjects.push_back(crossHair);
}
