#include <AEEngine.h>
#include "Enemy.h"
#include "pch.h"
#include "Globals.h"

static AEGfxTexture* enemyTex[static_cast<int>(EnemyType::Max)]{ nullptr };

struct projectile {
	AEVec2 position;
	float width, height;
};

Enemy::Enemy() {
	pos = { 0, 0 };
	type = EnemyType::slime;
}

void Enemy::LoadTex() {
	for (EnemyType i = static_cast<EnemyType>(0); i < EnemyType::Max; ++i) {
		const char* pTex{ nullptr };
		switch (i) {
		case EnemyType::slime:
			pTex = "./Assets/Enemy.png";
			break;
		default:
			throw "invalid tile type!";
			return;
		}
		enemyTex[static_cast<int>(i)] = AEGfxTextureLoad(pTex);
		AE_ASSERT_MESG(pTex, "Failed to load Enemy texture!");
	}
	std::cout << "\nLoaded tiles!";

}

void Enemy::Add(std::vector<Enemy>& tile, EnemyType type, const f32 width, const f32 height, AEVec2 pos) {
	tile.push_back(Enemy());
	Enemy& newEnemyInst = tile.back();
	newEnemyInst.type = type;
	AEVec2Set(&newEnemyInst.pos ,pos.x + width / 2.0f, pos.y + height / 2.0f - height / 2.0f);
}

void Enemy::updatePos(float& x, float& y) {
	static float xBoundary = x + 150.0f;
	static float xBegin = x;
	static bool flag = 1;
	if (flag == 1) {
		x += 2.0f;
		if (x >= xBoundary) flag = 0;
	}

	if (flag == 0) {
		x -= 2.0f;
		if (x <= xBegin) flag = 1;
	}
}

void Enemy::Render(int width, int height) {
	AEMtx33 transMatrix;
	static f32 HalfWinHeight, HalfWinWindow;
	HalfWinHeight = AEGetWindowHeight() / 2.0f;
	HalfWinWindow = AEGetWindowWidth() / 2.0f;
	AEMtx33	trans, rot, scale;
	AEMtx33Scale(&scale, width, height);
	AEMtx33Rot(&rot, AEDegToRad(0));
	AEMtx33Trans(&trans, -HalfWinWindow + pos.x, HalfWinHeight - pos.y);
	AEMtx33 temp;
	AEMtx33Concat(&temp, &rot, &scale);
	AEMtx33Concat(&transMatrix, &trans, &temp);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	if (Enemy::type == EnemyType::slime) {
		AEGfxTextureSet(enemyTex[0], 0.5, 1.0);
	}
	//else if (Tiles::type == TileType::Dirt) {
	//	AEGfxTextureSet(tileTex[1], 0, 0);
	//}
	AEGfxSetTransform(transMatrix.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(Mesh::Rect, AE_GFX_MDM_TRIANGLES);

}


void Enemy::Unload()
{
	for (size_t i = 0; i < static_cast<int>(EnemyType::Max); i++) {
		AEGfxTextureUnload(enemyTex[i]);
	}

}

EnemyType& operator++(EnemyType& rhs) {
	rhs = static_cast<EnemyType>((static_cast<int>(rhs) + 1));
	return rhs;
}

/*
void Enemy::EnemyState(Enemy *enemyInst) {
	switch (enemyInst->state) {
	case(STATE_GOING_LEFT):
		switch (enemyInst->innerState) {
		case(INNER_STATE_ON_ENTER):
			enemyInst->velCurr.x = -MOVING_VEL;
			enemyInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case(INNER_STATE_ON_UPDATE):
			if (enemyInst->gridCollisionFlag == 1 || GetCellValue(enemyInst->posCurr.x - 1, pInst->posCurr.y + 1) == TYPE_OBJECT_EMPTY) {
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
			}
			break;
		case(INNER_STATE_ON_EXIT):
			pInst->counter -= dt;
			if (pInst->counter < 0) {
				pInst->state = STATE_GOING_RIGHT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}
			break;
		}
		break;

	case(STATE_GOING_RIGHT):
		switch (pInst->innerState) {
		case(INNER_STATE_ON_ENTER):
			pInst->velCurr.x = MOVE_VELOCITY_ENEMY;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case(INNER_STATE_ON_UPDATE):
			if (pInst->gridCollisionFlag == 2 || GetCellValue(pInst->posCurr.x + 1, pInst->posCurr.y + 1) == TYPE_OBJECT_EMPTY) {
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
			}
			break;
		case(INNER_STATE_ON_EXIT):
			pInst->counter -= dt;
			if (pInst->counter < 0) {
				pInst->state = STATE_GOING_LEFT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}
			break;
		}
}
*/