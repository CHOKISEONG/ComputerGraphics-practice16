#pragma once

#include "global.h"

class Camera
{
private:
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 5.0f);		// 위치
	glm::vec3 direction = glm::vec3(0.0f,0.0f,0.0f);	// 바라보는 방향
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);			// 업벡터

	glm::vec3 orthoXZ_pos = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 orthoXZ_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 orthoXZ_up = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 orthoXY_pos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 orthoXY_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 orthoXY_up = glm::vec3(0.0f, 1.0f, 0.0f);

	float fovy = 90.0f;		// 뷰잉 각도
	float width = 1200.0f;	// 폭
	float height = 800.0f;	// 높이
	float zNear = 0.1f;		// 가까운 클리핑 평면까지의 거리
	float zFar = 100.0f;	// 먼	 클리핑 평면까지의 거리

	bool isRotating = false;
	bool isRotateSelf = false;
	float rotateSpeed = 0.0f;
public:
	Camera();
	Camera(Camera& other);
	Camera& operator=(const Camera& other);

	void settingCamera(GLuint shaderProgram, std::string str);
	void goBack(); // 처음 상태로 되돌아가기

	void update();
	void changeWidthHeight(float w, float h) { width = w, height = h; }
	void move(float x, float y, float z = 0.0f) 
	{ 
		pos += glm::vec3(x, y, z);
		orthoXZ_pos += glm::vec3(x, y, z);
		orthoXY_pos += glm::vec3(x, y, z);
	}

	// rotateX가 true면 x축회전, false면 y축회전으로 함
	void rotate(float theta); 
	void rotateFromView(float theta);
	void rotateStart(float speed) { isRotating = true; isRotateSelf = false; rotateSpeed = speed; }
	void rotateStartY(float speed) { isRotating = true; isRotateSelf = true; rotateSpeed = speed; }
	void rotateStop() { isRotating = false; rotateSpeed = 0.0f; }
};