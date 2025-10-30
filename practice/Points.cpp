#include "Points.h"
#include "global.h"

Points::Points(const float crx, const float cry)
{
	init();
	spiralOrgDis1[0] = crx + 0.1f;
	spiralOrgDis1[1] = cry;
	spiralOrgDis2[0] = crx - 0.1f;
	spiralOrgDis2[1] = cry;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Points::init() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Points::moveSpiral()
{
	if (distance >= 0.1013f)
		isLeft = false;

	if (isLeft)
	{
		if (isClockwise)
		{
			theta1 -= PI / 169;
			theta2 -= PI / 169;
		}
		else
		{
			theta1 += PI / 169;
			theta2 += PI / 169;
		}

		distance += 0.0001f;

		verticles.push_back(spiralOrgDis1[0] + distance * cos(theta1));
		verticles.push_back(spiralOrgDis1[1] + distance * sin(theta1));
		verticles.push_back(0.0f);

		verticles.push_back(spiralOrgDis2[0] + distance * cos(theta2));
		verticles.push_back(spiralOrgDis2[1] + distance * sin(theta2));
		verticles.push_back(0.0f);
	}
	else
	{
		if (verticles.size() > 0)
		{
			for (int i{}; i < 3; ++i)
			{
				verticles.erase(verticles.end() - 1);
			}
		}
		else
		{
			isLeft = true;
			distance = 0.0f;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Points::erase()
{
	verticles.clear();
}

void Points::draw(GLuint shaderProgram) const
{
	if (verticles.empty()) return;

	glBindVertexArray(vao);
	GLint location = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(location, color[0], color[1], color[2], 1.0f);
	glDrawArrays(GL_POINTS, 0, verticles.size() / 3);
	glBindVertexArray(0);
}

void Points::move(float x, float y, float z)
{
	for (size_t i = 0; i < verticles.size(); i += 3) {
		verticles[i] += x;
		verticles[i + 1] += y;
		verticles[i + 2] += z;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}