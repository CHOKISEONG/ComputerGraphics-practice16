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
    bool drawingIdx[12]{};          // true�� �鸸 �׸��� �뵵

    float midpoint[2];              // ������ �߽���
    unsigned short polygonType = 0; // ������ Ÿ��          
    float moveSpeed = 0.0f;

    // �ǽ� 18�� ����
    bool canMove = false;

public:
    //PolygonShape();
    PolygonShape(PolygonType type, const float* f);
    PolygonShape(std::vector<float>& v);
    PolygonShape(const PolygonShape& other);
    ~PolygonShape();

    void initBuffer();
    void updateVbo();

    unsigned short getPolytonType() { return polygonType; };
    std::vector<float> getPos() const { return positions; }

    void setColor(const float r, const float g, const float b);
    void setPos(std::vector<float> p) { positions = p; updateVbo(); }
    void setMidpoint(const float x, const float y);
    
    void drawAll() { std::fill(drawingIdx, drawingIdx + 12, true); }
    void drawNone() { std::fill(drawingIdx, drawingIdx + 12, false); }
    void drawSomething(int n) { drawingIdx[n * 2] = true; drawingIdx[n * 2 + 1] = true; }

    /// <summary>
    /// �ǽ� 18�� �Լ���
    /// </summary>
    void update() { return; }

    void startMove() { canMove = true; }
    void stopMove() { canMove = false; }
    bool getCanMove() { if (canMove) std::cout << "can move!\n";  return canMove; }
    

    void add(const float x, const float y);
    void add(const float x, const float y, const float z);

    virtual void draw(GLuint shaderProgram) const override;
    virtual void move(float x, float y) override;
};