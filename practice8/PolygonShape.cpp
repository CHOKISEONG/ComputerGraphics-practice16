#include "PolygonShape.h"

PolygonShape::PolygonShape() // 기본은 정육각형
{
	index = { 0,1,3,1,2,3,
			1,5,2,5,6,2,
			0,4,5,0,5,1,
			5,4,7,5,7,6,
			0,7,4,0,3,7,
			2,6,7,2,7,3 };
	initColor();
	slope = 0.0f;
	r = 1.0f;
	polygonType = PolygonType::POLYGON;
	moved = 0.0f;
	std::fill(drawingIdx, drawingIdx + 12, true);
	initBuffer();

	setMidpoint(0.0f, 0.0f);
	setRegularHexagon();
}

PolygonShape::PolygonShape(PolygonType type, const float* f)
{
	initColor();
	slope = 0.0f;
	r = 1.0f;
	if (type == PolygonType::LINE)
	{
		index = { 0,1 };
		
		polygonType = PolygonType::LINE;
		initBuffer();

		setMidpoint(0.0f, 0.0f);
		add(f[0], f[1], f[2]);
		add(f[3], f[4], f[5]);
	}
	else if (type == PolygonType::RECTSHAPE)
	{
		index = { 0,1,2,0,2,3 };
		polygonType = PolygonType::RECTSHAPE;

		initBuffer();

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
		initBuffer();

		setMidpoint(0.0f, 0.0f);
		for (int i{}; i < 15; i += 3)
		{
			add(f[0 + i], f[1 + i], f[2 + i]);
		}
	}
}

PolygonShape::PolygonShape(std::vector<float>& v) {
	verticles = v;
	initColor();
	initBuffer();
}

PolygonShape::PolygonShape(const PolygonShape& other)
	: vao(other.vao)
	, ebo(other.ebo)
	, verticles(other.verticles) // std::vector는 깊은 복사됨
	, index(other.index)
	, isMoving(other.isMoving)
	, canAnimation(other.canAnimation)
	, animType(other.animType)
	, moveSpeed(other.moveSpeed)
	, slope(other.slope)
	, distance(other.distance)
	, moved(other.moved)
	, r(other.r)
	, polygonType(other.polygonType)
{
	midpoint[0] = other.midpoint[0]; midpoint[1] = other.midpoint[1];
	initColor();
	initBuffer();
}

PolygonShape::~PolygonShape()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void PolygonShape::updateVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void PolygonShape::initColor()
{
	for (int j{}; j < 6; ++j)
	{
		float r = zeroToOne(gen);
		float g = zeroToOne(gen);
		float b = zeroToOne(gen);
		std::cout << r << "," << g << "," << b << " 색 설정\n";
		colors[0 + j * 6] = r;
		colors[1 + j * 6] = g;
		colors[2 + j * 6] = b;
		colors[3 + j * 6] = r;
		colors[4 + j * 6] = g;
		colors[5 + j * 6] = b;
	}
	
}

void PolygonShape::setColor(const float r, const float g, const float b)
{
	for (int i{}; i < 12; ++i)
	{
		colors[0 + (i * 3)] = r;
		colors[1 + (i * 3)] = g;
		colors[2 + (i * 3)] = b;
	}
}

void PolygonShape::setPaperFoldingShape()
{
	add(midpoint[0], midpoint[1] + r * ROOT2 * sin(PI / 2));
	add(midpoint[0] + r * ROOT2 * cos(PI), midpoint[1] );
	add(midpoint[0], midpoint[1] + r * ROOT2 * sin(-PI / 2));
	add(midpoint[0] + r * ROOT2, midpoint[1]);

	add(midpoint[0] + r * cos(3 * PI / 4), midpoint[1] + r * sin(PI / 4));
	add(midpoint[0] + r * cos(PI / 4), midpoint[1] + r * sin(3 * PI / 4));
	add(midpoint[0] + r * cos(5 * PI / 4), midpoint[1] + r * sin(5 * PI / 4));
	add(midpoint[0] + r * cos(-PI / 4), midpoint[1] + r * sin(-PI / 4));
}

void PolygonShape::setRegularHexagon()
{
	float d = 0.5f;
	add(d,d,d);
	add(-d,d,d);
	add(-d,-d,d);
	add(d,-d,d);

	add(d,d,-d);
	add(-d,d,-d);
	add(-d,-d,-d);
	add(d,-d,-d);
}

void PolygonShape::changeAnimType()
{
	if (canAnimation)
	{
		if (animType == 0)
			animType = zeroOrOne(gen) + 1;
		else
			animType = 0;
	}
}

