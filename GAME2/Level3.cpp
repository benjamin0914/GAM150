#include "pch.h"

AEGfxTexture* enemytex;
AEGfxTexture* playertex;
AEGfxTexture* platformtex;

AEGfxVertexList* enemymesh;
AEGfxVertexList* playermesh;
AEGfxVertexList* platformmesh;

AABB enemy;
AABB platform1;
AABB platform2;
AABB platform3;
AABB player;

AEVec2 playervel;
AEVec2 enemyvel;

bool jumpp;
bool gravity;
bool move_left;
bool move_right;

float jumptime;
float const jumpduration = 1.0f;

void Level3_Load()
{
	//Draw playermesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	playermesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(playermesh, "Failed to create player mesh");

	//Draw enemymesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.0f, -20.0f, 0x00FF00FF, 0.0f, 1.0f,
		20.0f, -20.0f, 0x00FFFF00, 0.25f, 1.0f,
		-20.0f, 20.0f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.0f, -20.0f, 0x00FFFFFF, 0.25f, 1.0f,
		20.0f, 20.0f, 0x00FFFFFF, 0.25f, 0.0f,
		-20.0f, 20.0f, 0x00FFFFFF, 0.0f, 0.0f);

	enemymesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(enemymesh, "Failed to create enemy mesh");

	//Draw platformmesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FF0FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0x00FFFFFF, 0.0f, 1.0f,
		0.5, -0.5f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0x00FFFFFF, 0.0f, 0.0f);

	platformmesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(platformmesh, "Failed to create square box mesh!!");

	//Load playertex
	playertex = AEGfxTextureLoad("Assets/player_cube.png");
	AE_ASSERT_MESG(playertex, "Failed to load enemy tex");

	//Load enemytex
	enemytex = AEGfxTextureLoad("Assets/slime.png");
	AE_ASSERT_MESG(enemytex, "Failed to load enemy tex");

	//Load platformtex
	platformtex = AEGfxTextureLoad("Assets/Special_Tile.png");
	AE_ASSERT_MESG(platformtex, "Failed to load platform tex");
}

void Level3_Initialize()
{
	enemy.min = { 110.0f, -130.0f };
	enemy.max = { 120.0f, -120.0f };

	player.min = { 0.f, 0.f };
	player.max= { 1.0f, 1.0f };

	platform1.min = { 0.f, -80.f };
	platform1.max = { 10.0f, -70.f };
	platform2.min = { -180.f, -180.f };
	platform2.max = { -170.f, -170.f };
	platform3.min = { 180.f, -180.f };
	platform3.max = { 190.f, -170.f };

	playervel = { 0.0f, 0.0f };
	enemyvel = { 0.0f, 0.0f };

	jumpp = { false };
	gravity = { false };
	move_left = { false };
	move_right = { false };
}

void Level3_Update()
{
	AEGfxSetCamPosition(player.min.x, player.min.y);
	updatePos(enemy.min.x, enemy.min.y);

	enemyvel.x += enemy.min.x;

	if (AEInputCheckTriggered(AEVK_ESCAPE)) 
		next = GS_QUIT;

	if (AEInputCheckCurr(AEVK_DOWN))
	{
		player.min.y -= 2.0f;
		player.max.y -= 2.0f;
		playervel.y -= 2.0f;
	}

	if (AEInputCheckCurr(AEVK_LEFT))
	{
		move_left = true;
		player.min.x -= 2.0f;
		playervel.x -= 2.0f;
	}

	if (AEInputCheckCurr(AEVK_RIGHT))
	{
		move_right = true;
		player.min.x += 2.0f;
		playervel.x += 2.0f;
	}

	if (CollisionIntersection_RectRect(player, playervel, enemy, enemyvel) == true)
		current = GS_QUIT;
}

void Level3_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(player.min.x, player.max.y);
	AEGfxTextureSet(playertex, 0.0f, 0.0f);
	AEGfxMeshDraw(playermesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetPosition(enemy.min.x, enemy.max.y);
	AEGfxTextureSet(enemytex, 0.0f, 0.0f);
	AEGfxMeshDraw(enemymesh, AE_GFX_MDM_TRIANGLES);

	// Drawing object platform 1
	AEGfxSetPosition(0.0f, 0.0f); //minus 20 y-axis for every platform position
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(platformtex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, platform1.min.x, platform1.min.y);
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(platformmesh, AE_GFX_MDM_TRIANGLES);

	// Drawing object platform 2
	AEGfxSetPosition(-180.0f, -180.0f); //minus 20 y-axis for every platform position
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(platformtex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, 0);
	translate = { 0 };
	AEMtx33Trans(&translate, platform2.min.x, platform2.min.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(platformmesh, AE_GFX_MDM_TRIANGLES);

	// Drawing object platform 3
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(180.0f, -180.0f); //minus 20 y-axis for every platform position
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(platformtex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	translate = { 0 };
	AEMtx33Trans(&translate, platform3.min.x, platform3.min.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(platformmesh, AE_GFX_MDM_TRIANGLES);
}

void Level3_Free()
{
}

void Level3_Unload()
{
	AEGfxTextureUnload(playertex);
	AEGfxMeshFree(playermesh);

	AEGfxTextureUnload(enemytex);
	AEGfxMeshFree(enemymesh);

	AEGfxTextureUnload(platformtex);
	AEGfxMeshFree(platformmesh);
}