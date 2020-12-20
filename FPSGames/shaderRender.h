#pragma once
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

#include "GLFW/glfw3.h"
#include "gun.h"
#include "utils.h"
#include "Font.h"
#include "Shape.h"
#include "control.h"



void renderCube(Shader& shader, unsigned int cubeVAO);

void renderSphere(btRigidBody* sphere, Shader& shader, unsigned int cubeVAO);

// 设置光照相关属性
void renderLight(Shader& shader, glm::mat4 lightSpaceMatrix, unsigned int depthMap);

void renderGUI(Shader& textShader, Shader& quadsShader, std::vector<Quads> quadsObjects, std::vector<Text> textObjects, std::vector<glm::vec2> textObjectsPos);

void renderGunAndCamera(Gun& curGun, Model& cameraModel, Shader& shader);
void renderCamera(Model& model, glm::mat4 modelMatrix, Shader& shader);
void renderEnemy(Model& model, Shader& shader);
void renderMap(Model& model, Shader& shader);

void renderSkyBox(Shader& shader, unsigned int cubemapTexture, unsigned int skyboxVAO);

//  Text Rendering
void textTextureInit(Shader& shader, std::vector<glm::vec2> textObjectsPos, std::vector<Text>& textObjects);

//  Quad Rendering: Ammo, Health, crossHair
void quadsTextureInit(std::vector<Quads>& quadsObjects, std::vector<glm::vec2> quadsObjectsPos, std::vector<float> quadsSize);
