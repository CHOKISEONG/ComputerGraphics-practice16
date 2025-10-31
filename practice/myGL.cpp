#define _CRT_SECURE_NO_WARNINGS 

#include "MyGL.h"
#include "global.h"

#include "PolygonShape.h"
#include "QuadricShape.h"
#include "Points.h"

MyGL* MyGL::my = nullptr;

/// <summary>
/// �׷��� �������� ��Ƴ��� ��
/// </summary>
std::vector<QuadricShape*> obj; // GLU �𵨵�
std::vector<GLfloat*> color;

//float d = 0.2f; // ť���� �ʱ� ������
//float cubePos[24]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d
//					,d,d,-d,-d,d,-d,-d,-d,-d,d,-d,-d };
//float pyramidPos[15]{ 0.0f,0.5f,0.0f
//			,0.2f,-0.2f,0.2f
//			,-0.2f,-0.2f,0.2f
//			,-0.2f,-0.2f,-0.2f
//			,0.2f,-0.2f,-0.2f };

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
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
	glutPostRedisplay();
}

void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	switch (key)
	{
	case '7':
	{
		cameraPos.z += 0.1;
		break;
	}
	case '8':
	{
		cameraPos.z -= 0.1;
		break;
	}
	case '9':
	{
		for (auto& o : obj)
		{
			o->move(1.0f, 0.0f, 0.0f);
		}

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
	// ���콺 ��ǥ�� openGL ��ǥ��� ��ȯ
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
	
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE, 1.5f, 20, 20));
	color.push_back(new float[3]{zeroToOne(gen),zeroToOne(gen),zeroToOne(gen)});
	obj.push_back(new QuadricShape(gluNewQuadric(), QuadricType::SPHERE));
	color.push_back(new float[3] {zeroToOne(gen), zeroToOne(gen), zeroToOne(gen)});
	obj[1]->move(0.0f, 1.0f);

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

	// ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(my->vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(my->vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
	else
		std::cout << "vertex shader ������ ����\n";
}
void MyGL::make_fragmentShaders()
{
	my->fragmentSource = filetobuf("fragment.glsl");
	my->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(my->fragmentShader, 1, (const GLchar**)&my->fragmentSource, NULL);
	glCompileShader(my->fragmentShader);

	// ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(my->fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(my->fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
		return;
	}
	else
		std::cout << "fragment shader ������ ����\n";
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

