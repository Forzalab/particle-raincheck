#ifndef GAME_H
#define GAME_H

#include "libs/include/Bridges.h"
#include "libs/include/ColorGrid.h"
#include "World.h"
typedef uint32_t GameTick;

class Game {
private:
	//Cannot default init with current setup- must be init in ::run() I may change this later when we are done and have time to refactor
	World world;
	GameTick frame{};
	//Default of 5. Tickrate is directly proportional to framerate. 60 tickrate -> 1 / tickrate = 60fps.
	GameTick tickrate = 30;
public:
	Game() : world(50, 70) {}
	GameTick get_tickrate() const;
	void start();
	void pause();
	void quit();
	void load();
	void save();
	void incr_fps();
	void dcrs_fps();
	void draw();
	void draw_BRIDGES(); // URL will be printed by BRIDGE
	void render();
	void run();
};

#endif
