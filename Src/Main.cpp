// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "function.h"
#include <iostream>



// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////
	// Variable declaration

	int gGameRunning = 1;
	AEGfxVertexList* pMesh1 = 0;
	AEGfxVertexList* pMesh2 = 0; // Pointer to Mesh (Model)
	AEGfxVertexList* pMeshLine;
	AEGfxVertexList* pMeshBox;
	AEGfxTexture* pTex1; // Pointer to Texture (Image)
	AEGfxTexture* pTex2; // Pointer to Texture (Image)

	//float obj1X = 0.0f, obj1Y = 0.0f; // Position variables for object 1
	float objtexX = 0, objtexY = 0; // Texture variables for object 2 and 3 texture

	bool jump{ false };
	bool falling{ false };
	bool movement_left{ false };
	bool movement_right{ false };

	float obj1Y_min;
	float obj1Y_max;
	float jump_timer = 0.0f;
	float const duration = 1.0f; //duration it takes to jump upwards

	AEVec2 obj1 = { 0.f, 0.f };
	AEVec2 plf1 = { 0.f, -80.f };
	AEVec2 plf2 = { -180.f, -180.f };
	AEVec2 plf3 = { 180.f, -180.f };

	AEVec2 pt1 = { -250.0f, 200.0f };
	AEVec2 pt2 = { -250.0f, -120.0f };
	AEVec2 pt3 = { -100.0f, -120.0f };
	AEVec2 pt4 = { -100.0f, -20.0f };
	AEVec2 pt5 = { 100.0f, -20.0f };
	AEVec2 pt6 = { 100.0f, -120.0f };
	AEVec2 pt7 = { 250.0f, -120.0f };
	AEVec2 pt8 = { 250.0f, 200.0f };


	//test
	AEVec2 test = { 0.f, 1.f };


	AEVec2 testtest = { 0.f, 0.f };
	AEVec2 testtest2 = { 0.f, 0.f };
	

	int counter = 0; // Counter to swap textures


	// Variable declaration end
	///////////////////////////


	/////////////////
	// Initialization

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	//set background color
	AEGfxSetBackgroundColor(0.4f, 0.5f, 0.9f);


	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	// Initialization end
	/////////////////////



	////////////////////////////////
	// Creating the objects (Shapes)

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");



	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh2

	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 2!!");



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
		-0.5f, 0.5f, 0x00FF00FF, 0.0f, 1.0f,
		-0.5f, -0.5f, 0x00FFFF00, 1.0f, 1.0f,
		0.5, -0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0x00FFFFFF, 0.0f, 0.0f);


	pMeshBox = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshBox, "Failed to create square box mesh!!");

	// Creating the objects (Shapes) end
	////////////////////////////////////



	////////////////////////////
	// Loading textures (images)


	// Texture 1: From file
	pTex1 = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AE_ASSERT_MESG(pTex1, "Failed to create texture1!!");

	// Texture 2: From file
	pTex2 = AEGfxTextureLoad("Assets/YellowTexture.png");
	AE_ASSERT_MESG(pTex2, "Failed to create texture2!!");


	// Loading textures (images) end
	//////////////////////////////////

	//////////////////////////////////
	// Creating Fonts	

	// Creating Fonts end
	//////////////////////////////////


	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		///////////////////
		// Game loop update

		
		// Blending mode
		if (AEInputCheckCurr(AEVK_1))
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
		else if (AEInputCheckCurr(AEVK_2))
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		else if (AEInputCheckCurr(AEVK_3))
			AEGfxSetBlendMode(AE_GFX_BM_ADD);


		// Object 1 Control
		if (AEInputCheckCurr(AEVK_UP))
			obj1.y += 2.0f;
		else if (AEInputCheckCurr(AEVK_DOWN))
			obj1.y -= 2.0f;

		if (AEInputCheckCurr(AEVK_LEFT))
			obj1.x -= 2.0f;
		else if (AEInputCheckCurr(AEVK_RIGHT))
			obj1.x += 2.0f;


		// Object 1 Control (modified) kekekekekekkekekekekkekekekekkekekekekekekekekekekekkekekekekkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
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
		//if ((AECalcDistRectToRect(&obj1, 60.f, 60.f, &plf1, 180.f, 60.f, nullptr) >= 1.0f) && (obj1.y > plf1.y))
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


		testtest = { 500.f, 500.f };
		testtest2 = { 0.f, 80.f };
		std::cout << AECalcDistRectToRect(&testtest, 20.f, 20.f, &testtest2, 20.f, 20.f, nullptr) << std::endl;





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

		// Game loop update end
		///////////////////////



		//////////////////
		// Game loop draw


		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for object 1
		AEGfxSetPosition(0.f, 0.f);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 60.f, 60.f);
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI * 2.f);
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, obj1.x, obj1.y);
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);

		/*
		// Drawing object 2 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for object 2
		AEGfxSetPosition(100.0f, -60.0f);	
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set Transparency
		AEGfxSetTransparency(1.0f);
		// Set texture
		++counter;
		if (counter < 120)
			AEGfxTextureSet(pTex1, objtexX, objtexY);		// Same object, different texture
		else if (counter < 240)
			AEGfxTextureSet(pTex2, objtexX, objtexY);		// Same object, different texture
		else
		{
			AEGfxTextureSet(pTex1, objtexX, objtexY);		// Same object, different texture
			counter = 0;
		}
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);*/


		//line strips 8 vertices
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(0.0f, -30.0f);
		AEGfxMeshDraw(pMeshLine, AE_GFX_MDM_LINES_STRIP);


		// Drawing object platform 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for platform 1
		AEGfxSetPosition(0.0f, 0.0f); //minus 20 y-axis for every platform position
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// No texture for platform 1
		AEGfxTextureSet(NULL, 0, 0);

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

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);


		// Drawing object platform 2
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for platform 2
		AEGfxSetPosition(-180.0f, -180.0f); //minus 20 y-axis for every platform position
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// No texture for platform 2
		AEGfxTextureSet(NULL, 0, 0);
		scale = { 0 };
		AEMtx33Scale(&scale, 180.f, 60.f);
		rotate = { 0 };
		AEMtx33Rot(&rotate, PI * 2.f);
		translate = { 0 };
		AEMtx33Trans(&translate, -180.f, -180.f);
		transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);

		// Drawing object platform 3
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for platform 3
		AEGfxSetPosition(180.0f, -180.0f); //minus 20 y-axis for every platform position
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// No texture for platform 3
		AEGfxTextureSet(NULL, 0, 0);
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
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMeshBox, AE_GFX_MDM_TRIANGLES);

		// Game loop draw end
		/////////////////////



		// Informing the system about the loop's end
		AESysFrameEnd();


		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxMeshFree(pMeshLine);
	AEGfxMeshFree(pMeshBox);
	AEGfxTextureUnload(pTex1);
	AEGfxTextureUnload(pTex2);

	// free the system
	AESysExit();
}