#include "control.h"
#include "universal.h"

// ---------------------------------
// 键盘/鼠标监听
// ---------------------------------
extern myBulletEngine my_bt;

void handleKeyInput(GLFWwindow* window)
{
    // esc退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    //角色
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        my_bt.movePlayer(_STOP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        my_bt.movePlayer(_JUMP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        SpeedAdjust = 0.5f;
        BodyEvent = GLFW_KEY_LEFT_CONTROL;
        Bodycount++;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        BodyEvent = 0xff;
        SpeedAdjust = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        MouseEvent = GLFW_KEY_R;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        MouseEvent = 0xff;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            my_bt.movePlayer(_LEFT, _HIGHSPEED, deltaTime);
            MouseEvent = GLFW_KEY_LEFT_SHIFT;
            Movecount++;
        }
        else
        {
            my_bt.movePlayer(_LEFT, _FORWARD, deltaTime);
            Movecount=0;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
                MouseEvent = 0xff;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            my_bt.movePlayer(_RIGHT, _HIGHSPEED, deltaTime);
            MouseEvent = GLFW_KEY_LEFT_SHIFT;
            Movecount++;
        }
        else
        {
            my_bt.movePlayer(_RIGHT, _FORWARD, deltaTime);
            Movecount = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
                MouseEvent = 0xff;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        my_bt.movePlayer(_LEFT, _BACKWARD, deltaTime);
        //MouseEvent = 0xff;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        my_bt.movePlayer(_RIGHT, _BACKWARD, deltaTime);
        //MouseEvent = 0xff;
    }
    else
    {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                my_bt.movePlayer(_HIGHSPEED, deltaTime);
                MouseEvent = GLFW_KEY_LEFT_SHIFT;
                Movecount++;
            }
            else
            {
                my_bt.movePlayer(_FORWARD, deltaTime);
                Movecount = 0;
            }
            //停止冲刺
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
                if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
                {
                    MouseEvent = 0xff;
                    Movecount = 0;
                }


        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
            {
                MouseEvent = 0xff;
                Movecount = 0; 
            }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            my_bt.movePlayer(_BACKWARD, deltaTime);
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
            {
                MouseEvent = 0xff;
                Movecount = 0;
            }
                
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            my_bt.movePlayer(_LEFT, deltaTime);
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
            {
                MouseEvent = 0xff;
                Movecount = 0;
            }

        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            my_bt.movePlayer(_RIGHT, deltaTime);
            if (MouseEvent == GLFW_KEY_LEFT_SHIFT)
            {
                MouseEvent = 0xff;
                Movecount = 0;
            }
        }        
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && MouseEvent != GLFW_KEY_LEFT_SHIFT && AimCount == 0)
    {
        AimCount = 1;
        ReturnHip = 1;
        MouseEvent = GLFW_MOUSE_BUTTON_RIGHT;
        std::cout << "Pressed MOUSE RIGHT BUTTON" << std::endl;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && MouseEvent != GLFW_KEY_LEFT_SHIFT&& Shootcount==0)
    {
        Shootcount = 1; 
        MouseEvent = GLFW_MOUSE_BUTTON_LEFT;
        std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
        glm::vec3 look = camera.Front;
        std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;
        my_bt.colisionDetect(look, camera);
        btRigidBody* sphere = my_bt.addSphere(1.0, camera.Position.x + look.x * 3, camera.Position.y + look.y * 3, camera.Position.z + look.z * 3, 1.0);
        sphere->setLinearVelocity(btVector3(200 * look.x, 200 * look.y, 200 * look.z));
    }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && MouseEvent != GLFW_KEY_LEFT_SHIFT&& Shootcount==1)
    {
        MouseEvent = 0xff;
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && MouseEvent== GLFW_MOUSE_BUTTON_LEFT)
    {
        MouseEvent = 0xff;
    }
    //std::cout << Shootcount << std::endl;

    // 回调监听按键（一个按键只会触发一次事件）
    glfwSetKeyCallback(window, key_callback);
    //glfwSetMouseButtonCallback(window, mouseButton_callback);
}

// 按键回调函数，使得一次按键只触发一次事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //预留给换弹
    //if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    //    std::cout << "Pressed R" << std::endl;
        //MouseEvent = GLFW_KEY_R;
   //     ;
    //}

    //else
        //MouseEvent =0xff;
    
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
    //std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
//    if ((action == GLFW_PRESS)&& MouseEvent!= GLFW_KEY_LEFT_SHIFT)
//    {
//        
//        switch (button)
//        {
//        case GLFW_MOUSE_BUTTON_LEFT:
//            MouseEvent = button;
//            //射击
//#ifdef DEBUG_MODE
//            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
//            glm::vec3 look = camera.Front;
//            std::cout << "camera.Front: " << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << std::endl;
//            my_bt.colisionDetect(look, camera);
//            btRigidBody* sphere = my_bt.addSphere(1.0, camera.Position.x + look.x, camera.Position.y + look.y, camera.Position.z + look.z, 1.0);
//            sphere->setLinearVelocity(btVector3(200 * look.x, 200 * look.y, 200 * look.z));
//#endif
//            break;
//        }
//    }
//    else 
//        MouseEvent = 0xff;
//    return;
}


// 改变窗口大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保窗口匹配的新窗口尺寸
    glViewport(0, 0, width, height);
}