// for the love of god, PLEASE DONT WRITE FUNCTION AND CSTOR
// IMPLEMENTATIONS IN *.h files!!!! declarations is a-ok :))

#ifndef WORLD_H
#define WORLD_H

#include <algorithm>
#include <cmath>
#include <cstddef>
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
using Wc = uint32_t; // World-coords

struct pXY {
        Pc row{}, col{};
};

struct worldXY {
		Wc row{}, col{};		
		bool operator==(const worldXY& rhs) const noexcept {
			return row == rhs.row && col == rhs.col;
		}
};

struct lifeHash {
	size_t operator()(const worldXY wxy) const noexcept {
		size_t h1 = std::hash<Wc>{}(wxy.row);
		size_t h2 = std::hash<Wc>{}(wxy.col);
		return h1 ^ (h2 >> 1);
	}
};

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
	Ps nextFrame;      //IGNORE- for life particles ;)
	// Pointer is used for flexible intercasting to
	// derived 'Particle' type
	void _updateMap(const Wc &x, const Wc &y, const P_Type &type);
	void updateMap(const Wc &x, const Wc &y, const P_Type &type);
	void _updateMap(P_ptr &p);
	void updateMapPtr(const Wc &x, const Wc &y, P_ptr &ptr);
	void parseParticlesFromJSON(std::string &s);
	bool inclusiveInRange(Wc min, Wc max, Wc val);
public:
	P_ptr nullp = nullptr; // null object pattern

	//Default cstor used to generate world size based on terminal size
	World() = default;

	// Reserve mem for map in construction
	World(const uint16_t &rows, const uint16_t &cols);

	Wc get_rows() const;
	Wc get_cols() const;
	void set_rows(const Wc &_rows);
	void set_cols(const Wc &_cols);

	void updateVecs();
	void updateMap();
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
	void add_life(P_ptr p);//IGNORE -for life particles
	std::unordered_map<worldXY, int, lifeHash> neighborCount;

	const Ps &getParticles();
	void save(const std::string &str);
	int load(const std::string &str);
};

#endif
