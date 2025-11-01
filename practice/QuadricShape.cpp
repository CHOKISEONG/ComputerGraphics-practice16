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

QuadricShape::~QuadricShape()
{
	gluDeleteQuadric(obj);
}

void QuadricShape::update()
{
	return;
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
		gluCylinder(obj, radius, radius, height, slices, stacks);
	else if (type == QuadricType::DISK)		
		gluDisk(obj, 0.0, radius, slices, stacks);
}

void QuadricShape::move(float x, float y, float z, bool changeTargetPos)
{
	pos += glm::vec3(x, y, z);
}


