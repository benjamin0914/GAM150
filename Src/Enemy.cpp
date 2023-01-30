#include <AEEngine.h>
#include "Enemy.h"

struct projectile {
	AEVec2 position;
	float width, height;
};

void updatePos(float &x, float &y) {
	static float xBoundary = x + 200.0f;
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

bool checkCollision(float playerX, float playerY, float pLength, float objX, float objY, float halfWidth, float halfHeight) {
	static float temp_X{ playerX };
	static float temp_Y{ playerY };

	if (playerX < objX - halfWidth) temp_X = objX - halfWidth;
	else if (playerX > objX + halfWidth) temp_X = objX + halfWidth;

	if (playerY < objY - halfHeight) temp_Y = objY - halfHeight;
	else if (playerY > objY - halfHeight) temp_Y = objY - halfHeight;

	
	float distanceX{ playerX - temp_X };
	float distanceY{ playerY - temp_Y };
	float distTotal = (distanceX * distanceX) + (distanceY * distanceY);

	float playerLengthSQUARED = pLength * pLength;

	if (distTotal <= playerLengthSQUARED) {
		return 1;
	}
	else return 0;
}