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
    QuadricType type; // � Ÿ���� ��������

    GLUquadric* obj;                                // �༺
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);    // �༺ ��ġ
    GLfloat color[3];                               // �༺ ����

    GLUquadric* moon;                               // ��
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat mColor[3];
    glm::vec3 mDir = mPos - pos;
    float mAmount = 0.0f;
    
    std::vector<basicInfo> orbit; // �˵�
    float orbitRadius = 0.04f;
    float timer = 0.0f;

    GLdouble radius;      // ������
    GLint slices = 50;    // �浵 ����
    GLint stacks = 20;    // ���� ����

    glm::vec3 targetPos = glm::vec3(0.0f,0.0f,0.0f);    // ������ �߽�
    glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);       // ���� ��
    bool isRevolution = true;                           // �����ϴ��� üũ��
    float speed = zeroToOne(gen) / 100 + 0.005f;        // ���� �ӵ�

    bool isSolidDraw = true;
public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 0.5f);

    const glm::vec3 getPos() const { return pos; }
    const glm::vec3 getTargetPos() const { return targetPos; }
    void setTargetPos(glm::vec3 p) { targetPos = p; }
    // �ϴ� �ǽ� 19�� ���缭 �� �����ϰ� ��
    void setAxis() { axis.x = -pos.y, axis.y = pos.x, axis.z = pos.z; }

    void makeChildren();
    void revolution(); // �����ϴ� �Լ�
    void toSolid() { isSolidDraw = true; }
    void toWire() { isSolidDraw = false; }
    void resizeOrbit(float amount) { orbitRadius += amount; }

    void rotateAxisZ(float theta);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};