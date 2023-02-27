#pragma once
#include "AEEngine.h"
#include <vector>

enum class EnemyType { slime = 0, Max };

EnemyType& operator++(EnemyType& rhs);

//void updatePos(float& x, float& y);

//bool checkCollision(float, float, float, float, float, float, float);

const f32 MOVING_VEL = 4.0f;
const double ENEMY_IDLE_TIME = 2.0;

enum STATE
{
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

enum INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};


struct point {
	float x;
	float y;
};

class Enemy
{
public:
	AEVec2 pos;
	EnemyType type;
	enum STATE state;
	enum INNER_STATE innerState;
	AEVec2 velCurr;
	int gridCollisionFlag;


	Enemy();
	void Render(int width, int height);
	static void Add(std::vector<Enemy>& tile, EnemyType type, const f32 width, const f32 height, AEVec2 pos);
	static void Unload(void);
	static void LoadTex(void);
	static void updatePos(float& x, float& y);
	static void spriteSwap();
	static void EnemyState(Enemy* enemyInst);
};