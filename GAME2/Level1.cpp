#include "pch.h"
#include "Level1.h"
#include "GameStateManager.h"
#include "AEEngine.h"
#include "function.hpp"
#include "Enemy.h"
#include <string>
#include "Globals.h"
#include "BinaryMap.h"
#include "Tiles.h"


AEGfxTexture* floorTex; // Pointer to floor image texture
AEGfxTexture* hole_big;
AEGfxTexture* hole_small;
AEGfxTexture* hole_none;
AEGfxTexture* slimeTexture;
AEGfxTexture* pTimerTex;

AEGfxTexture* man;

AEGfxVertexList* pMeshLine;
AEGfxVertexList* pMeshBox;
AEGfxVertexList* pMeshRect;

AEGfxVertexList* slime;
//float slimeX, slimeY;
AEVec2 slime_pos;


AEVec2 pt1 = { -250.0f, 200.0f };
AEVec2 pt2 = { -250.0f, -120.0f };
AEVec2 pt3 = { -100.0f, -120.0f };
AEVec2 pt4 = { -100.0f, -20.0f };
AEVec2 pt5 = { 100.0f, -20.0f };
AEVec2 pt6 = { 100.0f, -120.0f };
AEVec2 pt7 = { 250.0f, -120.0f };
AEVec2 pt8 = { 250.0f, 200.0f };



//float obj1X = 0.0f, obj1Y = 0.0f; // Position variables for object 1

bool jump;
bool falling;
bool movement_left;
bool movement_right;

float obj1Y_min;
float obj1Y_max;
float jump_timer;
float const duration = 1.0f; //duration it takes to jump upwards

AEVec2 obj1;
AEVec2 plf1;
AEVec2 plf2;
AEVec2 plf3;



//test
AEVec2 test = { 0.f, 1.f };

std::vector<Tiles> tilemap;
std::vector <std::vector <Tiles>*> tileManager;
extern AEVec2 EntitySizeArray[static_cast<int>(EntitySizes::MAX)];





int counter = 0; // Counter to swap textures

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 32;					// The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;			// The total number of different game object instances


const unsigned int	SHIP_INITIAL_NUM = 3;			// initial number of ship lives
const float			PLAYER_SIZE = 60.0f;					// player size
const float			PLAYER_MOVEMENT = 120.0f;				// player movement (how much movement in one second)
const float			SHIP_ACCEL_FORWARD = 40.0f;		// ship forward acceleration (in m/s^2)
const float			SHIP_ACCEL_BACKWARD = 40.0f;		// ship backward acceleration (in m/s^2)
const float			SHIP_ROT_SPEED = (2.0f * PI);	// ship rotation speed (degree/second)


const float			BULLET_SIZE = 12.0f;			// bullet size (self added)
const float			BULLET_SPEED = 100.0f;		// bullet speed (m/s)

const float         BOUNDING_RECT_SIZE = 1.0f;         // this is the normalized bounding rectangle (width and height) sizes - AABB collision data

// -----------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_PLAYER = 0,
	TYPE_BULLET,
	TYPE_ASTEROID,

	TYPE_NUM
};

// -----------------------------------------------------------------------------
// object flag definition

const unsigned long FLAG_ACTIVE = 0x00000001;

/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/

//Game object structure
struct GameObj
{
	unsigned long		type;		// object type
	AEGfxVertexList*	pMesh;		// This will hold the triangles which will form the shape of the object
};

// ---------------------------------------------------------------------------

//Game object instance structure
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

	//void				(*pfUpdate)(void);
	//void				(*pfDraw)(void);
};

