#pragma once
#include "AEEngine.h"

enum class type { blob = 0 };

void updatePos(float& x, float& y);

bool checkCollision(float, float, float, float, float, float, float);


struct point {
	float x;
	float y;
};

class Enemy {
	point center;
	float halfWidth, halfHeight;
	static float blob_speed, blob_counter, blob_offset;
	type enemytype;

	//void updatePos(float& x, float& y);

	//void setCollision();
};