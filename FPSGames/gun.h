#pragma once

#include "utils.h"



/*
	_GunModel	枪械模型
	_CurAmmo	当前弹药量
	_MaxAmmo	最大弹药量
	_Damage		武器伤害
*/
class Gun
{
private:
	Model _GunModel;
	int _CurAmmo, _MaxAmmo;
	int _Damage;

	void GunRotate(glm::mat4& modelMatrix, const glm::vec3& Point, float degree);
public:
	Gun() = delete;
	Gun(string const& ModelPath, int MaxAmmo, int Damage);
	~Gun();
	int GetCurAmmo();
	int GetMaxAmmo();
	int GetDamage();
	Model& GetModel();
	void Display_HoldGun(Camera& camera, Shader& shader, glm::mat4 modelMatrix);
};