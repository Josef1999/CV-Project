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
#include "physicalEngine.h"
#include "universal.h"

enum ObjIndex
{
	bt_Bullet,
	bt_Enemy,
	bt_Map,
	bt_Player,
	bt_Ground
};

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

void cubeInit(unsigned int& cubeVAO, unsigned int& cubeVBO)
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




myBulletEngine::myBulletEngine() {};

myBulletEngine::~myBulletEngine() {};

void myBulletEngine::btInit()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	world->setGravity(btVector3(0, -10, 0));
}

void myBulletEngine::btExit()
{
	delete dispatcher;
	delete collisionConfiguration;
	delete solver;
	delete world;
	delete broadphase;
}


void myBulletEngine::addGround() {
	//添加地面
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);

	body->getCollisionShape()->setUserIndex(bt_Ground);

	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);
}

void myBulletEngine::addMap(Model& model) {

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
		collision_shape->setUserIndex(bt_Map);

		btVector3 inertia = btVector3{ 0, 0, 0 };
		if (mass != 0.0) {
			collision_shape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo construction_info{ mass,
			motion, collision_shape, inertia };

		btRigidBody* my_obj = new btRigidBody{ construction_info };
		my_obj->setRestitution(btScalar(0.5));
		world->addRigidBody(my_obj);
		my_map_obj.push_back(my_obj);
	}
}



void myBulletEngine::addPlayer() {
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
	m_ghostObject->getCollisionShape()->setUserIndex(bt_Player);

	btScalar stepHeight = btScalar(0.35);
	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);

	m_character->setGravity(btVector3(0, -10000/80, 0));

	//向世界中添加碰撞对象
	world->addCollisionObject(
		m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
	);
	world->addAction(m_character);
}

btRigidBody* myBulletEngine::addSphere(float radius, float x, float y, float z, float mass)
{
	//初始化位姿信息
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//生成形状
	btSphereShape* sphere = new btSphereShape(radius);
	
	btVector3 inertia(0, 0, 0);
	//质量为零即静态物体
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//为物体增添运动姿态信息
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);
	//添加到世界中
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);

	body->getCollisionShape()->setUserIndex(bt_Enemy);



	bodies.push_back(body);

	return body;
}

btRigidBody* myBulletEngine::addEnemy(float scale, float x, float y, float z, float mass)
{
	//初始化位姿信息
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	//btBoxShape::btBoxShape(const btVector3 & boxHalfExtents)
	//生成形状
	btBoxShape* sphere = new btBoxShape(btVector3(1.0f* scale,2.4f* scale,1.0f* scale));
	btVector3 inertia(0, 0, 0);
	//质量为零即静态物体
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//为物体增添运动姿态信息
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);
	//添加到世界中
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);
	body->getCollisionShape()->setUserIndex(bt_Enemy);


	enemies.push_back(body);

	return body;
}

btRigidBody* myBulletEngine::addBullet(float radius, float x, float y, float z, float mass)
{
	//初始化位姿信息
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//生成形状
	btSphereShape* sphere = new btSphereShape(radius);
	btVector3 inertia(0, 0, 0);
	//质量为零即静态物体
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//为物体增添运动姿态信息
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);

	//添加到世界中
	body->setRestitution(btScalar(0.5));

	body->getCollisionShape()->setUserIndex(bt_Bullet);

	world->addRigidBody(body);


	bodies.push_back(body);

	return body;
}


