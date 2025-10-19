/// --------------------
///       global
/// 
/// for global variables and headers
/// --------------------
#pragma once
#include <random>
#include <vector>

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<float> dis;
extern std::uniform_real_distribution<float> udis;
extern std::uniform_real_distribution<float> zeroToOne;
extern std::uniform_real_distribution<float> randPos;
extern std::uniform_real_distribution<float> randRadian;
extern std::uniform_int_distribution<> zeroToFour;
extern std::uniform_int_distribution<> zeroOrOne;
extern std::uniform_int_distribution<> uid;

extern float pvx, pvy;  // previous x and y
extern float defaultSpeed;
extern float defaultSize;

extern float polygonDis;

extern int RECTCNT;
extern float PI;
extern float ROOT2;

enum WallDir {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT,
};