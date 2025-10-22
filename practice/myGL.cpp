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
std::vector<PolygonShape*> cube;
std::vector<PolygonShape*> pyramid;
std::vector<float> orgPyramidPos;
std::vector<PolygonShape*> lines;
bool isBackFaceCulling = false;
char trigger;
bool whatToDraw = false;
float d = 0.3f; // 큐브의 초기 반지름
bool yTrig = false;
float tmp = 0.0f;
unsigned long long pMoved = 0;
// 원래 큐브 면 마다 위치들
std::vector<float> cube1{ d,d,d,-d,d,d,-d,-d,d,d,-d,d };
std::vector<float> cube2{ -d,d,d,-d,d,-d,-d,-d,-d,-d,-d,d };
std::vector<float> cube3{ d,d,d,d,d,-d,-d,d,-d,-d,d,d };
std::vector<float> cube4{ d,d,-d,d,d,d,d,-d,d,d,-d,-d };
std::vector<float> cube5{ -d,d,-d,d,d,-d,d,-d,-d,-d,-d,-d };
std::vector<float> cube6{ d,-d,-d, -d,-d,-d,-d,-d,d,d,-d,d };
std::vector<float> orgPyr1{ 0.0f,0.5f,0.0f, -0.2f,-0.2f,0.2f, 0.2f,-0.2f,0.2f };
std::vector<float> orgPyr2{ 0.0f,0.5f,0.0f, -0.2f,-0.2f,-0.2f, -0.2f,-0.2f,0.2f };
std::vector<float> orgPyr3{ 0.0f,0.5f,0.0f, 0.2f,-0.2f,-0.2f, -0.2f,-0.2f,-0.2f };
std::vector<float> orgPyr4{ 0.0f,0.5f,0.0f, 0.2f,-0.2f,0.2f, 0.2f,-0.2f,-0.2f };
std::vector<float> orgPyr5{ 0.2f,-0.2f,0.2f, -0.2f,-0.2f,0.2f, -0.2f,-0.2f,-0.2f, 0.2f,-0.2f,-0.2f };
void MyGL::reShape(int w, int h)
{
	my->height = h;
	my->width = w;
	glViewport(0, 0, my->width, my->height);
}
void MyGL::idle()
{
	for (auto& c : cube)
	{
		if (c->getIsMoving())
		{
			c->rotate();
		}

		if (c->getIsRotating())
		{
			c->rotate2();
		}
	}
	
	++pMoved;
	int tmp = (pMoved / 10) % 4;
	for (auto& p : pyramid)
	{
		if (p->getIsMoving())
		{
			p->rotate();
		}

		if (p->getIsRotating())
		{
			p->rotate2();
			std::cout << tmp << "\n";
			if (p->getRotateType() == 6)
			{
				if (tmp == 0)
				{
					pyramid[0]->rotateR(0);
				}
				else if (tmp == 1)
				{
					pyramid[1]->rotateR(1);
				}
				else if (tmp == 2)
				{
					pyramid[2]->rotateR(2);
				}
				else
				{
					pyramid[3]->rotateR(3);
				}
			}
		}
	}

	glutPostRedisplay();
}

