/// --------------------
///       global
/// 
/// for global variables and headers
/// --------------------
#pragma once

#include <random>
#include <vector>
#include <utility>

#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp> // glm::value_ptr

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

// 실습 21에 쓸 상자 안의 랜덤 공 좌표용
extern std::uniform_real_distribution<float> randBallPosX;
extern std::uniform_real_distribution<float> randBallPosY;
extern std::uniform_real_distribution<float> randBallPosZ;

enum WallDir {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT,
};