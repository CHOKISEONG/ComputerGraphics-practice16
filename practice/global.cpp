#include "global.h"

#include <vector>

unsigned short type = 0;

float pvx, pvy;
float defaultSpeed = 0.004f;
float defaultSize = 0.15f;

float polygonDis = 0.1f;

int RECTCNT = 15;
float PI = 3.14159265358979f;
float ROOT2 = 1.41421356237f;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-0.05f, 0.05f);
std::uniform_real_distribution<float> udis(0.0f, 0.05f);
std::uniform_real_distribution<float> zeroToOne(0.0f, 1.0f);
std::uniform_real_distribution<float> randPos(0.0f,0.3f);
std::uniform_real_distribution<float> randRadian(0.0f, 2*PI);
std::uniform_int_distribution<> zeroOrOne(0, 1);
std::uniform_int_distribution<> zeroToFour(0, 4);
std::uniform_int_distribution<> uid(0, 200); 

std::uniform_real_distribution<float> randBallPosX(-4.0f / ROOT2, 4.0f / ROOT2);
std::uniform_real_distribution<float> randBallPosY(-4.0f / ROOT2, 4.0f / ROOT2);
std::uniform_real_distribution<float> randBallPosZ(0.5f, 5.0f/ROOT2);


