/* the due date you need to have:
1. A Particle class. You must use 'proper' class design. Each part
icle has:
--A position (.row, .col float) which indicates where it is on the
 screen
----No two Particles can be on the same location
--A velocity (.x_vel and .y_vel float) indicating how many cells p
er tick it moves
--A .type which must be an enum which indicates if it is air, dust
, fire, water, earth, dirt, lightning
----Each type has a different physics movement type:
------Air moves in a straight line (ignoring gravity) bouncing off
 solid
------Dust has a small amount of gravity and randomly moves left a
nd right every frame
------Fire is stationary and shoots sparks of lightning in differe
nt directions over time
------Water drips down and if it hits something solid it will slid
e sideways to find the lowest level. Water touching fire turns int
o air moving upwards
------Earth is always stationary and solid
------Lightning travels in a straight line and stops when it hits
something solid. If it touches water, the water turns into lightni
ng. If it hits earth it turns into dirt
------Dirt travels downwards and forms piles when it hits somethin
g solid
------You can add your own types as well for extra credit
--Color (.r .g .b uint8_t) that should be set initially by its typ
e (fire should be red) but can change
--A boolean named .stationary that if true means it is solid and d
oes not move/simulate physics
----So if you turn stationary true on water it will not move even
if a spot opens up beneath it
----Stationary Particles can still be destroyed or transformed by
other Particles
----Stationary particles have a lifetime of -1 (see below)
--A .lifetime variable which holds how many frames more it will ex
ist for
----Physics (see below) will decrement it each frame by 1 to a min
imum of 0.                                                        ----Fire and
lightning should have relatively short lifetimes, sto ne should have a lifetime
of -1.
----A lifetime of -1 means infinite lifetime (it will not go away
over time)
--Member variables must be in the private section, methods in the
public section
----Make the usual constructors, getters/setters, and so forth. Yo
u should use the Rule of 0
--A .physics() method that gets the world map passed in by referen
ce, that runs physics for the particle and updates the world map i
f it moved. If it runs into another particle, it calls .touch() on
 the other particle.
----Physics also decrements .lifetime by one each frame
--A .touch() method that gets called when another particle touches
 it.
--Between .physics and .touch you can handle all interactions (fir
e x water, water x lightning, etc.) */
#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstdint>
#include "World.h"
using namespace std;

// type enum. 3 TBDs if feasible, 1 unknown, and as requirement stated.
// init enum defaults to `none` (0).
enum Type {
	none,
	air,
	dust,
	fire,
	water,
	earth,
	dirt,
	lightning,
	tbd1,
	tbd2,
	tbd3
};

// X leftright, Y updown, starting at top left.
class Particle {
protected:
	float row{}, col{}; // coords of P, as P(row, col)
	float x_vel{}, y_vel{};  // velocity
	uint8_t r{}, g{}, b{}; // color RGB
	bool stationary = false; // self-explanatory
	int32_t lifetime{}; // choosing type int32_t is arbitrary
public:
	// These functions MUST BE IMPLEMENTED in derived classes.
	virtual void physics(World& world) = 0;
	virtual void touch() = 0; // Rationale: https://stackoverflow.com/questions/3644065/how-to-write-an-elegant-collision-handling-mechanism
};

using P = Particle; // for lazy fuckers like us

class Air : public P {};
class Dust : public P {};
class Fire : public P {};
class Water : public P {};
class Earth : public P {};
class Dirt : public P {};
class Lightning : public P {};
class TBD_1 : public P {};
class TBD_2 : public P {};
class TBD_3 : public P {};

#endif
