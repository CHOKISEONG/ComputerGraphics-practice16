#include "Camera.h"

void Camera::settingCamera(GLuint shaderProgram, std::string str)
{
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

	if (str == std::string("perspective")) // 원근 투영
	{
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(pos, direction, up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(fovy, width / height, zNear, zFar);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
	else if (str == std::string("ortho_xz")) // 직각 투영 (xz 평면)
	{
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(orthoXZ_pos, orthoXZ_direction, orthoXZ_up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -1.0f, 50.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
	else if (str == std::string("ortho_xy")) // 직각 투영 (xy 평면)
	{
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(orthoXY_pos, orthoXY_direction, orthoXY_up);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -1.0f, 50.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
	
}

void Camera::update()
{
	if (isRotating)
	{
		if (isRotateSelf)
			rotate(rotateSpeed, false);
		else
			rotateFromView(rotateSpeed, false);
	}
		
}

void Camera::rotate(float theta, bool rotateX)
{
	float rad = glm::radians(theta);

	glm::vec3 axis;
	if (rotateX)
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	else
		axis = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 look = direction - pos;
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rad, axis);
	look = glm::vec3(rotation * glm::vec4(look, 1.0f));
	direction = pos + look;
}

void Camera::rotateFromView(float theta, bool rotateX)
{
	float rad = glm::radians(theta);

	glm::vec3 axis;
	if (rotateX)
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	else
		axis = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rad, axis);
	pos = glm::vec3(rotation * glm::vec4(pos, 1.0f));
}
