#include "Camera.h"

void Camera::settingCamera(GLuint shaderProgram)
{
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(pos, direction, up);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(fovy, width / height, zNear, zFar);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