void PolygonShape::setMidpoint(const float x, const float y)
{
	midpoint[0] = x;
	midpoint[1] = y;

	updateVbo();
}

void PolygonShape::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Index buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void PolygonShape::initialSetting(PolygonType type)
{
	float randX = randPos(gen), randY = randPos(gen);
	initBuffer();
	for (int i{}; i < 5; ++i)
		add(randX, randY, 0.0f);
	setMidpoint(randX, randY);
	changeType(type);
	std::cout << "set type to " << type << "\n";
	std::cout << "x: " << randX << ", y: " << randY << "\n";
}

void PolygonShape::moving()
{
	if (polygonType == PolygonType::LINE)
		movingTriangle();
	else if (polygonType == PolygonType::TRIANGLE)
		movingRectangle();
	else if (polygonType == PolygonType::RECTSHAPE)
		movingPentagon();
	else if (polygonType == PolygonType::PENTAGON)
		movingLine();

	updateVbo();
}

void PolygonShape::movingTriangle()
{
	float targetX = midpoint[0] + polygonDis * cos(-PI / 4);
	float targetY = midpoint[1] + polygonDis * sin(-PI / 4);

	if (verticles[6] < targetX) {
		verticles[6] += moveSpeed;
		if (verticles[6] > targetX) verticles[6] = targetX;
	}

	if (verticles[7] > targetY) {
		verticles[7] -= moveSpeed;
		if (verticles[7] < targetY) verticles[7] = targetY;
	}

	if (verticles[6] >= targetX && verticles[7] <= targetY) {
		polygonType = PolygonType::TRIANGLE;
	}
}

void PolygonShape::movingRectangle()
{
	float targetX = midpoint[0] + polygonDis * cos(3 * PI / 4);
	float targetY = midpoint[1] + polygonDis * sin(3 * PI / 4);

	if (verticles[9] > targetX) {
		verticles[9] -= moveSpeed;
		if (verticles[9] < targetX) verticles[9] = targetX;
	}

	if (verticles[10] < targetY) {
		verticles[10] += moveSpeed;
		if (verticles[10] > targetY) verticles[10] = targetY;
	}

	if (verticles[9] <= targetX && verticles[10] >= targetY) {
		polygonType = PolygonType::RECTSHAPE;
	}
}

void PolygonShape::movingPentagon()
{
	float targetY = midpoint[1] + polygonDis * sin(PI / 2);

	if (verticles[13] < targetY) {
		verticles[13] += moveSpeed;
		if (verticles[13] > targetY) verticles[13] = targetY;
	}

	if (verticles[13] >= targetY) {
		polygonType = PolygonType::PENTAGON;
	}
}

void PolygonShape::movingLine()
{
	float targetX = midpoint[0];
	float targetY = midpoint[1];

	if (verticles[6] > targetX) {
		verticles[6] -= moveSpeed;
		if (verticles[6] < targetX) verticles[6] = targetX;
	}

	if (verticles[7] < targetY) {
		verticles[7] += moveSpeed;
		if (verticles[7] > targetY) verticles[7] = targetY;
	}

	if (verticles[9] < targetX) {
		verticles[9] += moveSpeed;
		if (verticles[9] > targetX) verticles[9] = targetX;
	}

	if (verticles[10] > targetY) {
		verticles[10] -= moveSpeed;
		if (verticles[10] < targetY) verticles[10] = targetY;
	}

	if (verticles[13] > targetY) {
		verticles[13] -= moveSpeed;
		if (verticles[13] < targetY) polygonType = PolygonType::LINE;
	}
}

void PolygonShape::add(const float x, const float y)
{
	verticles.push_back(x);
	verticles.push_back(y);
	verticles.push_back(0.0f);

	updateVbo();
}

void PolygonShape::add(const float x, const float y, const float z)
{
	std::cout << x << ", " << y << ", " << z <<" 추가\n";
	verticles.push_back(x);
	verticles.push_back(y);
	verticles.push_back(z);

	updateVbo();
}

void PolygonShape::draw(GLuint shaderProgram) const
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	GLint location = glGetUniformLocation(shaderProgram, "color");
	if (location == -1)
	{
		std::cout << "location 못 찾음\n";
	}
	glUniform3f(location, 1.0f,0.0f,0.0f);

	if (polygonType == PolygonType::LINE)
	{
		glUniform3f(location, colors[0], colors[1], colors[2]);
		glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		for (int i = 0; i < 12; ++i) {
			if (drawingIdx[i])
			{
				glUniform3f(location, colors[0 + (i * 3)], colors[1 + (i * 3)], colors[2 + (i * 3)]);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
			}
		}
	}
	glBindVertexArray(0);
}

