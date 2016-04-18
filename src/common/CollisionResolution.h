#pragma once

#include "VectorMath.h"

class World;

// Start and end need to have the same size
Vector2 resolveCollisions(Rect start, Rect end, const World& world);

bool collides(Rect start, Rect end, const World& world);