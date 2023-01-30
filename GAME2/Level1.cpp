#include "pch.h"

AEGfxVertexList* pMesh = 0;
AEGfxTexture* TimerTex = AEGfxTextureLoad("Timer/Darkness.png");

void Level1_Load()
{
}

void Level1_Initialize()
{
	current = GS_LEVEL1;
}

void Level1_Update()
{
	AEGfxMeshStart();
	AEGfxVertexList* pMesh = 0;

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMesh = AEGfxMeshEnd();

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(TimerTex, 0, 0);
	static f32 elapsed = 0;
	elapsed += AEFrameRateControllerGetFrameTime();
	AEMtx33 scale = { 0 };
	if ((550.f / 50.0f * elapsed) > 1) {
		std::cout << 550.f / 50.0f * elapsed << std::endl;
		AEMtx33Scale(&scale, 550.f / 50.0f * elapsed, 50.f);
	}
	else { AEMtx33Scale(&scale, 0, 0); }
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, PI);
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0, 0);
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Level1_Draw()
{
	AEGfxTexture* TimerTex = AEGfxTextureLoad("Timer/Darkness.png");
}

void Level1_Free()
{
	AEGfxMeshFree(pMesh);
}

void Level1_Unload()
{
}