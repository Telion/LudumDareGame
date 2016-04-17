#include "Tile.h"

bool Tile::isSolid() const
{
	switch (id)
	{
	case -1:
	case 6:
	case 7:
		return true;
	default:
		return false;
	}
}