/******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer to the ship object
static GameObjInst* spPlayer;										// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					sShipLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sScore;										// Current score

// flag to trigger printing
static bool					onValueChange;								// Printing flag
static bool					win;										// Win flag

// timer to trigger spawn
static float				timer_spawn;								// Spawn timer

// textures declaration
static AEGfxTexture*		playerTex;									// Player texture

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned long type, float scale,
	AEVec2* pPos, AEVec2* pVel, float dir);
void					gameObjInstDestroy(GameObjInst* pInst);
/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>

void Level1_Load()
{

	static const std::string BaseLevelString{ "./Assets/Levels/Level" };

	std::string File = BaseLevelString + std::to_string(LEVELS::Level) + ".txt";

	if (!ImportMapDataFromFile(File.c_str())) {
		next = GS_MAINMENU;
	}
	Tiles::LoadTex();
	
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
	// create the ship shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYER;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FF0FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0x00FFFFFF, 0.0f, 1.0f,
		0.5, -0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0x00FFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create square box mesh!!");
	//////////////////////////////////////////////////////////////////////////////////



	// Informing the library that we're about to start adding vertices
	AEGfxMeshStart();

	// This shape has 5 vertices
	AEGfxVertexAdd(pt1.x, pt1.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt2.x, pt2.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt3.x, pt3.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt4.x, pt4.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt5.x, pt5.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt6.x, pt6.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt7.x, pt7.y, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(pt8.x, pt8.y, 0xFFFFFFFF, 0.0f, 0.0f);


	pMeshLine = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshLine, "Failed to create line mesh!!");



	// Informing the library that we're about to start adding vertices
	AEGfxMeshStart();

	// This shape is a 1 by 1 square box, rmb to use scaling
	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FF0FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0x00FFFFFF, 0.0f, 1.0f,
		0.5, -0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0x00FFFFFF, 0.0f, 0.0f);


	pMeshBox = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshBox, "Failed to create square box mesh!!");

	//SLIME TEXT
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00FF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00FFFF00, 0.25f, 1.0f,
		-0.5f, 0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00FFFFFF, 0.25f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 0.25f, 0.0f,
		-0.5f, 0.5f, 0x00FFFFFF, 0.0f, 0.0f);

	slime = AEGfxMeshEnd();
	AE_ASSERT_MESG(slime, "Failed to create slime mesh");


	////////////////////////////
	// Loading textures (images)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,// Bottom Left
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, // Bottom Right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); // Top vertex

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, //	Bottom Right
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,	 //	Top Right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);//	Top Left
	pMeshRect = AEGfxMeshEnd();

	// Texture 1: From file
	floorTex = AEGfxTextureLoad("Assets/Special_Tile.png");
	hole_big = AEGfxTextureLoad("Assets/hole_big.png");
	hole_small = AEGfxTextureLoad("Assets/hole_small.png");
	hole_none = AEGfxTextureLoad("Assets/hole_none.png");
	man =  AEGfxTextureLoad("Assets/player_cube.png");
	AE_ASSERT_MESG(floorTex, "Failed to create texture1!!");


	slimeTexture = AEGfxTextureLoad("Assets/slime.png");
	AE_ASSERT_MESG(slimeTexture, "Failed to load iamge");

	pTimerTex = AEGfxTextureLoad("Assets/Timer.png");

	// Loading textures (images) end
	//////////////////////////////////




	// =========================
	// load the player texture
	// =========================
	playerTex = AEGfxTextureLoad("Assets/player_cube.png");
	AE_ASSERT_MESG(playerTex, "Failed to create player texture!!");
}

void Level1_Initialize()
{
	// Create the main ship
	AEVec2 init_ship_pos = { 0.f, 0.f };
	AEVec2 init_ship_vel = { 0.f, 0.f };
	spPlayer = gameObjInstCreate(TYPE_PLAYER, PLAYER_SIZE, &init_ship_pos, &init_ship_vel, PI / 2.f);
	AE_ASSERT(spPlayer);

	f32 grid_height{ static_cast<f32>(Map_Height) }, grid_width{ static_cast<f32>(Map_Width) };
	for (int i = 0; i < Map_Height; ++i)
	{
		for (int j = 0; j < Map_Width; ++j)
		{ // Iterate through mapdata array and construct objects at position [i][j] (Y/X)
			if (MapData[i][j] == static_cast<int>(TYPE_OBJECT::EMPTY) || MapData[i][j] >= static_cast<int>(TYPE_OBJECT::MAX))
			{
				continue;
			}
			else if (MapData[i][j] == static_cast<int>(TYPE_OBJECT::BLACK))
			{
				Tiles::AddTile(tilemap, TileType::Black, grid_width, grid_height, AEVec2Set(j * grid_width, i * grid_height));
			}
			else if (MapData[i][j] == static_cast<int>(TYPE_OBJECT::DIRT))
			{
				Tiles::AddTile(tilemap, TileType::Dirt, grid_width, grid_height, AEVec2Set(j * grid_width, i * grid_height));
			}
			else if (MapData[i][j] == static_cast<int>(TYPE_OBJECT::PLAYER))
			{
				//Supposed to set the player size, but no nid for now
				obj1.x = (j * grid_width) - (grid_width * 3.5f);
				obj1.y = -(i * grid_height) + (grid_height * 4.28f);
			}
			else if (MapData[i][j] == static_cast<int>(TYPE_OBJECT::SLIME))
			{
				//NOT WORKING YET 
				slime_pos.x = (j * grid_width) - (grid_width * 3.5f);
				slime_pos.y = -(i * grid_height) + (grid_height * 4.28f);

			}
		}
	}



	/*
	AEVec2Set(&pt1, -250.f, 200.0f);
	AEVec2Set(&pt2, -250.f, 170.0f);
	AEVec2Set(&pt3, -100.f, -120.0f);
	AEVec2Set(&pt4, -100.f, -20.0f);
	AEVec2Set(&pt5, 100.f, -20.0f);
	AEVec2Set(&pt6, 100.f, -120.0f);
	AEVec2Set(&pt7, 250.0f, -120.0f);
	AEVec2Set(&pt8, 250.f, 200.0f);
	*/

	slime_pos.x = 110.0f;
	slime_pos.y = -130.0f;

	jump_timer = 0.0f;

	//obj1 = { 0.f, 0.f };
	plf1 = { 0.f, -80.f };
	plf2 = { -180.f, -180.f };
	plf3 = { 180.f, -180.f };

	jump = { false };
	falling = { false };
	movement_left = { false };
	movement_right = { false };

	AEGfxSetBackgroundColor(0.4f, 0.5f, 0.9f);
	////////////////////////////////
