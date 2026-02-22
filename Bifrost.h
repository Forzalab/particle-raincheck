#ifndef B_H
#define BRIDGES_H

#include <cstdint>
#include "Particle.h"
#include "World.h"
#include "/libs/include/Bridges.h"

using ColorValue = uint8_t;
using Cv = ColorValue;

typedef struct ParticleColor {
	Cv r{}, g{}, b{};
};

typedef struct ParticleCoordinate {
        Pc row{}, col{};
};

using pRGB = ParticleColor;
using pXY = ParticleCoords;

class Bridges {
private:
	Bridges bridges;
	void setDataStructure();
public:
	std::string visualize(World &world);
};
 
#endif
