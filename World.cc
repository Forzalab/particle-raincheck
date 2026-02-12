#include "World.h"
#include "Particle.h"
#include <fstream>
#include <iostream>
#include <string>

const std::string SAVEFILE = "save.JSON";

P* World::at(const Pos& row, const Pos& col) const { return nullptr; } // .at() 
void World::physics(){ ; } // physics() iterates all P.
Amt World::size() const{ return 0; } // get amt of P
Amt World::alive_count() const{ return 0;} // get amt of LIVING P.
	
void World::add_particle(const P &p) {
	std::cout << "Here" << std::endl;
	ps.push_back(p);
}
// One preset save-file is enough?
//	Yeah. Will be saved in JSON Format. If we want, we can implement a way to save to a specific filename after finishing everything else.
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

void insertOFS(std::ofstream &ofs, const std::string &val) {
	ofs << val;
}

// This will be interesting to write as it's been like 6 years since I've written a JSON parser, but we ball
void World::save(){
	std::ofstream ofs(SAVEFILE);
	// Basic mem vars
	insertOFS(ofs, "{\n\t\"rows\": " + std::to_string(rows) + ",\n\t\"cols\": " + std::to_string(cols) + ",\n\t\"Ps\": [\n");
	if(ps.size() > size_t(0)) {
		std::cout << ps.size() << std::endl;
		size_t counter = 0;
		for(const auto &p : ps) {
			std::string val = "\t\t{";
			val += "\n\t\t\t\"row\": " + std::to_string(p.get_row());
			val += "\n\t\t\t\"col\": " + std::to_string(p.get_col());
			val += "\n\t\t\t\"x_vel\": " + std::to_string(p.get_x_vel());
			val += "\n\t\t\t\"y_vel\": " + std::to_string(p.get_y_vel());
			val += "\n\t\t\t\"r\": " + std::to_string(p.get_r());
			val += "\n\t\t\t\"g\": " + std::to_string(p.get_g());
			val += "\n\t\t\t\"b\": " + std::to_string(p.get_b());
			val += "\n\t\t\t\"stationary\": " + std::to_string(p.get_stationary());
			val += "\n\t\t\t\"lifetime\": " + std::to_string(p.get_lifetime());
			val += "\n\t\t\t\"type\": " + std::to_string(p.get_type());
			val += "\n\t\t}";
			if(counter < ps.size() - 1) val+= ",";
			counter++;
			val += "\n";
			insertOFS(ofs, val);
		}
	}
	insertOFS(ofs, "\t]\n}");
	ofs.close();
}

void World::load(){}
