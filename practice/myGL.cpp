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
QuadricShape* ground;
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
	cam->update();
	for (auto& o : obj)
	{
		o->update();
	}
	glutPostRedisplay();
}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);
	cam->settingCamera(my->shaderProgramID);

	ground->draw2(my->shaderProgramID, DrawType::DRAW_SOLID);
	for (int i{}; i < obj.size(); ++i)
	{
		obj[i]->draw2(my->shaderProgramID, DrawType::DRAW_SOLID);
	}

	glutSwapBuffers();
}
void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	switch (key)
	{
	case'z':
	{
		cam->move(0.0f, 0.0f, 1.0f);
		break;
	}
	case'Z':
	{
		cam->move(0.0f, 0.0f, -1.0f);
		break;
	}
	case'x':
	{
		cam->move(1.0f, 0.0f);
		break;
	}
	case'X':
	{
		cam->move(-1.0f, 0.0f);
		break;
	}
	case'y':
	{
		cam->rotateStartY(1.0f);
		break;
	}
	case'Y':
	{
		cam->rotateStartY(-1.0f);
		break;
	}
	case'r':
	{
		cam->rotateFromView(5.0f, false);
		break;
	}
	case'R':
	{
		cam->rotateFromView(5.0f, false);
		break;
	}
	case 'a':
	{
		cam->rotateStart(1.0f);
		break;
	}
	case'o':
	{
		cam->rotateStop();
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
		for (auto& o : obj)
		{
			o->move(0.0f, 0.0f, -1.0f);
		}
		break;
	case GLUT_KEY_DOWN:
		for (auto& o : obj)
		{
			o->move(0.0f, 0.0f, 1.0f);
		}
		break;
	case GLUT_KEY_LEFT:
		for (auto& o : obj)
		{
			o->move(-1.0f, 0.0f, 0.0f);
		}
		break;
	case GLUT_KEY_RIGHT:
		for (auto& o : obj)
		{
			o->move(1.0f, 0.0f, 0.0f);
		}
		break;
	}
	glutPostRedisplay();
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
	ground = new QuadricShape(DISK, 50.0f, 0.1f);
	ground->setColor(0.55f, 0.47f, 0.05f);
	ground->rotateX(90.0f);

	// 탱크 아래 몸체 (idx 0~2)
	obj.push_back(new QuadricShape(CYLINDER));
	obj.push_back(new QuadricShape(DISK));
	obj.push_back(new QuadricShape(DISK));
	obj[1]->setColor(obj[0]->getColor());
	obj[2]->setColor(obj[0]->getColor());
	obj[1]->move(0.0f, 0.0f, 5.0f);

	// 탱크 중앙 몸체 (idx 3~5)
	obj.push_back(new QuadricShape(CYLINDER, 1.0, 4.0));
	obj.push_back(new QuadricShape(DISK));
	obj.push_back(new QuadricShape(DISK));
	obj[4]->setColor(obj[3]->getColor());
	obj[5]->setColor(obj[3]->getColor());
	obj[3]->move(0.0f, 1.5f, 0.5f);
	obj[4]->move(0.0f, 1.5f, 0.5f);
	obj[5]->move(0.0f, 1.5f, 4.5f);

	for (auto& o : obj)
	{
		o->rotateZ(45.0f);
	}

	//glEnable(GL_CULL_FACE);
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

