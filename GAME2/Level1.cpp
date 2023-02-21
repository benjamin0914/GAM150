#include "pch.h"
#include "Level1.h"
#include "GameStateManager.h"
#include "AEEngine.h"
#include "function.hpp"
#include "Enemy.h"

AEGfxTexture* floorTex; // Pointer to floor image texture
AEGfxTexture* hole_big;
AEGfxTexture* playerTex;
AEGfxTexture* hole_small;
AEGfxTexture* hole_none;
AEGfxTexture* slimeTexture;
AEGfxTexture* pTimerTex;


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
float objtexX, objtexY; // Texture variables for object 2 and 3 texture

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





int counter = 0; // Counter to swap textures





void Level1_Load()
{
	//////////////////////////////////



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
	playerTex = AEGfxTextureLoad("Assets/player_cube.png");
	AE_ASSERT_MESG(floorTex, "Failed to create texture1!!");


	slimeTexture = AEGfxTextureLoad("Assets/slime.png");
	AE_ASSERT_MESG(slimeTexture, "Failed to load iamge");

	pTimerTex = AEGfxTextureLoad("Assets/Timer.png");

	// Loading textures (images) end
	//////////////////////////////////


}

void Level1_Initialize()
{
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
	objtexX = 0;
	objtexY = 0;

	jump_timer = 0.0f;

	obj1 = { 0.f, 0.f };
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




	// Object 1 Control
	if (AEInputCheckTriggered(AEVK_SPACE) && (false == falling) && (false == jump))
	{
		obj1Y_min = obj1.y;
		obj1Y_max = obj1.y + 130.0f;
		jump = true;
		//obj1Y += 130.0f;
	}
	else if (AEInputCheckCurr(AEVK_DOWN))
		obj1.y -= 2.0f;

	if (AEInputCheckCurr(AEVK_A))
		movement_left = true;
	else if (AEInputCheckCurr(AEVK_D))
		movement_right = true;
	else
	{
		movement_left = false;
		movement_right = false;
	}


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
		}*/
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


	/*
	//gravity mechanics //always start with higher platforms
	if ((AECalcDistPointToLineSeg(&obj1, &pt4, &pt5) >= 1.0f) && (obj1.x >= pt4.x) && (obj1.x <= pt5.x))
	{
		if (obj1.y > pt4.y)
		{
			obj1.y -= 2.0f;
			falling = true;
		}
	}
	else if ((AECalcDistPointToLineSeg(&obj1, &pt2, &pt3) >= 1.0f) && (obj1.x >= pt2.x) && (obj1.x <= pt3.x))
	{
		if (obj1.y > pt2.y)
		{
			obj1.y -= 2.0f;
			falling = true;
		}
	}
	else if ((AECalcDistPointToLineSeg(&obj1, &pt6, &pt7) >= 1.0f) && (obj1.x >= pt6.x) && (obj1.x <= pt7.x))
	{
		if (obj1.y > pt6.y)
		{
			obj1.y -= 2.0f;
			falling = true;
		}
	}
	else
	{
		falling = false;
	}*/   //gravity ends

	/*
	//left wall //start with right-most wall first
	if ((obj1.y <= pt1.y) && (obj1.y >= pt2.y))
	{
		if (AECalcDistPointToLineSeg(&obj1, &pt1, &pt2) <= 1.0f)
		{
			movement_left = false;
		}
		else
		{
			movement_left = true;
		}
	}*/
	/*
	if ((obj1.y <= pt1.y) && (obj1.y >= pt2.y))
	{
		if ((AECalcDistPointToLineSeg(&obj1, &pt1, &pt2) <= 1.0f) && (true == movement_left))
		{
			movement_left = false;
		}
	}*/

	if (true == movement_left)
	{
		if (AECalcDistPointToLineSeg(&obj1, &pt1, &pt2) <= 1.0f)
		{
			;
		}
		else
		{
			obj1.x -= 5.0f;
		}
	}
	if (true == movement_right)
	{
		if (AECalcDistPointToLineSeg(&obj1, &pt7, &pt8) <= 1.0f)
		{
			;
		}
		else
		{
			obj1.x += 5.0f;
		}
	}





	if (AEInputCheckTriggered(AEVK_M))
		obj1.y += 130.0f;
	if (AEInputCheckTriggered(AEVK_N))
		obj1.y -= 130.0f;



	// Texture offset
	if (AEInputCheckCurr(AEVK_L))
		objtexX -= 0.01f;
	else if (AEInputCheckCurr(AEVK_J))
		objtexX += 0.01f;

	if (AEInputCheckCurr(AEVK_I))
		objtexY += 0.01f;
	else if (AEInputCheckCurr(AEVK_K))
		objtexY -= 0.01f;

	updatePos(slime_pos.x, slime_pos.y);
	AEGfxSetCamPosition(obj1.x, obj1.y);
	// Game loop update end
	///////////////////////
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




	/// <Draw Player start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Player Position
	AEGfxSetPosition(0.f, 0.f);
	// Player no tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Player texture
	AEGfxTextureSet(playerTex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 60.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.5f);
	translate = { 0 };
	AEMtx33Trans(&translate, obj1.x, obj1.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);
	/// <Draw Player end>




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
	
}

void Level1_Free()
{
	
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
}