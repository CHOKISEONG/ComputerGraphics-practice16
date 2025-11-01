#pragma once

#include "global.h"

class Camera
{
private:
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 5.0f);			// 위치
	glm::vec3 direction = glm::vec3(0.0f,0.0f,0.0f);	// 바라보는 방향
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);			// 업벡터

	float fovy = 90.0f;		// 뷰잉 각도
	float width = 1200.0f;	// 폭
	float height = 800.0f;	// 높이
	float zNear = 0.1f;		// 가까운 클리핑 평면까지의 거리
	float zFar = 100.0f;	// 먼	 클리핑 평면까지의 거리

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
		// rotateX가 true면 x축회전, false면 y축회전으로 함
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