// Creating the objects (Shapes)
}

void Level1_Update()
{


			///////////////////
		// Game loop update


		// Press escape button to quit
	if (AEInputCheckTriggered(AEVK_ESCAPE)) next = GS_QUIT;


		// Blending mode
	if (AEInputCheckCurr(AEVK_1))
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
	else if (AEInputCheckCurr(AEVK_2))
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	else if (AEInputCheckCurr(AEVK_3))
		AEGfxSetBlendMode(AE_GFX_BM_ADD);

	AEGfxSetCamPosition(obj1.x, obj1.y);


// Object 1 Control

	if (AEInputCheckCurr(AEVK_W) )
	{
		 obj1.y += 10.0f;
		
		//obj1Y += 130.0f;
	}	if (AEInputCheckCurr(AEVK_D))
	{
		obj1.x += 10.0f;

		//obj1Y += 130.0f;
	}
	if (AEInputCheckCurr(AEVK_A))
	{
		obj1.x -= 10.0f;

		//obj1Y += 130.0f;
	}
	if (AEInputCheckCurr(AEVK_S))
	{
		obj1.y -= 10.0f;

		//obj1Y += 130.0f;
	}
	/*
	// Movement for left & right start
	if (!AEInputCheckCurr(AEVK_A) && !AEInputCheckCurr(AEVK_D))
	{
		movement_left = false;
		movement_right = false;
	}
	if (AEInputCheckCurr(AEVK_A))
	{
		movement_left = true;
	}
	if (AEInputCheckCurr(AEVK_D))
	{
		movement_right = true;
	}
	// Movement for left & right start


	//jumping mechanics
	if (true == jump)
	{

		jump_timer += static_cast<float>(AEFrameRateControllerGetFrameTime());

		if (jump_timer < duration)
		{
			obj1.y = EaseOutCubic(obj1Y_min, obj1Y_max, jump_timer / duration);
			//only part needed for jumping, gravity takes care of the rest
			//while not jumping && not on platform THEN gravity starts
		}
		/*else if (jump_timer < duration)
		{
			obj1.y = EaseOutCubic(obj1Y_max, obj1Y_min, jump_timer / duration);
		}
		else if (jump_timer >= duration)
		{
			jump_timer = 0.0f;
			jump = false;
		}
	}

	//std::cout << AECalcDistRectToRect(&obj1, 60.f, 60.f, &plf1, 180.f, 60.f, nullptr);
	//gravity mechanics //always start with higher platforms
	if ((AECalcDistPointToLineSeg(&obj1, &pt4, &pt5) >= 1.0f) && (obj1.x >= pt4.x) && (obj1.x <= pt5.x) && (obj1.y > pt4.y))
	{
		obj1.y -= 2.0f;
		falling = true;
	}
	else if ((AECalcDistPointToLineSeg(&obj1, &pt2, &pt3) >= 1.0f) && (obj1.x >= pt2.x) && (obj1.x <= pt3.x) && (obj1.y > pt2.y))
	{
		obj1.y -= 2.0f;
		falling = true;
	}
	else if ((AECalcDistPointToLineSeg(&obj1, &pt6, &pt7) >= 1.0f) && (obj1.x >= pt6.x) && (obj1.x <= pt7.x) && (obj1.y > pt6.y))
	{
		obj1.y -= 2.0f;
		falling = true;
	}
	else
	{
		falling = false;
	}


	*/




	if (AEInputCheckTriggered(AEVK_M))
		spPlayer->velCurr.x += 10.0f;
	if (AEInputCheckTriggered(AEVK_N))
		spPlayer->velCurr.x -= 10.0f;




	updatePos(slime_pos.x, slime_pos.y);
	//
	// AEGfxSetCamPosition(obj1.x, obj1.y);
	// Game loop update end
	////////////////////////


	// Time-based movement for player
	if (true == movement_left)
	{
		// Make sure to check for left wall before actually moving
		spPlayer->posCurr.x -= PLAYER_MOVEMENT * g_dt;
	}
	if (true == movement_right)
	{
		// Make sure to check for right wall before actually moving
		spPlayer->posCurr.x += PLAYER_MOVEMENT * g_dt;
	}


	// Constant update of Player position from its velocity
	AEVec2 playernewvel;
	AEVec2Scale(&playernewvel, &spPlayer->velCurr, g_dt);
	AEVec2Add(&spPlayer->posCurr, &spPlayer->posCurr, &playernewvel);


	// ======================================================
	// update physics of all active game object instances
	//  -- Get the AABB bounding rectangle of every active instance:
	//		boundingRect_min = -(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	//		boundingRect_max = +(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	//
	//	-- Positions of the instances are updated here with the already computed velocity (above)
	// ======================================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// Check if current instance is used
		if (sGameObjInstList[i].flag == FLAG_ACTIVE)
		{
			// Computes the AABB bounding rectangle of instances
			sGameObjInstList[i].boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.f) * sGameObjInstList[i].scale + sGameObjInstList[i].posCurr.x;
			sGameObjInstList[i].boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.f) * sGameObjInstList[i].scale + sGameObjInstList[i].posCurr.y;
			sGameObjInstList[i].boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.f) * sGameObjInstList[i].scale + sGameObjInstList[i].posCurr.x;
			sGameObjInstList[i].boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.f) * sGameObjInstList[i].scale + sGameObjInstList[i].posCurr.y;

			// Update positions of instances with computed velocity
			AEVec2 newvel;
			AEVec2Scale(&newvel, &sGameObjInstList[i].velCurr, g_dt);
			AEVec2Add(&sGameObjInstList[i].posCurr, &sGameObjInstList[i].posCurr, &newvel);
		}
	}






	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		trans = rot = scale = { 0 };

		UNREFERENCED_PARAMETER(trans);
		UNREFERENCED_PARAMETER(rot);
		UNREFERENCED_PARAMETER(scale);

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
		AEMtx33Concat(&(pInst->transform), &rot, &scale);
		AEMtx33Concat(&(pInst->transform), &trans, &pInst->transform);

	}
	
}

