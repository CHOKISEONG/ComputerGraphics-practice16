#include "PolygonShape.h"

PolygonShape::PolygonShape(PolygonType type, const float* f)
{
	setMidpoint(0.0f, 0.0f, 0.0f);
	std::fill(drawingIdx, drawingIdx + 12, true);
	if (type == PolygonType::LINE)
	{
		index = { 0,1 };

		polygonType = PolygonType::LINE;

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

		add(f[0], f[1], f[2]);
		add(f[3], f[4], f[5]);
		add(f[6], f[7], f[8]);
		add(f[9], f[10], f[11]);
	}
	else if (type == PolygonType::SQUAREPYRAMID) // 사각뿔
	{
		index = { 1,2,4,2,3,4,
			0,4,3,0,4,3,
			0,1,4,0,1,4,
			0,2,1,0,2,1,
			0,3,2,0,3,2,
		};
		polygonType = PolygonType::SQUAREPYRAMID;
		
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
		
		for (int i{}; i < 24; i += 3)
		{
			add(f[0 + i], f[1 + i], f[2 + i]);
		}
	}
	initBuffer();
}

PolygonShape::PolygonShape(const PolygonShape& other)
	: vao(other.vao)
	, ebo(other.ebo)
	, positions(other.positions) // std::vector는 깊은 복사됨
	, colors(other.colors)
	, index(other.index)
	, moveSpeed(other.moveSpeed)
	, polygonType(other.polygonType)
{
	std::fill(drawingIdx, drawingIdx + 12, true);
	midpoint[0] = other.midpoint[0]; 
	midpoint[1] = other.midpoint[1];
	midpoint[2] = other.midpoint[2];
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

void PolygonShape::setMidpoint(const float x, const float y, const float z)
{
	midpoint[0] = x;
	midpoint[1] = y;
	midpoint[2] = z;

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

void PolygonShape::update()
{
	if (!canMove) return;
	
	rotation();
	revolution();
	scaling();
	scalingByOrigin();
	moveT(1.0f);
	moveU();
	moveV();

	updateVbo();
}

/// <summary>
/// 자전하는 함수
/// </summary>
void PolygonShape::rotation()
{
	if (rotateSpeed[0] == 0.0f && rotateSpeed[1] == 0.0f) return;
	
	for (int i{}; i < positions.size(); i += 3)
	{
		glm::vec4 myVec(positions[i + 0], positions[i + 1], positions[i + 2], 1.0f);
		glm::mat4 transMatrix1 = glm::mat4(1.0f);
		transMatrix1 = glm::translate(transMatrix1, glm::vec3(-midpoint[0], -midpoint[1], -midpoint[2]));
		myVec = transMatrix1 * myVec;

		glm::mat4 rotMatrix = glm::mat4(1.0f);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(rotateSpeed[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		rotMatrix = glm::rotate(rotMatrix, glm::radians(rotateSpeed[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		myVec = rotMatrix * myVec;

		glm::mat4 transMatrix2 = glm::mat4(1.0f);
		transMatrix2 = glm::translate(transMatrix2, glm::vec3(midpoint[0], midpoint[1], midpoint[2]));
		myVec = transMatrix2 * myVec;

		positions[i + 0] = myVec.x;
		positions[i + 1] = myVec.y;
		positions[i + 2] = myVec.z;
	}
}

/// <summary>
/// 공전하는 함수
/// </summary>
void PolygonShape::revolution()
{
	if (!isYRotate) return;

	for (int i = 0; i < positions.size(); i += 3)
	{
		float x = positions[i + 0];
		float z = positions[i + 2];
		positions[i + 0] = x * glm::cos(moveSpeed) - z * glm::sin(moveSpeed);
		positions[i + 2] = x * glm::sin(moveSpeed) + z * glm::cos(moveSpeed);
	}

	float x = midpoint[0];
	float z = midpoint[2];
	midpoint[0] = x * glm::cos(moveSpeed) - z * glm::sin(moveSpeed);
	midpoint[2] = x * glm::sin(moveSpeed) + z * glm::cos(moveSpeed);
}

/// <summary>
/// 확대/축소하는 함수
/// </summary>
void PolygonShape::scaling()
{
	if (increaseSpeed == 0.0f || increasedAmount > 0.5f || increasedAmount < -0.5f) return;
	std::cout << "iA1= " << increasedAmount << "\n";
	for (int i{}; i < positions.size(); i += 3)
	{
		glm::vec4 myVec(positions[i + 0], positions[i + 1], positions[i + 2], 1.0f);
		glm::mat4 transMatrix1 = glm::mat4(1.0f);
		transMatrix1 = glm::translate(transMatrix1, glm::vec3(-midpoint[0], -midpoint[1], -midpoint[2]));
		myVec = transMatrix1 * myVec;

		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		float increaseRatio = 1.0f + increaseSpeed;
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(increaseRatio, increaseRatio, increaseRatio));
		myVec = scaleMatrix * myVec;

		glm::mat4 transMatrix2 = glm::mat4(1.0f);
		transMatrix2 = glm::translate(transMatrix2, glm::vec3(midpoint[0], midpoint[1], midpoint[2]));
		myVec = transMatrix2 * myVec;

		positions[i + 0] = myVec.x;
		positions[i + 1] = myVec.y;
		positions[i + 2] = myVec.z;
	}

	increasedAmount += increaseSpeed;
}

/// <summary>
/// 확대/축소를 원점에 대하여 하는 함수
/// </summary>
void PolygonShape::scalingByOrigin()
{
	if (!isIncreaseOrigin) return;
	std::cout << "iA2= " << increasedAmount2 << "\n";
	if (increaseSpeed == 0.0f || increasedAmount2 > 0.5f || increasedAmount2 < -0.5f) return;

	for (int i{}; i < positions.size(); i += 3)
	{
		glm::vec4 myVec(positions[i + 0], positions[i + 1], positions[i + 2], 1.0f);

		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		float increaseRatio = 1.0f + increaseSpeed;
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(increaseRatio, increaseRatio, increaseRatio));
		myVec = scaleMatrix * myVec;

		positions[i + 0] = myVec.x;
		positions[i + 1] = myVec.y;
		positions[i + 2] = myVec.z;
	}

	glm::vec4 myVec(midpoint[0], midpoint[1], midpoint[2], 1.0f);

	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	float increaseRatio = 1.0f + increaseSpeed;
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(increaseRatio, increaseRatio, increaseRatio));
	myVec = scaleMatrix * myVec;

	midpoint[0] = myVec.x;
	midpoint[1] = myVec.y;
	midpoint[2] = myVec.z;

	increasedAmount2 += increaseSpeed;
}

void PolygonShape::startMoveT(const float* dir)
{
	stopTUV();
	direction = glm::vec4(dir[0] - midpoint[0], dir[1] - midpoint[1], dir[2] - midpoint[2], 0.0f);
	tuvSpeed = 0.005f;
	tuvAmount = 0.0f;
}

void PolygonShape::moveT(float times)
{
	if (tuvSpeed == 0.0f) return;

	if (tuvAmount > 1.0f)
	{
		tuvAmount = 0.99f;
		tuvSpeed = -0.005f;
		return;
	}
	else if (tuvAmount < 0.0f)
	{
		tuvAmount = 0.01f;
		tuvSpeed = 0.005f;
		return;
	}
	move(direction.x * tuvSpeed * times, direction.y * tuvSpeed * times, direction.z * tuvSpeed * times);
	tuvAmount += tuvSpeed;
}

void PolygonShape::startMoveU(const float* dir)
{
	stopTUV();
	direction = glm::vec4(dir[0] - midpoint[0], dir[1] - midpoint[1], dir[2] - midpoint[2], 0.0f);
	tuvSpeed2 = 0.005f;
	tuvAmount2 = 0.0f;
}

void PolygonShape::moveU()
{
	if (tuvSpeed2 == 0.0f) return;

	if (tuvAmount2 < 0.25f)
	{
		if (polygonType == CUBE) move(0.0f, 0.01f, 0.0f);
		else move(0.0f, -0.01f, 0.0f);
	}
	else if (tuvAmount2 < 0.5f)
	{
		move(direction.x * tuvSpeed2 * 4, direction.y * tuvSpeed2 * 4, direction.z * tuvSpeed2 * 4);
	}
	else if (tuvAmount2 < 0.75f)
	{
		if (polygonType == CUBE) move(0.0f, -0.01f, 0.0f);
		else move(0.0f, 0.01f, 0.0f);
	}
	else if (tuvAmount2 < 1.0f)
	{
		move(direction.x * -tuvSpeed2 * 4, direction.y * -tuvSpeed2 * 4, direction.z * -tuvSpeed2 * 4);
	}
	else
	{
		tuvAmount2 = 0.0f;
	}

	tuvAmount2 += tuvSpeed2;
}

void PolygonShape::startMoveV(bool isIncrease)
{
	stopAll();
	stopTUV();
	if (isIncrease)
	{
		changeSpeed(1.0f, 0.0f);
		startIncrease(0.001f);
	}
	else
	{
		changeSpeed(-1.0f, 0.0f);
		startDecrease(0.001f);
	}
		
	startYRotate(0.01f);
	

}

void PolygonShape::moveV()
{
}

void PolygonShape::change()
{
	if (polygonType == CUBE)
	{
		float cubePos[24]{ randPos(gen),randPos(gen),randPos(gen),-randPos(gen),randPos(gen),randPos(gen)
			,-randPos(gen),-randPos(gen),randPos(gen),randPos(gen),-randPos(gen),randPos(gen)
			,randPos(gen),randPos(gen),-randPos(gen),-randPos(gen),randPos(gen),-randPos(gen)
			,-randPos(gen),-randPos(gen),-randPos(gen),randPos(gen),-randPos(gen),-randPos(gen) };
		colors.clear();
		for (int i{}; i < 24; ++i)
		{
			positions[i] = cubePos[i];
			colors.push_back(zeroToOne(gen));
		}
		setMidpoint(0.0f, 0.0f, 0.0f);
		move(-0.5f, 0.0f);
	}
	else if (polygonType == SQUAREPYRAMID)
	{
		float pyramidPos[15]{ 0.0f,randPos(gen) + 0.2f,0.0f
			,randPos(gen),-randPos(gen),randPos(gen)
			,-randPos(gen),-randPos(gen),randPos(gen)
			,-randPos(gen),-randPos(gen),-randPos(gen)
			,randPos(gen),-randPos(gen),-randPos(gen) };
		colors.clear();
		for (int i{}; i < 15; ++i)
		{
			positions[i] = pyramidPos[i];
			colors.push_back(zeroToOne(gen));
		}
		setMidpoint(0.0f, 0.0f, 0.0f);
		move(0.5f, 0.0f);
	}
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

void PolygonShape::move(float x, float y, float z)
{
	midpoint[0] += x;
	midpoint[1] += y;
	midpoint[2] += z;
	for (int i{}; i < static_cast<int>(positions.size()); i += 3)
	{
		positions[i] += x;
		positions[i + 1] += y;
		positions[i + 2] += z;
	}

	updateVbo();
}
