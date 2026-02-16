// I do not understand why, but without this World.h
// only partially compiles and throws an error.
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <numeric>
#pragma message("")

#include "World.h"
#include "Particle.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static_assert(sizeof(World) > 0);

const std::string SAVEFILE = "save.JSON";

void World::updateMap() {
	// If no particles, clear list and return early
	if (ps.size() == 0) {
		map.clear();
		return;
	}
	map.assign(map.size(), none);
	//Vector mask. saves positions that contain a particle.
	//Iter over list of Particles and update map at those indecies
	//Also add a true to the mask to prevent from being set to none.
	for(const auto &p : ps) {
		Wc index = std::floor(p->get_row()) * cols + std::floor(p->get_col());
		// std::cout << std::endl << p->get_row() << " " << p->get_col() << " " << rows << " " << cols;
		// std::cin.get();
		map.at(index) = p->get_type();
	}
}

// World::World(const Wc& rows, const Wc& cols) : rows(rows), cols(cols) {}

Wc World::get_rows() const { return rows; }

Wc World::get_cols() const { return cols; }

void World::erase(const Wc &row, const Wc &col) {
	auto it_rmv = map.begin() + [&]() { return col + row * cols; }();
	ps.remove(at(row, col));
	map.erase(it_rmv);
}

void World::set_cols(const Wc &_cols) { cols = _cols; }

void World::set_rows(const Wc &_rows) { rows = _rows; }

P_ptr& World::at(const Wc &row, const Wc &col) {
	auto p = ps.begin();
	for (; p != ps.end(); p++) {
		if (row == (*p)->get_row() && col == (*p)->get_col())
			break;
	}
	return (p != ps.end() ? *p : nullp);
} // .at()

// because cpp doesnt support range conditionals Sadge
// Returns true if the particle's coordinates is within the range
bool exclusiveInRange(Wc min, Wc max, Wc val) {
	return min < val && val < max;
}

bool World::isInBounds(const auto &p) {
	Wc col = std::floor(p->get_col());
	Wc row = std::floor(p->get_row());

	return (exclusiveInRange(0, cols, col) && exclusiveInRange(0, rows, row));
}

//Since this function is essentially the update loop of World
//Map map will be updated here too
void World::physics() { 
	
	if(size() == 0) return;	
	
	for(auto &p : ps) {
		//Do particle physics calls here
		p->physics(*this);
		//Decrement p lifetime if it is not a permanent particle
		if(p->get_lifetime() != -1) p->set_lifetime(p->get_lifetime()-1);		
	}
	//If the particle is "dead" aka lifetime is exactly 0
	//OR
	//If it's out of bounds
	std::erase_if(ps, [this](const auto &p) {
				return (p->get_lifetime() == 0 && p->get_stationary() == false) || !isInBounds(p);
			});

	updateMap(); // currently our limiting factor performance wise. 15-17 fps with, at least 60 without
}	 // physics() iterates all P.

Amt World::size() const {
	// casting just to get rid of annoying warning.
	return Amt(ps.size());
} // get amt of P
Amt World::alive_count() const {
	// Guard from empty list, return -1 as err
	if (ps.size() == 0)
		return -1;

	// 1st param of lambda is the current count, init to 0 in 3rd param of
	// std::accumulate 2nd param is the current element in the loop Returning 0
	// is still a valid count, -1 as error prevents exception via error as
	// return, Allowing us to detect empty list vs none above 0 lfetime
	// particles.
	return std::accumulate(
		ps.begin(), ps.end(), 0,
		[](int count, const auto &p) { return p->get_lifetime() > 0; });
} // get amt of LIVING P.

void World::add_particle(P_ptr p) { ps.push_back(p); }

// One preset save-file is enough?
//	Yeah. Will be saved in JSON (IMMITATED) Format. If we want, we can implement
// a way to save to a specific filename after finishing everything else.
/*
{
	"key": val,
	"key2": val,
	.
	.
	.
	"Ps": [
		{
			"key1": val,
			"key2": val,
			.
			.
			.
			"keyN": val
		},
		{
			"key1": val,
			"key2": val,
			.
			.
			.
			"keyN": val
		}
	]
}
*/

void insertOFS(std::ofstream &ofs, const std::string &val) { ofs << val; }

// This will be interesting to write as it's been like 6 years since I've
// written a JSON parser, but we ball
void World::save() {
	std::ofstream ofs("save2.JSON");
	// std::ofstream ofs(SAVEFILE);
	// Basic mem vars
	insertOFS(ofs, "{\n\t\"rows\": " + std::to_string(rows) +
					   ",\n\t\"cols\": " + std::to_string(cols) +
					   ",\n\t\"Ps\": [\n");
	if (ps.size() > size_t(0)) {
		size_t counter = 0;
		for (const auto &p : ps) {
			std::string temp = p->get_stationary() ? "true" : "false";
			std::string val = "\t\t{";
			val += "\n\t\t\t\"row\": " + std::to_string(p->get_row()) + ",";
			val += "\n\t\t\t\"col\": " + std::to_string(p->get_col()) + ",";
			val += "\n\t\t\t\"x_vel\": " + std::to_string(p->get_x_vel()) + ",";
			val += "\n\t\t\t\"y_vel\": " + std::to_string(p->get_y_vel()) + ",";
			val += "\n\t\t\t\"r\": " + std::to_string(p->get_r()) + ",";
			val += "\n\t\t\t\"g\": " + std::to_string(p->get_g()) + ",";
			val += "\n\t\t\t\"b\": " + std::to_string(p->get_b()) + ",";
			val += "\n\t\t\t\"stationary\": " + temp + ",";
			val +=
				"\n\t\t\t\"lifetime\": " + std::to_string(p->get_lifetime()) +
				",";
			val += "\n\t\t\t\"type\": " + std::to_string(p->get_type()) + ",";
			val += "\n\t\t}";
			if (counter < ps.size() - 1)
				val += ",";
			counter++;
			val += "\n";
			insertOFS(ofs, val);
		}
	}
	insertOFS(ofs, "\t]\n}");
	ofs.close();
}

