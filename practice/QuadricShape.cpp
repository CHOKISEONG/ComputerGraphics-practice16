#include "QuadricShape.h"

QuadricShape::QuadricShape(GLUquadric* o,QuadricType type, GLdouble rad, GLint slices, GLint stacks)
	: obj(o), type(type), radius(rad), slices(slices), stacks(stacks)
{
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;
	color[0] = zeroToOne(gen);
	color[1] = zeroToOne(gen);
	color[2] = zeroToOne(gen);
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

		gluQuadricDrawStyle(obj, GLU_LINE);
		//gluQuadricNormals(_obj, GLU_SMOOTH);
		//gluQuadricOrientation(_obj, GLU_OUTSIDE);

		GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
		glUniform3f(colorLoc, color[0], color[1], color[2]);
		gluSphere(obj, radius, slices, stacks);
	}
}

void QuadricShape::revolution()
{
	/// 할 일 : 궤도 그리기
	if (!isRevolution) return;

	glm::vec3 axis(0.0f, 1.0f, 0.0f);
	glm::vec3 relative = pos - targetPos;

	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), speed, glm::normalize(axis));
	glm::vec3 rotated = glm::vec3(rotMat * glm::vec4(relative, 1.0f));

	pos = targetPos + rotated;
}

void QuadricShape::move(float x, float y, float z)
{
	pos += glm::vec3(x, y, z);
}


