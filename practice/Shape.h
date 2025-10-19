#pragma once

#include <iostream>

class ShapeManager
{
public:
	ShapeManager() { std::cout << "도형 생성\n"; }
	virtual ~ShapeManager() { std::cout << "도형 제거\n"; }
	virtual void draw(unsigned int shaderProgram) const = 0;
	virtual void move(float x, float y) = 0;
	virtual bool isMouseInside(float x, float y) const = 0;
};