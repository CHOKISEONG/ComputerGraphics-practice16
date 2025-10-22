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
std::vector<PolygonShape*> pyramid;
std::vector<float> orgPyramidPos;
std::vector<PolygonShape*> lines;
bool isBackFaceCulling = false;
char trigger;
bool whatToDraw = false;
float d = 0.3f; // 큐브의 초기 반지름
float tmp = 0.0f;
unsigned long long pMoved = 0;
// 원래 큐브 위치
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
	
	glutPostRedisplay();
}

void MyGL::keyboard(unsigned char key, int x, int y)
{
	static bool isRotateR = false;
	trigger = key;
	switch (key)
	{
	case'1': {
		//왼쪽 객체만 출력
		break;
	}
	case'2': {
		// show right object
		break;
	}
	case'3': {
		// show both object
		break;
	}
	case'x':case'X': {
		// rotate object's x axis + -
		break;
	}
	case'y':case'Y': {
		// rotate object's y axis + -
		break;
	}
	case'r':case'R': {
		// rotate zeroPoint's y axis + -
		break;
	}
	case'a':case'A': {
		// scale
		break;
	}
	case'b':case'B': {
		// 원점에 대한 확대/축소
		break;
	}
	case'd':case'D': {
		// move objects's x
		break;
	}
	case'e':case'E': {
		// move object's y
		break;
	}
	case 't': {
		// 두 도형이 원점을 통과하며 상대방의 자리로 이동하는 애니메이션
		break;
	}
	case 'u': {
		// 두 도형이 한개는 위로 한개는 아래로 이동하면서 상대방의 자리로 이동하는 애니메이션
		break;
	}
	case 'v': {
		// 뭔말인지 모름
		break;
	}
	case 'c': {
		// 두 도형을 다른거로 바꾸기
		break;
	}
	case's': {
		//초기화
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

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
 	unsigned int modelLocation = glGetUniformLocation(my->shaderProgramID, "model_transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	for (const auto& l : lines)
		l->draw(my->shaderProgramID);
	for (const auto& p : pyramid)
		p->draw(my->shaderProgramID);
	

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

	/*float c1[12]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d };
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
	for (int i{}; i < cube.size(); ++i)
	{
		cube[i]->move(-0.5f, 0.0f);
	}*/
	float d = 0.3f;
	float cubePos[24]{ d,d,d,-d,d,d,-d,-d,d,d,-d,d
					,d,d,-d,-d,d,-d,-d,-d,-d,d,-d,-d };
	cube = new PolygonShape(PolygonType::CUBE, cubePos);
	
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
		pyramid[i]->move(0.5f, 0.0f);
	}

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

