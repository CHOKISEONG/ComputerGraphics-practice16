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

    float midpoint[3];              // ������ �߽���
    unsigned short polygonType = 0; // ������ Ÿ��          
    float moveSpeed = 0.001f;

    // �ǽ� 18�� ����
    bool canMove = false;   // ������ �� �ִ��� üũ
    float rotateSpeed[2]{}; // x,y �����ϴ��� üũ
    bool isYRotate = false; // y�� �����ϴ��� üũ
    float increaseSpeed = 0.0f; // Ȯ��/��� �ӵ�
    float increasedAmount = 0.0f; // �ڽſ� ���ؼ� Ȯ��/��ҵ� ��
    float increasedAmount2 = 0.0f; // ������ ���ؼ� Ȯ��/��ҵ� ��
    bool isIncreaseOrigin = false; // Ȯ��/��Ұ� ������ ���� Ȯ��/������� üũ
    float theta = 0.0f; // ������

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
    /// �ǽ� 18�� �Լ���
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
    // �ǽ� 18�� �Լ���


    void add(const float x, const float y);
    void add(const float x, const float y, const float z);

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};