#pragma once

#include "utils.h"



/*
	_GunModel	ǹеģ��
	_CurAmmo	��ǰ��ҩ��
	_MaxAmmo	���ҩ��
	_Damage		�����˺�
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