#include "Player.h"
#include "Graphics.h"
#include "Globals.h"

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>   
#include <cstring>

static f32 maxY, maxX;
AEGfxTexture* Player::playerTex{ nullptr };
Player::Player(AEGfxTexture* texture, const f32 width, const f32 height) : 
sprite(texture, Mesh::Rect, width, height),
active{ true }, gravity{ false }, jump{ false },startingPos{ 0, 0 }, vel{ 0, 0 }
{
	maxY = static_cast<f32>(9999);
	maxX = static_cast<f32>(9999);
}
Player::Player() :active{ true }, gravity{ false }, jump{ false }, 
 startingPos{ 0, 0 }, vel{ 0, 0 }  
{
	maxY = static_cast<f32>(9999);
	maxX = static_cast<f32>(9999);
}
void Player::Render(void)
{
	AEMtx33 scale = { 0 };
	AEMtx33 rotate = { 0 };
	AEMtx33 translate = { 0 };
	AEMtx33 transform = { 0 };
	/// <Draw Player start>
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Player Position
	//AEGfxSetPosition(5100, 630);
	// Player no tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Player texture
	AEGfxTextureSet(playerTex, 0, 0);
	scale = { 0 };
	AEMtx33Scale(&scale, 60.f, 60.f);
	rotate = { 0 };
	AEMtx33Rot(&rotate, PI * 2.0f);
	translate = { 0 };
	AEMtx33Trans(&translate, sprite.pos.x, sprite.pos.y);
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(Mesh::Rect, AE_GFX_MDM_TRIANGLES);
	//AEGfxSetCamPosition(sprite.pos.x - AEGetWindowWidth() / 2, AEGetWindowHeight() / 2 - sprite.pos.y);

	/// <Draw Player end>

}
void Player::SetPos(AEVec2 &pos) {
	// Player Control
	if (AEInputCheckTriggered(AEVK_D) || AEInputCheckCurr(AEVK_D)) {
		pos.x += 5.0f;
	}
	if (AEInputCheckTriggered(AEVK_A) || AEInputCheckCurr(AEVK_A)) {
		pos.x -= 5.0f;
	}
	if (AEInputCheckTriggered(AEVK_W) || AEInputCheckCurr(AEVK_W)) {
			pos.y += 5.0f;
	}
	if (AEInputCheckTriggered(AEVK_S) || AEInputCheckCurr(AEVK_S) ){
			pos.y -= 5.0f;
	}

	 sprite.pos.x = pos.x;
	 sprite.pos.y = pos.y;
	 AEGfxSetCamPosition(sprite.pos.x, sprite.pos.y);

}
void Player::LoadTex(void) {
	playerTex = AEGfxTextureLoad("Assets/player_cube.png");

}
void Player::Unload(void) {
	if (playerTex) {
		AEGfxTextureUnload(playerTex);
		playerTex = nullptr;
	}
}
void Player::CreatePlayer(Player& player, const AEVec2 pos, const f32 width, const f32 height)
{
	player.sprite.Set(playerTex, width, height, pos, Mesh::Rect);
	player.startingPos = pos;
	player.sprite.pos = player.startingPos;
}
