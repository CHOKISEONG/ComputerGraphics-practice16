#include "QuadricShape.h"

QuadricShape::QuadricShape(QuadricType type, GLdouble rad, GLdouble height
							, float x, float y, float z)
	: type(type)
	, radius(rad)
	, height(height)
	, pos(glm::vec3(x,y,z))
{
	obj = gluNewQuadric();
	color[0] = zeroToOne(gen);
	color[1] = zeroToOne(gen);
	color[2] = zeroToOne(gen);
}

QuadricShape::QuadricShape(QuadricShape& other)
	: ShapeManager(other),
	type(other.type),
	radius(other.radius),
	height(other.height),
	slices(other.slices),
	stacks(other.stacks),
	pos(other.pos),
	angle_x(other.angle_x),
	angle_y(other.angle_y),
	angle_z(other.angle_z),
	targetPos(other.targetPos),
	speed(other.speed),
	speed_G(other.speed_G),
	speed_P(other.speed_P),
	amount_T(other.amount_T),
	amount_L(other.amount_L),
	amount_G(other.amount_G),
	amount_P(other.amount_P)
{
	obj = gluNewQuadric();
	color[0] = other.color[0];
	color[1] = other.color[1];
	color[2] = other.color[2];
	for (int i = 0; i < 4; ++i) isMove[i] = other.isMove[i];
}
QuadricShape& QuadricShape::operator=(const QuadricShape& other)
{
	if (this == &other) return *this;

	type = other.type;
	radius = other.radius;
	height = other.height;
	slices = other.slices;
	stacks = other.stacks;
	pos = other.pos;
	color[0] = other.color[0];
	color[1] = other.color[1];
	color[2] = other.color[2];

	if (obj) gluDeleteQuadric(obj);
	obj = gluNewQuadric();

	angle_x = other.angle_x;
	angle_y = other.angle_y;
	angle_z = other.angle_z;
	for (int i = 0; i < 4; ++i) isMove[i] = other.isMove[i];
	targetPos = other.targetPos;
	speed = other.speed;
	speed_G = other.speed_G;
	speed_P = other.speed_P;
	amount_T = other.amount_T;
	amount_L = other.amount_L;
	amount_G = other.amount_G;
	amount_P = other.amount_P;

	return *this;
}

QuadricShape::~QuadricShape()
{
	gluDeleteQuadric(obj);
}


void QuadricShape::update()
{
	moveT();
	moveL();
	moveG();
	moveP();
	return;
}
void QuadricShape::moveT()
{
	if (!isMove[0]) return;

	if (amount_T <= 1.0f)
	{
		// T는 생각보다 코드가 많을 것 같아서 나중에 하기
		amount_T += speed;
	}
	
}
void QuadricShape::moveL()
{
	if (!isMove[1]) return;
	
	if (amount_L <= 1.0f)
	{
		pos.z += targetPos.z * speed;
		amount_L += speed;
	}
	
}
void QuadricShape::moveG()
{
	if (!isMove[2]) return;

	if (-15.0f <= amount_G && amount_G <= 15.0f)
	{
		rotateY(speed_G);
		amount_G += speed_G;
	}
	else
	{
		speed_G = -speed_G;
		amount_G += speed_G;
	}
		
}
void QuadricShape::moveP()
{
	if (!isMove[3]) return;

	rotateZ(speed_P);
}

void QuadricShape::draw(GLuint shaderProgram) const
{
	return;
}

void QuadricShape::draw2(GLuint shaderProgram, DrawType drawType) const
{
	glUseProgram(shaderProgram);

	// 중심 위치 결정
	glm::mat4 model(1.0f);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// 색상값 결정
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
	glUniform3f(colorLoc, color[0], color[1], color[2]);

	if		(drawType == DrawType::DRAW_SOLID)		gluQuadricDrawStyle(obj, GLU_FILL);
	else if (drawType == DrawType::DRAW_WIRE)		gluQuadricDrawStyle(obj, GLU_LINE);
	else if (drawType == DrawType::DRAW_SILHOUETTE)	gluQuadricDrawStyle(obj, GLU_SILHOUETTE);
	else if (drawType == DrawType::DRAW_POINT)		gluQuadricDrawStyle(obj, GLU_POINT);

	if (type == QuadricType::SPHERE)		
		gluSphere(obj, radius, slices, stacks);
	else if (type == QuadricType::CYLINDER)
	{
		gluDisk(obj, 0.0, radius, slices, stacks);
		gluCylinder(obj, radius, radius, height, slices, stacks);
		glm::mat4 top = model;
		top = glm::translate(top, glm::vec3(0.0f, 0.0f, height)); // z축으로 height만큼 이동

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(top));
		gluDisk(obj, 0.0, radius, slices, stacks);
	}
		
	else if (type == QuadricType::DISK)		
		gluDisk(obj, 0.0, radius, slices, stacks);
}

void QuadricShape::move(float x, float y, float z, bool changeTargetPos)
{
	pos += glm::vec3(x, y, z);
}


