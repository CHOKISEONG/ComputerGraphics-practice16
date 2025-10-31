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

    glm::vec3 pos;    // ��ġ
    GLfloat color[3]; // ����

    GLUquadric* obj; // ��ü
    GLdouble radius; // ������
    GLint slices;    // �浵 ����
    GLint stacks;    // ���� ����

    glm::vec3 targetPos;        // ������ �߽�
    bool isRevolution = true;  // �����ϴ��� üũ��
    float speed = 0.01f;         // ���� �ӵ�
public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 1.0f, GLint slices = 10, GLint stacks = 10);

    const glm::vec3 getPos() const { return pos; }

    void revolution(); // �����ϴ� �Լ�

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};