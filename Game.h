#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <thread>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

#include "Bifrost.h"
#include "colors.h"
#include "libs/include/Bridges.h"
#include "libs/include/ColorGrid.h"
#include "World.h"

typedef uint32_t GameTick;

class CallbackHandler { // Used for mouse click events.
private:
	P_Type type{air};
	Wc row{}, col{};
	World &world;
	P_ptr generateParticle();

public:
	CallbackHandler(World &inworld);
	void setRowCol(int inrow, int incol);
	void setPType(P_Type inType);
};

class Game {
private:
	// Cannot default init with current setup- must be init in ::run() I may
	// change this later when we are done and have time to refactor
	World world;
	GameTick frame{};
	Wc rows{}, cols{};
	// Default of 5. Tickrate is directly proportional to framerate. 60 tickrate
	// -> 1 / tickrate = 60fps.
	GameTick tickrate = 50;

public:
	Game() : world(rows, cols) {}
	GameTick get_tickrate() const;
	void quit();
	void load();
	void save();
	void incr_fps();
	void dcrs_fps();
	void draw();
	void draw_BRIDGES(); // URL will be printed by BRIDGE
	std::string render();
	void run();
};

#endif