void myBulletEngine::renderPlayer(Shader& shader, Camera& camera)
{
	//更新渲染对象
	float r = ((btSphereShape*)m_ghostObject->getCollisionShape())->getRadius();

	btTransform t = m_ghostObject->getWorldTransform();

	float mat[16];
	t.getOpenGLMatrix(mat);

	glm::mat4 positionTrans = glm::make_mat4(mat);

	if (BodyEvent == GLFW_KEY_LEFT_CONTROL)//下蹲
	{
		if (Bodycount == 1)
			_Cam_Animation_Timer.StartTimer();
		_Cam_Animation_Timer.StopTimer();
		float curTimer = _Cam_Animation_Timer.GetTimerSec();
		if (curTimer > 0.2)
			curTimer = 0.2;
		float Total_Timer = 0.2;
		float Y_Offset = 5.0f;
		//cout << curTimer << endl;
		camera.Position[0] = positionTrans[3][0];
		camera.Position[1] = positionTrans[3][1] + 5.0f - curTimer / Total_Timer * Y_Offset;
		camera.Position[2] = positionTrans[3][2];
	}
	else//起立
	{
		if (Bodycount != 0)
		{
			Bodycount = 0;
			_Cam_Animation_Timer.StartTimer();
		}
		_Cam_Animation_Timer.StopTimer();
		float curTimer = _Cam_Animation_Timer.GetTimerSec();
		if (curTimer > 0.2)
			curTimer = 0.2;
		float Total_Timer = 0.2;
		float Y_Offset = 5.0f;
		//cout << curTimer << endl;
		camera.Position[0] = positionTrans[3][0];
		camera.Position[1] = positionTrans[3][1] + curTimer / Total_Timer * Y_Offset;
		camera.Position[2] = positionTrans[3][2];
	}
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

void myBulletEngine::renderMyMap(Model& model, Shader& shader) {
	for (int i = 0; i < my_map_obj.size(); i++) {
		btTransform t;
		float mat[16];
		my_map_obj[i]->getMotionState()->getWorldTransform(t);

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

		model.Draw_Single(shader, i);

	
	}

}

void myBulletEngine::renderEnemy(Model& model, Shader& shader) {
	for (int i = 0; i < enemies.size(); i++) {
		btTransform t;
		float mat[16];
		enemies[i]->getMotionState()->getWorldTransform(t);
		float r = 0.1/1.5;// ((btBoxShape*)enemies[i]->getCollisionShape());

		t.getOpenGLMatrix(mat);

		glm::mat4 positionTrans = glm::make_mat4(mat);

		glm::mat4 modelMatrix;
		//    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		modelMatrix = positionTrans * modelMatrix;

		modelMatrix = glm::scale(modelMatrix, glm::vec3(r));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0.1*1.5,0));

		//glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		shader.setMat4("model", modelMatrix);

		// 视图转换
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		shader.setMat4("view", viewMatrix);

		// 投影转换
		glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
		shader.setMat4("projection", projMatrix);

		model.Draw(shader);
	}
}


