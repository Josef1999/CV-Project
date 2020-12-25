#pragma once
/**bullet*/
#include <stdio.h>
#include "btBulletDynamicsCommon.h"
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>


class myBulletEngine {
public:
	/**bullet 初始化**/
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;

	std::vector<btRigidBody*> bodies;
	std::vector<btRigidBody*> enemies;
	std::vector<btRigidBody*> my_map_obj;

	btRigidBody* cube = NULL;
	btRigidBody* ground = NULL;

	//控制角色类
	btKinematicCharacterController* m_character;
	btPairCachingGhostObject* m_ghostObject;

	myBulletEngine();

	~myBulletEngine();
	void btInit();

	void btExit();

	void addGround();
	void addMap(Model& model);
	void addPlayer();
	btRigidBody* addSphere(float radius, float x, float y, float z, float mass);
	btRigidBody* addBullet(float radius, float x, float y, float z, float mass);

	btRigidBody* addEnemy(float radius, float x, float y, float z, float mass);

	void renderPlayer(Shader& shader, Camera& camera);
	void renderMyMap(Model& model, Shader& shader);
	void renderEnemy(Model& model, Shader& shader);


	void movePlayer(Direction direction, float deltaTime);
	void movePlayer(Direction direction1, Direction direction2, float deltaTime);

	void colisionDetect(glm::vec3 look, Camera camera);
};

void cubeInit(unsigned int& cubeVAO, unsigned int& cubeVBO);

