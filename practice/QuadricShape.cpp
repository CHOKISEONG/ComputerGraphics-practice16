#include "QuadricShape.h"

QuadricShape::QuadricShape(QuadricType type, GLdouble rad, float x, float y, float z)
	: type(type), radius(rad), pos(glm::vec3(x, y, z))
{
	obj = gluNewQuadric();
	color[0] = zeroToOne(gen);
	color[1] = zeroToOne(gen);
	color[2] = zeroToOne(gen);

	if (type == SPHERE)
		setSlices(20);
}

QuadricShape::~QuadricShape()
{
	gluDeleteQuadric(obj);
}

void QuadricShape::draw(GLuint shaderProgram) const
{
	return;
}

// GPT 도움 받은 함수
static inline glm::mat4 buildLookAtOrientation(const glm::vec3& pos, const glm::vec3& target)
{
	glm::vec3 forward = target - pos;
	float len = glm::length(forward);
	if (len < 1e-6f) {
		// targetPos == pos 인 경우, 기본 +Z 축으로
		forward = glm::vec3(0, 0, 1);
	}
	else {
		forward /= len;
	}

	// World up 우선 사용
	glm::vec3 worldUp(0, 1, 0);
	// forward와 worldUp이 평행/반평행이면 다른 up 벡터 선택
	if (std::abs(glm::dot(forward, worldUp)) > 0.999f) {
		worldUp = glm::vec3(1, 0, 0);
	}

	glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
	glm::vec3 up = glm::cross(forward, right);

	// GLM은 열 우선(col-major)로 저장되므로, 각 열에 기저 벡터를 할당
	glm::mat4 R(1.0f);
	R[0] = glm::vec4(right, 0.0f); // X축
	R[1] = glm::vec4(up, 0.0f); // Y축
	R[2] = glm::vec4(forward, 0.0f); // Z축 (+Z가 바라보는 방향)

	glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	return T * R; // 모델 행렬(위치 * 방향 정렬)
}

void QuadricShape::draw2(GLuint shaderProgram, DrawType drawType) const
{
	glUseProgram(shaderProgram);

	glm::mat4 model = buildLookAtOrientation(pos, targetPos);

	if (type == QuadricType::CYLINDER || type == QuadricType::DISK) {
		model *= glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
	glUniform3f(colorLoc, color[0], color[1], color[2]);

	if (drawType == DrawType::DRAW_SOLID)		gluQuadricDrawStyle(obj, GLU_FILL);
	else if (drawType == DrawType::DRAW_WIRE)		gluQuadricDrawStyle(obj, GLU_LINE);
	else if (drawType == DrawType::DRAW_SILHOUETTE)	gluQuadricDrawStyle(obj, GLU_SILHOUETTE);
	else if (drawType == DrawType::DRAW_POINT)		gluQuadricDrawStyle(obj, GLU_POINT);

	
	if (type == QuadricType::SPHERE)
	{
		gluSphere(obj, radius, slices, stacks);
	}
	else if (type == QuadricType::CYLINDER)
	{
		float length = sqrt(pow(targetPos.x - pos.x, 2) + pow(targetPos.y - pos.y, 2) + pow(targetPos.z - pos.z, 2));
		gluCylinder(obj, radius, radius, length, slices, stacks);
		
		glm::mat4 top = model * glm::translate(glm::mat4(1.0f), glm::vec3(0,0, length));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(top));
		gluDisk(obj, 0.0, radius, slices, stacks);
	}
	else if (type == QuadricType::DISK)
	{
		gluDisk(obj, 0.0, radius, slices, stacks);
	}
}

void QuadricShape::move(float x, float y, float z, bool changeTargetPos)
{
	glm::vec3 delta(x, y, z);
	pos += delta;
	if (changeTargetPos) {
		targetPos += delta;
	}
}

void Ball::update()
{
	accel += gravity * 0.01f;

	if (dir == DOWN_RIGHT)
	{
		pos.x += accel * 0.01f;
		pos.y += accel * 0.01f;
	}
	else if (dir == UP_RIGHT)
	{
		pos.x += accel * 0.01f;
		pos.y += -accel * 0.01f;
	}
	else if (dir == UP_LEFT)
	{
		pos.x += -accel * 0.01f;
		pos.y += -accel * 0.01f;
	}
	else if (dir == DOWN_LEFT)
	{
		pos.x += -accel * 0.01f;
		pos.y += accel * 0.01f;
	}

	// 바닥/벽 충돌 처리
	if (pos.y < -4.5f / ROOT2)
	{
		pos.y = -4.5f / ROOT2;
		accel = accel * bounceFactor;
		if (std::abs(accel) < 0.1f) {
			accel = 0.0f;
		}
		dir = static_cast<BallDirection>((dir + 1) % 4);
	}
	if (pos.y > 4.5f / ROOT2)
	{
		pos.y = 4.5f / ROOT2;
		accel = accel * bounceFactor;
		if (std::abs(accel) < 0.1f) {
			accel = 0.0f;
		}
		dir = static_cast<BallDirection>((dir + 1) % 4);
	}
	if (pos.x < -4.5f / ROOT2)
	{
		pos.x = -4.5f / ROOT2;
		accel = accel * bounceFactor;
		if (std::abs(accel) < 0.1f) {
			accel = 0.0f;
		}
		dir = static_cast<BallDirection>((dir + 1) % 4);
	}
	if (pos.x > 4.5f / ROOT2)
	{
		pos.x = 4.5f / ROOT2;
		accel = accel * bounceFactor;
		if (std::abs(accel) < 0.1f) {
			accel = 0.0f;
		}
		dir = static_cast<BallDirection>((dir + 1) % 4);
	}

	// Ball은 이동만 하므로, 기본적으로 targetPos를 진행 방향으로 두고 싶다면
	// 예시: 항상 +Z로 1만큼 앞을 보게
	// setTargetPos(pos + glm::vec3(0,0,1));
}

void Box::move(float x, float y, float z, bool changeTargetPos)
{
	for (int i{}; i < 6; ++i)
	{
		glm::vec3 delta(x, y, z);
		pos += delta;
		if (changeTargetPos)
			targetPos += delta;
	}
}

void Box::update()
{
	return;
}

void Box::draw2(GLuint shaderProgram, DrawType drawType) const
{
	glUseProgram(shaderProgram);

	for (int i{}; i < 6; ++i)
	{
		glm::mat4 model = buildLookAtOrientation(box[i]->getPos(), box[i]->getTargetPos());
		model *= glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint colorLoc = glGetUniformLocation(shaderProgram, "obj_color");
		float* color = box[i]->getColor();
		glUniform3f(colorLoc, color[0], color[1], color[2]);

		if (drawType == DrawType::DRAW_SOLID)		gluQuadricDrawStyle(box[i]->getObj(), GLU_FILL);
		else if (drawType == DrawType::DRAW_WIRE)		gluQuadricDrawStyle(box[i]->getObj(), GLU_LINE);
		else if (drawType == DrawType::DRAW_SILHOUETTE)	gluQuadricDrawStyle(box[i]->getObj(), GLU_SILHOUETTE);
		else if (drawType == DrawType::DRAW_POINT)		gluQuadricDrawStyle(box[i]->getObj(), GLU_POINT);

		gluDisk(box[i]->getObj(), 0.0, radius, slices, stacks);
	}
	
}
