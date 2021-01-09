#pragma once

#include "utils.h"
#include "timer.h"

enum Gun_Animation
{
	Idle,
	Fire,
	Run,
	StopRun,
	Aim,
	StopAim,
	Reload
};

enum Idle_Mode
{
	Hip_Fire,
	ADS_Aim
};

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
	int _Stop;
	Timer _Animation_Timer;
	Gun_Animation _Animation_status;
	Idle_Mode _Idle_status;
	void GunRotate(glm::mat4& modelMatrix, const glm::vec3& Point, float degree);
public:
	Gun() = delete;
	Gun(string const& ModelPath, int MaxAmmo, int Damage);
	~Gun();
	int GetCurAmmo();
	int GetMaxAmmo();
	int GetDamage();
	Model& GetModel();
	void Display_HoldGun(Camera& camera, Shader& shader);
	void Display_FileGun(Camera& camera, Shader& shader);
	void Display_RunGun(Camera& camera, Shader& shader);
	void Display_StopRun(Camera& camera, Shader& shader);
	void Display_ReloadGun(Camera& camera, Shader& shader);
	void Display_AimGun(Camera& camera, Shader& shader);
	void Display(Camera& camera, Shader& shader);
};