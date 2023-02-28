
#include "BinaryMap.h"


#include "Graphics.h"



#include "Tiles.h"
#include "Enemy.h"

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <array>
#include <cassert>
#include <stdlib.h>
/*The number of horizontal elements*/
 int Map_Height;

/*The number of vertical elements*/
 int Map_Width;

/*This will contain all the data of the map, which will be retreived from a file
when the "ImportMapDataFromFile" function is called*/
 int** MapData;
 int** BinaryCollisionArray;

AEVec2 EntitySizeArray[static_cast<int>(EntitySizes::MAX)];
const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
const unsigned int	COLLISION_TOP = 0x00000004;	//0100
const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000



EntitySizes& operator++(EntitySizes& rhs)
{
	rhs = static_cast<EntitySizes>((int)rhs + 1);
	return rhs;
}

//IMPORTANT IF MAKING CHANGES TO TEXT FILE, REBUILD SOLUTION TO UPDATE
int ImportMapDataFromFile(const char* FileName)
{
	// Initialize var to 0
	Map_Height = 0;
	Map_Width = 0;
	std::ifstream ifs;
	ifs.open(FileName);
	if (ifs.fail()) // Unable to open file
	{
		ifs.close();
		printf("Unable to open file!\n");
		return 0;
	}

	for (int i = 0; i < 2; ++i) {
		std::string Map;
		std::getline(ifs, Map);
		Map.erase(0, Map.find_first_of("1234567890"));
		if (i == 0) // loop count 0 will read Map_Width.
			Map_Width = std::stoi(Map);
		else if (i == 1)     //loop 1 will read Map_Height
			Map_Height = std::stoi(Map);
	}
	for (EntitySizes i = static_cast<EntitySizes>(0); i < EntitySizes::MAX; ++i) {
		ifs >> EntitySizeArray[static_cast<int>(i)].x >> EntitySizeArray[static_cast<int>(i)].y;
	}


	MapData = new int* [Map_Height];

	for (int i = 0; i < Map_Height; i++) {
		MapData[i] = new int[Map_Width];
	}
	for (int i = 0; i < Map_Height; ++i)
	{
		for (int j = 0; j < Map_Width; ++j)
		{

			ifs >> MapData[i][j];
		}
	}
	ifs.close();
	BinaryCollisionArray = new int* [Map_Height];

	for (int i = 0; i < Map_Height; i++) {
		BinaryCollisionArray[i] = new int[Map_Width];
	}
	for (int i = 0; i < Map_Height; ++i)
	{
	

		for (int j = 0; j < Map_Width; ++j)
		{
			
			if (MapData[i][j] == 2) {
				BinaryCollisionArray[i][j] = 1;
			}
			else {
				BinaryCollisionArray[i][j] = 0;
			}
		}
	}




	return 1;
}

void  SnapToCell(float* Coordinate,float adjust)
{
	UNREFERENCED_PARAMETER(Coordinate);
	*Coordinate = static_cast<int> (*Coordinate) + adjust;

}
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.

	UNREFERENCED_PARAMETER(PosX);
	UNREFERENCED_PARAMETER(PosY);
	UNREFERENCED_PARAMETER(scaleX);
	UNREFERENCED_PARAMETER(scaleY);
	int flag = 0;

	// hotspot 1
	float x1 =  (PosX - scaleX / 2)/100;	//To reach the left side
	float y1 = (PosY + scaleY / 4)/70;	//To go up 1 / 4 of the height
	float x2 =  (PosX - scaleX / 2)/100;	//To reach the left side
	float y2 =  (PosY - scaleY / 4)/70;	//To go down 1 / 4 of the height
	x1 = fabs(x1);
	x2 = fabs(x2);
	y1 = fabs(y1);
	y2 = fabs(y2);
	if (BinaryCollisionArray[static_cast<int> (y1)][static_cast<int> (x1)] || BinaryCollisionArray[static_cast<int> (y2)][static_cast<int> (x2)]) {
		flag += COLLISION_LEFT;
	}


	x1 = (PosX + scaleX / 2)/100;	//To reach the right side
	y1 = (PosY + scaleY / 4)/70;	//To go up 1 / 4 of the height
	x2 = (PosX + scaleX / 2)/100;	//To reach the right side
	y2 = (PosY - scaleY / 4)/70;	//To go down 1 / 4 of the height


	if (BinaryCollisionArray[static_cast<int> (y1)][static_cast<int> (x1)] || BinaryCollisionArray[static_cast<int> (y2)][static_cast<int> (x2)]) {
		flag += COLLISION_RIGHT;
	}

	x1 = (PosX - scaleX / 4)/100;
	y1 = (PosY + 100 / 2)/70;
	x2 = (PosX + scaleX / 4)/100;
	y2 = (PosY + 100 / 2)/70;
	x1 = fabs(x1);
	x2 = fabs(x2);
	y1 = fabs(y1);
	y2 = fabs(y2);
	if (BinaryCollisionArray[static_cast<int> (y1)][static_cast<int> (x1)] || BinaryCollisionArray[static_cast<int> (y2)][static_cast<int> (x2)]) {
		flag += COLLISION_TOP;
	}

	x1 = (PosX - scaleX / 4)/100;
	y1 = (PosY - 15/ 2)/70;
	x2 = (PosX + scaleX / 4)/100;
	y2 = (PosY - 15 / 2)/70;
	x1 = fabs(x1);
	x2 = fabs(x2);
	y1 = fabs(y1);
	y2 = fabs(y2);
	if (BinaryCollisionArray[static_cast<int> (y1)][static_cast<int> (x1)] || BinaryCollisionArray[static_cast<int> (y2)][static_cast<int> (x2)]) {
		flag += COLLISION_BOTTOM;
	}

	//std::cout << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)];
	//std::cout << static_cast<int>(x1);
	return flag;
	return 0;
}

void FreeMapData(void)
{
	// Returns the memory to the freestore.
	for (int i = 0; i < Map_Height; ++i) {
		delete[] MapData[i];
	}
	delete[] MapData;
}

