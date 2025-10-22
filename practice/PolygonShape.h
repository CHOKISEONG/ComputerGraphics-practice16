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
    bool drawingIdx[12]{}; // true인 면만 그리는 용

    float midpoint[2]; // 도형의 중심점
    unsigned short polygonType = 0;
    int drawType = 1;
    float moveSpeed = 0.5f;
    bool isMoving = false;
    bool isRotating = false;
    int rotateDir = 4;

    float slope = 0.0f; // 이동할 기울기
    float offSlope = 0.0f; // 처음보다 얼마나 기울어졌는지 

    float distance = udis(gen);
    float moved = 0.0f; // 얼마나 이동했는지
    float r;

    int rotateType = 0;
    int shapeNum = -1;
public:
    //PolygonShape();
    PolygonShape(PolygonType type, const float* f);
    PolygonShape(std::vector<float>& v);
    PolygonShape(const PolygonShape& other);
    ~PolygonShape();

    void updateVbo();

    void setDrawType(int n) { drawType = n; }
    void setColor(const float r, const float g, const float b);
    void setRotateDir(int n) { rotateDir = n; isMoving = true; }
    void setPos(std::vector<float> p) { positions = p; updateVbo(); }
    void setShapeNum(int n) { shapeNum = n; }
    void initMoved() { moved = 0.0f; }

    std::vector<float> getPos() const { return positions; }

    void drawAll() { std::fill(drawingIdx, drawingIdx + 12, true); }
    void drawNone() { std::fill(drawingIdx, drawingIdx + 12, false); }
    void drawSomething(int n) { drawingIdx[n * 2] = true; drawingIdx[n * 2 + 1] = true; }

    void moveAccept() { isMoving = true; }
    void moveDecline() { isMoving = false; }
    bool getIsMoving() const { return isMoving; }
    void rotate();

    void changeSlope(float theta) { slope = theta; }
    void setMidpoint(const float x, const float y);
    unsigned short getPolytonType() { return polygonType; };
    void initBuffer();

    /// <summary> e동 컴
    /// 실습 17용 함수
    /// </summary>
    void rotateDecline() { isRotating = false; rotateType = 0; }
    void rotateAccept() { isRotating = true; }
    bool getIsRotating() { return isRotating; }
    void setRotateType(int n) { rotateType = n; }
    int getRotateType() { return rotateType; }
    void rotate2();
    void rotateT();
    void rotateF();
    void rotateS();
    void zoomInOut();
    void rotateO();
    void rotateR(int n);

    void add(const float x, const float y);
    void add(const float x, const float y, const float z);

    virtual void draw(GLuint shaderProgram) const override;
    virtual void move(float x, float y) override;
    virtual bool isMouseInside(float x, float y) const override;
};