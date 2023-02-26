#include "Tiles.h"
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
Tiles::Tiles(AEGfxTexture* filepath, const f32 width, const f32 height) : 
	ID{ 0 }, 
	type{ TileType::Black }, 
	spawnPos{ 0, 0 }
{
	
}
void Tiles::Render() {
	AEMtx33 transformMtx;
	static f32 HalfWinHeight, HalfWinWindow;
	HalfWinHeight = AEGetWindowHeight() / 2.0f;
	HalfWinWindow = AEGetWindowWidth() / 2.0f;
	AEMtx33	trans, rot, scale;
	AEMtx33Scale(&scale, 100,70);
	AEMtx33Rot(&rot, AEDegToRad(0));
	AEMtx33Trans(&trans, -HalfWinWindow + Tiles::spawnPos.x, HalfWinHeight - Tiles::spawnPos.y);
	AEMtx33 temp;
	AEMtx33Concat(&temp, &rot, &scale);
	AEMtx33Concat(&transformMtx, &trans, &temp);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	if (Tiles::type == TileType::Black) {
		AEGfxTextureSet(tileTex[0], 0, 0);
	}
	else if (Tiles::type == TileType::Dirt) {
		AEGfxTextureSet(tileTex[1], 0, 0);
	}
	AEGfxSetTransform(transformMtx.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(Mesh::Rect, AE_GFX_MDM_TRIANGLES);
	
}
void Tiles::AddTile(std::vector<Tiles>& tile, TileType type, const f32 width, const f32 height, AEVec2 pos) {
	AEGfxTexture* temp = tileTex[static_cast<int>(type)];
	float Height = height;
	tile.push_back(Tiles(temp, width, Height));
	Tiles& Tile = tile.back();
	Tile.type = type;
	Tile.spawnPos = AEVec2Set(pos.x + width / 2.0f, pos.y + height / 2.0f - Height / 2.0f);
}

void Tiles::LoadTex() {
	for (TileType i = static_cast<TileType>(0); i < TileType::Max; ++i) {
		const char* pTex{ nullptr };
		switch (i) {
		case TileType::Black:
			pTex = "./Assets/Tiles/Black_Tile.png";
			break;
		case TileType::Dirt:
			pTex = "./Assets/Tiles/Dirt_Tile.png";
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
