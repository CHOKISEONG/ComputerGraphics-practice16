#include "LineShape.h"
#include "global.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

Line::Line()
{
	memset(verticles, 0, sizeof(verticles));
	memset(color, 1, sizeof(color));
	//color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Line::Line(const float f1, const float f2)
{
	memset(verticles, 0, sizeof(verticles));
	//color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	verticles[0] = f1;
	verticles[1] = f2;
	verticles[3] = f1 + 0.0001f;
	verticles[4] = f2 + 0.0001f;
	initBuffer();
}

Line::Line(const float f1, const float f2, const float f3, const float f4)
{
	memset(verticles, 0, sizeof(verticles));
	//color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	color[0] = 1.0f, color[1] = 1.0f, color[2] = 1.0f;
	verticles[0] = f1;
	verticles[1] = f2;
	verticles[3] = f3;
	verticles[4] = f4;
	initBuffer();
}

Line::Line(const float* array) {
	memcpy(verticles, array, sizeof(verticles));
	//color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;
	memset(color, 1.0f, sizeof(color));
	initBuffer();
}

Line::Line(const Line& other)
{
	memcpy(verticles, other.verticles, sizeof(verticles));
	//color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	memset(color, 0, sizeof(color));
	initBuffer();
}

Line::~Line()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Line::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), verticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::draw(GLuint shaderProgram) const
{
	glPointSize(3.0f);

	glBindVertexArray(vao);
	GLint location = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(location, color[0], color[1], color[2], 1.0f);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void Line::move(float x, float y)
{
	verticles[0] += x;
	verticles[1] += y;
	verticles[3] += x;
	verticles[4] += y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), verticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

bool Line::isMouseInside(float x, float y) const
{
	// 선은 마우스가 안에 있는게 불가능하다고 판단
	return false;
}