void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	trigger = key;
	switch (key)
	{
	// 리셋하고 육면체 출력 (미완성)
	case'c': 
	{
		cube[0]->setPos(cube6);
		cube[1]->setPos(cube5);
		cube[2]->setPos(cube4);
		cube[3]->setPos(cube3);
		cube[4]->setPos(cube2);
		cube[5]->setPos(cube1);
		whatToDraw = false;
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		for (auto& c : cube)
		{
			c->moveDecline();
			c->rotateDecline();
		}
		for (auto& c : cube)
			c->setRotateDir(0);
		for (auto& p : pyramid)
		{
			p->moveDecline();
			p->rotateDecline();
		}
		for (auto& p : pyramid)
			p->setRotateDir(0);
		break;
	}

	// 육면체/사각뿔 버튼 누를 때마다 다르게 출력
	case'p': 
		whatToDraw = !whatToDraw;
		break;

	// 은면제거 적용/해제
	case'h':
	{
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
	}	

	// y축에 대해 자전
	case'y': 
	{
		yTrig = !yTrig;
		break;
	}

	// 윗면이 가운데를 중심으로 돌기
	case't':
	{
		static bool key_t = false;
		key_t = !key_t;
		for (auto& c : cube)
			c->rotateDecline();
		if (key_t)
		{
			cube[3]->rotateAccept();
			cube[3]->setRotateType(1);
		}
		
		break;
	}
	
	// 앞면이 위축을 기준으로 회전하여 열기/닫기
	case'f':
	{
		for (auto& c : cube)
			c->rotateDecline();

		static bool key_f = false;
		key_f = !key_f;
		if (key_f)
		{
			cube[5]->initMoved();
			cube[5]->rotateAccept();
			cube[5]->setRotateType(2);
		}
		break;
	}
	
	// 옆면이 중점을 기준으로 제자리에서 회전/멈추기
	case 's':
	{
		for (auto& c : cube)
			c->rotateDecline();
		static bool key_s = false;
		key_s = !key_s;
		if (key_s)
		{
			cube[2]->rotateAccept();
			cube[4]->rotateAccept();
			cube[2]->setRotateType(3);
			cube[4]->setRotateType(3);
		}
		break;
	}

	// 뒷면이 제자리에서 작아지면서 없어졌다 커지기 반복
	case'b':
	{
		for (auto& c : cube)
			c->rotateDecline();

		static bool key_b = false;
		key_b = !key_b;
		if (key_b)
		{
			cube[1]->initMoved();
			cube[1]->rotateAccept();
			cube[1]->setRotateType(4);
		}
		break;
	}
	
	//  사각뿔 옆면들이 열었다 닫았다.
	case'o': 
	{
		pyramid[0]->setPos(orgPyr4);
		pyramid[1]->setPos(orgPyr3);
		pyramid[2]->setPos(orgPyr2);
		pyramid[3]->setPos(orgPyr1);
		pyramid[4]->setPos(orgPyr5);
		for (auto& p : pyramid)
			p->rotateDecline();

		static bool key_o = false;
		key_o = !key_o;
		if (key_o)
		{
			for (int i{}; i < 4; ++i)
			{
				pyramid[i]->initMoved();
				pyramid[i]->rotateAccept();
				pyramid[i]->setRotateType(5);
			}
		}
		break;
	}
	case'r':
	{
		pyramid[0]->setPos(orgPyr4);
		pyramid[1]->setPos(orgPyr3);
		pyramid[2]->setPos(orgPyr2);
		pyramid[3]->setPos(orgPyr1);
		pyramid[4]->setPos(orgPyr5);
		for (auto& p : pyramid)
			p->rotateDecline();

		static bool key_r = false;
		key_r = !key_r;
		if (key_r)
		{
			for (int i{}; i < 4; ++i)
			{
				pyramid[i]->initMoved();
				pyramid[i]->rotateAccept();
				pyramid[i]->setRotateType(6);
			}
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
void MyGL::mouse(int button, int state, int x, int y)
{
	float crx = (2.0f * x - my->width) / my->width;
	float cry = -(2.0f * y - my->height) / my->height;

	

	glutPostRedisplay();
}
void MyGL::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(my->shaderProgramID);

	if (yTrig)
		tmp += 0.1f;
	glm::mat4 model = glm::mat4(1.0f);
	// x축 30도, y축 -30도 회전해서 그리기
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(tmp), glm::vec3(0.0, 1.0, 0.0));
 	unsigned int modelLocation = glGetUniformLocation(my->shaderProgramID, "model_transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	for (const auto& l : lines)
		l->draw(my->shaderProgramID);
	
	if (whatToDraw)
	{
		for (const auto& p : pyramid)
			p->draw(my->shaderProgramID);
	}
	else
	{
		for (const auto& c : cube)
			c->draw(my->shaderProgramID);
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

	float xLine[6]{ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	float yLine[6]{ 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float zLine[6]{ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };
	lines.push_back(new PolygonShape(PolygonType::LINE, xLine));
	lines.push_back(new PolygonShape(PolygonType::LINE, yLine));
	lines.push_back(new PolygonShape(PolygonType::LINE, zLine));
	lines[0]->setColor(1.0f, 0.0f, 0.0f);
	lines[1]->setColor(0.0f, 1.0f, 0.0f);
	lines[2]->setColor(0.0f, 0.0f, 1.0f);

	float c1[12]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d };
	float c2[12]{ -d,d,d,-d,d,-d,-d,-d,-d,-d,-d,d };
	float c3[12]{ d,d,d,d,d,-d,-d,d,-d,-d,d,d };
	float c4[12]{ d,d,-d,d,d,d,d,-d,d,d,-d,-d };
	float c5[12]{ -d,d,-d,d,d,-d,d,-d,-d,-d,-d,-d };
	float c6[12]{ d,-d,-d, -d,-d,-d,-d,-d,d,d,-d,d };
	
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c6));
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c5));
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c4));
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c3));
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c2));
	cube.push_back(new PolygonShape(PolygonType::RECTSHAPE, c1));
	
	float sp1[9]{ 0.0f,0.5f,0.0f, -0.2f,-0.2f,0.2f, 0.2f,-0.2f,0.2f };
	float sp2[9]{ 0.0f,0.5f,0.0f, -0.2f,-0.2f,-0.2f, -0.2f,-0.2f,0.2f };
	float sp3[9]{ 0.0f,0.5f,0.0f, 0.2f,-0.2f,-0.2f, -0.2f,-0.2f,-0.2f };
	float sp4[9]{ 0.0f,0.5f,0.0f, 0.2f,-0.2f,0.2f, 0.2f,-0.2f,-0.2f };
	float sp5[12]{ 0.2f,-0.2f,0.2f, -0.2f,-0.2f,0.2f, -0.2f,-0.2f,-0.2f, 0.2f,-0.2f,-0.2f };
	pyramid.push_back(new PolygonShape(PolygonType::TRIANGLE, sp4));
	pyramid.push_back(new PolygonShape(PolygonType::TRIANGLE, sp3));
	pyramid.push_back(new PolygonShape(PolygonType::TRIANGLE, sp2));
	pyramid.push_back(new PolygonShape(PolygonType::TRIANGLE, sp1));
	pyramid.push_back(new PolygonShape(PolygonType::RECTSHAPE, sp5));
	for (int i{}; i < 5; ++i)
	{
		pyramid[i]->setShapeNum(i);
	}

	glutDisplayFunc(MyGL::draw);
	glutReshapeFunc(MyGL::reShape);
	glutMouseFunc(MyGL::mouse);
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

