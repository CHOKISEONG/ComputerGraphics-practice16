#pragma once

#include "Shape.h"
#include "global.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Triangle : public ShapeManager
{
	GLuint vao, vbo;
	float verticles[9];
	float orgVerticles[9];
	GLfloat color[3];
	float speed = 0.0f; // �̵��� �ӵ�
	float slope = 0.0f; // �̵��� ����
	float distance = 0.5f;
	float moved = 0.0f; // �󸶳� �̵��ߴ���
	bool dir = false;

public:
	Triangle();
	Triangle(const float f1, const float f2);
	Triangle(const float* v);
	Triangle(const Triangle& other);
	~Triangle();
	void initBuffer();

	virtual void draw(GLuint shaderProgram) const override;
	virtual void move(float x, float y) override;
	virtual bool isMouseInside(float x, float y) const override;

	int isTouchedWall();
	void changeSpeed(float s) { speed = s; }
	void changeDistance(float dis) { distance = dis; }
	void changeSlope(float f) { slope = f; }
	void changeDir(bool d) { dir = d; }

	void setDistance(float f);
	void setSlope(float f);

	void Teleport(float distance, float slope);
	void moveSpring();
	void moveZigZag();
	void moveRectSpiral();
	void moveCircleSpiral();
};