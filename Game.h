#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <thread>
#include "Bifrost.h"
#include "colors.h"
#include <string>

#include "libs/include/Bridges.h"
#include "libs/include/ColorGrid.h"
#include "World.h"
typedef uint32_t GameTick;

class CallbackHandler { //Used for mouse click events.
	private:
		P_Type type{air};
		Wc row{}, col{};
		World &world;
		P_ptr generateParticle();
	public:
		CallbackHandler(World &inworld) : world(inworld) {}
		void setRowCol(int inrow, int incol) { 
			if(type == none) return;
			row = inrow; 
			col = incol; 
			if(world.atMap(row, col) == none) { //any value other than none will not allow particle generation. Prevents OOB and overlapping Particles
				P_ptr pt = generateParticle();
				pt->set_lifetime(1000); //arbitrary for testing
				if(pt != nullptr)  world.add_particle(pt); //Ensure it doesnt generate a nullptr 		
			}
		} //inrow and incol are previously verified in bounds. This is the function called on mousedown
		void setPType(P_Type inType) { type = inType; }
};

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
