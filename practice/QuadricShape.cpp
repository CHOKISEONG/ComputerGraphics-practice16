#include "QuadricShape.h"

QuadricShape::QuadricShape(GLUquadric* o,QuadricType type, GLdouble rad)
	: obj(o), radius(rad)
{
	color[0] = zeroToOne(gen);
	color[1] = zeroToOne(gen);
	color[2] = zeroToOne(gen);
}

void QuadricShape::rotateAxisZ(float theta)
{
	glm::vec3 tmp = axis;
	axis.x = glm::cos(theta) * tmp.x - glm::sin(theta) * tmp.y;
	axis.y = glm::sin(theta) * tmp.x + glm::cos(theta) * tmp.y;
}


void QuadricShape::update()
{
	// 아직 구현 안함
	return;
}

void QuadricShape::draw(GLuint shaderProgram) const
{
	glUseProgram(shaderProgram);

	if (type == QuadricType::SPHERE)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
		glUniform3f(colorLoc, color[0], color[1], color[2]);
		gluSphere(obj, radius, slices, stacks);
	}
}

void QuadricShape::move(float x, float y, float z, bool changeTargetPos)
{
	pos += glm::vec3(x, y, z);
}


