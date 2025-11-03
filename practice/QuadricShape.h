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
    QuadricType type; // 도형 타입

    GLUquadric* obj;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);       // 도형을 그릴 위치
    glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f); // 위치에서 그려질 방향
    GLfloat color[3];

    GLdouble radius;      // 반지름
    int slices = 4;       // 테두리 개수
    int stacks = 2;      

public:
    QuadricShape(QuadricType type, GLdouble rad = 1.0, float x = 0.0f, float y = 0.0f, float z = 0.0f);
    ~QuadricShape();

    void setColor(float r, float g, float b) { color[0] = r; color[1] = g; color[2] = b; }
    void setColor(float* c) { color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; }
    void setSlices(int n) { slices = n; stacks = n; }
    float* getColor() { return color; }

    GLUquadric* getObj() const { return obj; }
    glm::vec3 getPos() const { return pos; }
    const glm::vec3 getTargetPos() const { return targetPos; }
    void setPos(const glm::vec3& t) { pos = t; }
    void setTargetPos(const glm::vec3& t) { targetPos = t; }
    void lookAt(const glm::vec3& t) { targetPos = t; }

    virtual void update() { return; }
    virtual void draw(GLuint shaderProgram) const override;
    virtual void draw2(GLuint shaderProgram, DrawType drawType = DRAW_WIRE) const;
    virtual void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
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
    Ball(GLdouble rad = 1.0, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : QuadricShape(SPHERE, rad, x, y, z)
    {
    }
    void update(float rad);
};


class Box : public QuadricShape
{
private:
    std::vector<QuadricShape*> box;
    float boxRadius;

    float theta = 0.0f; // z축 회전만 있음
public:
    Box(GLdouble rad = 1.0, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : QuadricShape(DISK, rad, x, y, z), boxRadius(rad / glm::sqrt(rad))
    {
        float R = rad / glm::sqrt(rad);
        box.reserve(6);
        for (int i{}; i < 6; ++i)
            box.push_back(new QuadricShape(QuadricType::DISK, R));
        box[0]->move(0.0f, R, 0.0f, false);
        box[1]->move(0.0f, -R, 0.0f, false);
        box[2]->move(-R, 0.0f, 0.0f, false);
        box[3]->move(R, 0.0f, 0.0f, false);
        box[4]->move(0.0f, 0.0f, R, false);
        box[5]->move(0.0f, 0.0f, -R, false);
    }

    const float getRadius() const { return boxRadius; }

    void rotateZ(float theta);
    
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = true) override;
    void update() override;
    void draw2(GLuint shaderProgram, DrawType drawType = DRAW_WIRE) const override;
};