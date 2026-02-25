// for the love of god, PLEASE DONT WRITE FUNCTION AND CSTOR
// IMPLEMENTATIONS IN *.h files!!!! declarations is a-ok :))

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
	void setRowCol(Wc inrow, Wc incol);
	void setPType(P_Type inType);
};

class Game {
private:
	// Solved: init world in MIL of game().
	World world;
	GameTick frame{};
	Wc rows{}, cols{};
	GameTick tickrate{};

public:
	Game();
	Game(const uint16_t &rows, const uint16_t &cols);
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
