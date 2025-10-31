#define _CRT_SECURE_NO_WARNINGS 

#include "MyGL.h"
#include "global.h"

#include "PolygonShape.h"
#include "QuadricShape.h"
#include "Points.h"

MyGL* MyGL::my = nullptr;

/// <summary>
/// 그려질 도형들을 모아놓는 곳
/// </summary>
std::vector<QuadricShape*> obj; // GLU 모델들
std::vector<GLfloat*> color;

//float d = 0.2f; // 큐브의 초기 반지름
//float cubePos[24]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d
//					,d,d,-d,-d,d,-d,-d,-d,-d,d,-d,-d };
//float pyramidPos[15]{ 0.0f,0.5f,0.0f
//			,0.2f,-0.2f,0.2f
//			,-0.2f,-0.2f,0.2f
//			,-0.2f,-0.2f,-0.2f
//			,0.2f,-0.2f,-0.2f };

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool isOrtho = false; // 직각투영/원근투영 변환용

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
		o->revolution();
	}
	glutPostRedisplay();
}

void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	switch (key)
	{
	case'p':
	{
		isOrtho = true;
		break;
	}
	case'P':
	{
		isOrtho = false;
		break;
	}
	case'm':
	{
		for (auto& o : obj)
		{
			o->toSolid();
		}
		break;
	}
	case'M':
	{
		for (auto& o : obj)
		{
			o->toWire();
		}
		break;
	}
	case'w':
	{
		for (auto& o : obj)
		{
			o->move(0.0f,0.5f,0.0f, true);
		}
		break;
	}
	case'a':
	{
		for (auto& o : obj)
		{
			o->move(-0.5f, 0.0f, 0.0f, true);
		}
		break;
	}
	case's':
	{
		for (auto& o : obj)
		{
			o->move(0.0f, -0.5f, 0.0f, true);
		}
		break;
	}
	case'd':
	{
		for (auto& o : obj)
		{
			o->move(0.5f, 0.0f, 0.0f, true);
		}
		break;
	}
	case'+':
	{
		for (auto& o : obj)
		{
			o->move(0.0f, 0.0f, 0.5f, true);
		}
		break;
	}
	case'-':
	{
		for (auto& o : obj)
		{
			o->move(0.0f, 0.0f, -0.5f, true);
		}
		break;
	}
	case'y':
	{
		for (auto& o : obj)
		{
			o->resizeOrbit(0.01f);
		}
		break;
	}
	case'Y':
	{
		for (auto& o : obj)
		{
			o->resizeOrbit(-0.01f);
		}
		break;
	}
	case'z':
	{

		break;
	}
	case'Z':
	{

		break;
	}
	case 'q':
		exit(0);
	default: break;
	}

	glutPostRedisplay();
}
//void MyGL::specialKeyboard(int key, int x, int y)
//{
//	switch (key)
//	{
//	case GLUT_KEY_UP:
//		cube->move(0.0f, 0.05f);
//		pyramid->move(0.0f, 0.05f);
//		break;
//	case GLUT_KEY_DOWN:
//		cube->move(0.0f, -0.05f);
//		pyramid->move(0.0f, -0.05f);
//		break;
//	case GLUT_KEY_LEFT:
//		cube->move(-0.05f, 0.0f);
//		pyramid->move(-0.05f, 0.0f);
//		break;
//	case GLUT_KEY_RIGHT:
//		cube->move(0.05f, 0.0f);
//		pyramid->move(0.05f, 0.0f);
//		break;
//	}
//	glutPostRedisplay();
//}
//void MyGL::mouse(int button, int state, int x, int y)
//{
//	float crx = (2.0f * x - my->width) / my->width;
//	float cry = -(2.0f * y - my->height) / my->height;
//
//	
//
//	glutPostRedisplay();
//}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);

	unsigned int modelLoc = glGetUniformLocation(my->shaderProgramID, "model");
	unsigned int viewLoc = glGetUniformLocation(my->shaderProgramID, "view");
	unsigned int projLoc = glGetUniformLocation(my->shaderProgramID, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);

	if (isOrtho)
		projection = glm::ortho(-20.0f,20.0f, -20.0f,20.0f, -50.0f,50.0f);
	else
		projection = glm::perspective(glm::radians(45.0f), (float)my->width / (float)my->height, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	for (int i{}; i < obj.size(); ++i)
	{
		obj[i]->draw(my->shaderProgramID);
	}

	glutSwapBuffers();
}

void MyGL::passiveMotion(int x, int y)
{
	pvx = (2.0f * x - my->width) / my->width;
	pvy = -(2.0f * y - my->height) / my->height;
}
void MyGL::motion(int x, int y)
{
	// 마우스 좌표를 openGL 좌표계로 변환
	float crx = (2.0f * x - my->width) / my->width;
	float cry = -(2.0f * y - my->height) / my->height;

	pvx = crx;
	pvy = cry;

	glutPostRedisplay();
}
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
	
	// 중심 구
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE, 1.5f));
	color.push_back(new float[3]{zeroToOne(gen),zeroToOne(gen),zeroToOne(gen)});

	// 중심 구를 기준으로 공전하는 행성 3개
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE));
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE));
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE));
	color.push_back(new float[3] {zeroToOne(gen), zeroToOne(gen), zeroToOne(gen)});
	color.push_back(new float[3] {zeroToOne(gen), zeroToOne(gen), zeroToOne(gen)});
	color.push_back(new float[3] {zeroToOne(gen), zeroToOne(gen), zeroToOne(gen)});
	obj[1]->move(5.0f, 0.0f);
	obj[2]->move(-3.0f, -3.0f);
	obj[3]->move(3.5f, -3.5f);
	obj[1]->setAxis();
	obj[2]->setAxis();
	obj[3]->setAxis();
	obj[1]->makeChildren();
	obj[2]->makeChildren();
	obj[3]->makeChildren();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(MyGL::draw);
	glutReshapeFunc(MyGL::reShape);
	//glutMouseFunc(MyGL::mouse);
	//glutMotionFunc(MyGL::motion);
	//glutPassiveMotionFunc(MyGL::passiveMotion);
	glutKeyboardFunc(MyGL::keyboard);
	//glutSpecialFunc(MyGL::specialKeyboard);
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

