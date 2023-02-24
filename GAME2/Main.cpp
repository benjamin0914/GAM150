// ---------------------------------------------------------------------------
// includes

#include "Enemy.h"
#include "pch.h"
#include "AEEngine.h"
#include "function.hpp"
#include <iostream>
#include "Enemy.h"
#include "Graphics.h"

#include "GameStateManager.h"
#include "GameStateList.h"



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

	//int gGameRunning = 1;

	// Variable declaration end
	///////////////////////////


	/////////////////
	// Initialization
	
	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	//set background color
	AEGfxSetBackgroundColor(0.4f, 0.5f, 0.9f);

	 Roboto = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 40);
	// Changing the window title
	AESysSetWindowTitle("Lights Out");

	// reset the system modules

	AESysReset();
	// Initialization end
	/////////////////////


	
	// Creating the objects (Shapes) end
	////////////////////////////////////


	// WHERE LOADING TEXTURES SUPPOSED TO BE
	Graphics::Load_Meshes();

	//////////////////////////////////
	// Creating Fonts	

	// Creating Fonts end
	//////////////////////////////////


	//////////////////////////////////
	// Intializing FUNCTION POINTERS - ben

	GSM_Initialize(GS_MAINMENU);

	// Game Loop
	while (current != GS_QUIT)
	{

		if (current != GS_RESTART) {
			GSM_Update();

			fpLoad();
		}
		else {
			next = current = previous;
		}

		fpInitialize();
		//the game loop
		while (current == next) {
			// Informing the system about the loop's start
			AESysFrameStart();

			// Handling Input
			AEInputUpdate();
			g_dt = static_cast<f32>(AEFrameRateControllerGetFrameTime()); //Getting delta time

			if (g_dt >= 0.01667f) // 1 / 60fps;
			{
				g_dt = 0.01667f;
			}
			fpUpdate();
			fpDraw();

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
		fpFree();

		if (next != GS_RESTART) {
			fpUnload();
		}

		previous = current;
		current = next;

	}


	
	// Freeing the objects and textures


	// free the system
	AESysExit();

}