void Level1_Draw()
{
	//////////////////
		// Game loop draw

	AEMtx33 scale = { 0 };
	AEMtx33 rotate = { 0 };
	AEMtx33 translate = { 0 };
	AEMtx33 transform = { 0 };
	
	



	//line strips 8 vertices
	
	for (size_t i = 0; i < tilemap.size(); ++i)
	{
		tilemap[i].Render();
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set position
	AEGfxSetPosition(0.0f, 0.0f); //minus 20 y-axis for every platform position
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Floor texture
	AEGfxTextureSet(man, 0, 0);

	scale = { 0 };
	AEMtx33Scale(&scale, 50.f, 50.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	translate = { 0 };
	AEMtx33Trans(&translate, obj1.x, obj1.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(Mesh::Rect, AE_GFX_MDM_TRIANGLES);
	/// <Draw top platform start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set position
	AEGfxSetPosition(0.0f, 0.0f); //minus 20 y-axis for every platform position
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Floor texture
	AEGfxTextureSet(floorTex, 0, 0);

	scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	translate = { 0 };
	AEMtx33Trans(&translate, plf1.x, plf1.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);
	/// <Draw top platform end>


	/// <Draw left platform start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set position
	AEGfxSetPosition(-180.0f, -180.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Floor texture
	AEGfxTextureSet(floorTex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, 0);
	translate = { 0 };
	AEMtx33Trans(&translate, -180.f, -180.f);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);
	/// <Draw left platform end>



	/// <Draw right platform start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set position
	AEGfxSetPosition(180.0f, -180.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Floor texture
	AEGfxTextureSet(floorTex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 180.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	translate = { 0 };
	AEMtx33Trans(&translate, 180.f, -180.f);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);
	/// <Draw right platform end>






	/// <Draw Slime start>
	static f32 slime_timer = 0;
	slime_timer += g_dt;
	static f32 slime_offset = 0.f;
	if (slime_timer > 0.2f)
	{
		slime_timer = 0.f;
		slime_offset += 0.25f;
	}
	//slime draw
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.f, 0.f);
	AEGfxTextureSet(slimeTexture, slime_offset, 0.f);
	scale = { 0 };
	AEMtx33Scale(&scale, 40.f, 40.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.f);
	translate = { 0 };
	AEMtx33Trans(&translate, slime_pos.x, slime_pos.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(slime, AE_GFX_MDM_TRIANGLES);
	/// <Draw Slime end>
	

	/// <Draw left platform start>
	static f32 elapsed = 0;
	elapsed += g_dt;
	AEGfxSetTransparency(1.0f);
	if (elapsed < 10.f)
	{
		AEGfxTextureSet(hole_big, 0.0f, 0.0f);
	}
	else if (elapsed < 10.5f)
	{
		AEGfxTextureSet(hole_none, 0.0f, 0.0f);
	}
	else if (elapsed < 11.f)
	{
		AEGfxTextureSet(hole_big, 0.0f, 0.0f);
	}
	else if (elapsed < 11.5f)
	{
		AEGfxTextureSet(hole_none, 0.0f, 0.0f);
	}
	else if (elapsed < 12.f)
	{
		AEGfxTextureSet(hole_big, 0.0f, 0.0f);
	}
	else if (elapsed < 12.5f)
	{
		AEGfxTextureSet(hole_none, 0.0f, 0.0f);
	}
	else if (elapsed < 13.f)
	{
		AEGfxTextureSet(hole_big, 0.0f, 0.0f);
	}
	else if (elapsed < 14.f)
	{
		AEGfxTextureSet(hole_none, 0.0f, 0.0f);
	}
	else
	{
		AEGfxTextureSet(hole_small, 0.0f, 0.0f);
	}
	scale = { 0 };
	AEMtx33Scale(&scale, 810.f, 700.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, 0);
	translate = { 0 };
	AEMtx33Trans(&translate, obj1.x, obj1.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(pMeshRect, AE_GFX_MDM_TRIANGLES);






	/// <Draw Timer Bar start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(pTimerTex, 0, 0);
	if ((500.f - 50.0f *elapsed) > 0) {
		AEMtx33Scale(&scale, 500.f - 50.0f * elapsed, 50.f);
	}
	else {
		AEMtx33Scale(&scale, 0, 0);
	}
	
	AEMtx33Rot(&rotate, PI);

	AEMtx33Trans(&translate, obj1.x, obj1.y+250);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMeshRect, AE_GFX_MDM_TRIANGLES);
	/// <Draw Timer Bar end>


	/// <summary>
	/// ///////////////////////////////////////////////////////////////////////////////////////////////////
	/// </summary>
	char strBuffer[1024];

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);


	// =========================
	// Draw all object instances in the list
	// =========================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;


		if (pInst->pObject->type == TYPE_PLAYER)
		{
			// Set texture for all asteroid object
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(playerTex, 0.f, 0.f);
		}
		else
		{
			// No texture for everything else
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(NULL, 0.f, 0.f);
		}


		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
	//////////////////////////////////////////////////////////////////////////////////////
}

void Level1_Free()
{
	// =========================
	// Set all object instance flag to 0
	// =========================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		gameObjInstDestroy(sGameObjInstList + i);
	}

	// =========================
	// Set player flag to 0
	// =========================
	gameObjInstDestroy(spPlayer);
}

void Level1_Unload()
{
	AEGfxTextureUnload(floorTex);
	AEGfxTextureUnload(pTimerTex);

	// Free the circle hole textures
	AEGfxTextureUnload(hole_big);
	AEGfxTextureUnload(hole_small);
	AEGfxTextureUnload(hole_none);


	
	AEGfxMeshFree(pMeshLine);
	AEGfxMeshFree(pMeshBox);
	AEGfxMeshFree(pMeshRect);



	// =========================
	// Free all meshes
	// =========================
	while (--sGameObjNum != 0)
	{
		AEGfxMeshFree((sGameObjList + sGameObjNum)->pMesh);
	}
	AEGfxMeshFree((sGameObjList + sGameObjNum)->pMesh);


	// =========================
	// Unload all textures
	// =========================
	AEGfxTextureUnload(playerTex);
}






/******************************************************************************/
/*!
	Object Instance create function of this state
*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned long type,
	float scale,
	AEVec2* pPos,
	AEVec2* pVel,
	float dir)
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

/******************************************************************************/
/*!
	Object Instance destroy function of this state
*/
/******************************************************************************/
void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}