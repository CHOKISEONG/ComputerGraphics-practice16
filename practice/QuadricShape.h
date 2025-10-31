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

    GLUquadric* obj;                                // 행성
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);    // 행성 위치
    GLfloat color[3];                               // 행성 색상

    GLUquadric* moon;                               // 달
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat mColor[3];
    glm::vec3 mDir = mPos - pos;
    float mAmount = 0.0f;
    
    std::vector<basicInfo> orbit; // 궤도
    float orbitRadius = 0.04f;
    float timer = 0.0f;

    GLdouble radius;      // 반지름
    GLint slices = 50;    // 경도 개수
    GLint stacks = 20;    // 위도 개수

    glm::vec3 targetPos = glm::vec3(0.0f,0.0f,0.0f);    // 공전의 중심
    glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);       // 공전 축
    bool isRevolution = true;                           // 공전하는지 체크용
    float speed = zeroToOne(gen) / 100 + 0.005f;        // 공전 속도

    bool isSolidDraw = true;
public:
    QuadricShape(GLUquadric* o, QuadricType type, GLdouble rad = 0.5f);

    const glm::vec3 getPos() const { return pos; }
    const glm::vec3 getTargetPos() const { return targetPos; }
    void setTargetPos(glm::vec3 p) { targetPos = p; }
    // 일단 실습 19에 맞춰서 축 세팅하게 함
    void setAxis() { axis.x = -pos.y, axis.y = pos.x, axis.z = pos.z; }

    void makeChildren();
    void revolution(); // 공전하는 함수
    void toSolid() { isSolidDraw = true; }
    void toWire() { isSolidDraw = false; }
    void resizeOrbit(float amount) { orbitRadius += amount; }

    void rotateAxisZ(float theta);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f, bool changeTargetPos = false);
};