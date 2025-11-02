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

class QuadricShape : public ShapeManager
{
protected:
    QuadricType type; // 어떤 타입의 도형인지

    GLUquadric* obj;                               
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    GLfloat color[3];                   
    GLdouble radius;      // 반지름
    GLdouble height;
    int slices = 4;    // 경도 개수
    int stacks = 10;    // 위도 개수

    // 각 축마다 몇 도 기울어졌는지
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    float angle_z = 0.0f;

public:
    QuadricShape(QuadricType type, GLdouble rad = 1.0, GLdouble height = 5.0, float x = 0.0f, float y = 0.0f, float z = 0.0f);
    ~QuadricShape();

    void setColor(float r, float g, float b) { color[0] = r; color[1] = g; color[2] = b; }
    void setColor(float* c) { color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; }
    void setSlices(int n) { slices = n; stacks = n; }
    float* getColor() { return color; }
    const glm::vec3 getPos() const { return pos; }

    glm::vec3 getAngle() { return glm::vec3(angle_x, angle_y, angle_z); }
    void rotateX(float theta) { angle_x += theta; }
    void rotateY(float theta) { angle_y += theta; }
    void rotateZ(float theta) { angle_z += theta; }

    virtual void update() { return; }
    virtual void draw(GLuint shaderProgram) const override;
    void draw2(GLuint shaderProgram, DrawType drawType = DRAW_WIRE) const;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};

enum BallDirection
{
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_RIGHT,
    UP_LEFT
};

class Ball : public QuadricShape
{
private:
    const float floor = 4.5f / ROOT2;
    const float gravity = -7.0f;
    const float bounceFactor = 0.7f;
    float accel = 0.0f;

    BallDirection dir = BallDirection::DOWN_RIGHT;
public:
    Ball(QuadricType type, GLdouble rad = 1.0, GLdouble height = 5.0, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : QuadricShape(type, rad, height, x, y, z)
    {
    }
    void update() override;
};

class Box : public QuadricShape
{
private:
    const float floor = -4.5f / ROOT2;

public:
    Box(QuadricType type, GLdouble rad = 1.0, GLdouble height = 5.0, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : QuadricShape(type, rad, height, x, y, z)
    {
    }
    void update() override;
};