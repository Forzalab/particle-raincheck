#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <list>
#include <memory>
#include <cstdint>
#include <vector>
#include <string>
#include "Particle.h"

using P = Particle;

typedef uint32_t Wc; // World-coords
typedef int32_t Amt;
typedef std::list<P_ptr> Ps; // ParticleS
typedef std::vector<P_Type> Map;

class World {
private:
	Wc rows{}, cols{}; // WxH of World
	Map map;		   // FLATTENED map
	Ps ps;			   // list of Particles' POINTERS
	// Pointer is used for flexible intercasting to
	// derived 'Particle' type

	void updateMap();
	void parseParticlesFromJSON(std::string &s);

public:
	P_ptr nullp = nullptr; // null object pattern
	// Reserve mem for map in construction.
	World(const Wc &rows = 50, const Wc &cols = 70) : rows(rows), cols(cols) {
		map.resize(size_t(rows) * size_t(cols));
	}

	Wc get_rows() const;
	Wc get_cols() const;
	void set_rows(const Wc &_rows);
	void set_cols(const Wc &_cols);

	P_Type atMap(Wc row, Wc col);

	void updateVecs() {
		map.resize(size_t(rows) * size_t(cols));
	}
	
	void erase(const Wc &row, const Wc &col);
	P_ptr& at(const Pc &row, const Pc &col); // .at()
	// Helper func to make World::physics() cleaner
	bool isInBounds(const auto &p);
	int physics();			 // physics() iterates all P.
	Amt size() const;		 // get amt of P
	Amt alive_count() const; // get amt of LIVING P.

	void add_particle(P_ptr p);

	const Ps& getParticles() { return ps; }
	// One preset save-file is enough?
	void save();
	void load();
};

#endif
