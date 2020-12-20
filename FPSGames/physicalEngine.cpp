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


enum ObjIndex
{
	bt_Bullet,
	bt_Enemy,
	bt_Map,
	bt_Player,
	bt_Ground
};


// Cube��������
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
	//��ӵ���
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

	//��ʼ��λ����Ϣ
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, -5, 0));
	//Ϊ���������˶���̬��Ϣ
	btScalar mass = 0;

	for (int i = 0; i < model.meshes.size(); i++) {
		//��������
		cout << "meshes : " << i << endl;
		btMotionState* motion = new btDefaultMotionState(t);
		btConvexHullShape* collisionShape = new btConvexHullShape(); //�����ӵĵ㼯������һ��͹����ײģ�ͣ���Ȼ�����ǰ���
		cout << "ww" << endl;
		btConvexHullShape* unoptimized_hull = new btConvexHullShape{};
		for (int j = 0; j < model.meshes[i].vertices.size(); j++) {
			//����
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
		collision_shape->calculateLocalInertia(mass, inertia);

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

	m_character->setGravity(btVector3(0, -10, 0));

	//�������������ײ����
	world->addCollisionObject(
		m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
	);
	world->addAction(m_character);
}

btRigidBody* myBulletEngine::addSphere(float radius, float x, float y, float z, float mass)
{
	//��ʼ��λ����Ϣ
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//������״
	btSphereShape* sphere = new btSphereShape(radius);
	btVector3 inertia(0, 0, 0);
	//����Ϊ�㼴��̬����
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//Ϊ���������˶���̬��Ϣ
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);
	//��ӵ�������
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);
	body->getCollisionShape()->setUserIndex(bt_Enemy);
	bodies.push_back(body);

	return body;
}
btRigidBody* myBulletEngine::addBullet(float radius, float x, float y, float z, float mass)
{
	//��ʼ��λ����Ϣ
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//������״
	btSphereShape* sphere = new btSphereShape(radius);
	btVector3 inertia(0, 0, 0);
	//����Ϊ�㼴��̬����
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//Ϊ���������˶���̬��Ϣ
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);

	//��ӵ�������
	body->setRestitution(btScalar(0.5));

	body->getCollisionShape()->setUserIndex(bt_Bullet);

	world->addRigidBody(body);

	bodies.push_back(body);

	return body;
}


void myBulletEngine::renderPlayer(Shader& shader, Camera& camera)
{
	//������Ⱦ����
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

void myBulletEngine::renderMyMap(Model& model, Shader& shader) {
	for (int i = 0; i < my_map_obj.size(); i++) {
		btTransform t;
		float mat[16];
		t.getOpenGLMatrix(mat);
		my_map_obj[i]->getMotionState()->getWorldTransform(t);

	}

}



void myBulletEngine::movePlayer(Direction direction, float deltaTime) {
	if (world)
	{
		
		btScalar walkVelocity = btScalar(1.1) * 8.0; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * deltaTime * 2.0f;
		btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);

		//��ȡ������������,���ҵ�λ��
		btVector3 forwardDir, rightDir;
		forwardDir[0] = camera.Move.x;
		forwardDir[1] = camera.Move.y;
		forwardDir[2] = camera.Move.z;
		rightDir[0] = camera.Right.x;
		rightDir[1] = camera.Right.y;
		rightDir[2] = camera.Right.z;
		//cout << forwardDir[0]<< "  " << forwardDir[1]<< "  "<<forwardDir[2]<< endl;


		//����ǰ������,�Լ���ת����,
		if (direction == _RIGHT)
		{
			walkDirection = rightDir;
			//���շ����ƶ���ɫ
		}

		if (direction == _LEFT)
		{
			walkDirection = -rightDir;
			//���շ����ƶ���ɫ

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
			walkSpeed *= 2;
			//���շ����ƶ���ɫ
		}
		if (direction == _JUMP && m_character && m_character->canJump())
		{
			m_character->jump();
			//���շ����ƶ���ɫ

		}
		//cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
		m_character->setWalkDirection(walkDirection * walkSpeed);


	}
}
void myBulletEngine::movePlayer(Direction direction1, Direction direction2, float deltaTime) {
	if (world)
	{
		btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
		btScalar walkVelocity = btScalar(1.1) * 8.0 * 10; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * deltaTime * 2.0f;

		//��ȡ������������,���ҵ�λ��
		btVector3 forwardDir;
		btVector3 rightwardDir;
		forwardDir[0] = camera.Move.x;
		forwardDir[1] = camera.Move.y;
		forwardDir[2] = camera.Move.z;
		rightwardDir[0] = camera.Right.x;
		rightwardDir[1] = camera.Right.y;
		rightwardDir[2] = camera.Right.z;
		//cout << forwardDir[0] << "  " << forwardDir[1] << "  "<<forwardDir[2] << endl;


		//����ǰ������,�Լ���ת����,
		if (direction1 == _RIGHT)
		{
			if (direction2 == _FORWARD)
			{
				walkDirection = (forwardDir + rightwardDir).normalized();
				//���շ����ƶ���ɫ
			}
			else if (direction2 == _BACKWARD)
				walkDirection = (-forwardDir + rightwardDir).normalized();

		}

		if (direction1 == _LEFT)
		{
			if (direction2 == _FORWARD)
			{
				walkDirection = (forwardDir - rightwardDir).normalized();
				//���շ����ƶ���ɫ
			}
			else if (direction2 == _BACKWARD)
				walkDirection = (-forwardDir - rightwardDir).normalized();

		}

		//cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
		m_character->setWalkDirection(walkDirection * walkSpeed);




	}
}


void myBulletEngine::colisionDetect(glm::vec3 look, Camera camera){
	double scalar = 1000;
	glm::vec3 look_nor = glm::normalize(look);
	// �ӳ�ָ������
	glm::vec3 look_ahead = glm::vec3(look_nor.x * scalar, look_nor.y * scalar, look_nor.z * scalar);
	glm::vec3 look_dest = camera.Position + look_ahead;

	std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;

	btVector3 btFrom(camera.Position.x, camera.Position.y, camera.Position.z);
	btVector3 btTo(look_dest.x, look_dest.y, look_dest.z);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	world->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

	if (res.hasHit()) {
		///res.m_collisionObject->getCollisionShape()->getName
		int get_num = res.m_collisionObject->getCollisionShape()->getUserIndex();
		switch (get_num)
		{
		case bt_Enemy:
			printf("Collision bt_Enemy at: <%.2f, %.2f, %.2f>\n", res.m_hitPointWorld.getX(), res.m_hitPointWorld.getY(), res.m_hitPointWorld.getZ());
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