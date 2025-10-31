#include "QuadricShape.h"

QuadricShape::QuadricShape(GLUquadric* o,QuadricType type, GLdouble rad)
	: obj(o), moon(o), type(type), radius(rad)
{
	color[0] = zeroToOne(gen);
	color[1] = zeroToOne(gen);
	color[2] = zeroToOne(gen);
	mColor[0] = 1.0f - color[0];
	mColor[1] = 1.0f - color[1];
	mColor[2] = 1.0f - color[2];
	if (zeroOrOne(gen)) speed = -speed;
}

void QuadricShape::draw(GLuint shaderProgram) const
{
	glUseProgram(shaderProgram);
	
	if (!isSolidDraw)
	{
		gluQuadricDrawStyle(obj, GLU_LINE);
		gluQuadricDrawStyle(moon, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(obj, GLU_FILL);
		gluQuadricDrawStyle(moon, GLU_FILL);
	}

	for (int i{}; i < orbit.size(); ++i)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, orbit[i].pos);
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		gluSphere(orbit[i].obj, orbitRadius, slices, stacks);
	}

	if (type == QuadricType::SPHERE)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
		glUniform3f(colorLoc, color[0], color[1], color[2]);
		gluSphere(obj, radius, slices, stacks);

		model = glm::mat4(1.0f);
		model = glm::translate(model, mPos);
		modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
		glUniform3f(colorLoc, mColor[0], mColor[1], mColor[2]);
		gluSphere(moon, radius/2, slices, stacks);
	}
}

void QuadricShape::makeChildren()
{
	moon = gluNewQuadric(); 
	mPos = pos * 1.2f;
	mColor[0] = 1 - color[0];
	mColor[1] = 1 - color[1];
	mColor[2] = 1 - color[2];
	mDir = mPos - pos;
}

void QuadricShape::revolution()
{
	/// 할 일 : 궤도 그리기
	if (!isRevolution) return;

	timer += 0.1f;
	if (timer >= 1.0f)
	{
		timer = 0.0f;
		orbit.push_back(basicInfo(gluNewQuadric(), pos));
	}
	if (orbit.size() > 7)
	{
		orbit.erase(orbit.begin());
	}

	glm::vec3 dir = pos - targetPos;
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), speed, glm::normalize(axis));
	glm::vec3 rotated = glm::vec3(rotMat * glm::vec4(dir, 1.0f));
	pos = targetPos + rotated;

	mAmount += -2 * speed ;
	glm::mat4 mRotMat = glm::rotate(glm::mat4(1.0f), mAmount, glm::normalize(axis));
	glm::vec3 mRotated = glm::vec3(mRotMat * glm::vec4(mDir, 1.0f));
	mPos = pos + mRotated;
}

void QuadricShape::move(float x, float y, float z, bool changeTargetPos)
{
	pos += glm::vec3(x, y, z);
	mPos += glm::vec3(x, y, z);
	if (changeTargetPos)
		targetPos += glm::vec3(x, y, z);
}


