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
    QuadricType type; // � Ÿ���� ��������

    GLUquadric* obj;                               
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);   
    GLfloat color[3];                   
    GLdouble radius;      // ������
    GLdouble height;
    int slices = 4;    // �浵 ����
    int stacks = 10;    // ���� ����

    // �� �ึ�� �� �� ����������
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    float angle_z = 0.0f;
public:
    QuadricShape(QuadricType type, GLdouble rad = 1.0, GLdouble height = 5.0, float x = 0.0f, float y = 0.0f, float z = 0.0f);
    ~QuadricShape();

    void setColor(float r, float g, float b) { color[0] = r; color[1] = g; color[2] = b; }
    void setColor(float* c) { color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; }
    float* getColor() { return color; }
    const glm::vec3 getPos() const { return pos; }

    void rotateX(float theta) { angle_x += theta; }
    void rotateY(float theta) { angle_y += theta; }
    void rotateZ(float theta) { angle_z += theta; }

    void update();
    virtual void draw(GLuint shaderProgram) const override;
    void draw2(GLuint shaderProgram, DrawType drawType = DRAW_WIRE) const;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};