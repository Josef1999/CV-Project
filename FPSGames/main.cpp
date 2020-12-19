
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

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "freetype.lib")


GLFWwindow* windowInit();
bool init();
void depthMapFBOInit();
void skyboxInit();
void textTextureInit(Shader& shader);
void quadsTextureInit();

void setDeltaTime();


// 使用“&”引用性能更好
void renderLight(Shader& shader);
void renderGunAndCamera(Gun& curGun, Model& cameraModel, Shader& shader);
void renderCamera(Model& model, glm::mat4 modelMatrix, Shader& shader);
void renderMap(Model& model, Shader& shader);
void renderEnemy(Model& model, Shader& shader);
void renderSkyBox(Shader& shader);
void renderGUI(Shader& textShader, Shader& quadsShader);

//bullet
void btInit();
void btExit();
void cubeInit();

//bullet导入物理模型
void addMap(Model& model);
void addGround();
void addPlayer();

//bullet导入物理渲染
void renderPlayer(Shader& shader);
void renderMyMap(Model& model, Shader& shader);
void renderSphere(btRigidBody* sphere, Shader& shader);
void renderCube(Shader& shader);


/**bullet 初始化**/
btDynamicsWorld* world;
btDispatcher* dispatcher;
btCollisionConfiguration* collisionConfiguration;
btBroadphaseInterface* broadphase;
btConstraintSolver* solver;

std::vector<btRigidBody*> bodies;
std::vector<btRigidBody*> my_map_obj;

btRigidBody* cube = NULL;
btRigidBody* ground = NULL;

//控制角色类
btKinematicCharacterController* m_character;
btPairCachingGhostObject* m_ghostObject;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

void handleKeyInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned int loadCubemap(vector<std::string> faces);




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

// ------------------------------------------
// main函数
// ------------------------------------------
//Cube 
unsigned int cubeVAO, cubeVBO;

// Cube定点数据
float cube_vertices[] = {
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
	cubeInit();
	btInit();

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
	Model objModel(FileSystem::getPath("asset/models/obj//Map/Castle Tower.obj"));


	//初始化物理引擎
	addGround();
	addMap(objModel);

	addSphere(1.0, 20, 50, 0, 1.0);
	addSphere(1.0, 30, 30, 0, 1.0);
	addSphere(1.0, 40, 90, 0, 1.0);
	addSphere(1.0, 50, 100, 0, 1.0);

	addPlayer();

	// 枪械模型


	Gun curGun(FileSystem::getPath(AK_Path), 31, 30);

	// Gun curGun(FileSystem::getPath(AWP_Path), 31, 30);
	//	texture conflicts ?
	//	put quads and text texture rendering after gun rendering
	quadsTextureInit();
	textTextureInit(textShader);

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
		world->stepSimulation(deltaTime);

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
		//depthShader.use();
		//depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		// 改变视口大小以便于进行深度的渲染
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//Shadow Render start
		//renderGunAndCamera(curGun, cameraModel, depthShader);
		//renderMap(mapModel, depthShader);
		//renderMyMap(objModel, depthShader);
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
		renderLight(shader);

		// 渲染地面
		renderMap(mapModel, shader);
		// 渲染场景
		renderMyMap(objModel, shader);

		for (int i = 0; i < bodies.size(); i++)
		{
			renderSphere(bodies[i], shader);
		}
		renderPlayer(shader);

		// 使用shader渲染Gun和Camera（层级模型）
		renderGunAndCamera(curGun, cameraModel, shader);
		curGun.Display(camera, shader);

		//renderEnemy(enemyModel, shader);

		// --------------
		// 最后再渲染天空盒

		// 改变深度测试，使深度等于1.0时为无穷远
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		renderSkyBox(skyboxShader);
		// 复原深度测试
		glDepthFunc(GL_LESS);

		//  Text Rendering
		renderGUI(textShader, quadsShader);

		// 交换缓冲区和调查IO事件（按下的按键,鼠标移动等）
		glfwSwapBuffers(window);

		// 轮询事件
		glfwPollEvents();
	}

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

