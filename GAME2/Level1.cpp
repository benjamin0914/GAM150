#include "pch.h"
#include "Level1.h"
#include "GameStateManager.h"

int Level1_Lives = 0;
int Level1_Counter = 0; //Initialize 'Level1_Counter'

void Level1_Load()
{
}

void Level1_Initialize()
{
}

void Level1_Update()
{
	Level1_Counter -= 1;

	if (Level1_Counter == 0)
	{
		Level1_Lives -= 1;

		if (Level1_Lives == 0)
			next = GS_LEVEL2;
		else if (Level1_Lives > 0)
			next = GS_RESTART;
	}
}

void Level1_Draw()
{
}

void Level1_Free()
{
}

void Level1_Unload()
{
}