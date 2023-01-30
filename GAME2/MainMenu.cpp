#include "AEEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"


char strBuffer[100];
s8 fontId = 0;

void MainMenu_Load() {

}

void MainMenu_Init() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
}

void MainMenu_Update() {
	if (AEInputCheckTriggered(AEVK_RETURN)) next = GS_LEVEL1;
}

void MainMenu_Draw() {

}

void MainMenu_Free() {

}

void MainMenu_Unload() {

}