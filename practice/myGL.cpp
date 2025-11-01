#define _CRT_SECURE_NO_WARNINGS 

#include "MyGL.h"
#include "global.h"

#include "Camera.h"
#include "PolygonShape.h"
#include "QuadricShape.h"
#include "Points.h"

MyGL* MyGL::my = nullptr;

Camera* cam;
/// <summary>
/// 그려질 도형들을 모아놓는 곳
/// </summary>
std::vector<QuadricShape*> obj; // GLU 모델들
std::vector<GLfloat*> color;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void MyGL::reShape(int w, int h)
{
	my->height = h;
	my->width = w;
	glViewport(0, 0, my->width, my->height);
}
void MyGL::idle()
{
	for (auto& o : obj)
	{
		o->update();
	}
	glutPostRedisplay();
}
void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	switch (key)
	{
	case'w':
	{
		cam->move(0.0f, 1.0f);
		break;
	}
	case'a':
	{
		cam->move(-1.0f, 0.0f);
		break;
	}
	case's':
	{
		cam->move(0.0f, -1.0f);
		break;
	}
	case'd':
	{
		cam->move(1.0f, 0.0f);
		break;
	}
	case'e':
	{
		cam->move(0.0f, 0.0f, 1.0f);
		break;
	}
	case'E':
	{
		cam->move(0.0f, 0.0f, -1.0f);
		break;
	}
	case'r':
	{
		break;
	}
	case'R':
	{
		break;
	}
	case'z':
	{
		break;
	}
	case 'q':
		exit(0);
	default: break;
	}

	glutPostRedisplay();
}
void MyGL::specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	case GLUT_KEY_LEFT:

		break;
	case GLUT_KEY_RIGHT:

		break;
	}
	glutPostRedisplay();
}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);
	cam->settingCamera(my->shaderProgramID);

	for (int i{}; i < obj.size(); ++i)
	{
		obj[i]->draw2(my->shaderProgramID, DRAW_WIRE);
	}

	glutSwapBuffers();
}
//void MyGL::passiveMotion(int x, int y)
//{
//	pvx = (2.0f * x - my->width) / my->width;
//	pvy = -(2.0f * y - my->height) / my->height;
//}
//void MyGL::motion(int x, int y)
//{
//	// 마우스 좌표를 openGL 좌표계로 변환
//	float crx = (2.0f * x - my->width) / my->width;
//	float cry = -(2.0f * y - my->height) / my->height;
//
//	pvx = crx;
//	pvy = cry;
//
//	glutPostRedisplay();
//}
void MyGL::run(int argc, char** argv)
{
	my = this;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(my->width, my->height);
	glutCreateWindow("openGL practice 16");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_shaderProgram();

	cam = new Camera();

	obj.push_back(new QuadricShape(CYLINDER, 50.0f, 0.1f));
	obj[0]->rotateX(90.0f);
	obj.push_back(new QuadricShape(CYLINDER));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(MyGL::draw);
	glutReshapeFunc(MyGL::reShape);
	//glutMouseFunc(MyGL::mouse);
	//glutMotionFunc(MyGL::motion);
	//glutPassiveMotionFunc(MyGL::passiveMotion);
	glutKeyboardFunc(MyGL::keyboard);
	glutSpecialFunc(MyGL::specialKeyboard);
	glutIdleFunc(MyGL::idle);
	glutMainLoop();
}
void MyGL::make_shaderProgram()
{
	make_vertexShaders();
	make_fragmentShaders();

	my->shaderProgramID = glCreateProgram();

	glAttachShader(my->shaderProgramID, my->vertexShader);
	glAttachShader(my->shaderProgramID, my->fragmentShader);
	glLinkProgram(my->shaderProgramID);

	glDeleteShader(my->vertexShader);
	glDeleteShader(my->fragmentShader);

	glUseProgram(my->shaderProgramID);
}
void MyGL::make_vertexShaders()
{
	my->vertexSource = filetobuf("vertex.glsl");
	my->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(my->vertexShader, 1, (const GLchar**)&my->vertexSource, NULL);
	glCompileShader(my->vertexShader);

	// 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(my->vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(my->vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
	else
		std::cout << "vertex shader 컴파일 성공\n";
}
void MyGL::make_fragmentShaders()
{
	my->fragmentSource = filetobuf("fragment.glsl");
	my->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(my->fragmentShader, 1, (const GLchar**)&my->fragmentSource, NULL);
	glCompileShader(my->fragmentShader);

	// 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(my->fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(my->fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
	else
		std::cout << "fragment shader 컴파일 성공\n";
}
char* MyGL::filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

