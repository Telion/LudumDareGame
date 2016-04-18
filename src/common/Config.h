#pragma once

#define TRACE
//#define TRACE printf("Trace: %s (%s:%d)\n", __func__, __FILE__, __LINE__);

// The server runs the game at this delay.
// Clients should double this.
const int interpolationDelay = 200000;

const int screenWidth = 960;
const int screenHeight = 720;