#define _CRT_SECURE_NO_WARNINGS 

#include "MyGL.h"
#include "global.h"

#include "PolygonShape.h"
#include "Points.h"

MyGL* MyGL::my = nullptr;

/// <summary>
/// 그려질 도형들을 모아놓는 변수
/// </summary>
/// 좌클릭 - 정육면체 그리기, 우클릭 - 사각뿔그리기
PolygonShape* cube;
PolygonShape* squarePyramid;
std::vector<PolygonShape*> lines;
bool isBackFaceCulling = false;
char trigger;

void MyGL::reShape(int w, int h)
{
	my->height = h;
	my->width = w;
	glViewport(0, 0, my->width, my->height);
}
void MyGL::idle()
{
	
	glutPostRedisplay();
}

void MyGL::keyboard(unsigned char key, int x, int y)
{
	trigger = key;
	switch (key)
	{
	case'c':
		// 육면체 띄우기
		break;
	case'p':
		// 사각뿔 띄우기
		break;
	case'h':
		// 은면제거 적용/해제
		isBackFaceCulling = !isBackFaceCulling;
		if (isBackFaceCulling)
		{
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case'w':
		break;
	case'W':
		break;
	case'x':
		break;
	case'X':
		break;
	case'y':
		break;
	case'Y':
		break;
	case's':
		break;
	case 't':
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		cube->drawNone();
		squarePyramid->drawNone();
		squarePyramid->drawSomething(uid(gen) % 4);
		squarePyramid->drawSomething(4);
		break;
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
		cube->move(0.0f, 0.05f);
		squarePyramid->move(0.0f, 0.05f);
		break;
	case GLUT_KEY_DOWN:
		cube->move(0.0f, -0.05f);
		squarePyramid->move(0.0f, -0.05f);
		break;
	case GLUT_KEY_LEFT:
		cube->move(-0.05f, 0.0f);
		squarePyramid->move(-0.05f, 0.0f);
		break;
	case GLUT_KEY_RIGHT:
		cube->move(0.05f, 0.0f);
		squarePyramid->move(0.05f, 0.0f);
		break;
	}
	glutPostRedisplay();
}
void MyGL::mouse(int button, int state, int x, int y)
{
	float crx = (2.0f * x - my->width) / my->width;
	float cry = -(2.0f * y - my->height) / my->height;

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		cube->drawAll();
		squarePyramid->drawNone();
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
	{
		squarePyramid->drawAll();
		cube->drawNone();
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	glutPostRedisplay();
}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);

	
	glm::mat4 model = glm::mat4(1.0f);
	// x축 30도, y축 -30도 회전해서 그리기
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(my->shaderProgramID, "model_transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	for (const auto& l : lines)
		l->draw(my->shaderProgramID);
	cube->draw(my->shaderProgramID);
	squarePyramid->draw(my->shaderProgramID);
	
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

	float xLine[6]{ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	float yLine[6]{ 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float zLine[6]{ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };
	lines.push_back(new PolygonShape(PolygonType::LINE, xLine));
	lines.push_back(new PolygonShape(PolygonType::LINE, yLine));
	lines.push_back(new PolygonShape(PolygonType::LINE, zLine));
	lines[0]->setColor(1.0f, 0.0f, 0.0f);
	lines[1]->setColor(0.0f, 1.0f, 0.0f);
	lines[2]->setColor(0.0f, 0.0f, 1.0f);

	cube = new PolygonShape();

	float sp[15]{ 0.0f,0.5f,0.0f
			,0.2f,-0.2f,0.2f
			,-0.2f,-0.2f,0.2f
			,-0.2f,-0.2f,-0.2f
			,0.2f,-0.2f,-0.2f };
	squarePyramid = new PolygonShape(PolygonType::SQUAREPYRAMID, sp);
	squarePyramid->drawNone();

	

	glutDisplayFunc(MyGL::draw);
	glutReshapeFunc(MyGL::reShape);
	glutMouseFunc(MyGL::mouse);
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

