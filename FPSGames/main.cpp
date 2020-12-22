// 12.22 解决了地图模型导入的问题 可以上坡 爬箱子
//                p.s. 地图是真的丑

#define DEBUG_WITHOUT_BULLET_MAP


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
#include "shaderRender.h"


#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "freetype.lib")


//  Text Rendering
std::vector<Text> textObjects;
const std::vector<glm::vec2> textObjectsPos = {
	glm::vec2(SCR_WIDTH / 32 + 100.0f,	SCR_HEIGHT / 12),
	glm::vec2(SCR_WIDTH * 5 / 32 + 100.0f,	SCR_HEIGHT / 12)
};

//  Quads Rendering: AmmoIcon, HealthIcon, CrossHair
std::vector<Quads> quadsObjects;
const std::vector<glm::vec2> quadsObjectsPos = {
	glm::vec2(SCR_WIDTH / 32,	SCR_HEIGHT / 12),
	glm::vec2(SCR_WIDTH * 5 / 32,	SCR_HEIGHT / 12)
};

const std::vector<float> quadsSize = {
	0.05f, 0.05f, 0.1f
};


GLFWwindow* windowInit();
bool init();
void depthMapFBOInit();
void skyboxInit();
void setDeltaTime();
unsigned int loadCubemap(vector<std::string> faces);

myBulletEngine my_bt;

Camera camera(cameraPos);
glm::mat4 lightSpaceMatrix;

// 深度Map的ID
unsigned int depthMap;
unsigned int depthMapFBO;

// 将鼠标设置在屏幕中心
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing 用来平衡不同电脑渲染水平所产生的速度变化
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 天空盒
unsigned int cubemapTexture;
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubeVAO, cubeVBO;


