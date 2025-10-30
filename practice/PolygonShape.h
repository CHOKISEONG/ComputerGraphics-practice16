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
    SQUAREPYRAMID,
    CUBE
};

class PolygonShape : public ShapeManager
{
    GLuint vao = 0, vbo[2] = {0,0}, ebo = 0;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> colors;
    std::vector<unsigned int> index;
    bool drawingIdx[12]{};          // true인 면만 그리는 용도

    float midpoint[3];              // 도형의 중심점
    unsigned short polygonType = 0; // 도형의 타입          
    float moveSpeed = 0.001f;

    // 실습 18용 변수
    bool canMove = false;   // 움직일 수 있는지 체크
    float rotateSpeed[2]{}; // x,y 자전하는지 체크
    bool isYRotate = false; // y축 공전하는지 체크
    float increaseSpeed = 0.0f; // 확대/축소 속도
    float increasedAmount = 0.0f; // 자신에 대해서 확대/축소된 양
    float increasedAmount2 = 0.0f; // 원점에 대해서 확대/축소된 양
    bool isIncreaseOrigin = false; // 확대/축소가 원점에 대한 확대/축소인지 체크
    float theta = 0.0f; // 공전량

public:
    //PolygonShape();
    PolygonShape(PolygonType type, const float* f);
    PolygonShape(const PolygonShape& other);
    ~PolygonShape();

    void initBuffer();
    void updateVbo();

    unsigned short getPolytonType() { return polygonType; };
    std::vector<float> getPos() const { return positions; }

    void setColor(const float r, const float g, const float b);
    void setPos(std::vector<float> p) { positions = p; updateVbo(); }
    void setMidpoint(const float x, const float y, const float z);
    
    void drawAll() { std::fill(drawingIdx, drawingIdx + 12, true); }
    void drawNone() { std::fill(drawingIdx, drawingIdx + 12, false); }
    void drawSomething(int n) { drawingIdx[n * 2] = true; drawingIdx[n * 2 + 1] = true; }

    /// <summary>
    /// 실습 18용 함수들
    /// </summary>
    void update();

    void startMove() { canMove = true; }
    void stopMove() { canMove = false; }
    bool getCanMove() { return canMove; }

    void changeSpeed(float xSpeed, float ySpeed) { rotateSpeed[0] = xSpeed; rotateSpeed[1] = ySpeed; }
    void startYRotate(float speed) { isYRotate = true; moveSpeed = speed; }
    void startIncrease(float speed, bool isOrigin = false) 
    { 
        isIncreaseOrigin = isOrigin;
        if (isOrigin == false || increasedAmount < -0.5f) increasedAmount = -0.5f;
        if (isOrigin == true || increasedAmount2 < -0.5f) increasedAmount2 = -0.5f;
        increaseSpeed = speed; 
    }
    void startDecrease(float speed, bool isOrigin = false) 
    { 
        isIncreaseOrigin = isOrigin;
        if (isOrigin == false || increasedAmount > 0.5f) increasedAmount = 0.5f;
        if (isOrigin == true || increasedAmount2 > 0.5f) increasedAmount2 = 0.5f;
        increaseSpeed = -speed; 
    }

    void rotation();
    void revolution();
    void scaling();
    void scalingByOrigin();
    // 실습 18용 함수들


    void add(const float x, const float y);
    void add(const float x, const float y, const float z);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};