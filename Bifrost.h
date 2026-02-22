#ifndef BIFROST_H
#define BIFROST_H

#include <cstdint>
#include "Particle.h"
#include "World.h"
#include <Bridges.h>
#include <Color.h>
#include <ColorGrid.h>

//==================================
// Typedef galore
//===================================

// typedef from BRIDGES lib
using Bridge = bridges::Bridges;
using BColor = bridges::datastructure::Color;
using BColorGrid = bridges::datastructure::ColorGrid;

// typedef carried over from Particle.h
// for clarity purpose
using ColorValue = Color;
using CoordsValue = Pc;

// typedef for color and particles from World
typedef struct ParticleColor {
	ColorValue red{}, green{}, blue{};
};

typedef struct ParticleCoords {
        CoordsValue row{}, col{};
};

using pRGB = ParticleColor;
using pXY = ParticleCoords;


//==============================
// The class itself
//==============================

class Bifrost {
private:
	// Bridge-related member vars
	const std::string api_key;
	const std::string username;
	Bridge bridge; //instantiate in .cc Todo

	// Internal constructor. Singular.
	Bifrost(const std::string &api_key, const std::string &username);

	//==============================
	// Auxilirary functions
	//==============================
public:
	// Out-of-the-box BRIDGES visualizer
	std::string visualize(World &world_snapshot);
};
 
#endif