// this function operates under the assumption that the value of ss is formatted
// as "\"str\": val,\n". it tosses out the value before the :, then tosses out
// the leading space and the , and leaves returns val
std::string extractVal(std::string &s) {
	std::vector<std::string> retvec;
	// returns a substring of s from : + 2 until the comma
	// eg "key": val, woruld return only val.
	return s.substr(s.find_first_of(':') + 2, s.find_first_of(','));
}

std::vector<std::string> explodeStr(std::string &s) {
	std::stringstream ss(s);
	std::string temp;
	std::vector<std::string> ret;
	getline(ss, temp, ',');
	while (!ss.eof()) {
		ret.push_back(extractVal(temp));
		getline(ss, temp, ',');
	}
	return ret;
}

// Extracts vals from JSON and creates a new particle
// Very disgusting manual function, buuuuuuuut writing a fully functioning JSON
// parser is out of the question for this assignment. Was JSON necessary for
// this? Absolutely not, for the scope of what we are doing. HOWEVER- our save
// files will look fancy. NO ONE needs to see this mess but us and the Kernel.
P_ptr extractParticle(std::string &s) {
	// Any Particle-derived class shares the same
	// function pointers. Thus, we hack shit together.
	// Choose any particular Particle-derived
	// pointer type (like Air) as placeholder
	// for "generic Particle" data without
	// offending the Abstract-Class god.
	// Air *p = new Air;

	std::vector<std::string> Pvals = explodeStr(s);
	P_Type type = static_cast<P_Type>(stoi(Pvals.at(9)));
	Pc row(stof(Pvals.at(0)));
	Pc col(stof(Pvals.at(1)));
	P_ptr p;
	//If type == 0 this BREAKS, so we need to be very sure no type = 0 exists in a save
	if(type == 1) p = std::make_shared<Air>(row, col);
	if(type == 2) p = std::make_shared<Dust>(row, col);
	if(type == 3) p = std::make_shared<Fire>(row, col);
	if(type == 4) p = std::make_shared<Water>(row, col);
	if(type == 5) p = std::make_shared<Earth>(row, col);
	if(type == 6) p = std::make_shared<Dirt>(row, col);
	if(type == 7) p = std::make_shared<Lightning>(row, col);

	p->set_x_vel(stof(Pvals.at(2)));
	p->set_y_vel(stof(Pvals.at(3)));
	//TODO: stretch goal -> custom particles use these V but default particles use default cstor vals.
	// p->set_r(stoi(Pvals.at(4)));
	// p->set_g(stoi(Pvals.at(5)));
	// p->set_b(stoi(Pvals.at(6)));
	p->set_stationary((Pvals.at(7)) == "true");
	p->set_lifetime(stoi(Pvals.at(8)));

	return p;
}

// seperate string manip function for particles
void World::parseParticlesFromJSON(std::string &s) {
	std::stringstream ss(s);
	std::string particle;
	std::getline(ss, particle, '}');
	while (!ss.eof()) {
		P_ptr p = extractParticle(particle);
		add_particle(p);
		std::getline(ss, particle,
					 ','); // Throw out comma inbetween each particle.
		std::getline(ss, particle, '}');
	}
}

void World::load() {
	std::ifstream ifs(SAVEFILE);
	if (!ifs) {
		std::cerr << "Save file failed to open.\n";
	}
	std::string s;
	// Throw out the first linw, as ut is always just a open curly brace.
	// This JSON parser will not be portable in the slightest, it will just work
	// for this case.
	std::getline(ifs, s, '\n');
	// Member primitives
	// rows
	std::getline(ifs, s, '\n');
	try {
		rows = stoi(extractVal(s));
	} catch (std::exception &e) {
		std::cerr << "Val extracted for ROWS invalid.\n";
		exit(EXIT_FAILURE);
	}
	// cols
	std::getline(ifs, s, '\n');
	try {
		cols = stoi(extractVal(s));
	} catch (std::exception &e) {
		std::cerr << "Val extracted for COLS invalid.\n";
		exit(EXIT_FAILURE);
	}

	std::getline(ifs, s, '\n'); // "Ps": [ line
	std::getline(ifs, s, ']');	// Get entire json array of particles, if any.
	if (s.length() > 1) {		// == 1 means it is empty
		parseParticlesFromJSON(s);
	}
}
