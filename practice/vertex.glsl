#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
out vec3 out_color;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main(void)
{
	gl_Position = projection_transform * view_transform * model_transform * vec4(in_position, 1.0);
	out_color = in_color;
}