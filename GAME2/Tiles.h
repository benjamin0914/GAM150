#pragma once
#include "AEEngine.h"
#include <vector>



enum class TileType {	Black = 0, Dirt, Max };
TileType& operator++(TileType& rhs);
AEVec2 AEVec2Set(const f32 x, const f32 y);
class Tiles
{
public:

	Tiles();
	AEVec2 spawnPos;
	TileType type;
	void Render(int width, int height);
	static void AddTile(std::vector<Tiles>& tile, TileType type, const f32 width, const f32 height, AEVec2 pos);
	static void Unload(void);
	static void LoadTex(void);
};