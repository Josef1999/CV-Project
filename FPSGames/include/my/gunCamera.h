#ifndef CAR_H
#define CAR_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <queue>

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

class GunCamera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Left;
	float Yaw;
	float Pitch;

	float MovementSpeed;
	float TurningSpeed;


	GunCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		: MovementSpeed(30.0f)
		, TurningSpeed(90.0f)
		, Yaw(0.0f)
		, Pitch(0.0f)
	{
		Position = position;
		updateFront();
		updateLeft();
	}

	glm::vec3 getPosition()
	{
		return Position;
	}

	float getYaw()
	{
		return Yaw;
	}
	float getPitch()
	{
		return Pitch;
	}




	// 计算视图矩阵
	glm::mat4 GetViewMatrix(glm::vec3 cameraPosition)
	{
		return glm::lookAt(cameraPosition, Position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// 接受键盘输入
	void ProcessKeyboard(Direction direction, float deltaTime)
	{
		if (direction == _HIGHSPEED)
			Position += Front * MovementSpeed * deltaTime *3.0f;
		if (direction == _FORWARD)
			Position += Front * MovementSpeed * deltaTime;
		if (direction == _BACKWARD)
			Position -= Front * MovementSpeed * deltaTime;
		if (direction == _LEFT)
			Position += Left * MovementSpeed * deltaTime;
		if (direction == _RIGHT)
			Position -= Left * MovementSpeed * deltaTime;
		updateFront();
		updateLeft();
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
		const float max_Pitch = 60.0f;
		const float min_Pitch = -60.0f;
		if (Pitch > max_Pitch)
			Pitch = max_Pitch;
		if (Pitch < min_Pitch)
			Pitch = min_Pitch;

		updateFront();
		updateLeft();
	}
private:
	// 计算新的 Front 向量
	void updateFront()
	{
		glm::vec3 front;
		front.x = -sin(glm::radians(getYaw()));
		front.y = 0.0f;
		front.z = -cos(glm::radians(getYaw()));
		Front = glm::normalize(front);
	}
	void updateLeft()
	{
		glm::vec3 left;
		glm::vec3 up(0, 1, 0);
		left = glm::cross(up, Front);
		Left = glm::normalize(left);
	}
};

#endif