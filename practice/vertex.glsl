#version 330 core

layout (location = 0) in vec3 in_position;

uniform mat4 model;
uniform mat4 model_transform;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * view * model_transform * model * vec4(in_position, 1.0);
}