void PolygonShape::move(float x, float y)
{
	//if (!canAnimation) return;
	midpoint[0] += x;
	midpoint[1] += y;
	for (int i{}; i < static_cast<int>(verticles.size()); i += 3)
	{
		verticles[i] += x;
		verticles[i + 1] += y;
	}
	
	updateVbo();
}

int PolygonShape::isTouchedWall()
{
	if (verticles[1] > 1.0f || verticles[13] > 1.0f)
	{
		move(0.0f, -moveSpeed);
		return (int)WallDir::UP;
	}
	if (verticles[3] < -1.0f)
	{
		move(moveSpeed, 0.0f);
		return (int)WallDir::LEFT;
	}
	if (verticles[4] < -1.0f)
	{
		move(0.0f, moveSpeed);
		return (int)WallDir::DOWN;
	}
	if (verticles[0] > 1.0f)
	{
		move(-moveSpeed, 0.0f);
		return (int)WallDir::RIGHT;
	}
	return 0;
}

bool PolygonShape::isCollide(PolygonShape*& other)
{
	float x = midpoint[0];
	float y = midpoint[1];
	if (polygonType == PolygonType::POINTTYPE)
	{
		if (abs(x - other->midpoint[0]) < r / 10 && abs(y - other->midpoint[1]) < r / 10)
			return true;
	}
	else if (polygonType == PolygonType::LINE)
	{
		float sx = verticles[0] - verticles[3];
		float sy = verticles[1] - verticles[4];
		float p[2]{ other->verticles[3],other->verticles[4] };

		for (int i{}; i < 20; ++i)
		{
			p[0] += 0.05f * sx;
			p[1] += 0.05f * sy;
			if (abs(x - p[0]) < 0.05f && abs(y - p[1]) < 0.05f)
				return true;
		}
	}
	else
	{
		// 느슨한 바운딩 구 검사로 함
		if (sqrt(pow(x - other->midpoint[0], 2) + pow(y - other->midpoint[1], 2)) <= r )
			return true;
	}

	return false;
}

//void PolygonShape::movePaperFolding()
//{
//	moved += moveSpeed;
//	if (moved <= 0.0f) 
//	{
//		moved = 0.0f;
//		moveSpeed = -moveSpeed;
//	}
//	else if (moved >= r * ROOT2) 
//	{
//		moved = r * ROOT2;
//		moveSpeed = -moveSpeed;
//	}
//
//	for (int i{}; i < 12; i += 3)
//	{
//		verticles[i] -= midpoint[0];
//		verticles[i + 1] -= midpoint[1];
//	}
//
//	verticles[0] = moved * cos(PI/2 +offSlope);
//	verticles[1] = moved * sin(PI / 2 + offSlope);
//
//	verticles[3] = moved * cos(PI + offSlope);
//	verticles[4] = moved * sin(PI + offSlope);
//
//	verticles[6] = moved * cos(3 * PI / 2 + offSlope);
//	verticles[7] = moved * sin(3 * PI / 2 + offSlope);
//
//	verticles[9] = moved * cos(offSlope);
//	verticles[10] = moved * sin(offSlope);
//	
//	for (int i{}; i < 12; i += 3)
//	{
//		verticles[i] += midpoint[0];
//		verticles[i + 1] += midpoint[1];
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//}
//
//void PolygonShape::moveRotate()
//{
//	for (int i{ 12 }; i < static_cast<int>(verticles.size()); i += 3)
//	{
//		verticles[i] -= midpoint[0];
//		verticles[i + 1] -= midpoint[1];
//	}
//
//	float d = moved / ROOT2 / 2 + r/2;
//	verticles[12] = d * cos(3 * PI / 4 + offSlope);
//	verticles[13] = d * sin(3 * PI / 4 + offSlope);
//
//	verticles[15] = d * cos(PI / 4 + offSlope);
//	verticles[16] = d * sin(PI / 4 + offSlope);
//
//	verticles[18] = d * cos(5 * PI / 4 + offSlope);
//	verticles[19] = d * sin(5 * PI / 4 + offSlope);
//
//	verticles[21] = d * cos(7 * PI / 4 + offSlope);
//	verticles[22] = d * sin(7 * PI / 4 + offSlope);
//
//	for (int i{ 12 }; i < static_cast<int>(verticles.size()); i += 3)
//	{
//		verticles[i] += midpoint[0];
//		verticles[i + 1] += midpoint[1];
//	}
//	offSlope += slope;
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//}
//
//void PolygonShape::teleport(float x, float y)
//{
//	canAnimation = false;
//	midpoint[0] = x;
//	midpoint[1] = y;
//	offSlope = 0.0f;
//
//	verticles.clear();
//	setPaperFoldingShape();
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//}
//
//void PolygonShape::moveAnimation()
//{
//	if (animType == 1)
//		moveSpring();
//	else if (animType == 2)
//		moveZigZag();
//}
//
//void PolygonShape::moveSpring()
//{
//	int tmp = isTouchedWall();
//	if (tmp == WallDir::UP || tmp == WallDir::DOWN)
//	{
//		slope = 2 * PI - slope;
//	}
//	else if (tmp == WallDir::LEFT || tmp == WallDir::RIGHT)
//	{
//		slope = PI - slope;
//	}
//	move(moveSpeed * cos(slope), moveSpeed * (sin(slope)));
//}
//
//void PolygonShape::moveZigZag()
//{
//	moved += 0.0003f;
//	int tmp = isTouchedWall();
//	if (tmp == WallDir::LEFT || tmp == WallDir::RIGHT)
//	{
//		slope = PI - slope;
//		moved = 0.0f;
//		distance = udis(gen);
//	}
//	if (moved <= distance)
//		move(0.0f, -moveSpeed);
//	else move(moveSpeed * cos(slope), 0.0f);
//}

