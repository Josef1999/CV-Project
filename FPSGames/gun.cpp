#include "gun.h"
#include "universal.h"
#include "GLFW/glfw3.h"
Gun::Gun(string const& ModelPath, int MaxAmmo, int Damage): _GunModel(ModelPath)
{
	_MaxAmmo = MaxAmmo;
	_Damage = Damage;
	_CurAmmo = 0;
    _Animation_status = Gun_Animation::Idle;
}

Gun::~Gun()
{
}

int Gun::GetCurAmmo()
{
	return _CurAmmo;
}

int Gun::GetMaxAmmo()
{
	return _MaxAmmo;
}

int Gun::GetDamage()
{
	return _Damage;
}

Model& Gun::GetModel()
{
	return _GunModel;
}
 
void Gun::GunRotate(glm::mat4& modelMatrix, const glm::vec3& Point, float degree)
{
    modelMatrix = glm::translate(modelMatrix, Point);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), WORLD_X);
    modelMatrix = glm::translate(modelMatrix, -Point);

}

void Gun::Display_HoldGun(Camera & camera, Shader& shader)
{
   
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera.getPosition());
    glm::vec3 look = camera.Front;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw()), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getPitch()), WORLD_X);
    // modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), look);
    // modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw() / 2), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), WORLD_UP);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.12f, 0.35f));

    
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));

    // GunRotate(modelMatrix, glm::vec3(0.0f, 0.0f, 10.0f), -(car.getPitch() - car.getDelayPitch() / 2));
    // 
    // Ó¦ÓÃ±ä»»¾ØÕó
    shader.setMat4("model", modelMatrix);

    _GunModel.Draw(shader);
}

void Gun::Display_FileGun(Camera& camera, Shader& shader)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera.getPosition());
    glm::vec3 look = camera.Front;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw()), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getPitch()), WORLD_X);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), WORLD_UP);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.12f, 0.35f));

    _Animation_Timer.StopTimer();
    float curTimer = _Animation_Timer.GetTimerSec();
    if (curTimer > 0.1)
    {
        curTimer = 0.1 ;
        Shootcount = 0;
        _Animation_status = Gun_Animation::Idle;
    }
    float Total_Timer = 0.1;
    float Z_Offset = 0.2;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0, - Z_Offset* curTimer / Total_Timer));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));

    shader.setMat4("model", modelMatrix);

    _GunModel.Draw(shader);
}

void Gun::Display_RunGun(Camera& camera, Shader& shader)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera.getPosition());
    glm::vec3 look = camera.Front;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw()), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getPitch()), WORLD_X);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), WORLD_UP);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f+15.0f), WORLD_X);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.12f, 0.35f));
    _Animation_Timer.StopTimer();
    float curTimer = _Animation_Timer.GetTimerSec();
    if (curTimer > 0.2)
    {
        curTimer = 0.2;
        Shootcount = 0;
        _Animation_status = Gun_Animation::StopRun;
    }
    float Total_Timer = 0.2;
    float X_Offset = 80.0f;
    float Y_Offset = 20.0f;
    float Move = -0.1f;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-Move *5* curTimer / Total_Timer, Move *2* curTimer / Total_Timer, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(curTimer/ Total_Timer*X_Offset), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(curTimer / Total_Timer * Y_Offset), WORLD_X);
    
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));

    shader.setMat4("model", modelMatrix);

    _GunModel.Draw(shader);
}

void Gun::Display_StopRun(Camera& camera, Shader& shader)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera.getPosition());
    glm::vec3 look = camera.Front;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw()), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getPitch()), WORLD_X);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), WORLD_UP);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f+15.0f), WORLD_X);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.12f, 0.35f));

    _Animation_Timer.StopTimer();
    float curTimer = _Animation_Timer.GetTimerSec();
    if (curTimer > 0.2)
    {
        curTimer = 0.2;
        _Animation_status = Gun_Animation::Idle;
    }
    float Total_Timer = 0.2;
    float X_Offset = 80.0f;
    float Y_Offset = 20.0f;
    float Move = -0.1f;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, -0.2f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(80.0f), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), WORLD_X);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(-curTimer / Total_Timer * Y_Offset), WORLD_X);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-curTimer / Total_Timer * X_Offset), WORLD_UP);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(+Move * 5 * curTimer / Total_Timer, -Move * 2 * curTimer / Total_Timer, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));

    shader.setMat4("model", modelMatrix);

    _GunModel.Draw(shader);
}

void Gun::Display(Camera& camera, Shader& shader)
{
    if (MouseEvent == 0xff)
    {
        if (_Animation_status == Gun_Animation::Idle)
            Display_HoldGun(camera, shader);
        if (_Animation_status == Gun_Animation::Fire)
            Display_FileGun(camera, shader);
        if (_Animation_status == Gun_Animation::Run)
            Display_RunGun(camera, shader);
        if (_Animation_status == Gun_Animation::StopRun)
        {
            if (_Stop == 0)
            {
                _Animation_Timer.StartTimer();
                _Stop = 1;
            }
            Display_StopRun(camera, shader);
        }
    }
    
    switch (MouseEvent)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        _Animation_status = Gun_Animation::Fire;
        _Animation_Timer.StartTimer();
        Display_FileGun(camera, shader);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        _Stop = 0;
        _Animation_status = Gun_Animation::Run;
        if (Movecount == 1)
            _Animation_Timer.StartTimer();
        Display_RunGun(camera, shader);
        break;
    case GLFW_KEY_R:
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        break;
    default:
        break;
    }
}

