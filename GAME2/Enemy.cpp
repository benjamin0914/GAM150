#include <AEEngine.h>
#include "Enemy.h"
#include "pch.h"

struct projectile {
	AEVec2 position;
	float width, height;
};

void updatePos(float &x, float &y) {
	static float xBoundary = x + 150.0f;
	static float xBegin = x;
	static bool flag = 1;
	if (flag == 1) {
		x += 2.0f;
		if (x >= xBoundary) flag = 0;
	}

	if (flag == 0) {
		x -= 2.0f;
		if (x<= xBegin) flag = 1;
	}
}

bool checkCollision(float playerX, float playerY, float pLength, float objX, float objY, float halfWidth, float velocity) {

	float pMinX = playerX - pLength;
	float pMinY = playerY - pLength;
	float pMaxX = playerX + pLength;
	float pMaxY = playerY + pLength;

	float bMinX = objX - halfWidth;
	float bMinY = objY - halfWidth;
	float bMaxX = objX + halfWidth;
	float bMaxY = objY + halfWidth;

	//static collision
	if (pMinX > bMaxX) return false;
	if (bMinX > pMaxX) return false;
	if (pMaxY < bMinY) return false;
	if (bMaxY < pMinY) return false;

	float tFirst = 0;
	float tLast = AEFrameRateControllerGetFrameTime();

	float vel = velocity;

	if (vel > 0) {
		if (pMaxX > bMinX) return false;
	}

	return true;
}