int main()
{
	// 窗口初始化
	GLFWwindow* window = windowInit();
	// OpenGL初始化
	bool isInit = init();
	if (window == NULL || !isInit) {
		return -1;
	}
	// 深度Map的FBO配置
	depthMapFBOInit();
	// 天空盒的配置
	skyboxInit();
	//cube的配置
	cubeInit(cubeVAO, cubeVBO);
	my_bt.btInit();

	// 为所有物体添加光照和阴影的shader
	Shader shader("shader/light_and_shadow.vs", "shader/light_and_shadow.fs");
	// 从太阳平行光角度生成深度信息的shader
	Shader depthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
	// 天空盒shader
	Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
	// GUI
	// Quad Texture Shader
	Shader quadsShader("shader/Quads.vs", "shader/Quads.fs");
	Shader textShader("shader/Text.vs", "shader/Text.fs");

	// ------------------------------
	// 模型加载
	// ------------------------------

	// 时间增量Deltatime
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

	// 相机模型
	Model cameraModel(FileSystem::getPath("asset/models/obj/camera-cube/camera-cube.obj"));
	// 地面模型
	Model mapModel(FileSystem::getPath("asset/models/obj/Map/floor.obj"));
	// 地图
	Model objModel(FileSystem::getPath("asset/models/obj//Map/new_scene.obj"));//Castle Tower.obj
	//Model objModel_for_physical(FileSystem::getPath("asset/models/obj//Map/new_scene.obj"));//Castle Tower.obj



	//初始化物理引擎
	my_bt.addGround();
#ifdef DEBUG_WITHOUT_BULLET_MAP
	my_bt.addMap(objModel);
#endif // DEBUG_WITHOUT_BULLET_MAP

	my_bt.addSphere(1.0, 20, 50, 0, 1.0);
	my_bt.addSphere(1.0, 30, 30, 0, 1.0);
	my_bt.addSphere(1.0, 40, 90, 0, 1.0);
	my_bt.addSphere(1.0, 50, 100, 0, 1.0);

	my_bt.addPlayer();

	// 枪械模型
	Gun curGun(FileSystem::getPath(AK_Path), 31, 30);

	// Gun curGun(FileSystem::getPath(AWP_Path), 31, 30);
	//	texture conflicts ?
	//	put quads and text texture rendering after gun rendering
	quadsTextureInit(quadsObjects, quadsObjectsPos, quadsSize);
	textTextureInit(textShader, textObjectsPos, textObjects);

	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 15); // 这里的15是指"GL_TEXTURE15"，需要与后面的对应

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	quadsShader.use();
	quadsShader.setInt("texture1", 0);

	// ---------------------------------
	// 循环渲染
	// ---------------------------------

	while (!glfwWindowShouldClose(window)) {
		// 计算一帧的时间长度以便于使帧绘制速度均匀
		setDeltaTime();

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 物理模拟时长
		my_bt.world->stepSimulation(deltaTime);

		// 检查事件
		glfwPollEvents();
		// 监听按键
		handleKeyInput(window);

		// 渲染背景
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	由于Font.Configure()后Blend设置为了RGBA的混合值, 故需要重置Blend, 
		//	否则会产生黑边/黑底
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		// ---------------------------------
		// 渲染获得场景的深度信息
		// ---------------------------------

		// 定义光源视见体，即阴影生成范围的正交投影矩阵
		glm::mat4 lightProjection = glm::ortho(
			-200.0f, 200.0f,
			-200.0f, 200.0f,
			-200.0f, 200.0f);
		// TODO lightPos跟随相机位置进行移动，使相机周围的地方总会生成影子
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), WORLD_UP);
		lightSpaceMatrix = lightProjection * lightView;

		// 从光源角度渲染整个场景
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// 改变视口大小以便于进行深度的渲染
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//Shadow Render start
		//renderGunAndCamera(curGun, cameraModel, depthShader);
		//renderMap(mapModel, depthShader);

		//my_bt.renderMyMap(objModel, depthShader);

		// Render end
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 复原视口
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ---------------------------------
		// 模型渲染
		// ---------------------------------

		shader.use();

		// 设置光照相关属性
		renderLight(shader, lightSpaceMatrix, depthMap);

		// 渲染地面
		renderMap(mapModel, shader);

		// 渲染物理
		my_bt.renderMyMap(objModel, shader);
		for (int i = 0; i < my_bt.bodies.size(); i++)
		{
			renderSphere(my_bt.bodies[i], shader, cubeVAO);
		}
		my_bt.renderPlayer(shader, camera);


		// 使用shader渲染Gun和Camera（层级模型）
		renderGunAndCamera(curGun, cameraModel, shader);
		curGun.Display(camera, shader);

		//renderEnemy(enemyModel, shader);
		// --------------
		// 最后再渲染天空盒

		// 改变深度测试，使深度等于1.0时为无穷远
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		renderSkyBox(skyboxShader, cubemapTexture, skyboxVAO);
		// 复原深度测试
		glDepthFunc(GL_LESS);

		//  Text Rendering
		renderGUI(textShader, quadsShader, quadsObjects, textObjects, textObjectsPos);

		// 交换缓冲区和调查IO事件（按下的按键,鼠标移动等）
		glfwSwapBuffers(window);

		// 轮询事件
		glfwPollEvents();
	}
	//my_bt.btExit();

	// 关闭glfw
	glfwTerminate();
	return 0;

}


GLFWwindow* windowInit()
{
	// 初始化配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, u8"FPSDemo", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);


	// 令GLFW捕捉用户的鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}



bool init()
{
	// 加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("pause");
		return false;
	}

	// 配置全局openGL状态
	glEnable(GL_DEPTH_TEST);

	return true;
}

// 深度图配置
void depthMapFBOInit()
{
	glGenFramebuffers(1, &depthMapFBO);
	// 创建深度纹理
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 把生成的深度纹理作为帧缓冲的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 天空盒配置
void skyboxInit()
{
	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// 纹理加载
	cubemapTexture = loadCubemap(faces);
}


// 计算一帧的时间长度
void setDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}



// 将六份纹理加载为一个cubemap纹理
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
