#pragma once

#include "Shape.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>

enum QuadricType
{
    SPHERE,
    CYLINDER,
    DISK
};

class QuadricShape : public ShapeManager
{
    GLuint vao = 0, vbo[2] = { 0,0 }, ebo = 0;
    GLdouble radius;

public:
    //PolygonShape();
    QuadricShape(QuadricShape type, const float* f);
    QuadricShape(const QuadricShape& other);
    ~QuadricShape();

    virtual void draw(GLuint shaderProgram) const override;
    void move(float x, float y, float z = 0.0f);
};