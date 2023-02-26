
#include "Graphics.h"
#include "Globals.h"
#include <iostream>




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


