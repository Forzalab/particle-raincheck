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

// 1) Aliased typedef

// typedef from BRIDGES lib
using Bridge = bridges::Bridges;
using BColor = bridges::datastructure::Color;
using BColorGrid = bridges::datastructure::ColorGrid;

// typedef carried over from Particle.h World.h
// for clarity purpose
using ParticleColorValue = Color;
using ParticleCoordsValue = Pc;
using WorldCoordsValue = Wc;

// typedef for color and particles and size from World
using ParticleColor = struct {
	ParticleColorValue red{}, green{}, blue{};
};

using ParticleCoords = struct {
	ParticleCoordsValue row{}, col{};
};

using ParticleState = struct {
	ParticleCoords xy{};
	ParticleColor rgb{};

	// x y to .xy
	void setXY(const ParticleCoordsValue &x, const ParticleCoordsValue &y);
	// r g b to .rgb
	void setRGB(const ParticleColorValue &r, const ParticleColorValue &g,
				const ParticleColorValue &b);
};

using WorldCoords = struct {
	WorldCoordsValue row{}, col{};
};

using pRGB = ParticleColor;
using pXY = ParticleCoords;
using pState = ParticleState;
using wXY = WorldCoords;

// 2) Internal typedef

// input param typedef
using WorldSnapshot = World;
using Param = const std::string;

// output typedef
using VisualizerURL = std::string;

//==============================
// The class itself
//==============================

class Bifrost {
private:
	// Bridge-related member vars
	Param api_key, username;
	Bridge bridge; // instantiate in .cc Todo

	// Internal constructor
	Bifrost(Param &api_key, Param &username);

	// Getters OUTSIDE Bifrost
	static wXY getWorldBound(const WorldSnapshot &ws);

public:
	// Convention for op func:
	// - operator<< means insert
	// - operator<<= means "assign partially/narrowing",
	//   involving type/data loss (it looks like a funnel)
	// - operator<= means total conversion (it looks like
	//  an arrow)

	// Chainable ops would return NOT void type!

	// of stage (1), particle.rgb into BColor
	friend void operator<=(BColor &bc, const pRGB &rgb);

	// two stages:
	// (1) resolve color -> color and xy -> x y.
	// (2) feed into BCG
	friend void operator<<(BColorGrid &bcg, const pState &particle);

	// (particle <<= p_ptr) syntax
	friend pState &operator<<=(pState &particle, const P_ptr &p);

	// BCG <<= ws
	// Map color of a world snapshot.
	friend void operator<<=(BColorGrid &bcg, WorldSnapshot &ws);

	// url <= BCG
	friend void operator<=(VisualizerURL &url, const BColorGrid &bcg);

	// Out-of-the-box BRIDGES visualizer
	friend void operator<<=(VisualizerURL &url, WorldSnapshot &ws);
};

#endif
