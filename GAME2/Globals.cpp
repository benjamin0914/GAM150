#include "Globals.h"

namespace LEVELS {

	unsigned short Level = 0U;
}
namespace Mesh {
	AEGfxVertexList* Rect = nullptr;
	AEGfxVertexList* Circle = nullptr;
	AEGfxVertexList* Slime = nullptr;
	
}
namespace FILEP {
	namespace TILE {
		const char* Black = "./Assets/Tiles/Black_Tile.png";
		const char* Dirt = "./Assets/Tiles/Dirt_Tile.png";
	}
	namespace PLAYER {
		const char* man = "./Assets/player_cube.png";
	}
	namespace ENEMY {
		const char* slime = "./Assets/slime.png";

	}
}
float player_objtexX = 0.0f;