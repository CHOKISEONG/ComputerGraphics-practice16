#include "TriShape.h"
#include "global.h"

Triangle::Triangle()
{
	memset(verticles, 0, sizeof(verticles));
	memcpy(orgVerticles, verticles, sizeof(orgVerticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Triangle::Triangle(const float f1, const float f2)
{
	memset(verticles, 0, sizeof(verticles));

	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	verticles[0] = f1;
	verticles[1] = f2 + 0.1f;

	verticles[3] = f1 - 0.05f;
	verticles[4] = f2 - 0.1f;

	verticles[6] = f1 + 0.05f;
	verticles[7] = f2 - 0.1f;
	memcpy(orgVerticles, verticles, sizeof(orgVerticles));

	initBuffer();
}

Triangle::Triangle(const float* vertices) {
	memcpy(verticles, vertices, sizeof(verticles));
	memcpy(orgVerticles, verticles, sizeof(orgVerticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Triangle::Triangle(const Triangle& other)
{
	memcpy(verticles, other.verticles, sizeof(verticles));
	memcpy(orgVerticles, verticles, sizeof(orgVerticles));
	color[0] = zeroToOne(gen); color[1] = zeroToOne(gen); color[2] = zeroToOne(gen);
	initBuffer();
}

Triangle::~Triangle()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Triangle::initBuffer()
{
	glGenVertexArrays(1, &vao); 
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Triangle::draw(GLuint shaderProgram) const
{
	glBindVertexArray(vao);
	GLint location = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(location, color[0], color[1], color[2], 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Triangle::move(float x, float y)
{
	verticles[0] += x;
	verticles[1] += y;
	verticles[3] += x;
	verticles[4] += y;
	verticles[6] += x;
	verticles[7] += y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verticles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// x나 y만 움직일 때 정상
	// 둘다 움직일 때 고치기
	moved += abs(x);
	moved += abs(y);
}

int Triangle::isTouchedWall()
{
	if (verticles[1] > 1.0f)
	{
		move(0.0f, -0.02f);
		return (int)WallDir::UP;
	}
	if (verticles[3] < -1.0f)
	{
		move(0.02f, 0.0f);
		return (int)WallDir::LEFT;
	}
	if (verticles[4] < -1.0f) 
	{
		move(0.0f, +0.02f);
		return (int)WallDir::DOWN;
	}
	if (verticles[6] > 1.0f)
	{
		move(-0.02f, 0.0f);
		return (int)WallDir::RIGHT;
	}
	return 0;
}

bool Triangle::isMouseInside(float x, float y) const
{
	float x1 = verticles[0], y1 = verticles[1];
	float x2 = verticles[3], y2 = verticles[4];
	float x3 = verticles[6], y3 = verticles[7];

	float d1 = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
	float d2 = (x - x2) * (y3 - y2) - (y - y2) * (x3 - x2);
	float d3 = (x - x3) * (y1 - y3) - (y - y3) * (x1 - x3);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

void Triangle::setDistance(float f)
{
	distance = f;
}

void Triangle::setSlope(float f)
{
	slope = f;
}

void Triangle::Teleport(float distance, float slope)
{
	verticles[0] = orgVerticles[0] + distance * cos(slope);  verticles[1] = orgVerticles[1] + distance * sin(slope);
	verticles[3] = orgVerticles[3] + distance * cos(slope);  verticles[4] = orgVerticles[4] + distance * sin(slope);
	verticles[6] = orgVerticles[6] + distance * cos(slope);  verticles[7] = orgVerticles[7] + distance * sin(slope);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verticles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::moveSpring()
{
	int tmp = isTouchedWall();
	if (tmp == WallDir::UP || tmp == WallDir::DOWN)
	{
		slope = 2 * PI - slope;
	}
	else if (tmp == WallDir::LEFT || tmp == WallDir::RIGHT)
	{
		slope = PI - slope;
	}
	move(0.005f * cos(slope), 0.005f * (sin(slope)));
}

void Triangle::moveZigZag()
{
	int tmp = isTouchedWall();
	if (tmp == WallDir::LEFT || tmp == WallDir::RIGHT)
	{
		slope = PI - slope;
		moved = 0.0f;
		distance = udis(gen);
	}
	if (moved <= distance)
		move(0.0f, -0.005f);
	else move(0.005f * cos(slope), 0.0f);
}

void Triangle::moveRectSpiral()
{
	if (distance <= 0.0f) return;

	if (isTouchedWall())
	{
		slope = slope + 90.0f;
		if (slope >= 360.0f) slope = 0.0f;
	}
	if (moved > distance)
	{
		moved = 0.0f;
		distance -= 0.02f;
		slope = slope + 90.0f;
		if (slope >= 360.0f) slope = 0.0f;
	}

	if (slope >= 270.0f)
	{
		move(0.0f, -0.005f);
	}
	else if (slope >= 180.0f)
	{
		move(-0.005f, 0.0f);
	}
	else if (slope >= 90.0f)
	{
		move(0.0f, 0.005f);
	}
	else if (slope >= 0.0f)
	{
		move(0.005f, 0.0f);
	}

	// 첫 방향은 0,90,180,270도 중 하나
	// 꼭짓점과 반대 꼭짓점을 이은 X자에 삼각형 중심이 닿으면  
	//중앙쪽으로 사각형 돌면서 내려가기
}

void Triangle::moveCircleSpiral()
{
	if (isTouchedWall())
		dir = !dir;

	if (dir)
	{
		distance -= 0.002f;
		slope -= 0.03f;
	}
	else
	{
		distance += 0.002f;
		slope += 0.03f;
	}

	Teleport(distance, slope);
}
