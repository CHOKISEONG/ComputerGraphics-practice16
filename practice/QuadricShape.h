#pragma once

#include "Shape.h"
#include "global.h"

enum QuadricType
{
    SPHERE,
    CYLINDER,
    DISK
};

enum DrawType
{
    DRAW_SOLID,
    DRAW_WIRE,
    DRAW_SILHOUETTE,
    DRAW_POINT
};

struct basicInfo
{
    GLUquadric* obj;
    glm::vec3 pos;
};

class QuadricShape : public ShapeManager
{
    QuadricType type; // 어떤 타입의 도형인지

    GLUquadric* obj;                               
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat color[3];                   
    GLdouble radius;      // 반지름
    GLdouble height;
    int slices = 4;    // 경도 개수
    int stacks = 10;    // 위도 개수

    // 각 축마다 몇 도 기울어졌는지
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    float angle_z = 0.0f;

    bool isMove[4]{ false,false,false,false };
    glm::vec3 targetPos = glm::vec3(0.0f);
    float speed = 0.01f;  // 이동 속도
    float speed_G = 0.2f;
    float speed_P = 0.2f;
    float amount_T = 0.0f;
    float amount_L = 0.0f; // 이동한 양
    float amount_G = 0.0f; // 이동한 양
    float amount_P = 0.0f; // 이동한 양
public:
    QuadricShape(QuadricType type, GLdouble rad = 1.0, GLdouble height = 5.0, float x = 0.0f, float y = 0.0f, float z = 0.0f);
    QuadricShape(QuadricShape& other);
    QuadricShape& operator=(const QuadricShape& other);
    ~QuadricShape();

    void setColor(float r, float g, float b) { color[0] = r; color[1] = g; color[2] = b; }
    void setColor(float* c) { color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; }
    float* getColor() { return color; }
    const glm::vec3 getPos() const { return pos; }

    void rotateX(float theta) { angle_x += theta; }
    void rotateY(float theta) { angle_y += theta; }
    void rotateZ(float theta) { angle_z += theta; }

    //실습 20
    void stopMove() { for (int i{}; i < 4; ++i) isMove[i] = false; }
    void startT() { isMove[0] = true; }
    void startL(const glm::vec3 p) { isMove[1] = true; targetPos = p - pos; amount_L = 0.0f; }
    void startG(bool invert) { isMove[2] = true; amount_G = 0.0f; if (invert) speed_G = -speed_G; }
    void startP(bool isCCW) { isMove[3] = true; amount_P = 0.0f; if (isCCW) speed_P = -speed_P; }
    void moveT();
    void moveL();
    void moveG();
    void moveP();

    void update();
    virtual void draw(GLuint shaderProgram) const override;
    void draw2(GLuint shaderProgram, DrawType drawType = DRAW_WIRE) const;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};