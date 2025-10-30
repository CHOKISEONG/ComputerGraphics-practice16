#include "PolygonShape.h"

//PolygonShape::PolygonShape() // ±âº»Àº Á¤À°°¢Çü
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
	else if (type == PolygonType::SQUAREPYRAMID) // »ç°¢»Ô
	{
		index = { 1,2,4,2,3,4,
			0,4,3,0,4,3,
			0,1,4,0,1,4,
			0,2,1,0,2,1,
			0,3,2,0,3,2,
		};
		polygonType = PolygonType::SQUAREPYRAMID;

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
	, positions(other.positions) // std::vector´Â ±íÀº º¹»çµÊ
	, index(other.index)
	, moveSpeed(other.moveSpeed)
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

void PolygonShape::setMidpoint(const float x, const float y)
{
	midpoint[0] = x;
	midpoint[1] = y;

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
	else
	{
		for (int i = 0; i < index.size() / 3; ++i) {
			if (drawingIdx[i])
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
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
