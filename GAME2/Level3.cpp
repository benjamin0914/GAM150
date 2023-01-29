#include "pch.h"

int Level3_Lives = 0; //Initialize 'Level3_Lives'
int Level3_Counter = 0; //Initialize 'Level3_Counter'

void Level3_Load()
{
}

void Level3_Initialize()
{
}

void Level3_Update()
{
	Level3_Counter -= 1;

	if (Level3_Counter == 0)
	{
		Level3_Lives -= 1;

		if (Level3_Lives == 0)
			next = GS_QUIT;
		else if (Level3_Lives > 0)
			next = GS_RESTART;
	}
}

void Level3_Draw()
{
}

void Level3_Free()
{
}

void Level3_Unload()
{
}