void renderCube(Shader& shader) {
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

void renderSphere(btRigidBody* sphere, Shader& shader)
{
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE) {
		//        std::cout << RED << "sphere error" << RESET << std::endl;
		return;
	}
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

//  Text Rendering
void textTextureInit(Shader& shader)
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
void quadsTextureInit()
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

// 计算一帧的时间长度
void setDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

// 设置光照相关属性
void renderLight(Shader& shader)
{
	shader.setVec3("viewPos", camera.getPosition());
	shader.setVec3("lightDirection", lightDirection);
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, depthMap);
}
void renderGUI(Shader& textShader, Shader& quadsShader)
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
void addMap(Model& model) {

	//初始化位姿信息
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, -5, 0));
	//为物体增添运动姿态信息
	btScalar mass = 0;

	for (int i = 0; i < model.meshes.size(); i++) {
		//单个物体
		cout << "meshes : " << i << endl;
		btMotionState* motion = new btDefaultMotionState(t);
		btConvexHullShape* collisionShape = new btConvexHullShape(); //用桌子的点集构建了一个凸的碰撞模型，虽然桌子是凹的
		cout << "ww" << endl;
		btConvexHullShape* unoptimized_hull = new btConvexHullShape{};
		for (int j = 0; j < model.meshes[i].vertices.size(); j++) {
			//顶点
			cout << "  " << j << endl;
			btVector3 vertex{
				model.meshes[i].vertices[j].Position.x,
				model.meshes[i].vertices[j].Position.y,
				model.meshes[i].vertices[j].Position.z };
			cout << model.meshes[i].vertices[j].Position.x << " " << model.meshes[i].vertices[j].Position.y << " " << model.meshes[i].vertices[j].Position.z << endl;
			unoptimized_hull->addPoint(vertex);
		}
		btCollisionShape* collision_shape = new btConvexHullShape{ *unoptimized_hull };

		btVector3 inertia = btVector3{ 0, 0, 0 };
		collision_shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo construction_info{ mass,
			motion, collision_shape, inertia };

		btRigidBody* my_obj = new btRigidBody{ construction_info };
		my_obj->setRestitution(btScalar(0.5));
		world->addRigidBody(my_obj);
		my_map_obj.push_back(my_obj);
	}
}

void cubeInit()
{
	int stride = (3 + 3 + 2);

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);

	/**position attribute*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/**color attribute*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void btInit()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	world->setGravity(btVector3(0, -10, 0));
}

void btExit()
{
	delete dispatcher;
	delete collisionConfiguration;
	delete solver;
	delete world;
	delete broadphase;
}

void renderSphere(btRigidBody* sphere, Shader& shader);
void renderCube(Shader& shader);

void addGround() {
	//添加地面
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);
}

void addPlayer() {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(10, 0, 0));

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(t);

	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	btScalar characterHeight = 2.0f;
	btScalar characterWidth = 2.0f;
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(0.35);
	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);

	m_character->setGravity(btVector3(0, -10, 0));

	//向世界中添加碰撞对象
	world->addCollisionObject(
		m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
	);
	world->addAction(m_character);
}


void renderPlayer(Shader& shader)
{
	//更新渲染对象
	float r = ((btSphereShape*)m_ghostObject->getCollisionShape())->getRadius();

	btTransform t = m_ghostObject->getWorldTransform();

	float mat[16];
	t.getOpenGLMatrix(mat);

	glm::mat4 positionTrans = glm::make_mat4(mat);

	camera.Position[0] = positionTrans[3][0];
	camera.Position[1] = positionTrans[3][1];
	camera.Position[2] = positionTrans[3][2];
	glm::mat4 model;
	//    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = positionTrans * model;
	//model = glm::scale(model, glm::vec3(r));
	//glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	shader.setMat4("model", model);

	/*glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);*/
}


void renderMyMap(Model& model, Shader& shader) {
	for (int i = 0; i < my_map_obj.size(); i++) {
		btTransform t;
		float mat[16];
		t.getOpenGLMatrix(mat);

		glm::mat4 positionTrans = glm::make_mat4(mat);

		glm::mat4 modelMatrix;
		//    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		modelMatrix = positionTrans * modelMatrix;
		//glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		shader.setMat4("model", modelMatrix);

		// 视图转换
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		shader.setMat4("view", viewMatrix);

		// 投影转换
		glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
		shader.setMat4("projection", projMatrix);

		model.Draw(shader);
		my_map_obj[i]->getMotionState()->getWorldTransform(t);

	}

}

void renderSkyBox(Shader& shader)
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
// 改变窗口大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 确保窗口匹配的新窗口尺寸
	glViewport(0, 0, width, height);
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
