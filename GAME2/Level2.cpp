#include "pch.h"




void Level2_Load()
{
	
}

void Level2_Initialize()
{
	
}

void Level2_Update()
{
	if (AEInputCheckTriggered(AEVK_RETURN))
		next = GS_LEVEL3;

	if (AEInputCheckTriggered(AEVK_ESCAPE)) next = GS_QUIT;

	

	
}

void Level2_Draw()
{
	
	
}

void Level2_Free()
{
	
}

void Level2_Unload()
{
	
}

