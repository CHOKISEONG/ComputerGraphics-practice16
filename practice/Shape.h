#pragma once

#include <iostream>

class ShapeManager
{
public:
	ShapeManager() { std::cout << "���� ����\n"; }
	virtual ~ShapeManager() { std::cout << "���� ����\n"; }
	virtual void draw(unsigned int shaderProgram) const = 0;
	virtual void move(float x, float y) = 0;
	virtual bool isMouseInside(float x, float y) const = 0;
};