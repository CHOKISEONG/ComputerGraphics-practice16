#pragma once

#include "Shape.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Line : public ShapeManager
{
	GLuint vao, vbo;
	float verticles[6];
	GLfloat color[3];

public:
	Line();
	Line(const float f1, const float f2);
	Line(const float f1, const float f2, const float f3, const float f4);
	Line(const float* v);
	Line(const Line& other);
	~Line();
	void initBuffer();

	virtual void draw(GLuint shaderProgram) const override;
	virtual void move(float x, float y) override;
	virtual bool isMouseInside(float x, float y) const override;
};