#pragma once

#include "Shape.h"
#include "global.h"

enum QuadricType
{
    SPHERE,
    CYLINDER,
    DISK
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
    GLint slices = 4;    // 경도 개수
    GLint stacks = 10;    // 위도 개수

    glm::vec3 axis;
public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 0.5f);

    const glm::vec3 getPos() const { return pos; }

    void rotateAxisZ(float theta);

    void update();
    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};