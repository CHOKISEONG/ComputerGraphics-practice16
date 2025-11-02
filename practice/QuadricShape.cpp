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

	// 처음에 실린더나 디스크 그리면 마름모처럼 그려져서 돌림
	rotateZ(45.0f);

	if (type == SPHERE)
		setSlices(20);
}

QuadricShape::~QuadricShape()
{
	gluDeleteQuadric(obj);
}

void QuadricShape::update(bool isUpdate)
{
	if (!isUpdate) return;

	accel += gravity * 0.01f; 

	pos.y += accel * 0.01f;

	// 바닥 닿으면 튕기기
	if (pos.y < -4.5f / ROOT2)
	{
		pos.y = -4.5f/ROOT2; 
		accel = -accel * bounceFactor;
		if (std::abs(accel) < 0.1f) { 
			accel = 0.0f;
		}
	}

	std::cout << "pos.y: " << pos.y << ", accel: " << accel << "\n";
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


