#include "AEEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include <string>
#include "function.hpp"

#include <iostream>

char strBuffer[100];
s8 fontId = 0;
std::string buffer = "LIGHT'S OUT";
std::string enter = "Press Enter";
float textmove = 1.0f;


	// Declare all assets here
static AEGfxVertexList* pMeshMenu;
static AEGfxTexture* button_start;

	// Declare all variables here
s32 cursor_x, cursor_y;



void MainMenu_Load() {
	// Informing the library that we're about to start adding vertices
	AEGfxMeshStart();

	// This shape is a 1 by 1 square box, rmb to use scaling
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00FF0FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		-0.5, 0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0x00FFFFFF, 0.0f, 0.0f);




	pMeshMenu = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshMenu, "Failed to create menu box mesh!!");

	button_start = AEGfxTextureLoad("Assets/start_button.png");

}

void MainMenu_Init() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);


	
}

void MainMenu_Update() {
	
	// Comment this block out to see button texture
	if (AEInputCheckTriggered(AEVK_RETURN)) next = GS_LEVEL1;
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	if (textmove > -0.23f){
		textmove -= g_dt;
		
}
	AEGfxPrint(Roboto, const_cast<s8*>(buffer.c_str()), textmove, 0.5f, 1, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Roboto, const_cast<s8*>(enter.c_str()), -0.2f, 0.0f, 1, 1.0f, 1.0f, 1.0f);


	/*
	AEInputGetCursorPosition(&cursor_x, &cursor_y);
	cursor_x - 395.f, -cursor_y + 290.f;
	if (MouseClick(0.f, -180.f, 180.f, 60.f, cursor_x - 395.f, -cursor_y + 290.f)) std::cout << "bruh";*/
}

void MainMenu_Draw() {
	AEMtx33 scale;
	AEMtx33 translate;
	AEMtx33 transform;

	
	
	// Prepare to set texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set position for platform 3
	AEGfxSetPosition(0.f, 0.f); 
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set button texture
	AEGfxTextureSet(button_start, 0.f, 0.f);
	// Scale and translate button position
	AEMtx33Scale(&scale, 180.f, 60.f);
	AEMtx33Trans(&translate, 0.f, -180.f);
	transform = { 0 };
	AEMtx33Concat(&transform, &translate, &scale);
	AEGfxSetTransform(transform.m);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMeshMenu, AE_GFX_MDM_TRIANGLES);


	//left = pos_x - width * 2.f;
	//right = pos_x + width * 2.f;
	//bottom = pos_y - height * 2.f;
	//top = pos_y + height * 2.f;
}

void MainMenu_Free() {

}

void MainMenu_Unload() {
	AEGfxMeshFree(pMeshMenu);

	AEGfxTextureUnload(button_start);
}