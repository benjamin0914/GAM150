#include "Tiles.h"
#include "Image.h"
#include "GameStateManager.h"
#include "Globals.h"


#include <iostream>
#include <array>

static AEGfxTexture* tileTex[static_cast<int>(TileType::Max)]{ nullptr };

//Overloaded alpha eng func move to util.cpp in future - shaq
AEVec2 AEVec2Set(const f32 x, const f32 y)
{
	return AEVec2{ x, y };
}
//overload ++ operator to use with Tiletype type - shaq
TileType& operator++(TileType& rhs) {
	rhs = static_cast<TileType>((static_cast<int>(rhs) + 1));
	return rhs;
}
Tiles::Tiles(AEGfxTexture* filepath, const f32 width, const f32 height) : image(filepath, Mesh::Rect, width, height), ID{ 0 }, type{ TileType::Grey }, spawnPos{ 0, 0 }
{
	
}
void Tiles::Render() {
	
		image.Draw_Texture(255.0f);
	
	
}
void Tiles::AddTile(std::vector<Tiles>& tile, TileType type, const f32 width, const f32 height, AEVec2 pos) {
	AEGfxTexture* temp = tileTex[static_cast<int>(type)];
	float Height = height;

	tile.push_back(Tiles(temp, width, Height));
	Tiles& Tile = tile.back();
	Tile.type = type;



	Tile.spawnPos = Tile.image.pos;
	Tile.image.pos = AEVec2Set(pos.x + Tile.image.width / 2.0f, pos.y + height / 2.0f - Height / 2.0f);
}

void Tiles::LoadTex() {
	for (TileType i = static_cast<TileType>(0); i < TileType::Max; ++i) {
		const char* pTex{ nullptr };
		switch (i) {
		case TileType::Grass:
			pTex = FILEP::TILE::Grass;
			break;
		case TileType::Grey:
			pTex = FILEP::TILE::Grey;
			break;
		default:
			throw "invalid tile type!";
			return;
		}
		tileTex[static_cast<int>(i)] = AEGfxTextureLoad(pTex);
		AE_ASSERT_MESG(pTex, "Failed to create Tile texture!");
	}
	std::cout << "\nLoaded tiles!";

}
void Tiles::Unload()
{
	for (size_t i = 0; i < static_cast<int>(TileType::Max); i++) {
		AEGfxTextureUnload(tileTex[i]);
	}

}
