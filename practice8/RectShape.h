#pragma once

#include "Shape.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

class Rect : public ShapeManager
{
    GLuint vao, vbo, ebo;
    float verticles[12];
    unsigned int index[6]{ 0,1,3,1,2,3 };
    GLfloat color[3];

public:
    Rect();
    Rect(const float f1, const float f2);
    Rect(const float* v);
    Rect(const Rect& other);
    ~Rect();

    void initBuffer();

    virtual void draw(GLuint shaderProgram) const override;
    virtual void move(float x, float y) override;
    virtual bool isMouseInside(float x, float y) const override;
};