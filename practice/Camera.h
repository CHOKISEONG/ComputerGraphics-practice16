#pragma once

#include "global.h"

class Camera
{
private:
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 5.0f);			// ��ġ
	glm::vec3 direction = glm::vec3(0.0f,0.0f,0.0f);	// �ٶ󺸴� ����
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);			// ������

	float fovy = 90.0f;		// ���� ����
	float width = 1200.0f;	// ��
	float height = 800.0f;	// ����
	float zNear = 0.1f;		// ����� Ŭ���� �������� �Ÿ�
	float zFar = 100.0f;	// ��	 Ŭ���� �������� �Ÿ�

public:
	void settingCamera(GLuint shaderProgram);

	void changeWidthHeight(float w, float h) { width = w, height = h; }
	void move(float x, float y, float z = 0.0f) 
	{
		pos.x += x; pos.y += y; pos.z += z;
		direction += x; direction.y += y; direction.z += z;
	}
	void rotate(float theta, bool rotateX)
	{
		float rad = glm::radians(theta);

		glm::vec3 axis;
		// rotateX�� true�� x��ȸ��, false�� y��ȸ������ ��
		if (rotateX)
			axis = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			axis = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 look = direction - pos;
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rad, axis); 
		look = glm::vec3(rotation * glm::vec4(look, 1.0f));
		direction = pos + look;
	}
};