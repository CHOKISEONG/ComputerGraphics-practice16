#pragma once

#include "Shape.h"
#include "global.h"

enum QuadricType
{
    SPHERE,
    CYLINDER,
    DISK
};

class QuadricShape : public ShapeManager
{
    QuadricType type; // 어떤 타입의 도형인지

    glm::vec3 pos;
    GLUquadric* obj; // 물체
    GLdouble radius; // 반지름
    GLint slices;    // 경도 개수
    GLint stacks;    // 위도 개수

public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 1.0f, GLint slices = 10, GLint stacks = 10);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};