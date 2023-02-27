#pragma once
#include "AEEngine.h"
enum class TYPE_OBJECT
{
	EMPTY,			//0
	BLACK,			//1
	DIRT,			//2
	PLAYER,			//3
	SLIME,			//4
	MAX				//5
};

enum class EntitySizes {

	PLAYER,
	MAX
};


EntitySizes& operator++(EntitySizes& rhs);

extern int** MapData;
extern int** BinaryCollisionArray;
extern int Map_Width;
extern int Map_Height;

int		ImportMapDataFromFile(const char* FileName);
void	FreeMapData(void);
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY);
void SnapToCell(float* Coordinate,float adjust);