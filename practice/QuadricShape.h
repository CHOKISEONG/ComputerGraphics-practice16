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

    glm::vec3 pos;    // 위치
    GLfloat color[3]; // 색상

    GLUquadric* obj; // 물체
    GLdouble radius; // 반지름
    GLint slices;    // 경도 개수
    GLint stacks;    // 위도 개수

    glm::vec3 targetPos;        // 공전의 중심
    bool isRevolution = true;  // 공전하는지 체크용
    float speed = 0.01f;         // 공전 속도
public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 1.0f, GLint slices = 10, GLint stacks = 10);

    const glm::vec3 getPos() const { return pos; }

    void revolution(); // 공전하는 함수

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};