#pragma once
#include "AEEngine.h"

enum class type { blob = 0 };

void updatePos(float& x, float& y);

bool checkCollision(float, float, float, float, float, float, float);


struct point {
	float x;
	float y;
};
