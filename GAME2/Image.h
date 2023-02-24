#pragma once
#include "AEEngine.h"
#include "Graphics.h"

class Image
{
private:
	AEGfxVertexList* pMesh;
	AEGfxTexture* pTex;
	AEMtx33 transformMtx;
	void SetMatrix();
	void SetMatrix(AEVec2 pos);

public:
	Image(const AEGfxTexture* pTex, AEGfxVertexList* Mesh, const f32 width, const f32 height, const f32 dir = 0); //ctor
	Image();
	void Set(const AEGfxTexture* texture, const f32 width, const f32 height, const AEVec2 pos, AEGfxVertexList* mesh, const f32 rotation = 0);
	void Load(const char* pFile, const f32 width, const f32 height, const AEVec2 pos, const f32 rotation = 0, AEGfxVertexList* = Mesh::Rect);
	inline AEGfxTexture* GetTexturePtr(void) { return pTex; }
	f32 height, width, rotation;
	AEVec2 pos;
	Color color;

	inline void ReflectAboutYAxis() { width *= -1.0f; }
	void Free(void);

	void Set_Texture(AEGfxTexture* texture);

	void Draw_Texture(int counter, float offset, AEGfxVertexList* mesh, const f32 alpha, const f32 r = 255.0f, const f32 g = 255.0f, const f32 b = 255.0f, const f32 alpha2 = 255.0f);
	// Draw to class member pos.
	void Draw_Texture(const f32 alpha, const f32 r = 255.0f, const f32 g = 255.0f, const f32 b = 255.0f, const f32 alpha2 = 255.0f);
	// Draw to param position.
	void Draw_Texture(AEVec2 pos, const f32 alpha, const f32 r = 255.0f, const f32 g = 255.0f, const f32 b = 255.0f, const f32 alpha2 = 255.0f);
};