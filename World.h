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
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "Particle.h"

using P = Particle;

struct pXY {
        Pc row{}, col{};
};

using Wc = uint32_t; // World-coords
using Amt = int32_t;
using Ps = std::list<P_ptr>; // ParticleS
using Map = std::vector<P_Type>;
using Map_ptr = std::vector<P_ptr>;
using PtrXY = std::unordered_map<P_ptr, pXY>;

class World {
private:
	Wc rows{}, cols{}; // WxH of World
	// 2vectors instead of 1 pair<P_Type, P_Ptr> for 
	// non-disruptive integration
	Map map;		   // FLATTENED map
	Map_ptr map_ptr;
	PtrXY prev_pos;
	Ps ps;			   // list of Particles' POINTERS
	// Pointer is used for flexible intercasting to
	// derived 'Particle' type
	void _updateMap(const Wc &x, const Wc &y, const P_Type &type);
	void updateMap(const Wc &x, const Wc &y, const P_Type &type);
	void _updateMap(P_ptr &p);
	void updateMapPtr(const Wc &x, const Wc &y, P_ptr &ptr);
	void parseParticlesFromJSON(std::string &s);

public:
	P_ptr nullp = nullptr; // null object pattern

	// Reserve mem for map in construction
	World(const uint16_t &rows, const uint16_t &cols);

	Wc get_rows() const;
	Wc get_cols() const;
	void set_rows(const Wc &_rows);
	void set_cols(const Wc &_cols);

//	who wrote this line shod go to gulag
//	P_Type atMap(Wc row, Wc col);

	void updateVecs();

	void updateMapPtr(P_ptr &p);
	void updateMap(P_ptr &p);
	void updateMapPrev(const Wc &y, const Wc &x, const P_ptr &p);

	void erase(const Wc &row, const Wc &col);

	P_ptr &at(const Pc &row, const Pc &col); // .at()
	P_ptr &atMap_ptr(const Wc &row, const Wc &col);
	P_Type atMap(const Wc &row, const Wc &col);

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
