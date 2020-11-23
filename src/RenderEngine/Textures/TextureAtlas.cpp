#include "TextureAtlas.hpp"

float* TextureAtlas::getUVCoords(int xT, int yT, int xS, int yS, int xB, int yB)
{
	double a = .0625;
	double uvs[] =
	{
		0 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), .0625 + (a * yS),
		0 + (a * xS), .0625 + (a * yS),

		0 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), .0625 + (a * yS),
		0 + (a * xS), .0625 + (a * yS),

		0 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), .0625 + (a * yS),
		0 + (a * xS), .0625 + (a * yS),

		0 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), 0 + (a * yS),
		.0625 + (a * xS), .0625 + (a * yS),
		0 + (a * xS), .0625 + (a * yS),

		0 + (a * xT), 0 + (a * yT),
		.0625 + (a * xT), 0 + (a * yT),
		.0625 + (a * xT), .0625 + (a * yT),
		0 + (a * xT), .0625 + (a * yT),

		0 + (a * xB), 0 + (a * yB),
		.0625 + (a * xB), 0 + (a * yB),
		.0625 + (a * xB), .0625 + (a * yB),
		0 + (a * xB), .0625 + (a * yB)
	};

	float* uvsF = new float[sizeof(uvs) / sizeof(uvs[0])]; // DELETE *

	for (int i = 0; i < sizeof(uvs) / sizeof(uvs[0]); i++)
	{
		uvsF[i] = static_cast<float>(uvs[i]);
	}

	return uvsF;
}