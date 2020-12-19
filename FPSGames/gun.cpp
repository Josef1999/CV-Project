#include "gun.h"


Gun::Gun(string const& ModelPath, int MaxAmmo, int Damage): _GunModel(ModelPath)
{
	_MaxAmmo = MaxAmmo;
	_Damage = Damage;
	_CurAmmo = 0;
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
    // modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw() - camera.getYaw() / 2), WORLD_UP);
    // GunRotate(modelMatrix, glm::vec3(0.0f, 0.0f, 4.0f), (camera.getPitch() - camera.getPitch() / 2));

    // modelMatrix = glm::rotate(modelMatrix, glm::radians(-(car.getPitch() - car.getDelayPitch() / 2)), WORLD_X);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera.getPosition());
    glm::vec3 look = camera.Front;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw()), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getPitch()), WORLD_X);
    // modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), look);
    // modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.getYaw() / 2), WORLD_UP);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), WORLD_UP);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, -0.12f, 0.35f));

    
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));

    // GunRotate(modelMatrix, glm::vec3(0.0f, 0.0f, 10.0f), -(car.getPitch() - car.getDelayPitch() / 2));
    // 
    // ”¶”√±‰ªªæÿ’Û
    shader.setMat4("model", modelMatrix);

    _GunModel.Draw(shader);
}
