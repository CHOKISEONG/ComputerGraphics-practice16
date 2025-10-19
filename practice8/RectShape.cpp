#include "RectShape.h"
#include "global.h"

Rect::Rect()
{
	memset(verticles, 0, sizeof(verticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}
Rect::Rect(const float f1, const float f2)
{
	memset(verticles, 0, sizeof(verticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	verticles[0] = f1 + 0.05f;
	verticles[1] = f2 + 0.05f;

	verticles[3] = f1 + 0.05f;
	verticles[4] = f2 - 0.05f;

	verticles[6] = f1 - 0.05f;
	verticles[7] = f2 - 0.05f;

	verticles[9] = f1 - 0.05f;
	verticles[10] = f2 + 0.05f;

	initBuffer();
}
Rect::Rect(const float* vertices) {
	memcpy(verticles, vertices, sizeof(verticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Rect::Rect(const Rect& other)
{
	memcpy(verticles, other.verticles, sizeof(verticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Rect::~Rect()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Rect::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Rect::draw(GLuint shaderProgram) const
{
	glBindVertexArray(vao);
	GLint location = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(location, color[0], color[1], color[2], 1.0f);
	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Rect::move(float x, float y)
{
	verticles[0] += x;
	verticles[1] += y;
	verticles[3] += x;
	verticles[4] += y;
	verticles[6] += x;
	verticles[7] += y;
	verticles[9] += x;
	verticles[10] += y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

bool Rect::isMouseInside(float x, float y) const
{
	if (verticles[0] >= x && verticles[6] <= x && verticles[1] >= y && verticles[7] <= y)
		return true;
	return false;
}

