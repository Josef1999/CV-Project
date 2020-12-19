#include"include/glad/glad.h"
#include "GLFW/glfw3.h"
#include"utils.h"
#include <stdio.h>
#include "btBulletDynamicsCommon.h"

#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

extern Camera camera;
extern float deltaTime;
extern float lastX, lastY;
extern bool firstMouse;

extern btDynamicsWorld* world;
extern std::vector<btRigidBody*> bodies;
extern btKinematicCharacterController* m_character;
extern btPairCachingGhostObject* m_ghostObject;

#define DEBUG_MODE
void handleKeyInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

btRigidBody* addSphere(float radius, float x, float y, float z, float mass);
void movePlayer(Direction direction, float deltaTime);
