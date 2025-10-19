#include "PolygonShape.h"

PolygonShape::PolygonShape() // 기본은 정육각형
{
	index = { 0,1,3,1,2,3,
			1,5,2,5,6,2,
			0,4,5,0,5,1,
			5,4,7,5,7,6,
			0,7,4,0,3,7,
			2,6,7,2,7,3 };
	slope = 0.0f;
	r = 1.0f;
	polygonType = PolygonType::POLYGON;
	moved = 0.0f;
	std::fill(drawingIdx, drawingIdx + 12, true);

	setMidpoint(0.0f, 0.0f);
	setRegularHexagon();
	initBuffer();
}

PolygonShape::PolygonShape(PolygonType type, const float* f)
{
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
	initBuffer();
}

PolygonShape::~PolygonShape()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vboPos);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &ebo);
}

void PolygonShape::updateVbo()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
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
}

void PolygonShape::setPaperFoldingShape()
{
	add(midpoint[0], midpoint[1] + r * ROOT2 * sin(PI / 2));
	add(midpoint[0] + r * ROOT2 * cos(PI), midpoint[1]);
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
	add(d, d, d);
	add(-d, d, d);
	add(-d, -d, d);
	add(d, -d, d);

	add(d, d, -d);
	add(-d, d, -d);
	add(-d, -d, -d);
	add(d, -d, -d);
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
	glGenBuffers(1, &vboPos);
	glGenBuffers(1, &vboColor);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	// 위치 연결
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// 색상 연결
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// 인덱스 연결
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

	if (positions[6] < targetX) {
		positions[6] += moveSpeed;
		if (positions[6] > targetX) positions[6] = targetX;
	}

	if (positions[7] > targetY) {
		positions[7] -= moveSpeed;
		if (positions[7] < targetY) positions[7] = targetY;
	}

	if (positions[6] >= targetX && positions[7] <= targetY) {
		polygonType = PolygonType::TRIANGLE;
	}
}

void PolygonShape::movingRectangle()
{
	float targetX = midpoint[0] + polygonDis * cos(3 * PI / 4);
	float targetY = midpoint[1] + polygonDis * sin(3 * PI / 4);

	if (positions[9] > targetX) {
		positions[9] -= moveSpeed;
		if (positions[9] < targetX) positions[9] = targetX;
	}

	if (positions[10] < targetY) {
		positions[10] += moveSpeed;
		if (positions[10] > targetY) positions[10] = targetY;
	}

	if (positions[9] <= targetX && positions[10] >= targetY) {
		polygonType = PolygonType::RECTSHAPE;
	}
}

void PolygonShape::movingPentagon()
{
	float targetY = midpoint[1] + polygonDis * sin(PI / 2);

	if (positions[13] < targetY) {
		positions[13] += moveSpeed;
		if (positions[13] > targetY) positions[13] = targetY;
	}

	if (positions[13] >= targetY) {
		polygonType = PolygonType::PENTAGON;
	}
}

void PolygonShape::movingLine()
{
	float targetX = midpoint[0];
	float targetY = midpoint[1];

	if (positions[6] > targetX) {
		positions[6] -= moveSpeed;
		if (positions[6] < targetX) positions[6] = targetX;
	}

	if (positions[7] < targetY) {
		positions[7] += moveSpeed;
		if (positions[7] > targetY) positions[7] = targetY;
	}

	if (positions[9] < targetX) {
		positions[9] += moveSpeed;
		if (positions[9] > targetX) positions[9] = targetX;
	}

	if (positions[10] > targetY) {
		positions[10] -= moveSpeed;
		if (positions[10] < targetY) positions[10] = targetY;
	}

	if (positions[13] > targetY) {
		positions[13] -= moveSpeed;
		if (positions[13] < targetY) polygonType = PolygonType::LINE;
	}
}

void PolygonShape::add(const float x, const float y)
{
	positions.push_back(x);
	positions.push_back(y);
	positions.push_back(0.0f);
	float r = zeroToOne(gen);
	float g = zeroToOne(gen);
	float b = zeroToOne(gen);
	std::cout << r << "," << g << "," << b << " 색 설정\n";
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	updateVbo();
}

void PolygonShape::add(const float x, const float y, const float z)
{
	std::cout << x << ", " << y << ", " << z << " 추가\n";
	positions.push_back(x);
	positions.push_back(y);
	positions.push_back(z);
	float r = zeroToOne(gen);
	float g = zeroToOne(gen);
	float b = zeroToOne(gen);
	std::cout << r << "," << g << "," << b << " 색 설정\n";
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	updateVbo();
}

