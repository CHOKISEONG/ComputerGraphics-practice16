#include "PolygonShape.h"

//PolygonShape::PolygonShape() // 기본은 정육각형
//{
//	
//	slope = 0.0f;
//	r = 1.0f;
//	polygonType = PolygonType::POLYGON;
//	moved = 0.0f;
//	std::fill(drawingIdx, drawingIdx + 12, true);
//
//	setMidpoint(0.0f, 0.0f);
//	setRegularHexagon();
//	initBuffer();
//}

PolygonShape::PolygonShape(PolygonType type, const float* f)
{
	std::fill(drawingIdx, drawingIdx + 12, true);
	slope = 0.0f;
	r = 1.0f;
	if (type == PolygonType::LINE)
	{
		index = { 0,1 };

		polygonType = PolygonType::LINE;


		setMidpoint(0.0f, 0.0f);
		add(f[0], f[1], f[2]);
		add(f[3], f[4], f[5]);
	}
	else if (type == PolygonType::TRIANGLE)
	{
		index = { 0,1,2 };
		polygonType = PolygonType::TRIANGLE;

		add(f[0], f[1], f[2]);
		add(f[3], f[4], f[5]);
		add(f[6], f[7], f[8]);
	}
	else if (type == PolygonType::RECTSHAPE)
	{
		index = { 0,1,2,0,2,3 };
		polygonType = PolygonType::RECTSHAPE;

		setMidpoint(0.0f, 0.0f);
		add(f[0], f[1], f[2]);
		add(f[3], f[4], f[5]);
		add(f[6], f[7], f[8]);
		add(f[9], f[10], f[11]);
	}
	else if (type == PolygonType::SQUAREPYRAMID) // 사각뿔
	{
		index = { 0,2,1,0,2,1,0,3,2,0,3,2,0,4,3,0,4,3,0,1,4,0,1,4,1,2,4,2,3,4 };
		polygonType = PolygonType::SQUAREPYRAMID;

		std::fill(drawingIdx, drawingIdx + 12, true);

		setMidpoint(0.0f, 0.0f);
		for (int i{}; i < 15; i += 3)
		{
			add(f[0 + i], f[1 + i], f[2 + i]);
		}
	}
	else if (type == PolygonType::CUBE)
	{
		index =
		{
			0, 3, 7, // 4
			0, 7, 4,
			4, 7, 6, // 5
			4, 6, 5,
			7, 2, 6, // 6
			7, 3, 2,
			0, 1, 2, // 1
			0, 2, 3,
			1, 5, 6, // 2
			1, 6, 2,
			4, 5, 1, // 3
			4, 1, 0,
		};
		polygonType = CUBE;
		setMidpoint(0.0f, 0.0f);
		for (int i{}; i < 24; i += 3)
		{
			add(f[0 + i], f[1 + i], f[2 + i]);
		}
	}
	initBuffer();
}

PolygonShape::PolygonShape(std::vector<float>& v) {
	positions = v;
	initBuffer();
}

PolygonShape::PolygonShape(const PolygonShape& other)
	: vao(other.vao)
	, ebo(other.ebo)
	, positions(other.positions) // std::vector는 깊은 복사됨
	, index(other.index)
	, isMoving(other.isMoving)
	, moveSpeed(other.moveSpeed)
	, slope(other.slope)
	, distance(other.distance)
	, moved(other.moved)
	, r(other.r)
	, polygonType(other.polygonType)
{
	midpoint[0] = other.midpoint[0]; midpoint[1] = other.midpoint[1];
	initBuffer();
}

PolygonShape::~PolygonShape()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(1, &ebo);
}

void PolygonShape::initBuffer() {
	if (!vao) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbo);
		glGenBuffers(1, &ebo);
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void PolygonShape::updateVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
}

void PolygonShape::setColor(const float r, const float g, const float b)
{
	for (int i{}; i < positions.size() / 3; ++i)
	{
		colors[0 + (i * 3)] = r;
		colors[1 + (i * 3)] = g;
		colors[2 + (i * 3)] = b;
	}
	updateVbo();
}

