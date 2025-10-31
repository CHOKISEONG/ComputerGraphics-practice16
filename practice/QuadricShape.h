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
    QuadricType type; // � Ÿ���� ��������

    glm::vec3 pos;
    GLUquadric* obj; // ��ü
    GLdouble radius; // ������
    GLint slices;    // �浵 ����
    GLint stacks;    // ���� ����

public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 1.0f, GLint slices = 10, GLint stacks = 10);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};