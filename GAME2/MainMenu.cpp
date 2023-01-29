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
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "Press Enter to Start Demo");

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(fontId, strBuffer, -0.95, -0.95, 1.0f, 1.f, 1.f, 1.f);

	f32 TextWidth, TextHeight;
	AEGfxGetPrintSize(fontId, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(fontId, strBuffer, 0.99 - TextWidth, 0.99 - TextHeight, 1.0f, 1.f, 1.f, 1.f);
}

void MainMenu_Free() {

}

void MainMenu_Unload() {

}