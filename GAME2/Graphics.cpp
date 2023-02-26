
#include "Graphics.h"
#include "Globals.h"
#include <iostream>




void Graphics::Load_Meshes(void)
{
	Mesh::Rect = Graphics::Mesh_Rectangle();
	AE_ASSERT_MESG(Mesh::Rect, "fail to create object!!");

}



void Graphics::Free() {

	AEGfxMeshFree(Mesh::Rect);

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




