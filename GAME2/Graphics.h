#pragma once
#include "AEEngine.h"
#include "Globals.h"
#include <string>



namespace Graphics
{
	// Sets the mesh for a rectangle and returns a pointer to the AEGfxVertexList.
	// Only call once at the start of application!!
	AEGfxVertexList* Mesh_Circle(void);
	AEGfxVertexList* Mesh_Rectangle(void);
	AEGfxVertexList* Mesh_Slime(void);
	void Load_Meshes(void);
	void Free();

	
}
