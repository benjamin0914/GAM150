#pragma once
#include "AEEngine.h"
#include <vector>
#include "Image.h"


enum class TileType {	Black = 0, Dirt, Max };
TileType& operator++(TileType& rhs);
AEVec2 AEVec2Set(const f32 x, const f32 y);
class Tiles
{
public:
	// Reference to a vector, containing a vector of Tiles*
	using TileMgr = std::vector<std::vector<Tiles>*>&;

private:
	TileType type;
public:
	Tiles(AEGfxTexture*, const f32 width, const f32 height);
	Image image;
	AEVec2 spawnPos;
	short ID;
	void Render(void);
	static void AddTile(std::vector<Tiles>& tile, TileType type, const f32 width, const f32 height, AEVec2 pos);
	static void Unload(void);
	static void LoadTex(void);
};