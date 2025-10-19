#pragma once

#include "Shape.h"
#include "global.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

enum PolygonType
{
    POINTTYPE,
    LINE,
    TRIANGLE,
    RECTSHAPE,
    PENTAGON,
    POLYGON,
    SQUAREPYRAMID
};

class PolygonShape : public ShapeManager
{
    GLuint vao, vboPos, vboColor, ebo;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> colors;
    std::vector<unsigned int> index;


    // 실습 15용 변수
    bool drawingIdx[12]{}; // true인 면만 그리는 용

    int isFolding = 1; // 접는 중 : 1, 펴는 중 : -1
    bool isMoving = false;
    bool canAnimation = false; // 애니메이션 가능한지
    int animType = zeroOrOne(gen) + 1; // 1 = moveSpring, 2 = moveZigZag

    float midpoint[2]; // 도형의 중심점
    unsigned short polygonType = 0;
    float moveSpeed = 0.003f;

    float slope = 0.0f; // 이동할 기울기
    float offSlope = 0.0f; // 처음보다 얼마나 기울어졌는지 

    float distance = udis(gen);
    float moved = 0.0f; // 얼마나 이동했는지
    float r;

public:
    PolygonShape();
    PolygonShape(PolygonType type, const float* f);
    PolygonShape(std::vector<float>& v);
    PolygonShape(const PolygonShape& other);
    ~PolygonShape();

    void updateVbo();

    void setColor(const float r, const float g, const float b);
    void setPaperFoldingShape(); // 실습 14용 삼각형으로 세팅
    void setRegularHexagon();

    // 실습 15용 함수
    void drawAll() { std::fill(drawingIdx, drawingIdx + 12, true); }
    void drawNone() { std::fill(drawingIdx, drawingIdx + 12, false); }
    void drawSomething(int n) { drawingIdx[n * 2] = true; drawingIdx[n * 2 + 1] = true; }

    void moveAccept() { isMoving = true; }
    void moveDecline() { isMoving = false; }
    bool getIsMoving() const { return isMoving; }

    void setCanAnimation(bool t) { canAnimation = t; }
    bool CanAnimation() { return canAnimation; }
    void changeAnimType();
    void changeSlope(float theta) { slope = theta; }
    void setMidpoint(const float x, const float y);
    unsigned short getPolytonType() { return polygonType; };
    void initBuffer();
    void initialSetting(PolygonType type);

    void moving();
    void movingTriangle();
    void movingRectangle();
    void movingPentagon();
    void movingLine();

    int isTouchedWall();
    bool isCollide(PolygonShape*& other);

    //// 실습 14용 함수
    //void movePaperFolding();
    //void moveRotate();
    //void teleport(float x, float y);

    //// 실습 13용 함수
    //void moveAnimation();
    //void moveSpring();
    //void moveZigZag();

    void changeType(PolygonType pType);
    void add(const float x, const float y);
    void add(const float x, const float y, const float z);


    virtual void draw(GLuint shaderProgram) const override;
    virtual void move(float x, float y) override;
    virtual bool isMouseInside(float x, float y) const override;
};