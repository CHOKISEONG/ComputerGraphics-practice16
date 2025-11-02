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
std::vector<QuadricShape*> box; 
std::vector<QuadricShape*> ball;
std::vector<PolygonShape*> cube;
std::vector<GLfloat*> color;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float rotateAmount = 0.0f;

void MyGL::reShape(int w, int h)
{
	my->height = h;
	my->width = w;
	glViewport(0, 0, my->width, my->height);
}
void MyGL::idle()
{
	cam->update();

	for (auto& ball : ball)
		ball->update();

	for (auto& c : cube)
		c->update();

	glutPostRedisplay();
}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);
	cam->settingCamera(my->shaderProgramID);

	for (auto& b : box)
		b->draw2(my->shaderProgramID, DrawType::DRAW_WIRE);

	for (auto& b : ball)
		b->draw2(my->shaderProgramID, DrawType::DRAW_WIRE);

	glutSwapBuffers();
}
void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	switch (key)
	{
	case'z':
	{
		break;
	}
	case'Z':
	{
		break;
	}
	case'y':
	{
		cam->rotateStart(0.1f);
		break;
	}
	case'Y':
	{
		cam->rotateStart(-0.1f);
		break;
	}
	case'B':
	{
		if (ball.size() < 5)
		{
			ball.push_back(new Ball(QuadricType::SPHERE, 0.5, 0.0, randBallPosX(gen), randBallPosY(gen), randBallPosZ(gen)));
			std::cout << "make Ball\n";
		}
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
void MyGL::passiveMotion(int x, int y)
{
	float crx = (2.0f * x - my->width) / my->width;
	float cry = -(2.0f * y - my->height) / my->height;

	//cam->move((crx - pvx) * 40, (cry - pvy) * 40);

	// 위 아래는 rotateY
	if (crx - pvx > 0.0f)
	{
		if (rotateAmount < 60.0f)
		{
			rotateAmount += 0.3f;
			for (auto& o : box)
				o->rotateZ(0.3f);
		}
	}
	else if (crx - pvx < 0.1f)
	{
		if (rotateAmount > -60.0f)
		{
			rotateAmount -= 0.3f;
			for (auto& o : box)
				o->rotateZ(-0.3f);
		}
	}

	pvx = crx;
	pvy = cry;
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

	cam = new Camera();
	//cam->rotateStartY(1.0f);

	double radius = 5.0;

	// 뒤쪽 면
	box.push_back(new QuadricShape(QuadricType::DISK, radius));
	box[0]->move(0.0f, 0.0f, ROOT2 * radius);

	box.push_back(new QuadricShape(QuadricType::CYLINDER, radius, 2 * radius, 0.0f, 0.0f, -radius/ROOT2));

	// 상자 뒤쪽 중앙 (0, 0, 7)
	// 상자 왼쪽 중앙(-3.5, 0, 3.5)
	// 상자 오른쪽 중앙(3.5, 0, 3.5)
	// 상자 위쪽 중앙

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(MyGL::draw);
	glutReshapeFunc(MyGL::reShape);
	//glutMouseFunc(MyGL::mouse);
	//glutMotionFunc(MyGL::motion);
	glutPassiveMotionFunc(MyGL::passiveMotion);
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

