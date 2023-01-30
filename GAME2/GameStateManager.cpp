#include "pch.h"
#include "GameStateManager.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr, fpLoadMesh = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;
}

void GSM_Update()
{
	switch (current)
	{
		case GS_MAINMENU:
			fpLoad = MainMenu_Load;
			fpInitialize = MainMenu_Init;
			fpUpdate = MainMenu_Update;
			fpDraw = MainMenu_Draw;
			fpFree = MainMenu_Free;
			fpUnload = MainMenu_Unload;
			break;
		case GS_LEVEL1:
			fpLoad = Level1_Load;
			fpInitialize = Level1_Initialize;
			fpUpdate = Level1_Update;
			fpDraw = Level1_Draw;
			fpFree = Level1_Free;
			fpUnload = Level1_Unload;
			break;
		case GS_LEVEL2:
			fpLoad = Level2_Load;
			fpInitialize = Level2_Initialize;
			fpUpdate = Level2_Update;
			fpDraw = Level2_Draw;
			fpFree = Level2_Free;
			fpUnload = Level2_Unload;
			break;
		case GS_LEVEL3:
			fpLoad = Level3_Load;
			fpInitialize = Level3_Initialize;
			fpUpdate = Level3_Update;
			fpDraw = Level3_Draw;
			fpFree = Level3_Free;
			fpUnload = Level3_Unload;
			break;
		case GS_RESTART:
			break;
		case GS_QUIT:
			break;
		default:
			break;
	}
}