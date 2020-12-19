#include "control.h"
#include "universal.h"
// ---------------------------------
// 键盘/鼠标监听
// ---------------------------------
void movePlayer(Direction direction1, Direction direction2, float deltaTime);
void handleKeyInput(GLFWwindow* window)
{
    // esc退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    ////移动
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //{
    //    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    //        camera.ProcessKeyboard(_HIGHSPEED, deltaTime);
    //    else
    //        camera.ProcessKeyboard(_FORWARD, deltaTime);
    //}
    //   
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera.ProcessKeyboard(_LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera.ProcessKeyboard(_RIGHT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.ProcessKeyboard(_BACKWARD, deltaTime);

    //角色
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        movePlayer(_STOP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        movePlayer(_JUMP, deltaTime);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS&& glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movePlayer(_LEFT,_FORWARD,deltaTime);

    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movePlayer(_RIGHT, _FORWARD, deltaTime);
    
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movePlayer(_LEFT, _BACKWARD, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movePlayer(_RIGHT, _BACKWARD, deltaTime);
    else
    {
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            movePlayer(_FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            movePlayer(_HIGHSPEED, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            movePlayer(_BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            movePlayer(_LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            movePlayer(_RIGHT, deltaTime);
        
    }
    

    // 回调监听按键（一个按键只会触发一次事件）
    glfwSetKeyCallback(window, key_callback);
}

// 按键回调函数，使得一次按键只触发一次事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //预留给换弹
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
#ifdef DEBUG_MODE
        std::cout << "Pressed R" << std::endl;
#endif
        ;
    }

}

// 鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if (!isCameraFixed) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 坐标翻转以对应坐标系

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
//鼠标按键
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        MouseEvent = button;
        switch (button)
        {

        case GLFW_MOUSE_BUTTON_LEFT:
            //射击
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
            // 按空格发射小球
    //else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    //{
            glm::vec3 look = camera.Front;
            std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;
            btRigidBody* sphere = addSphere(1.0, camera.Position.x + look.x, camera.Position.y + look.y, camera.Position.z + look.z, 1.0);



            sphere->setLinearVelocity(btVector3(200 * look.x, 200 * look.y, 200 * look.z));
            //}
#endif
            break;
        }
    }
    MouseEvent = 0xff;
    return;
}

btRigidBody* addSphere(float radius, float x, float y, float z, float mass)
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

    bodies.push_back(body);

    return body;
}

void movePlayer(Direction direction, float deltaTime) {
    if (world)
    {
        btScalar walkVelocity = btScalar(1.1) * 8.0; // 4 km/h -> 1.1 m/s
        btScalar walkSpeed = walkVelocity * deltaTime * 2.0f;
        btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);

        //获取本地坐标向量,并且单位化
        btVector3 forwardDir,rightDir;
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
            //按照方向移动角色

        }

        if (direction == _BACKWARD)
        {
            walkDirection = -forwardDir;
            //按照方向移动角色
        }
        if (direction == _HIGHSPEED)
        {
            walkDirection = forwardDir;
            walkSpeed *= 2;
            //按照方向移动角色
        }
        if (direction == _JUMP && m_character && m_character->canJump())
        {
            m_character->jump();
            //按照方向移动角色
           
        }
        //cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
        m_character->setWalkDirection(walkDirection * walkSpeed);

        

    }
}
void movePlayer(Direction direction1, Direction direction2,float deltaTime) {
    if (world)
    {
        btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
        btScalar walkVelocity = btScalar(1.1) * 8.0 *10; // 4 km/h -> 1.1 m/s
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
                walkDirection = (forwardDir+ rightwardDir).normalized();
                //按照方向移动角色
            }
            else if(direction2 == _BACKWARD)
                walkDirection = (-forwardDir + rightwardDir).normalized();

        }

        if (direction1 == _LEFT)
        {
            if (direction2 == _FORWARD)
            {
                walkDirection = (forwardDir - rightwardDir).normalized();
                //按照方向移动角色
            }
            else if (direction2 == _BACKWARD)
                walkDirection = (-forwardDir - rightwardDir).normalized();

        }

        //cout << walkDirection[0] <<"  "<< walkDirection[1] <<"    "<< walkDirection [2]<<endl;
        m_character->setWalkDirection(walkDirection * walkSpeed);




    }
}