#include"control.h"
// ---------------------------------
// ����/������
// ---------------------------------
void movePlayer(Direction direction1, Direction direction2, float deltaTime);
void handleKeyInput(GLFWwindow* window)
{
    // esc�˳�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    ////�ƶ�
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

    //��ɫ
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
    

    // �ص�����������һ������ֻ�ᴥ��һ���¼���
    glfwSetKeyCallback(window, key_callback);
}

// �����ص�������ʹ��һ�ΰ���ֻ����һ���¼�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Ԥ��������
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
#ifdef DEBUG_MODE
        std::cout << "Pressed R" << std::endl;
#endif
        ;
    }

}

// ����ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if (!isCameraFixed) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ���귭ת�Զ�Ӧ����ϵ

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
//��갴��
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //���
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
            // ���ո���С��
    //else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    //{
            glm::vec3 look = camera.Front;
            std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;
            btRigidBody* sphere = addSphere(1.0, camera.Position.x+ look.x, camera.Position.y+ look.y, camera.Position.z+ look.z, 1.0);

            

            sphere->setLinearVelocity(btVector3(200 * look.x, 200 * look.y, 200 * look.z));
            //}
#endif
            break;
        }
    return;
}

btRigidBody* addSphere(float radius, float x, float y, float z, float mass)
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

    bodies.push_back(body);

    return body;
}

void movePlayer(Direction direction, float deltaTime) {
    if (world)
    {
        btScalar walkVelocity = btScalar(1.1) * 8.0; // 4 km/h -> 1.1 m/s
        btScalar walkSpeed = walkVelocity * deltaTime * 2.0f;
        btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);

        //��ȡ������������,���ҵ�λ��
        btVector3 forwardDir,rightDir;
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
            //���շ����ƶ���ɫ

        }

        if (direction == _BACKWARD)
        {
            walkDirection = -forwardDir;
            //���շ����ƶ���ɫ
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
void movePlayer(Direction direction1, Direction direction2,float deltaTime) {
    if (world)
    {
        btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
        btScalar walkVelocity = btScalar(1.1) * 8.0 *10; // 4 km/h -> 1.1 m/s
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
                walkDirection = (forwardDir+ rightwardDir).normalized();
                //���շ����ƶ���ɫ
            }
            else if(direction2 == _BACKWARD)
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