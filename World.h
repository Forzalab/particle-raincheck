/*
World class that has a 2D grid of Particles (or are empty). W
orld has:
--A size (.rows .cols int) that is the boundary of the world.
----Any point which leaves the boundary must be deleted
--A map (rows x cols in size of chars) which indicates which parti
cle type (or nothing) is at each place in the game world
--A list (must be std::list) of Particles which holds all particle
s in the world
--An .at(row,col) method that returns a pointer to the Particle at
 the location row x col in the world (or nullptr if there is no pa
rticle there)
--A .physics() method which runs through the list of Particles and
 runs .physics() on each one
----If a particle is now out of bounds (no YOU'RE out of bounds) t
hen delete it from the list
----If a particle now has a lifetime of exactly 0, delete it from
the list
--A .size() method which returns how many particles are in the wor
ld
--A .alive_count() method which returns how many particles with li
fetime > 0 are in the world
--A .save() method which saves the state of the world to disk
--A .load() method which loads the state of the world to disk
*/

#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <list>
#include <vector>
#include "Particle.h"

typedef int Pos;
typedef int Amt;
typedef std::list<P> Ps; // ParticleS
typedef std::vector<char> Map;

class World {
private:
	Pos rows{}, cols{}; // WxH of World
	Map map; // FLATTENED map
	Ps ps; // list of Particles
public:
	P* at(const Pos& row, const Pos& col) const; // .at() 
	void physics(); // physics() iterates all P.
	Amt size() const; // get amt of P
	Amt alive_count() const; // get amt of LIVING P.

	void add_particle(const P &p);
	
	// One preset save-file is enough?
	void save();
	void load();
};

#endif
