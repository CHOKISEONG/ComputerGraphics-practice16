#pragma once

#include "Shape.h"
#include "global.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Points : public ShapeManager
{
	GLuint vao, vbo;
	std::vector<float> verticles;
	
	// ��� ���� �Ȱ��� �ϰ� ����. ������ �ٸ� ������ �ʿ��ϸ� �׶�...
	GLfloat color[3]{ 0.5f,0.5f,0.5f };

	float distance = 0.0001f;

	float theta1 = PI; 
	float theta2 = 0.0f;

	float spiralOrgDis1[2];
	float spiralOrgDis2[2];

	bool isLeft = true;

	// false or true ���ؼ� ���������� �׸��� ����
	bool isClockwise = zeroOrOne(gen);

public:
	Points(const float crx, const float cry);

	void init();
	void moveSpiral();
	int size() { return (int)verticles.size() / 3; }

	void erase();

	virtual void draw(GLuint shaderProgram) const override;
	virtual void move(float x, float y) override;
	virtual bool isMouseInside(float x, float y) const override {
		return false;
	};
};