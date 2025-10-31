#define _CRT_SECURE_NO_WARNINGS 

#include "MyGL.h"
#include "global.h"

#include "PolygonShape.h"
#include "Points.h"

MyGL* MyGL::my = nullptr;

/// <summary>
/// �׷��� �������� ��Ƴ��� ��
/// </summary>
std::vector<PolygonShape*> axis; // x,y,z ��
std::vector<PolygonShape*> shapes;				 // ť��
std::vector<PolygonShape*> orgShapes;

float d = 0.2f; // ť���� �ʱ� ������
float cubePos[24]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d
					,d,d,-d,-d,d,-d,-d,-d,-d,d,-d,-d };
float pyramidPos[15]{ 0.0f,0.5f,0.0f
			,0.2f,-0.2f,0.2f
			,-0.2f,-0.2f,0.2f
			,-0.2f,-0.2f,-0.2f
			,0.2f,-0.2f,-0.2f };

glm::vec3 cameraPos = glm::vec3(0.0, 0.0f, 5.0f);
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
	for (auto& s : shapes)
		s->update();
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
		return;
	}
	case '8':
	{
		cameraPos.z -= 0.1;
		return;
	}
	case'1': {
		// ���� ��ü�� ����
		std::cout << "press 1\n";
		for (int i{}; i < shapes.size(); ++i)
		{
			if (i % 2)
				shapes[i]->stopMove();
			else
				shapes[i]->startMove();
		}
		break;
	}
	case'2': {
		// ������ ��ü�� ����
		std::cout << "press 2\n";
		for (int i{}; i < shapes.size(); ++i)
		{
			if (i % 2)
				shapes[i]->startMove();
			else
				shapes[i]->stopMove();
		}
		break;
	}
	case'3': {
		// �� �� ����
		std::cout << "press 3\n";
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startMove();
		}
		break;
	}
	case'd': {
		// move objects's x
		for (int i{}; i < shapes.size(); ++i)
		{
			if (shapes[i]->getCanMove())
				shapes[i]->move(-0.05f, 0.0f);
		}
		break;
	}
	case'D': {
		// move objects's x
		for (int i{}; i < shapes.size(); ++i)
		{
			if (shapes[i]->getCanMove())
				shapes[i]->move(0.05f, 0.0f);
		}
		break;
	}
	case'e': {
		// move object's y
		for (int i{}; i < shapes.size(); ++i)
		{
			if (shapes[i]->getCanMove())
				shapes[i]->move(0.0f, -0.05f);
		}
		break;
	}
	case'E': {
		// move object's y
		for (int i{}; i < shapes.size(); ++i)
		{
			if (shapes[i]->getCanMove())
				shapes[i]->move(0.0f, 0.05f);
		}
		break;
	}
	case'x':
	{
		// rotate object's x axis +
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->changeSpeed(1.0f, 0.0f);
		}
		break;
	}
	case'X': 
	{
		// rotate object's x axis -
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->changeSpeed(-1.0f, 0.0f);
		}
		break;
	}
	case'y':
	{
		// rotate object's y axis +
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->changeSpeed(0.0f, 1.0f);
		}
		break;
	}
	case'Y': 
	{
		// rotate object's y axis -
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->changeSpeed(0.0f, -1.0f);
		}
		break;
	}
	case'r': 
	{
		// rotate zeroPoint's y axis + -
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startYRotate(0.01f);
		}
		break;
	}
	case'R': {
		// rotate zeroPoint's y axis + -
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startYRotate(-0.01f);
		}
		break;
	}
	case'a': {
		// scale
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startIncrease(0.001f, false);
		}
		break;
	}
	case'A': {
		// scale
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startDecrease(0.001f, false);
		}
		break;
	}
	case'b': {
		// ������ ���� Ȯ��/���
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startIncrease(0.001f, true);
		}
		break;
	}
	case'B': {
		// ������ ���� Ȯ��/���
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->startDecrease(0.001f, true);
		}
		break;
	}
	case 't': {
		// �� ������ ������ ����ϸ� ������ �ڸ��� �̵��ϴ� �ִϸ��̼�
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->stopAll();
		}
		shapes[0]->startMoveT(shapes[1]->getMidpoint());
		shapes[1]->startMoveT(shapes[0]->getMidpoint());
		break;
	}
	case 'u': {
		// �� ������ �Ѱ��� ���� �Ѱ��� �Ʒ��� �̵��ϸ鼭 ������ �ڸ��� �̵��ϴ� �ִϸ��̼�
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->stopAll();
		}
		shapes[0]->startMoveU(shapes[1]->getMidpoint());
		shapes[1]->startMoveU(shapes[0]->getMidpoint());
		break;
	}
	case 'v': {
		// �������� ��
		shapes[0]->startMoveV(true);
		shapes[1]->startMoveV(false);
		break;
	}
	case 'c': {
		// �� ������ �ٸ��ŷ� �ٲٱ�
		for (int i{}; i < shapes.size(); ++i)
		{
			shapes[i]->change();
		}
		break;
	}
	case's': {
		//�ʱ�ȭ
		for (int i{}; i < shapes.size(); ++i)
		{
			*shapes[i] = std::move(*orgShapes[i]);
			shapes[i]->updateVbo();
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

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
	unsigned int projectionLocation = glGetUniformLocation(my->shaderProgramID, "projection_transform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(my->shaderProgramID, "view_transform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
 	unsigned int modelLocation = glGetUniformLocation(my->shaderProgramID, "model_transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	for (const auto& l : axis)
		l->draw(my->shaderProgramID);
	for (const auto& s : shapes)
		s->draw(my->shaderProgramID);

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

	// x y z �� ����
	float xLine[6]{ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	float yLine[6]{ 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float zLine[6]{ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };
	axis.push_back(new PolygonShape(PolygonType::LINE, xLine));
	axis.push_back(new PolygonShape(PolygonType::LINE, yLine));
	axis.push_back(new PolygonShape(PolygonType::LINE, zLine));
	axis[0]->setColor(1.0f, 0.0f, 0.0f);
	axis[1]->setColor(0.0f, 1.0f, 0.0f);
	axis[2]->setColor(0.0f, 0.0f, 1.0f);
	
	shapes.push_back(new PolygonShape(PolygonType::CUBE, cubePos));
	orgShapes.push_back(new PolygonShape(PolygonType::CUBE, cubePos));
	shapes[0]->move(-0.5f, 0.0f);
	orgShapes[0]->move(-0.5f, 0.0f);

	shapes.push_back(new PolygonShape(PolygonType::SQUAREPYRAMID, pyramidPos));
	orgShapes.push_back(new PolygonShape(PolygonType::SQUAREPYRAMID, pyramidPos));
	shapes[1]->move(0.5f, 0.0f);
	orgShapes[1]->move(0.5f, 0.0f);
	

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

