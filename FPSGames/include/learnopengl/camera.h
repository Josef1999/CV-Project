#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
using namespace std;
// 定义人物移动的几个可能的选项
enum Direction {
	_FORWARD,
	_BACKWARD,
	_LEFT,
	_RIGHT,
	_HIGHSPEED,
	_JUMP,
	_STOP
};

// 默认值
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 80.0f;

const float STADIA = 1000.0f;

const glm::vec3 WorldUp(0.0f,1.0f,0.0f);
class Camera {
private:
	
	
	
	
	glm::vec3 Up;		//相机上
	
	
	// 欧拉角
	float Yaw;		// 偏航角
	float Pitch;	// 倾斜角

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
public:
	glm::vec3 Position;	//相机位置
	glm::vec3 Front;	//相机看向的位置
	glm::vec3 Right;	//相机右
	glm::vec3 Move;		//人物移动的方向（Move.x=Front.x , Move.z=Front.z, Move.y=0)
	// 构造函数
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, MovementSpeed(SPEED)
		, MouseSensitivity(SENSITIVITY)
		, Zoom(ZOOM)
		, Move(Front)
		, Position(position)
		, Yaw(yaw)
		, Pitch(pitch)
	{
		updateCameraVectors();
	}

	
	
	// 计算视图矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// 计算投影矩阵
	glm::mat4 GetProjMatrix(float aspect)
	{
		return glm::perspective(glm::radians(Zoom), aspect, 0.1f, STADIA);
	}

	// 接受键盘输入
	void ProcessKeyboard(Direction direction, float deltaTime)
	{
		if (direction == _HIGHSPEED)
			Position += Move * MovementSpeed * deltaTime * 3.0f;
		if (direction == _FORWARD)
			Position += Move * MovementSpeed * deltaTime;
		if (direction == _BACKWARD)
			Position -= Move * MovementSpeed * deltaTime;
		if (direction == _LEFT)
			Position -= Right * MovementSpeed * deltaTime;
		if (direction == _RIGHT)
			Position += Right * MovementSpeed * deltaTime;
		updateCameraVectors();
	}

	// 接受鼠标移动
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		float MouseSensitivity = 0.1f;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw -= xoffset;
		Pitch += yoffset;

		// 限制角度以避免翻转问题
		const float max_Pitch = 80.0f;
		const float min_Pitch = -80.0f;
		if (Pitch > max_Pitch)
			Pitch = max_Pitch;
		if (Pitch < min_Pitch)
			Pitch = min_Pitch;

		// 使用欧拉角更新 Front、Up、Right 向量
		updateCameraVectors();
	}

	float getYaw()
	{
		return this->Yaw;
	}
	float getPitch()
	{
		return this->Pitch;
	}
	glm::vec3 getPosition()
	{
		return this->Position;
	}
	glm::vec3 getFront()
	{
		return this->Front;
	}
private:
	void updateCameraVectors()
	{
		Front.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front.y = sin(glm::radians(Pitch));
		Front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(Front);
		//cout << Front.x << "  " << Front.y << "  " << Front.z << endl;
		Move = glm::vec3(Front.x, 0, Front.z);
		Move = glm::normalize(Move);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif