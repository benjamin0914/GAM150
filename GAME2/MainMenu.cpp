#include "AEEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include <string>


char strBuffer[100];
s8 fontId = 0;
std::string buffer = "LIGHT'S OUT";
std::string enter = "Press Enter";
float textmove = 1.0f;
void MainMenu_Load() {

}

void MainMenu_Init() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);


	
}

void MainMenu_Update() {

	if (AEInputCheckTriggered(AEVK_RETURN)) next = GS_LEVEL1;
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	if (textmove > -0.23f){
		textmove -= g_dt;
		
}
	AEGfxPrint(Roboto, const_cast<s8*>(buffer.c_str()), textmove, 0.5f, 1, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Roboto, const_cast<s8*>(enter.c_str()), -0.2f, 0.0f, 1, 1.0f, 1.0f, 1.0f);

}

void MainMenu_Draw() {

}

void MainMenu_Free() {

}

void MainMenu_Unload() {

}