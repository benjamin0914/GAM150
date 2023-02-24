
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


AEVec2 EntitySizeArray[static_cast<int>(EntitySizes::MAX)];




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
	return 1;
}


void FreeMapData(void)
{
	// Returns the memory to the freestore.
	for (int i = 0; i < Map_Height; ++i) {
		delete[] MapData[i];
	}
	delete[] MapData;
}

