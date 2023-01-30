#include "pch.h"

//int Level2_Lives = 0; //Initialize 'Level2_Lives'
//int Level2_Counter = 0; //Initialize 'Level2_Counter'
AEGfxTexture* DarknessTex = AEGfxTextureLoad("Timer/Darkness.png");
AEGfxVertexList* pMeshdarkness = 0;

void Level2_Load()
{
}

void Level2_Initialize()
{
}

void Level2_Update()
{
	AEGfxMeshStart();
	AEGfxVertexList* pMeshdarkness = 0;

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMeshdarkness = AEGfxMeshEnd();

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(DarknessTex, 0, 0);
	static f32 elapsed = 0;
	elapsed += AEFrameRateControllerGetFrameTime();
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 2750.f - 100.0f * elapsed, 1500.f - 50.0f * elapsed);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, PI);
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0, 0);
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMeshdarkness, AE_GFX_MDM_TRIANGLES);

	/*Level2_Counter -= 1;

	if (Level2_Counter == 0)
	{
		Level2_Lives -= 1;

		if (Level2_Lives == 0)
			next = GS_LEVEL3;
		else if (Level2_Lives > 0)
			next = GS_RESTART;
	}*/
}

void Level2_Draw()
{
	AEGfxTexture* TimerTex = AEGfxTextureLoad("Timer/Darkness.png");
}

void Level2_Free()
{
	AEGfxMeshFree(pMeshdarkness);
}

void Level2_Unload()
{
}