#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Camera& other)
	:pos(other.pos),
	direction(other.direction),
	up(other.up),
	
	orthoXZ_pos(other.orthoXZ_pos),
	orthoXZ_direction(other.orthoXZ_direction),
	orthoXZ_up(other.orthoXZ_up),
	
	orthoXY_pos(other.orthoXY_pos),
	orthoXY_direction(other.orthoXY_direction),
	orthoXY_up(other.orthoXY_up),
	
	fovy(other.fovy),
	width(other.width),
	height(other.height),
	zNear(other.zNear),
	zFar(other.zFar),
	
	isRotating(other.isRotating),
	isRotateSelf(other.isRotateSelf),
	rotateSpeed(other.rotateSpeed)
{
}

Camera& Camera::operator=(const Camera& other)
{
	if (this == &other) return *this;

	pos = other.pos;		
	direction = other.direction;	
	up = other.up;			

	orthoXZ_pos = other.orthoXZ_pos;
	orthoXZ_direction = other.orthoXZ_direction;
	orthoXZ_up = other.orthoXZ_up;

	orthoXY_pos = other.orthoXY_pos;
	orthoXY_direction = other.orthoXY_direction;
	orthoXY_up = other.orthoXY_up;

	fovy = other.fovy;	
	width = other.width;	
	height = other.height;
	zNear = other.zNear;	
	zFar = other.zFar;	

	isRotating = other.isRotating;
	isRotateSelf = other.isRotateSelf;
	rotateSpeed = other.rotateSpeed;

	return *this;
}

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

void Camera::goBack()
{
	pos = glm::vec3(0.0f, 5.0f, 5.0f);		
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);		

	orthoXZ_pos = glm::vec3(0.0f, 5.0f, 0.0f);
	orthoXZ_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	orthoXZ_up = glm::vec3(0.0f, 0.0f, -1.0f);

	orthoXY_pos = glm::vec3(0.0f, 0.0f, 10.0f);
	orthoXY_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	orthoXY_up = glm::vec3(0.0f, 1.0f, 0.0f);

	fovy = 90.0f;		
	width = 1200.0f;	
	height = 800.0f;	
	zNear = 0.1f;		
	zFar = 100.0f;	

	isRotating = false;
	isRotateSelf = false;
	rotateSpeed = 0.0f;
}

void Camera::update()
{
	if (isRotating)
	{
		if (isRotateSelf)
			rotate(rotateSpeed);
		else
			rotateFromView(rotateSpeed);
	}
		
}

void Camera::rotate(float theta)
{
	float rad = glm::radians(theta);

	glm::vec3 look = direction - pos;
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rad, up);
	look = glm::vec3(rotation * glm::vec4(look, 1.0f));
	direction = pos + look;

	look = orthoXZ_direction - orthoXZ_pos;
	rotation = glm::rotate(glm::mat4(1.0f), rad, orthoXZ_up);
	look = glm::vec3(rotation * glm::vec4(look, 1.0f));
	orthoXZ_direction = orthoXZ_pos + look;

	look = orthoXY_direction - orthoXY_pos;
	rotation = glm::rotate(glm::mat4(1.0f), rad, orthoXY_up);
	look = glm::vec3(rotation * glm::vec4(look, 1.0f));
	orthoXY_direction = orthoXY_pos + look;
}

void Camera::rotateFromView(float theta)
{
	float rad = glm::radians(theta);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rad, up);
	pos = glm::vec3(rotation * glm::vec4(pos, 1.0f));

	rotation = glm::rotate(glm::mat4(1.0f), rad, orthoXZ_up);
	orthoXZ_pos = glm::vec3(rotation * glm::vec4(orthoXZ_pos, 1.0f));

	rotation = glm::rotate(glm::mat4(1.0f), rad, orthoXY_up);
	orthoXY_pos = glm::vec3(rotation * glm::vec4(orthoXY_pos, 1.0f));
}