void PolygonShape::changeType(PolygonType pType)
{
	if (verticles.size() > 15) return;
	polygonType = pType;
	// 하드코딩 가보자잇
	if (pType == PolygonType::LINE)
	{
		verticles[0] = midpoint[0] + r * cos(PI / 4);
		verticles[1] = midpoint[1] + r * sin(PI / 4);

		verticles[3] = midpoint[0] + r * cos(5 * PI / 4);
		verticles[4] = midpoint[1] + r * sin(5 * PI / 4);

		verticles[6] = midpoint[0] + 0.001f;
		verticles[7] = midpoint[1] - 0.001f;

		verticles[9] = midpoint[0];
		verticles[10] = midpoint[1];

		verticles[12] = midpoint[0];
		verticles[13] = midpoint[1];
	}
	else if (pType == PolygonType::TRIANGLE)
	{
		verticles[0] = midpoint[0] + r * cos(PI / 4);
		verticles[1] = midpoint[1] + r * sin(PI / 4);

		verticles[3] = midpoint[0] + r * cos(5 * PI / 4);
		verticles[4] = midpoint[1] + r * sin(5 * PI / 4);

		verticles[6] = midpoint[0] + r * cos(-PI / 4);
		verticles[7] = midpoint[1] + r * sin(-PI / 4);

		verticles[9] = midpoint[0];
		verticles[10] = midpoint[1];

		verticles[12] = midpoint[0];
		verticles[13] = midpoint[1];
	}
	else if (pType == PolygonType::RECTSHAPE)
	{
		verticles[0] = midpoint[0] + r * cos(PI / 4);
		verticles[1] = midpoint[1] + r * sin(PI / 4);

		verticles[3] = midpoint[0] + r * cos(5 * PI / 4);
		verticles[4] = midpoint[1] + r * sin(5 * PI / 4);

		verticles[6] = midpoint[0] + r * cos(-PI / 4);
		verticles[7] = midpoint[1] + r * sin(-PI / 4);

		verticles[9] = midpoint[0] + r * cos(3 * PI / 4);
		verticles[10] = midpoint[1] + r * sin(3 * PI / 4);

		verticles[12] = midpoint[0];
		verticles[13] = midpoint[1];
	}
	else if (pType == PolygonType::PENTAGON)
	{
		verticles[0] = midpoint[0] + r * cos(PI / 4);
		verticles[1] = midpoint[1] + r * sin(PI / 4);

		verticles[3] = midpoint[0] + r * cos(5 * PI / 4);
		verticles[4] = midpoint[1] + r * sin(5 * PI / 4);

		verticles[6] = midpoint[0] + r * cos(-PI / 4);
		verticles[7] = midpoint[1] + r * sin(-PI / 4);

		verticles[9] = midpoint[0] + r * cos(3 * PI / 4);
		verticles[10] = midpoint[1] + r * sin(3 * PI / 4);

		verticles[12] = midpoint[0] + r * cos(PI / 2);
		verticles[13] = midpoint[1] + r * sin(PI / 2);

	}
	else if (pType == PolygonType::POINTTYPE)
	{
		for (int i{}; i < 15; i += 3)
		{
			verticles[i] = midpoint[0];
			verticles[i + 1] = midpoint[1];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
		float sx = verticles[0] - verticles[3];
		float sy = verticles[1] - verticles[4];
		float p[2]{ verticles[3],verticles[4] };

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