void myBulletEngine::movePlayer(Direction direction, float deltaTime) {
	if (world)
	{
		
		btScalar walkVelocity = btScalar(1.1) * 8.0 * 2; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * deltaTime * 2.0f* SpeedAdjust;
		btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);

		//获取本地坐标向量,并且单位化
		btVector3 forwardDir, rightDir;
		forwardDir[0] = camera.Move.x;
		forwardDir[1] = camera.Move.y;
		forwardDir[2] = camera.Move.z;
		rightDir[0] = camera.Right.x;
		rightDir[1] = camera.Right.y;
		rightDir[2] = camera.Right.z;
		//cout << forwardDir[0]<< "  " << forwardDir[1]<< "  "<<forwardDir[2]<< endl;


		//控制前后行走,以及旋转方向,
		if (direction == _RIGHT)
		{
			walkDirection = rightDir;
			//按照方向移动角色
		}

		if (direction == _LEFT)
		{
			walkDirection = -rightDir;
			//按照方向移动角色

		}

		if (direction == _FORWARD)
		{
			walkDirection = forwardDir;

		}

		if (direction == _BACKWARD)
		{
			walkDirection = -forwardDir;
		}
		if (direction == _HIGHSPEED)
		{
			walkDirection = forwardDir;
			walkSpeed *= 1.5;
			//按照方向移动角色
		}
		if (direction == _JUMP && m_character && m_character->canJump())
		{
			btVector3 v = btVector3(0, 100/2, 0);
			m_character->jump(v);
			//按照方向移动角色

		}
		//cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
		m_character->setWalkDirection(walkDirection * walkSpeed);


	}
}
void myBulletEngine::movePlayer(Direction direction1, Direction direction2, float deltaTime) {
	if (world)
	{
		btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
		btScalar walkVelocity = btScalar(1.1) * 8.0 *2* SpeedAdjust; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * deltaTime * 2.0f;

		//获取本地坐标向量,并且单位化
		btVector3 forwardDir;
		btVector3 rightwardDir;
		forwardDir[0] = camera.Move.x;
		forwardDir[1] = camera.Move.y;
		forwardDir[2] = camera.Move.z;
		rightwardDir[0] = camera.Right.x;
		rightwardDir[1] = camera.Right.y;
		rightwardDir[2] = camera.Right.z;
		//cout << forwardDir[0] << "  " << forwardDir[1] << "  "<<forwardDir[2] << endl;


		//控制前后行走,以及旋转方向,
		if (direction1 == _RIGHT)
		{
			if (direction2 == _FORWARD)
			{
				walkDirection = (forwardDir + rightwardDir) .normalized();
				//按照方向移动角色
			}
			else if (direction2 == _BACKWARD)
				walkDirection = (-forwardDir + rightwardDir).normalized();
			else if (direction2 == _HIGHSPEED)
			{
				walkDirection = (forwardDir  + rightwardDir).normalized();
				walkSpeed *= 1.5;
			}
		}

		if (direction1 == _LEFT)
		{
			if (direction2 == _FORWARD)
			{
				walkDirection = (forwardDir - rightwardDir).normalized();
				//按照方向移动角色
			}
			else if (direction2 == _BACKWARD)
				walkDirection = (-forwardDir - rightwardDir) .normalized();
			else if (direction2 == _HIGHSPEED)
			{
				walkDirection = (forwardDir  - rightwardDir) .normalized();
				walkSpeed *= 1.5;
			}
		}

		//cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
		m_character->setWalkDirection(walkDirection * walkSpeed);




	}
}


void myBulletEngine::colisionDetect(glm::vec3 look, Camera camera){
	double scalar = 1000;
	glm::vec3 look_nor = glm::normalize(look);
	// 延长指向击打点
	glm::vec3 look_ahead = glm::vec3(look_nor.x * scalar, look_nor.y * scalar, look_nor.z * scalar);
	glm::vec3 look_dest = camera.Position + look_ahead;

	std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;

	btVector3 btFrom(camera.Position.x, camera.Position.y, camera.Position.z);
	btVector3 btTo(look_dest.x, look_dest.y, look_dest.z);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	world->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

	if (res.hasHit()) {
		int get_num = res.m_collisionObject->getCollisionShape()->getUserIndex();
		switch (abs(get_num))
		{
		case bt_Enemy:
			for (int i = 0; i < enemies.size(); i++) {
				auto cur_enemy = enemies[i];
				auto cur_collison_obj = cur_enemy->getCollisionShape();

				if (cur_collison_obj == res.m_collisionObject->getCollisionShape()) {
					printf("Collision bt_Enemy[%d]_%d at: <%.2f, %.2f, %.2f>\n", i, cur_collison_obj->getUserIndex(),
													res.m_hitPointWorld.getX(), res.m_hitPointWorld.getY(), res.m_hitPointWorld.getZ());
					
					btVector3 cur_v = cur_enemy->getLinearVelocity();
					if(abs(cur_v.getY()) < 0.01f )
						cur_enemy->setLinearVelocity(btVector3(0, 10, 0));

				}
			}
			
			break;
		case bt_Map:
			printf("Collision bt_Map at: <%.2f, %.2f, %.2f>\n", res.m_hitPointWorld.getX(), res.m_hitPointWorld.getY(), res.m_hitPointWorld.getZ());
			break;
		case bt_Ground:
			printf("Collision bt_Ground at: <%.2f, %.2f, %.2f>\n", res.m_hitPointWorld.getX(), res.m_hitPointWorld.getY(), res.m_hitPointWorld.getZ());
			break;
		default:
			break;
		}

	}
}
