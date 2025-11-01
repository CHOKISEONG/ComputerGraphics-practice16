#pragma once

#include "global.h"

class Camera
{
private:
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 5.0f);		// ��ġ
	glm::vec3 direction = glm::vec3(0.0f,0.0f,0.0f);	// �ٶ󺸴� ����
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);			// ������

	float fovy = 90.0f;		// ���� ����
	float width = 1200.0f;	// ��
	float height = 800.0f;	// ����
	float zNear = 0.1f;		// ����� Ŭ���� �������� �Ÿ�
	float zFar = 100.0f;	// ��	 Ŭ���� �������� �Ÿ�

	bool isRotating = false;
	float rotateSpeed = 0.0f;
public:
	void settingCamera(GLuint shaderProgram);

	void update();
	void changeWidthHeight(float w, float h) { width = w, height = h; }
	void move(float x, float y, float z = 0.0f) { pos.x += x; pos.y += y; pos.z += z; }

	// rotateX�� true�� x��ȸ��, false�� y��ȸ������ ��
	void rotate(float theta, bool rotateX); 
	void rotateFromView(float theta, bool rotateX);
	void rotateStartY(float speed) { isRotating = true; rotateSpeed = speed; }
	void rotateStop() { isRotating = false; rotateSpeed = 0.0f; }
};