#pragma once

#include <string>
#include <iostream>
#include "Texture.hpp"

struct TextureAtlas
{

	static float* getUVCoords(int xT, int yT, int xS, int yS, int xB, int yB);

};