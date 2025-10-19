#pragma once

#include "Shape.h"
#include "global.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Points : public ShapeManager
{
	GLuint vao, vbo;
	std::vector<float> verticles;
	
	// 계속 색깔 똑같이 하게 했음. 점마다 다른 색깔은 필요하면 그때...
	GLfloat color[3]{ 0.5f,0.5f,0.5f };

	float distance = 0.0001f;

	float theta1 = PI; 
	float theta2 = 0.0f;

	float spiralOrgDis1[2];
	float spiralOrgDis2[2];

	bool isLeft = true;

	// false or true 정해서 어디방향으로 그릴지 결정
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