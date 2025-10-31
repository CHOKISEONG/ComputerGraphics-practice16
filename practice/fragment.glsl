#version 330 core

uniform vec3 obj_color;

out vec4 FragColor;

void main(void)
{
	FragColor = vec4(obj_color, 1.0);
}