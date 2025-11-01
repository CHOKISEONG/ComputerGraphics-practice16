#pragma once

#include "global.h"

class Camera
{
private:
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 5.0f);			// ��ġ
	glm::vec3 direction = glm::vec3(0.0f,0.0f,0.0f);	// �ٶ󺸴� ����
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);			// ������

	float fovy = 45.0f;		// ���� ����
	float width = 1200.0f;	// ��
	float height = 800.0f;	// ����
	float zNear = 0.1f;		// ����� Ŭ���� �������� �Ÿ�
	float zFar = 100.0f;	// ��	 Ŭ���� �������� �Ÿ�

public:
	void settingCamera(GLuint shaderProgram);

	void changeWidthHeight(float w, float h) { width = w, height = h; }
	void camMove(float x, float y, float z) { pos.x += x; pos.y += y; pos.z += z; }
};