void PolygonShape::draw(GLuint shaderProgram) const
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

	if (polygonType == PolygonType::LINE)
	{
		glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		for (int i = 0; i < 12; ++i) {
			if (drawingIdx[i])
			{
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
	for (int i{}; i < static_cast<int>(positions.size()); i += 3)
	{
		positions[i] += x;
		positions[i + 1] += y;
	}

	updateVbo();
}

int PolygonShape::isTouchedWall()
{
	if (positions[1] > 1.0f || positions[13] > 1.0f)
	{
		move(0.0f, -moveSpeed);
		return (int)WallDir::UP;
	}
	if (positions[3] < -1.0f)
	{
		move(moveSpeed, 0.0f);
		return (int)WallDir::LEFT;
	}
	if (positions[4] < -1.0f)
	{
		move(0.0f, moveSpeed);
		return (int)WallDir::DOWN;
	}
	if (positions[0] > 1.0f)
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
		float sx = positions[0] - positions[3];
		float sy = positions[1] - positions[4];
		float p[2]{ other->positions[3],other->positions[4] };

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
		if (sqrt(pow(x - other->midpoint[0], 2) + pow(y - other->midpoint[1], 2)) <= r)
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
//		positions[i] -= midpoint[0];
//		positions[i + 1] -= midpoint[1];
//	}
//
//	positions[0] = moved * cos(PI/2 +offSlope);
//	positions[1] = moved * sin(PI / 2 + offSlope);
//
//	positions[3] = moved * cos(PI + offSlope);
//	positions[4] = moved * sin(PI + offSlope);
//
//	positions[6] = moved * cos(3 * PI / 2 + offSlope);
//	positions[7] = moved * sin(3 * PI / 2 + offSlope);
//
//	positions[9] = moved * cos(offSlope);
//	positions[10] = moved * sin(offSlope);
//	
//	for (int i{}; i < 12; i += 3)
//	{
//		positions[i] += midpoint[0];
//		positions[i + 1] += midpoint[1];
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//}
//
//void PolygonShape::moveRotate()
//{
//	for (int i{ 12 }; i < static_cast<int>(positions.size()); i += 3)
//	{
//		positions[i] -= midpoint[0];
//		positions[i + 1] -= midpoint[1];
//	}
//
//	float d = moved / ROOT2 / 2 + r/2;
//	positions[12] = d * cos(3 * PI / 4 + offSlope);
//	positions[13] = d * sin(3 * PI / 4 + offSlope);
//
//	positions[15] = d * cos(PI / 4 + offSlope);
//	positions[16] = d * sin(PI / 4 + offSlope);
//
//	positions[18] = d * cos(5 * PI / 4 + offSlope);
//	positions[19] = d * sin(5 * PI / 4 + offSlope);
//
//	positions[21] = d * cos(7 * PI / 4 + offSlope);
//	positions[22] = d * sin(7 * PI / 4 + offSlope);
//
//	for (int i{ 12 }; i < static_cast<int>(positions.size()); i += 3)
//	{
//		positions[i] += midpoint[0];
//		positions[i + 1] += midpoint[1];
//	}
//	offSlope += slope;
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
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
//	positions.clear();
//	setPaperFoldingShape();
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
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
	if (positions.size() > 15) return;
	polygonType = pType;
	// 하드코딩 가보자잇
	if (pType == PolygonType::LINE)
	{
		positions[0] = midpoint[0] + r * cos(PI / 4);
		positions[1] = midpoint[1] + r * sin(PI / 4);

		positions[3] = midpoint[0] + r * cos(5 * PI / 4);
		positions[4] = midpoint[1] + r * sin(5 * PI / 4);

		positions[6] = midpoint[0] + 0.001f;
		positions[7] = midpoint[1] - 0.001f;

		positions[9] = midpoint[0];
		positions[10] = midpoint[1];

		positions[12] = midpoint[0];
		positions[13] = midpoint[1];
	}
	else if (pType == PolygonType::TRIANGLE)
	{
		positions[0] = midpoint[0] + r * cos(PI / 4);
		positions[1] = midpoint[1] + r * sin(PI / 4);

		positions[3] = midpoint[0] + r * cos(5 * PI / 4);
		positions[4] = midpoint[1] + r * sin(5 * PI / 4);

		positions[6] = midpoint[0] + r * cos(-PI / 4);
		positions[7] = midpoint[1] + r * sin(-PI / 4);

		positions[9] = midpoint[0];
		positions[10] = midpoint[1];

		positions[12] = midpoint[0];
		positions[13] = midpoint[1];
	}
	else if (pType == PolygonType::RECTSHAPE)
	{
		positions[0] = midpoint[0] + r * cos(PI / 4);
		positions[1] = midpoint[1] + r * sin(PI / 4);

		positions[3] = midpoint[0] + r * cos(5 * PI / 4);
		positions[4] = midpoint[1] + r * sin(5 * PI / 4);

		positions[6] = midpoint[0] + r * cos(-PI / 4);
		positions[7] = midpoint[1] + r * sin(-PI / 4);

		positions[9] = midpoint[0] + r * cos(3 * PI / 4);
		positions[10] = midpoint[1] + r * sin(3 * PI / 4);

		positions[12] = midpoint[0];
		positions[13] = midpoint[1];
	}
	else if (pType == PolygonType::PENTAGON)
	{
		positions[0] = midpoint[0] + r * cos(PI / 4);
		positions[1] = midpoint[1] + r * sin(PI / 4);

		positions[3] = midpoint[0] + r * cos(5 * PI / 4);
		positions[4] = midpoint[1] + r * sin(5 * PI / 4);

		positions[6] = midpoint[0] + r * cos(-PI / 4);
		positions[7] = midpoint[1] + r * sin(-PI / 4);

		positions[9] = midpoint[0] + r * cos(3 * PI / 4);
		positions[10] = midpoint[1] + r * sin(3 * PI / 4);

		positions[12] = midpoint[0] + r * cos(PI / 2);
		positions[13] = midpoint[1] + r * sin(PI / 2);

	}
	else if (pType == PolygonType::POINTTYPE)
	{
		for (int i{}; i < 15; i += 3)
		{
			positions[i] = midpoint[0];
			positions[i + 1] = midpoint[1];
		}
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

