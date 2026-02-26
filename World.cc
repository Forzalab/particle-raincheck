// put all #include in header file pls

#include "World.h"

static_assert(sizeof(World) > 0);

const std::string SAVEFILE = "save.JSON";

// Reserve mem for map in construction.
// World::World(const uint16_t &rows = 50, const uint16_t &cols = 70)
	// : rows(rows), cols(cols) {
	// map.resize(size_t(rows) * size_t(cols));
	// map.assign(map.size(), none);
// }

const Ps &World::getParticles() { return ps; }

void World::updateVecs() {
	//the spread of cout debugging grows
	// std::cout << std::endl;
	// std::cout << rows << " " << cols << " " << size_t(rows) * size_t(cols) << endl;
	map.resize(size_t(rows) * size_t(cols));
	// cout << map.size();
	map.assign(map.size(), none);

	// yes i know.
        // i welcome all deref null efforts
	map_ptr.resize(size_t(rows) * size_t(cols));
//	map_ptr.assign(map_ptr.size(), nullp);
}

/*
void World::updateMap() {
	// If no particles, clear list and return early
	if (ps.size() == 0) {
		map.clear();
		return;
	}
	map.assign(map.size(), none);
	// Vector mask. saves positions that contain a particle.
	// Iter over list of Particles and update map at those indecies
	// Also add a true to the mask to prevent from being set to none.
	for (const auto &p : ps) {
		Wc rawInd = int(p->get_row()) * cols + int(p->get_col());
		if(rawInd >= map.size()) continue;
		map.at(rawInd) = p->get_type();
	}
}
*/

void World::_updateMap(const Wc &y, const Wc &x, const P_Type &type) {
	// Vector mask. saves positions that contain a particle.
	// Iter over list of Particles and update map at those indecies
	// Also add a true to the mask to prevent from being set to none.
	Wc rawInd = int(y) * cols + int(x);
	if (rawInd < map.size())
		map.at(rawInd) = type;
}

void World::_updateMap(P_ptr &p) {
	_updateMap(p->get_row(), p->get_col(), p->get_type());
}

void World::updateMapPtr(const Wc &y, const Wc &x, P_ptr &ptr) {
        // Vector mask. saves positions that contain a particle.
        // Iter over list of Particles and update map at those indecies
        // Also add a true to the mask to prevent from being set to none.
        Wc rawInd = int(y) * cols + int(x);
        if (rawInd < map_ptr.size())
                map_ptr.at(rawInd) = ptr;
}

void World::updateMapPtr(P_ptr &p) {
	updateMapPtr(Wc(p->get_row()),Wc(p->get_col()), p);
}

void World::updateMap(const Wc &y, const Wc &x, const P_Type &type) {
	_updateMap(y, x, type);
}

// YES, THIS LOOKS LIKE PURE CRAP: 1 func running 2 similarily named funcs :))
// BUT THIS FOR LEGACY CODE :(((
void World::updateMap(P_ptr &p) {
        _updateMap(p);
	updateMapPtr(p);
}

Wc World::get_rows() const { return rows; }

Wc World::get_cols() const { return cols; }

void World::erase(const Wc &row, const Wc &col) {
	auto it_rmv = map.begin() + [&]() { return col + row * cols; }();
	ps.remove(at(row, col));
	*it_rmv = none;
}

//Setters should NOT resize vectors like this. What happens if you accidentally set rows or cols to 0?
//setters should only set the variable they are intended to set. This is fragile, and bug indusive
void World::set_cols(const Wc &_cols) {
	cols = _cols;
	// updateVecs();
}

void World::set_rows(const Wc &_rows) {
	rows = _rows;
	// updateVecs();
}

// because cpp doesnt support range conditionals Sadge
// Returns true if the particle's coordinates is within the range
bool inclusiveInRange(Wc min, Wc max, Wc val) { return min <= val && val <= max; }

P_Type World::atMap(Wc row, Wc col) {
	if (inclusiveInRange(0, rows - 1, row) &&
		inclusiveInRange(0, cols - 1, col)) {
		return map.at(cols * row + col);
	} else {
		return OOB;
	} // If you're checking out of bounds
}

P_ptr &World::at(const Pc &row, const Pc &col) {
	auto p = ps.begin();
	for (; p != ps.end(); p++) {
		if (P::is_equal(row, (*p)->get_row()) &&
			P::is_equal(col, (*p)->get_col()))
			break;
	}
	return (p != ps.end() ? *p : nullp);
} // .at()

P_ptr &World::atMap_ptr(const Wc &row, const Wc &col) {
        if (inclusiveInRange(0, rows - 1, row) &&
                inlusiveInRange(0, cols - 1, col)) {
                return map_ptr.at(cols * row + col);
        } else {
                return nullp;
        }
} // .at()

bool World::isInBounds(const auto &p) {
	Wc col = int(p->get_col());
	Wc row = int(p->get_row());

	return (inclusiveInRange(0, cols, col) && inclusiveInRange(0, rows, row));
}

bool World::has_gap_at(const Wc &y, const Wc &x) {
	// TODO: hashmap existing particle!!!!!!
	return !(P::is_solid(this->atMap(y, x)));
}

// To mutate prevPos, capturing old state WHEN running each particle touch/physics_spec
void World::updateMapPrev(const Wc &y, const Wc &x, const P_ptr &p) {
        prev_pos[p] = {Pc(x), Pc(y)};
}

