
#include "Graphics.h"
#include "Image.h"

#include "Globals.h"

#include <iostream>

const f32 Color::RGBA_MAX{ 255.0f };

Color::Color(float r, float g, float b, float a)
{
	this->r = r / RGBA_MAX;
	this->g = g / RGBA_MAX;
	this->b = b / RGBA_MAX;
	this->alpha = a / RGBA_MAX;
}

Color::Color() : r{ 255.0f }, g{ 255.0f }, b{ 255.0f }, alpha{ 255.0f } {}

void Color::Set(Color color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->alpha = color.alpha;
}



bool Color::operator==(const Color& rhs)
{
	static const float Epsilon{ 0.001f };
	if (r - rhs.r < Epsilon && g - rhs.g < Epsilon && b - rhs.b < Epsilon && alpha - rhs.alpha < Epsilon)
		return true;

	return false;
}



void Color::Print(const Color& color)
{	// For debug
	std::cout << "r: " << color.r << " g: " << color.g << " b: " << color.b << " a: " << color.alpha << std::endl;
}


void Graphics::Load_Meshes(void)
{
	Mesh::Rect = Graphics::Mesh_Rectangle();
	AE_ASSERT_MESG(Mesh::Rect, "fail to create object!!");

	Mesh::Circle = Graphics::Mesh_Circle();
	AE_ASSERT_MESG(Mesh::Circle, "fail to create object!!");

	Mesh::Slime = Graphics::Mesh_Slime();
	AE_ASSERT_MESG(Mesh::Circle, "fail to create object!!");

}



void Graphics::Free() {



	AEGfxMeshFree(Mesh::Rect);
	AEGfxMeshFree(Mesh::Circle);
	AEGfxMeshFree(Mesh::Slime);

}

AEGfxVertexList* Graphics::Mesh_Rectangle(void)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,// Bottom Left
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, // Bottom Right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); // Top vertex

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, //	Bottom Right
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,	 //	Top Right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);//	Top Left
	return AEGfxMeshEnd();
}
AEGfxVertexList* Graphics::Mesh_Slime(void) {
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00FF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00FFFF00, 0.25f, 1.0f,
		-0.5f, 0.5f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00FFFFFF, 0.25f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 0.25f, 0.0f,
		-0.5f, 0.5f, 0x00FFFFFF, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* Graphics::Mesh_Circle(void)
{
	AEGfxMeshStart();
	//Creating the circle shape
	int Parts = 30;
	for (float i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
			0.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
			cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
	}
	return AEGfxMeshEnd();
}



void Graphics::Rect::SetMatrix(void)
{
	static f32 HalfWinHeight{ AEGetWindowHeight() / 2.0f };
	static f32 HalfWinWindow{ AEGetWindowWidth() / 2.0f };
	AEMtx33	trans, rot, scale;
	AEMtx33Scale(&scale, width, height);
	AEMtx33Rot(&rot, AEDegToRad(rotation));
	AEMtx33Trans(&trans, -HalfWinWindow + pos.x, HalfWinHeight - pos.y);
	AEMtx33 temp;
	AEMtx33Concat(&temp, &rot, &scale);
	AEMtx33Concat(&transformMtx, &trans, &temp);
}

void Graphics::Rect::SetMatrix(AEVec2 Pos)
{
	static f32 HalfWinHeight, HalfWinWindow;
	HalfWinHeight = AEGetWindowHeight() / 2.0f;
	HalfWinWindow = AEGetWindowWidth() / 2.0f;
	AEMtx33	trans, rot, scale;
	AEMtx33Scale(&scale, width, height);
	AEMtx33Rot(&rot, AEDegToRad(rotation));
	AEMtx33Trans(&trans, -HalfWinWindow + Pos.x, HalfWinHeight - Pos.y);
	AEMtx33 temp;
	AEMtx33Concat(&temp, &rot, &scale);
	AEMtx33Concat(&transformMtx, &trans, &temp);
}
Graphics::Rect::Rect(const f32 width, const f32 height, const f32 direction, AEGfxVertexList* Mesh) : rotation{ direction }, transformMtx{ NULL },
width{ width }, height{ height }, pos{ 0, 0 }, pMesh{ Mesh }
{
	color.Set(Color{ 255, 255, 255, 255 });
}
void Graphics::Rect::Draw(const f32 alpha)
{
	SetMatrix();
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTintColor(color.r, color.g, color.b, color.alpha);
	AEGfxSetTransparency(alpha / Color::RGBA_MAX);
	AEGfxSetTransform(transformMtx.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Graphics::Rect::Draw(Color C, const f32 alpha)
{
	SetMatrix();
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTintColor(C.r, C.g, C.b, C.alpha);
	AEGfxSetTransparency(alpha / Color::RGBA_MAX);
	AEGfxSetTransform(transformMtx.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Graphics::Rect::DrawTexture(AEGfxTexture* pTex, Color C, const f32 alpha)
{
	SetMatrix();
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(pTex, 0.0f, 0.0f);
	AEGfxSetTintColor(C.r, C.g, C.b, C.alpha);
	AEGfxSetTransparency(alpha / Color::RGBA_MAX);
	AEGfxSetTransform(transformMtx.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Graphics::Rect::SetMesh(AEGfxVertexList* Mesh)
{
	pMesh = Mesh;
}

Graphics::Circle::Circle(const f32 radius, const f32 direction, AEGfxVertexList* Mesh) : Rect(radius, radius, direction, Mesh)
{
	color.Set(Color{ 255, 255, 255, 255 });
}

Graphics::Circle::Circle() : Rect(0.0f, 0.0f, 0.0f, Mesh::Circle) {}


void Graphics::Text::SetText(std::string text) {
	buffer = text;
}



AEVec2 Graphics::Text::GetBufferSize()
{
	AEGfxGetPrintSize(ID, const_cast<s8*>(buffer.c_str()), scale, width, height);

	return AEVec2{ width / 2 * AEGetWindowWidth(), height / 2 * AEGetWindowHeight() };
}
