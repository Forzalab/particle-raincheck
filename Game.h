/*
Game class that is responsible for processing input
--It has the world (.world World) as a member variable
----The world should be initialized with the screen_size as th
e row and col, but leave some lines open on the bottom of the
screen to show the frame count and a helpful list of commands
--A .frame member variable that starts at 0 and counts up by 1
 each time physics is run
--It has to handle all input from mouse and keyboard using non
-blocking I/O
--Implement a command to start simulating the world
--Implement a command to pause simulating the world
--Implement a command to quit
--Implement a command to load the world from disk
--Implement a command to save the world to disk
--Implement a command to increase frame rate
--Implement a command to decrease frame rate

----Simulation should also automatically pause when world.aliv
e_count is 0
--A .render() method which draws all the particles in the list
 to the screen using their position and color
----Below the world you should display the frame count and sho
w the commands possible (L)oad S)ave P)ause etc)
----It must be rendered across the full screen using the color
 library, using 32 bit colors
--The user must be able to edit the game world by adding and d
eleting particles while the world is paused
----It must work with mouse input
--A .run() method that starts the game. Your main() function s
hould just be this: Game game; game.run();
----On launch draw a splash screen
----Then go into an infinite loop of: handling user input, run
ning physics, drawing the world, delaying
----It should delay long enough to guarantee a fixed frame rat
e of 5 fps (you can adjust this number)
----Use clock (or std::chrono) to see how long it has been sin
ce the last frame began
----Use usleep() to delay execution so you get a guaranteed fr
ame rate of 5 fps or whatever

4. You need to make a BRIDGES output option. When the user hit
s the "D)raw the World on BRIDGES" command you should render t
he world on BRIDGES using the ColorGrid class:
--Read the BRIDGES documentation: https://bridgesuncc.github.i
o/ and comprehend it
--Add in your username and API key
--Make a ColorGrid class and fill it with the colors of the wo
rld (or test data)
--Add in a call to bridges.setDataStructure() to tell BRIDGES
to render the ColorGrid
--Add in a call to bridges.visualize() to send it off to the w
eb site
----This will give you a URL that you can use to visualize you
r world in a browser

--Implement a command to draw the world on BRIDGES (see below


*/

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
	GameTick tickrate = 60;
public:
	Game() : world(50, 70) {}
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
