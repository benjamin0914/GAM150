#include "pch.h"
#include "Level2.h"
#include "GameStateManager.h"

int Level2_Lives = 0; //Initialize 'Level2_Lives'
int Level2_Counter = 0; //Initialize 'Level2_Counter'

void Level2_Load()
{
}

void Level2_Initialize()
{
}

void Level2_Update()
{
	Level2_Counter -= 1;

	if (Level2_Counter == 0)
	{
		Level2_Lives -= 1;

		if (Level2_Lives == 0)
			next = GS_LEVEL3;
		else if (Level2_Lives > 0)
			next = GS_RESTART;
	}
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