#pragma once
#include "AEEngine.h"
enum class TYPE_OBJECT
{
	EMPTY,						//0
	GRASS,				//1
	GREY,			//2
	MAX							//3
};

enum class EntitySizes {

	PLAYER,
	MAX
};


EntitySizes& operator++(EntitySizes& rhs);

extern int** MapData;
extern int Map_Width;
extern int Map_Height;

int		ImportMapDataFromFile(const char* FileName);
void	FreeMapData(void);