#pragma once
#include "AEEngine.h"
namespace LEVELS {

	extern unsigned short Level;

}
namespace Mesh { // Base Meshes
	extern AEGfxVertexList* Rect;
	extern AEGfxVertexList* Circle;
	extern AEGfxVertexList* Slime;


}

namespace FILEP {
	
	namespace TILE {
		extern const char* Black;
		extern const char* Dirt;
	}
	namespace PLAYER {
		extern const char* man;
	}
	namespace ENEMY {
		extern const char* slime;
	}

	

	
}
extern float player_objtexX;