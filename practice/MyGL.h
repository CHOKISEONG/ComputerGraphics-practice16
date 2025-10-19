/// --------------------
///         MyGL
/// 
/// my OpenGL class
/// --------------------
#pragma once
#include <iostream>
#include <Windows.h> // for Sleep()
#include <stdlib.h>
#include <stdio.h>
#include <format>
#include <cmath>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Shape.h"

class MyGL
{
public:
	static MyGL* getInstance() { return my; }

	static void make_vertexShaders();
	static void make_fragmentShaders();
	static void make_shaderProgram();
	static char* filetobuf(const char* file);
	static void draw();
	static void reShape(int w, int h);
	static void motion(int x, int y);
	static void passiveMotion(int x, int y);
	static void idle();
	static void mouse(int button, int state, int x, int y);
	static void keyboard(unsigned char key, int x, int y);
	static void specialKeyboard(int key, int x, int y);
	

	void run(int argc, char** argv);

	int screenWidth() { return width; }
	int screenHeight() { return height; }
	void screenWResize(int width) { my->width = width; }
	void screenHResize(int height) { my->height = height; }


private:
	static MyGL* my;
	GLuint width = 1200, height = 800;
	GLchar* vertexSource, * fragmentSource;
	GLuint vertexShader, fragmentShader;
	GLuint shaderProgramID;
};
