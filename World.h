// for the love of god, PLEASE DONT WRITE FUNCTION AND CSTOR
// IMPLEMENTATIONS IN *.h files!!!! declarations is a-ok :))

#ifndef WORLD_H
#define WORLD_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <cstdint>
#include <vector>

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
	void updateMap(const Wc &x, const Wc &y, const P_Type &type);
	void parseParticlesFromJSON(std::string &s);

public:
	P_ptr nullp = nullptr; // null object pattern

	// Reserve mem for map in construction
	World(const uint16_t &rows, const uint16_t &cols);

	Wc get_rows() const;
	Wc get_cols() const;
	void set_rows(const Wc &_rows);
	void set_cols(const Wc &_cols);

	P_Type atMap(Wc row, Wc col);

	void updateVecs();
	void updateMap(const P_ptr &p);

	void erase(const Wc &row, const Wc &col);
	P_ptr &at(const Pc &row, const Pc &col); // .at()
	// Helper func to make World::physics() cleaner
	bool isInBounds(const auto &p);
	bool has_gap_at(const Wc &y, const Wc &x);
	int physics();			 // physics() iterates all P.
	Amt size() const;		 // get amt of P
	Amt alive_count() const; // get amt of LIVING P.

	void add_particle(P_ptr p);

	const Ps &getParticles();
	void save(const std::string &str);
	int load(const std::string &str);
};

#endif