void PolygonShape::rotate()
{
	glm::vec3 pivot = glm::vec3(midpoint[0], midpoint[1], 0.0f);
	glm::mat4 rotMatrix(1.0f);
	
	if (rotateDir == 1) // x축기준 양
	{
		rotMatrix = glm::translate(rotMatrix, pivot);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(moveSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		rotMatrix = glm::translate(rotMatrix, -pivot);
	}
	else if (rotateDir == 2) // x축기준 음
	{
		rotMatrix = glm::translate(rotMatrix, pivot);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(-moveSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		rotMatrix = glm::translate(rotMatrix, -pivot);
	}
	else if (rotateDir == 3) // y축기준 양
	{
		rotMatrix = glm::translate(rotMatrix, pivot);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(moveSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
		rotMatrix = glm::translate(rotMatrix, -pivot);
	}
	else if (rotateDir == 4) // y축기준 음
	{
		rotMatrix = glm::translate(rotMatrix, pivot);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(-moveSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
		rotMatrix = glm::translate(rotMatrix, -pivot);
	}

	for (size_t i = 0; i < positions.size() / 3; ++i)
	{
		glm::vec4 tmp(
			positions[i * 3 + 0],
			positions[i * 3 + 1],
			positions[i * 3 + 2],
			1.0f
		);

		tmp = rotMatrix * tmp;

		positions[i * 3 + 0] = tmp.x;
		positions[i * 3 + 1] = tmp.y;
		positions[i * 3 + 2] = tmp.z;
	}
	updateVbo();
}

void PolygonShape::setMidpoint(const float x, const float y)
{
	midpoint[0] = x;
	midpoint[1] = y;

	updateVbo();
}

void PolygonShape::rotate2()
{
	if (rotateType == 0) return;
	else if (rotateType == 1) rotateT();
	else if (rotateType == 2) rotateF();
	else if (rotateType == 3) rotateS();
	else if (rotateType == 4) zoomInOut();
	else if (rotateType == 5) rotateO();
}

void PolygonShape::rotateT()
{
	glm::vec3 pivot(0.0f, 0.3f, 0.0f);

	for (size_t i = 0; i < positions.size() / 3; ++i)
	{
		glm::vec4 tmp(
			positions[i * 3 + 0],
			positions[i * 3 + 1],
			positions[i * 3 + 2],
			1.0f
		);
		glm::mat4 rotMatrix(1.0f);

		rotMatrix = glm::translate(rotMatrix, -pivot);
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(moveSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		rotMatrix = glm::translate(rotMatrix, pivot);
		tmp = rotMatrix * tmp;

		positions[i * 3 + 0] = tmp.x;
		positions[i * 3 + 1] = tmp.y;
		positions[i * 3 + 2] = tmp.z;
	}

	updateVbo();
}

void PolygonShape::rotateF()
{
	glm::vec3 pivot(0.0f, 0.3f, 0.3f);

	for (size_t i = 0; i < positions.size() / 3; ++i)
	{
		glm::vec4 tmp(
			positions[i * 3 + 0],
			positions[i * 3 + 1],
			positions[i * 3 + 2],
			1.0f
		);
		glm::mat4 rotMatrix(1.0f);

		rotMatrix = glm::translate(rotMatrix, -pivot);
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		std::cout << moveSpeed << "\n";
		moved += moveSpeed;
		if (moved > 150 * PI)
		{
			moveSpeed = -moveSpeed;
			moved = 150 * PI;
		}
		if (moved < 0.0f)
		{
			moveSpeed = -moveSpeed;
			moved = 0.0f;
		}
		rotMatrix = glm::rotate(rotMatrix, glm::radians(-moveSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		rotMatrix = glm::translate(rotMatrix, pivot);
		tmp = rotMatrix * tmp;

		positions[i * 3 + 0] = tmp.x;
		positions[i * 3 + 1] = tmp.y;
		positions[i * 3 + 2] = tmp.z;
	}

	updateVbo();
}

void PolygonShape::rotateS()
{
	for (size_t i = 0; i < positions.size() / 3; ++i)
	{
		glm::vec4 tmp(
			positions[i * 3 + 0],
			positions[i * 3 + 1],
			positions[i * 3 + 2],
			1.0f
		);
		glm::mat4 rotMatrix(1.0f);

		rotMatrix = glm::rotate(rotMatrix, glm::radians(-moveSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = rotMatrix * tmp;

		positions[i * 3 + 0] = tmp.x;
		positions[i * 3 + 1] = tmp.y;
		positions[i * 3 + 2] = tmp.z;
	}

	updateVbo();
}

void PolygonShape::zoomInOut()
{
	glm::vec3 pivot(0.0f, 0.0f, -0.3f);

	for (size_t i = 0; i < positions.size() / 3; ++i)
	{
		glm::vec4 tmp(
			positions[i * 3 + 0],
			positions[i * 3 + 1],
			positions[i * 3 + 2],
			1.0f
		);
		glm::mat4 rotMatrix(1.0f);

		rotMatrix = glm::translate(rotMatrix, -pivot);
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		std::cout << moveSpeed << "\n";
		moved += moveSpeed;
		if (moved > 200 * PI)
		{
			moveSpeed = -moveSpeed;
			moved = 200 * PI;
		}
		if (moved < 0.0f)
		{
			moveSpeed = -moveSpeed;
			moved = 0.0f;
		}
		rotMatrix = glm::scale(rotMatrix, glm::vec3(1.0f - moveSpeed/50, 1.0f - moveSpeed / 50, 0.0f));
		tmp = rotMatrix * tmp;

		rotMatrix = glm::mat4(1.0f);
		rotMatrix = glm::translate(rotMatrix, pivot);
		tmp = rotMatrix * tmp;

		positions[i * 3 + 0] = tmp.x;
		positions[i * 3 + 1] = tmp.y;
		positions[i * 3 + 2] = tmp.z;
	}

	updateVbo();
}

void PolygonShape::rotateO()
{ 
	glm::vec3 pivot;
	glm::vec3 axis;

	if (shapeNum == 0)
	{
		pivot = glm::vec3(0.2f, 0.0f, 0.0f);
		axis = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (shapeNum == 1)
	{
		pivot = glm::vec3(0.0f, 0.0f, -0.2f);
		axis = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (shapeNum == 2)
	{
		pivot = glm::vec3(-0.2f, 0.0f, 0.0f);
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (shapeNum == 3)
	{
		pivot = glm::vec3(0.0f, 0.0f, 0.2f);
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec4 tmp(
		positions[0],
		positions[1],
		positions[2],
		1.0f
	);
	glm::mat4 rotMatrix(1.0f);

	rotMatrix = glm::translate(rotMatrix, -pivot);
	tmp = rotMatrix * tmp;

	rotMatrix = glm::mat4(1.0f);
	std::cout << moveSpeed << "\n";
	moved += moveSpeed;
	if (moved > 70 * PI)
	{
		moveSpeed = -moveSpeed;
		moved = 70 * PI;
	}
	if (moved < 0.0f)
	{
		moveSpeed = -moveSpeed;
		moved = 0.0f;
	}

	rotMatrix = glm::rotate(rotMatrix, glm::radians(moveSpeed), axis);
	tmp = rotMatrix * tmp;

	rotMatrix = glm::mat4(1.0f);
	rotMatrix = glm::translate(rotMatrix, pivot);
	tmp = rotMatrix * tmp;

	positions[0] = tmp.x;
	positions[1] = tmp.y;
	positions[2] = tmp.z;

	updateVbo();
}

void PolygonShape::rotateR(int n)
{

	glm::vec3 pivot;
	glm::vec3 axis;

	if (n == 0)
	{
		pivot = glm::vec3(0.2f, 0.0f, 0.0f);
		axis = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (n == 1)
	{
		pivot = glm::vec3(0.0f, 0.0f, -0.2f);
		axis = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (n == 2)
	{
		pivot = glm::vec3(-0.2f, 0.0f, 0.0f);
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (n == 3)
	{
		pivot = glm::vec3(0.0f, 0.0f, 0.2f);
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec4 tmp(
		positions[0],
		positions[1],
		positions[2],
		1.0f
	);
	glm::mat4 rotMatrix(1.0f);

	rotMatrix = glm::translate(rotMatrix, -pivot);
	tmp = rotMatrix * tmp;

	rotMatrix = glm::mat4(1.0f);
	std::cout << moveSpeed << "\n";
	moved += moveSpeed;
	if (moved > 50 * PI)
	{
		moveSpeed = -moveSpeed;
		moved = 50 * PI;
	}
	if (moved < 0.0f)
	{
		moveSpeed = -moveSpeed;
		moved = 0.0f;
	}

	rotMatrix = glm::rotate(rotMatrix, glm::radians(moveSpeed), axis);
	tmp = rotMatrix * tmp;

	rotMatrix = glm::mat4(1.0f);
	rotMatrix = glm::translate(rotMatrix, pivot);
	tmp = rotMatrix * tmp;

	positions[0] = tmp.x;
	positions[1] = tmp.y;
	positions[2] = tmp.z;

	updateVbo();
}

void PolygonShape::add(const float x, const float y)
{
	positions.push_back(x);
	positions.push_back(y);
	positions.push_back(0.0f);
	float r = zeroToOne(gen);
	float g = zeroToOne(gen);
	float b = zeroToOne(gen);
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	updateVbo();
}

void PolygonShape::add(const float x, const float y, const float z)
{
	positions.push_back(x);
	positions.push_back(y);
	positions.push_back(z);
	float r = zeroToOne(gen);
	float g = zeroToOne(gen);
	float b = zeroToOne(gen);
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	updateVbo();
}

void PolygonShape::draw(GLuint shaderProgram) const
{
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);
	
	if (polygonType == PolygonType::LINE)
	{
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}
	else if (polygonType == PolygonType::RECTSHAPE)
	{
		for (int i = 0; i < 2; ++i) {
			if (drawingIdx[i])
			{
				if (drawType == 1)
				{
					//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
				}
				if (drawType == 2)
				{
					//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 12; ++i) {
			if (drawingIdx[i])
			{
				if (drawType == 1)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
				}
				if (drawType == 2)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
				}
			}
		}
	}
	glBindVertexArray(0);	
}

void PolygonShape::move(float x, float y)
{
	midpoint[0] += x;
	midpoint[1] += y;
	for (int i{}; i < static_cast<int>(positions.size()); i += 3)
	{
		positions[i] += x;
		positions[i + 1] += y;
	}

	updateVbo();
}

bool PolygonShape::isMouseInside(float x, float y) const
{
	if (polygonType == PolygonType::POINTTYPE)
	{
		if (abs(x - midpoint[0]) < r / 10 && abs(y - midpoint[1]) < r / 10)
			return true;
	}
	else if (polygonType == PolygonType::LINE)
	{
		float sx = positions[0] - positions[3];
		float sy = positions[1] - positions[4];
		float p[2]{ positions[3],positions[4] };

		for (int i{}; i < 20; ++i)
		{
			p[0] += 0.05f * sx;
			p[1] += 0.05f * sy;
			if (abs(x - p[0]) < 0.05f && abs(y - p[1]) < 0.05f)
				return true;
		}
	}
	else if (polygonType == PolygonType::TRIANGLE)
	{
		// 느슨한 바운딩 구 검사로 함
		if (sqrt(pow(x - midpoint[0], 2) + pow(y - midpoint[1], 2)) <= r)
			return true;
	}
	else if (polygonType == PolygonType::RECTSHAPE)
	{
		// 느슨한 바운딩 구 검사로 함
		if (sqrt(pow(x - midpoint[0], 2) + pow(y - midpoint[1], 2)) <= r)
			return true;
	}
	else if (polygonType == PolygonType::PENTAGON)
	{
		// 느슨한 바운딩 구 검사로 함
		if (sqrt(pow(x - midpoint[0], 2) + pow(y - midpoint[1], 2)) <= r)
			return true;
	}

	return false;
}