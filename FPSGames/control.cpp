#include "control.h"
#include "universal.h"

// ---------------------------------
// ����/������
// ---------------------------------
extern myBulletEngine my_bt;

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
        my_bt.movePlayer(_STOP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        my_bt.movePlayer(_JUMP, deltaTime);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS&& glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        my_bt.movePlayer(_LEFT,_FORWARD,deltaTime);

    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        my_bt.movePlayer(_RIGHT, _FORWARD, deltaTime);
    
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        my_bt.movePlayer(_LEFT, _BACKWARD, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        my_bt.movePlayer(_RIGHT, _BACKWARD, deltaTime);
    else
    {
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            my_bt.movePlayer(_FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            my_bt.movePlayer(_HIGHSPEED, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            my_bt.movePlayer(_BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            my_bt.movePlayer(_LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            my_bt.movePlayer(_RIGHT, deltaTime);
        
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
    {
        switch (button)
        {

        case GLFW_MOUSE_BUTTON_LEFT:
            MouseEvent = button;
            //���
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
            // ���ո���С��
    //else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    //{
            glm::vec3 look = camera.Front;
            std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;
            my_bt.colisionDetect(look, camera);
            btRigidBody* sphere = my_bt.addSphere(1.0, camera.Position.x + look.x, camera.Position.y + look.y, camera.Position.z + look.z, 1.0);
            sphere->setLinearVelocity(btVector3(200 * look.x, 200 * look.y, 200 * look.z));

            //}
#endif
            break;
        }
    }
    else
        MouseEvent = 0xff;
    return;
}


// �ı䴰�ڴ�С�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ȷ������ƥ����´��ڳߴ�
    glViewport(0, 0, width, height);
}