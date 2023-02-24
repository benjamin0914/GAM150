#pragma once
#include "AEEngine.h"
#include "Image.h"
class Player
{
private:
	



public:
	static AEGfxTexture* playerTex;
	static AEGfxTexture* playerMovTex;
	static AEGfxTexture* playerParticle;
	Player(AEGfxTexture*, const f32 width, const f32 height);
	Player();
	Image sprite;
	AEVec2 startingPos, vel;
	bool active, jump, gravity;
	


	void Render(void);
	


	void SetPos(AEVec2 &pos);


	static void LoadTex(void);
	static void Unload(void);
	static void CreatePlayer(Player& player, const AEVec2 pos, const f32 width, const f32 height);


};