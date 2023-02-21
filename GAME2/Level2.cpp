#include "pch.h"

AEGfxTexture* pPlayerTex;
AEGfxTexture* pPlatformTex;
AEGfxTexture* pEnemyTex;

const unsigned int	GAME_OBJ_NUM_MAX = 2048;			// The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;			// The total number of different game object instances

const unsigned int	PLAYER_LIVES = 3;			// initial number of player lives
const float			PLAYER_SIZE = 50.0f;		// player size
const float			PLAYER_ACCEL_LEFT = 40.0f;		// player forward acceleration (in m/s^2)
const float			PLAYER_ACCEL_RIGHT = 40.0f;		// player backward acceleration (in m/s^2)
const float			PLAYER_JUMP = 0.0f;			//player jump/gravity??

const float         BOUNDING_RECT_SIZE = 1.0f;         // this is the normalized bounding rectangle (width and height) sizes - AABB collision data
//static bool onValueChange = true;

const unsigned long FLAG_ACTIVE = 0x00000001;

enum TYPE
{
	// list of game object types
	TYPE_PLAYER = 0,
	TYPE_ENEMY,
	TYPE_PLATFORM,

	//TYPE_NUM
};


struct GameObj
{
	unsigned long	 type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
};

struct GameObjInst
{
	GameObj*			pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction
	AABB				boundingBox;// object bouding box that encapsulates the object
	AEMtx33				transform;	// object transformation matrix: Each frame, 
									// calculate the object instance's transformation matrix and save it here
};

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer to the ship object
static GameObjInst*			spPlayer;										// Pointer to the "Player" game object instance

// number of ship available (lives 0 = game over)
static long					sPlayerLives;									// The number of lives left

GameObjInst* gameObjInstCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir);
void		 gameObjInstDestroy(GameObjInst* pInst);


void Level2_Load()
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	spPlayer = nullptr;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the player shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYER;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

	// =======================
	// create the enemy shape
	// =======================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_ENEMY;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.1f, 0.1f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.1f, -0.1f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.1f, 0.1f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.1f, -0.1f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.1f, -0.1f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.1f, 0.1f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

	// =========================
	// create the platform shape
	// =========================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLATFORM;

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

	//Load textures

	pPlayerTex = AEGfxTextureLoad("Assets/player_cube.png");
	AE_ASSERT_MESG(pPlayerTex, "Failed to create texture1!!");
	pEnemyTex = AEGfxTextureLoad("Assets/slime.png");
	AE_ASSERT_MESG(pEnemyTex, "Failed to create texture1!!");
	pPlatformTex = AEGfxTextureLoad("Assets/Special_Tile.png");
	AE_ASSERT_MESG(pPlatformTex, "Failed to create texture1!!");
}

void Level2_Initialize()
{
	AEGfxSetBackgroundColor(0.4f, 0.5f, 0.9f);

	// create the main player
	spPlayer = gameObjInstCreate(TYPE_PLAYER, PLAYER_SIZE, nullptr, nullptr, 0.0f);
	AE_ASSERT(spPlayer);

	// reset the number of player lives
	sPlayerLives = PLAYER_LIVES;
}

void Level2_Update()
{
	if (AEInputCheckTriggered(AEVK_RETURN))
		next = GS_LEVEL3;

	if (AEInputCheckTriggered(AEVK_ESCAPE)) next = GS_QUIT;

	// =========================
	// update according to input
	// =========================

	if (AEInputCheckCurr(AEVK_LEFT))
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(spPlayer->dirCurr), 0);
		////Accelerate when left button is pressed
		//AEVec2 acceleration;
		//AEVec2Set(&acceleration, added.x * 100, 0);
		//AEVec2Set(&spPlayer->velCurr, acceleration.x * AEFrameRateControllerGetFrameTime() + spPlayer->velCurr.x, 0);
		//spPlayer->velCurr.x *= 0.99;
		////spPlayer->velCurr.y *= 0.99;

		spPlayer->dirCurr += PLAYER_ACCEL_LEFT * (float)(AEFrameRateControllerGetFrameTime());
		spPlayer->dirCurr = AEWrap(spPlayer->dirCurr, -PI, 0);
	}

	if (AEInputCheckCurr(AEVK_RIGHT))
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(spPlayer->dirCurr), 0);
		////Accelerate when right button is pressed
		//AEVec2 acceleration;
		//AEVec2Set(&acceleration, added.x * 100, 0);
		//AEVec2Set(&spPlayer->velCurr, acceleration.x * AEFrameRateControllerGetFrameTime() + spPlayer->velCurr.x, 0);
		//spPlayer->velCurr.x *= 0.99;
		////spPlayer->velCurr.y *= 0.99;

		spPlayer->dirCurr += PLAYER_ACCEL_RIGHT * (float)(AEFrameRateControllerGetFrameTime());
		spPlayer->dirCurr = AEWrap(spPlayer->dirCurr, -PI, 0);
	}

	// ======================================================
	// update physics of all active game object instances
	//  -- Get the AABB bounding rectangle of every active instance
	// ======================================================

	//--Positions of the instances are updated here with the already computed velocity

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		pInst->posCurr.x += pInst->velCurr.x * AEFrameRateControllerGetFrameTime();
		//pInst->posCurr.y += pInst->velCurr.y * AEFrameRateControllerGetFrameTime();

		pInst->boundingBox.min = { -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x,
		-(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y };

		pInst->boundingBox.max = { (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x,
		(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y };
	}

	//ADD COLLISION CHECK

	// ===================================
	// update active game object instances
	// ===================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// check if the object is a ship
		if (pInst->pObject->type == TYPE_PLAYER)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - PLAYER_SIZE,
				AEGfxGetWinMaxX() + PLAYER_SIZE);
			//pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - PLAYER_SIZE, AEGfxGetWinMaxY() + PLAYER_SIZE);
		}

		if (pInst->pObject->type == TYPE_ENEMY)
		{
			// wrap the asteroids from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX(),
				AEGfxGetWinMaxX());
			//pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY(), AEGfxGetWinMaxY());
		}

		if (pInst->pObject->type == TYPE_PLATFORM)
		{
			// wrap the platform from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX(),
				AEGfxGetWinMaxX());
			//pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY(), AEGfxGetWinMaxY());
		}
	}

	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &rot, &scale);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
	}
}

void Level2_Draw()
{
	//char strBuffer[1024];

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for objects
		AEGfxSetPosition(0.f, 0.f);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxSetTransform(pInst->transform.m);
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

		if (pInst->pObject->type == TYPE_PLAYER)
			AEGfxTextureSet(pPlayerTex, 0, 0);

		if (pInst->pObject->type == TYPE_PLATFORM)
			AEGfxTextureSet(pPlatformTex, 0, 0);

		if (pInst->pObject->type == TYPE_ENEMY)
			AEGfxTextureSet(pEnemyTex, 0, 0);
	}
}

void Level2_Free()
{
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag == FLAG_ACTIVE)
			gameObjInstDestroy(pInst);
	}
}

void Level2_Unload()
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pObj = sGameObjList + i;
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}
}



GameObjInst* gameObjInstCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}