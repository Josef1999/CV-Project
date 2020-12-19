#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
using namespace std;
// ���������ƶ��ļ������ܵ�ѡ��
enum Direction {
	_FORWARD,
	_BACKWARD,
	_LEFT,
	_RIGHT,
	_HIGHSPEED,
	_JUMP,
	_STOP
};

// Ĭ��ֵ
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 80.0f;

const float STADIA = 1000.0f;

const glm::vec3 WorldUp(0.0f,1.0f,0.0f);
class Camera {
private:
	
	
	
	
	glm::vec3 Up;		//�����
	
	
	// ŷ����
	float Yaw;		// ƫ����
	float Pitch;	// ��б��

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
public:
	glm::vec3 Position;	//���λ��
	glm::vec3 Front;	//��������λ��
	glm::vec3 Right;	//�����
	glm::vec3 Move;		//�����ƶ��ķ���Move.x=Front.x , Move.z=Front.z, Move.y=0)
	// ���캯��
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

	
	
	// ������ͼ����
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// ����ͶӰ����
	glm::mat4 GetProjMatrix(float aspect)
	{
		return glm::perspective(glm::radians(Zoom), aspect, 0.1f, STADIA);
	}

	// ���ܼ�������
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

	// ��������ƶ�
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		float MouseSensitivity = 0.1f;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw -= xoffset;
		Pitch += yoffset;

		// ���ƽǶ��Ա��ⷭת����
		const float max_Pitch = 80.0f;
		const float min_Pitch = -80.0f;
		if (Pitch > max_Pitch)
			Pitch = max_Pitch;
		if (Pitch < min_Pitch)
			Pitch = min_Pitch;

		// ʹ��ŷ���Ǹ��� Front��Up��Right ����
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