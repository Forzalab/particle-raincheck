#pragma message("")// I do not understand why, but without this World.h only partially compiles and throws an error.
#include "World.h"
static_assert(sizeof(World) > 0);
#include "Particle.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const std::string SAVEFILE = "save.JSON";

P* World::at(const Pos& row, const Pos& col) const { 
	P* p = Ps.find(row + col*cols);
	return (p!=Ps.end()?p:nullptr);
} // .at() 

void World::physics(){ ; } // physics() iterates all P.
Amt World::size() const{ return 0; } // get amt of P
Amt World::alive_count() const{ return 0;} // get amt of LIVING P.
	
void World::add_particle(P& p) {
	ps.push_back(&p);
}
// One preset save-file is enough?
//	Yeah. Will be saved in JSON (IMMITATED) Format. If we want, we can implement a way to save to a specific filename after finishing everything else.
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
	std::ofstream ofs("save2.JSON");
	// std::ofstream ofs(SAVEFILE);
	// Basic mem vars
	insertOFS(ofs, "{\n\t\"rows\": " + std::to_string(rows) + ",\n\t\"cols\": " + std::to_string(cols) + ",\n\t\"Ps\": [\n");
	if(ps.size() > size_t(0)) {
		size_t counter = 0;
		for(const auto &p : ps) {
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
			val += "\n\t\t\t\"lifetime\": " + std::to_string(p->get_lifetime()) + ",";
			val += "\n\t\t\t\"type\": " + std::to_string(p->get_type()) + ",";
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

//this function operates under the assumption that the value of ss is formatted as "\"str\": val,\n". 
//it tosses out the value before the :, then tosses out the leading space and the , and leaves returns val
std::string extractVal(std::string &s) {
	std::vector<std::string> retvec;
	//returns a substring of s from : + 2 until the comma 
	//eg "key": val, woruld return only val.
	return s.substr(s.find_first_of(':') + 2, s.find_first_of(','));
}

std::vector<std::string> explodeStr(std::string &s) {
	std::stringstream ss(s);
	std::string temp;
	std::vector<std::string> ret;
	getline(ss, temp, ',');
	while(!ss.eof()) {
		ret.push_back(extractVal(temp));
		getline(ss, temp, ',');
	}
	return ret;
}

//Extracts vals from JSON and creates a new particle
//Very disgusting manual function, buuuuuuuut writing a fully functioning JSON parser is out of the question for this assignment.
//Was JSON necessary for this? Absolutely not, for the scope of what we are doing. HOWEVER- our save files will look fancy. NO ONE needs to see this mess but us and the Kernel.
Particle* extractParticle(std::string &s) {
	// Any Particle-derived class shares the same
	// function pointers. Thus, we hack shit together.
	// Choose any particular Particle-derived
	// pointer type (like Air) as placeholder
	// for "generic Particle" data without
	// offending the Abstract-Class god.
	Air* p = new Air;

	// We will cast back to Particle* near return...
	std::vector<std::string> Pvals = explodeStr(s);
	p->set_row(stof(Pvals.at(0)));
	p->set_col(stof(Pvals.at(1)));
	p->set_x_vel(stof(Pvals.at(2)));
	p->set_y_vel(stof(Pvals.at(3)));
	p->set_r(stoi(Pvals.at(4)));
	p->set_g(stoi(Pvals.at(5)));
	p->set_b(stoi(Pvals.at(6)));
	p->set_stationary((Pvals.at(7)) == "true");
	p->set_lifetime(stoi(Pvals.at(8)));
	p->set_type(static_cast<Type>(stoi(Pvals.at(9))));
	
	// ...right here. Magic lol
	Particle* pp = static_cast<Air*>(p);

	return pp;
}

//seperate string manip function for particles
void World::parseParticlesFromJSON(std::string &s) {
	std::stringstream ss(s);	
	std::string particle;
	std::getline(ss, particle, '}');
	while(!ss.eof()) {
		Particle* p = extractParticle(particle);
		add_particle(*p);
		std::getline(ss, particle, ',');//Throw out comma inbetween each particle.
		std::getline(ss, particle, '}');
	}
}

void World::load(){
	std::ifstream ifs(SAVEFILE);
	if(!ifs) {
		std::cerr << "Save file failed to open.\n";
	}
	std::string s;
	//Throw out the first linw, as ut is always just a open curly brace.
	//This JSON parser will not be portable in the slightest, it will just work for this case.
	std::getline(ifs, s, '\n');
	//Member primitives
	//rows
	std::getline(ifs, s, '\n');
	try {
		rows = stoi(extractVal(s));
	} catch (std::exception &e) {
		std::cerr << "Val extracted for ROWS invalid.\n";
		exit(EXIT_FAILURE);
	}
	//cols
	std::getline(ifs, s, '\n');
	try {
		cols = stoi(extractVal(s));
	} catch (std::exception &e) {
		std::cerr << "Val extracted for COLS invalid.\n";
		exit(EXIT_FAILURE);
	}

	std::getline(ifs, s, '\n'); // "Ps": [ line
	std::getline(ifs, s, ']'); // Get entire json array of particles, if any.
	if(s.length() > 1) { // == 1 means it is empty
		parseParticlesFromJSON(s);	
	}
}