// Since this function is essentially the update loop of World
// Map map will be updated here too
// This returns an int used to increment Game.frame.
int World::physics() {

	if (alive_count() == 0)
		return 0;

	// If no particles, clear list and return early
	if (ps.size() == 0) {
		map.clear();
		return 2;
	}

	// double suffering... oops, i mean double buffering
	// algo that brings sufferings

	// why 2 loops? we dont want half-mutated Map and ps,
	// we want each "container" to be completed b4 working
	// on to the next one

	// sentinel to GUARD element count in prev_pos.
	// prev_pos is guranteed NOT to be deleted any elements!
	// as deletion is executed AFTER these 2 for-loops below.
	uint16_t count = 0;
	auto end = ps.end();

	// First loop represent old state ps
	for (auto p = ps.begin(); p != end; p++) {
		Wc x = (*p)->get_col();
		Wc y = (*p)->get_row();
		
		// The map carries on ITS PREV STATE to next loop as it isn't updated!
		// AND we need prev pos data for clearing cell
		// BUT mutated cell obvs wont be captured
		// So need be careful. Lock bound? Yes!
		updateMapPrev(y, x, *p);
		count++;

		// Do particle physics calls here
		(*p)->physics(*this);
	}
	
	// Second loop iterate thru MUTATED ps
	// ps is guranteed to NOT go down in elem.
	for (auto p = ps.begin(); p != ps.end(); p++) {
		// After phsyics! ps includes mutated particles.
		updateMap(*p);

		Wc x_new = (*p)->get_col();
                Wc y_new = (*p)->get_row();

		bool st = (*p)->get_stationary();
		
		// note the deximal truncation.
		// equal = "in floor range"
		// w/o this, accidental deletion of slow-moving
		// particles (which will be realized in frame
		// eventually, but affects fading)
		bool stay = (x_new == prev_pos.at(*p).col && y_new == prev_pos.at(*p).row);

		// count check for sentinel explained above
		// stay check, as abive, wont delete "unmoving"
		// particles
		if (!stay && count > 0) {
			updateMap(prev_pos.at(*p).col, prev_pos.at(*p).row, none); // Old particle pos
			count--;
		}
		
		// 'Map' is now updated with new ps location
		// type check for lifetime decrement
		P_Type type_new = this->atMap(y_new, x_new);

		// Decrement p lifetime if it is not a permanent particle
		if (type_new != none && (*p)->get_lifetime() > 0)
			(*p)->set_lifetime((*p)->get_lifetime() - 1);
	}

	// i need a double cheeseburger after this

	// If the particle is "dead" aka lifetime is exactly 0
	// OR
	// If it's out of bounds
	std::erase_if(ps, [this](const auto &p) {
		// OOB or 0 lifetime = ded
		return (p->get_lifetime() == 0) || !isInBounds(p);
	});

	return 1;
} // physics() iterates all P.

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
	return std::count_if(ps.begin(), ps.end(), [](const P_ptr &p) {
		const Amt pl = p->get_lifetime();
		return pl > 0;
	});
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
void World::save(const std::string &str) {
	std::ofstream ofs(str);
	// std::ofstream ofs("save2.JSON");
	//  std::ofstream ofs(SAVEFILE);
	//  Basic mem vars
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
	return s.substr(s.find_first_of(':') + 2,
					s.find_first_of(',') - s.find_first_of(':') - 1);
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
	// If type == 0 this BREAKS, so we need to be very sure no type = 0 exists
	// in a save
	if (type == 1)
		p = std::make_shared<Air>(row, col);
	if (type == 2)
		p = std::make_shared<Dust>(row, col);
	if (type == 3)
		p = std::make_shared<Fire>(row, col);
	if (type == 4)
		p = std::make_shared<Water>(row, col);
	if (type == 5)
		p = std::make_shared<Earth>(row, col);
	if (type == 6)
		p = std::make_shared<Dirt>(row, col);
	if (type == 7)
		p = std::make_shared<Lightning>(row, col);

	p->set_x_vel(stof(Pvals.at(2)));
	p->set_y_vel(stof(Pvals.at(3)));
	// TODO: stretch goal -> custom particles use these V but default particles
	// use default cstor vals.
	//  p->set_r(stoi(Pvals.at(4)));
	//  p->set_g(stoi(Pvals.at(5)));
	//  p->set_b(stoi(Pvals.at(6)));
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
		// updateMap(p);
		std::getline(ss, particle,
					 ','); // Throw out comma inbetween each particle.
		std::getline(ss, particle, '}');
	}
}

int World::load(const std::string &str) {
	std::ifstream ifs(str);
	// std::ifstream ifs(SAVEFILE);
	if (!ifs) {
		return -1; // reported error file not found
	}
	// if file successfully opens, clear out the old data.
	ps.clear();
	map.clear();
	std::string s;
	// Throw out the first linw, as ut is always just a open curly brace.
	// This JSON parser will not be portable in the slightest, it will just work
	// for this case.
	std::getline(ifs, s, '\n');
	// Member primitives
	// rows
	std::getline(ifs, s, '\n');
	//LOAD THROWS OUT ROWS, COLS. THESE ARE DETERMINED AT RUNTIME BY GET_TERMINAL_SIZE.
	// try {
		// rows = stoi(extractVal(s));
	// } catch (std::exception &e) {
		// std::cerr << "Val extracted for ROWS invalid.\n";
		// exit(EXIT_FAILURE);
	// }
	// cols
	std::getline(ifs, s, '\n');
	// try {
		// cols = stoi(extractVal(s));
	// } catch (std::exception &e) {
		// std::cerr << "Val extracted for COLS invalid.\n";
		// exit(EXIT_FAILURE);
	// }

	updateVecs();

	std::getline(ifs, s, '\n'); // "Ps": [ line
	std::getline(ifs, s, ']');	// Get entire json array of particles, if any.
	if (s.length() > 1) {		// == 1 means it is empty
		parseParticlesFromJSON(s);
	}
	ifs.close();
	return 0; // no error
}
