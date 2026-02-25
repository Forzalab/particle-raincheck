#include "Particle.h"
#include "World.h"

using P = Particle;

std::random_device P::rd;
std::mt19937 P::gen(P::rd());
std::binomial_distribution<> P::bd(50, 0.5);

// getters
Pc P::get_row() const { return row; }
Pc P::get_col() const { return col; }
Pc P::get_x_vel() const { return x_vel; }
Pc P::get_y_vel() const { return y_vel; }
Color P::get_r() const { return r; }
Color P::get_g() const { return g; }
Color P::get_b() const { return b; }
bool P::get_stationary() const { return stationary; }
P_Type P::get_type() const { return type; }
Tick P::get_lifetime() const { return lifetime; }
bool P::get_solid() const { return false; }
bool P_solid::get_solid() const { return true; }
bool P::is_equal(const Pc &lhs, const Pc &rhs) {
	return std::abs(lhs - rhs) < 1E-6;
}
bool P::is_solid(const P_Type &pt) { return (pt == earth || pt == dirt); }

// setters
void P::set_row(const Pc &_row) {
	if (_row < 0)
		throw std::runtime_error("Row OOB.");
	row = _row;
}

void P::set_col(const Pc &_col) {
	if (_col < 0)
		throw std::runtime_error("Col OOB.");
	col = _col;
}

void P::set_x_vel(const Pc &_x_vel) { x_vel = _x_vel; }
void P::set_y_vel(const Pc &_y_vel) { y_vel = _y_vel; }
void P::set_r(const Color &_r) { r = _r; }
void P::set_g(const Color &_g) { g = _g; }
void P::set_b(const Color &_b) { b = _b; }
void P::set_stationary(const bool &_stationary) {
	if (_stationary == true) {
		set_x_vel(0);
		set_y_vel(0);
		set_lifetime(-1);
	}
	stationary = _stationary;
}

void P::set_lifetime(const Tick &_lifetime) {
	if (_lifetime < -1)
		throw std::runtime_error("Lifetime OOB.");
	lifetime = _lifetime;
}

void P::set_type(const P_Type &_type) {
	if (!((E_MIN <= _type) && (_type <= E_MAX)))
		throw std::runtime_error("P_Type OOB.");
	type = _type;
}

void P::physics(World &world) {
	// general guard for all particles
	/* here */

	// type-specific physics
	physics_spec(world);

	// DO NOT INPLMENT ANYTHING IN THIS SPACE
	return;
}

// TODO: add constructor WITH MEM-INIT-LIST
//       (r,g,b, stationary, lifetime) to definitions
void Air::physics_spec(World &world) {

	// if not (not st.) = stay still
	// ==> change nothing about pos
	if (!get_stationary()) {
		set_row(get_row() + get_y_vel());
		set_col(get_col() + get_x_vel());
	}

	return;
}

void Air::touch(const P_ptr &nbr, World &world) {}

void Dust::physics_spec(World &world) {
	Pc gravity = 0.00025;

	Pc rand_sign = (((P::bd(P::gen)) >= 26) ? -1 : 1);
	Pc dx_scale = 0.125 * rand_sign * (((P::bd(P::gen)) >= 25) ? -1 : 1);
	Pc dy_scale = gravity * (((P::bd(P::gen)) > 27) ? -1 : 1);

	set_x_vel([&]() { return dx_scale; }());
	set_y_vel(get_y_vel() + dy_scale);

	if (!get_stationary()) {
		set_row(get_row() + get_y_vel());
		set_col(get_col() + get_x_vel());
	}

	return;
}

void Dust::touch(const P_ptr &nbr, World &world) {}
void Fire::physics_spec(World &world) {
	// Fire is stationary by default
	Pc x = int(this->get_col());
	Pc y = int(this->get_row());
	Pc speed_scale = 0.2;

	// Lighting spawn
	bool light = (P::bd(P::gen)) > 31; // P(X >= 33) = 1.6%
	Pc sign_x = (P::bd(P::gen)) % 3 - 1;
	Pc sign_y = (P::bd(P::gen)) % 3 - 1;
	Pc x_spawn = ((P::bd(P::gen)) % 3) + x - 1;
	Pc y_spawn = ((P::bd(P::gen)) % 3) + y - 1;
	Pc dx_spawn = speed_scale * sign_x * (((P::bd(P::gen)) % 3 - 1));
	Pc dy_spawn = speed_scale * sign_y * (((P::bd(P::gen)) % 3 - 1));

	// Chexks for currently gen x,y spawn
	// no overlap spawn
	bool overlap_itself = (Wc(x_spawn) == Wc(x) && Wc(y_spawn) == Wc(y));
	bool overlap_border = is_solid(world.atMap(y_spawn, x_spawn));

	// inbound check
	bool in_bound_x = (x_spawn) < world.get_cols() && (x_spawn) >= 0;
	bool in_bound_y = (y_spawn) < world.get_rows() && (y_spawn) >= 0;

	// speed check
	bool moving = (std::abs(dx_spawn/speed_scale) >= 1 || std::abs(dy_spawn/speed_scale) >= 1);

	// moving-to-fire check
	bool mtf = world.atMap((y_spawn + dy_spawn), (x_spawn + dx_spawn)) == fire;

	// assign the lighting particle
	if (light && in_bound_x && in_bound_y && !overlap_itself &&
		!overlap_border && moving && !mtf) {
		Lightning l(y_spawn, x_spawn);
		l.set_x_vel(dx_spawn);
		l.set_y_vel(dy_spawn);
		P_ptr p_l = std::make_shared<Lightning>(l);
		P_ptr &p_world = world.at(y_spawn, x_spawn);
		if (p_world)
			p_world = p_l;
		else
			world.add_particle(p_l);
		world.updateMap(p_l);
	}
}

void Fire::touch(const P_ptr &nbr, World &world) {
	if (nbr->get_type() == water) {
		// create new upwards Air
		Air a(nbr->get_row(), nbr->get_col());
		P_ptr p_a = std::make_shared<Air>(a);
		P_ptr &p_world = world.at(nbr->get_row(), nbr->get_col());
		if (p_world)
			p_world = p_a;
		else
			world.add_particle(p_a);

		world.updateMap(p_a);
		// make it go upwards
		// y starts at 0 and ends with world.height
		// so going up means decreasing y.
		nbr->set_y_vel(std::abs(get_y_vel()) * (-4));

		// delete water? it has been "replaced"
		std::cout << "Water touched\n";
	}
}

void Water::physics_spec(World &world) {
	// (Physics are the same as dirt for
	if (get_stationary()) {
		return;
	}
	Pc gravity = 0.1;
	set_x_vel(0); // no lateral movement in air
	// initial velocity + acceleration due to gravity
	set_y_vel(std::clamp(float(get_y_vel() + gravity), 0.0f, 1.0f));

	// if ((world.at(get_row() + 1, get_col())->get_type() == none) &&
	// 	world.at(get_row() + 1, get_col()) == nullptr) {
	// Above code errors. This handles OOB for you. The list that .at() searches
	// is for actual particle types. None is a placeholder type for in bounds
	// but no particle at location.
	if (!P::is_solid(world.atMap(get_row() + 1, get_col()))) {
		set_row(get_row() + get_y_vel());
		return;
	}
	// only if something is below it do we now trst where it can slide
	else {
		// if ((world.at(get_row() + 1, get_col() - 1)->get_type() == none) &&
		// 	world.at(get_row() + 1, get_col() - 1) == nullptr) {
		if (world.atMap(get_row() + 1, get_col() - 1) == none) {
			set_row(get_row() + 1);
			set_col(get_col() - 1);
			return;
			// } else if ((world.at(get_row() + 1, get_col() + 1)->get_type() ==
			// 			none) &&
			// 		   world.at(get_row() + 1, get_col() + 1) == nullptr) {
		} else if (world.atMap(get_row() + 1, get_col() + 1) == none) {
			set_row(get_row() + 1);
			set_col(get_col() + 1);
			return;
		}
		/*	else if (world.at(get_row(), get_col() - 1)->get_type() == none) {
				set_col(get_col() - 1);
				return;
			}
			else if (world.at(get_row(), get_col() + 1)->get_type() == none) {
				set_col(get_col() + 1);
				return;
			}
		*/
		else {
			set_stationary(true);
			return;
		}
	}
	return;
}

void Water::touch(const P_ptr &nbr, World &world) {
	// Every interaction between a certain particle type and water has already
	// been taken care of in the other particle's touch function to my knowledge
}

void Earth::physics_spec(World &world) {
	// nothing 2 impl.
	// Earth is stationary!
}

void Earth::touch(const P_ptr &nbr, World &world) {}

void Dirt::physics_spec(World &world) {

	if (get_stationary()) {
		return;
	}

	Pc gravity = 0.1;
	set_x_vel(0); // no lateral movement in air
	// initial velocity + acceleration due to gravity
	//	set_y_vel(get_y_vel() + gravity);
	set_y_vel(std::clamp(float(get_y_vel() + gravity), 0.0f, 1.0f));

	// init pos
	Wc y = get_row();
	Wc x = get_col();

	// if ((world.at(y + 1, x)->get_type() == none) &&
	// world.at(y + 1, x) == nullptr) {
	if (world.has_gap_at(y + 1, x)) {
		set_row(y + 1);
		set_col(x);
	} else if (world.has_gap_at(y + 1, x - 1)) {
		set_row(y + 1);
		set_col(x - 1);
		// } else if ((world.at(y + 1, x + 1)->get_type() ==
		// none) &&
		// world.at(y + 1, x + 1) == nullptr) {
	} else if (world.has_gap_at(y + 1, x + 1)) {
		set_row(y + 1);
		set_col(x + 1);
	} else {
		set_stationary(true);
		return;
	}
}

// primitive touch for the time being, can revisit if causes problems
void Dirt::touch(const P_ptr &nbr, World &world) {
	if (nbr->get_type() == water &&
		(nbr->get_row() == get_row() + 1 && nbr->get_col() == get_col())) {
		// if dirt on top of water, switch places
		set_row(get_row() + 1);
		nbr->set_row(get_row());
	}
}

void Lightning::physics_spec(World &world) {
	Pc x = this->get_col();
	Pc y = this->get_row();
	Pc dx = this->get_x_vel();
	Pc dy = this->get_y_vel();

	Pc x_dx = int(x + dx);
	Pc y_dy = int(y + dy);
	P_Type pt = world.atMap(y_dy, x_dx);

	if (pt == fire || is_solid(pt)) {
		set_lifetime(0);
		return;
	}

	if (!get_stationary()) {
		set_col(get_col() + dx);
		set_row(get_row() + dy);
	}
}

void Lightning::touch(const P_ptr &nbr, World &world) {
	if (nbr->get_type() == earth || nbr->get_type() == water) {
		P_ptr p;
		if (nbr->get_type() == earth) {
			Dirt d(nbr->get_row(), nbr->get_col());
			p = std::make_shared<Dirt>(d);

		} else if (nbr->get_type() == water) {
			Lightning l(nbr->get_row(), nbr->get_col());
			p = std::make_shared<Lightning>(l);
		}
		P_ptr &p_world = world.at(nbr->get_row(), nbr->get_col());
		if (p_world)
			p_world = p;
		else
			world.add_particle(p);
		world.updateMap(p);
		this->set_lifetime(0);
	}
}

void TBD_1::physics_spec(World &world) {}
void TBD_1::touch(const P_ptr &nbr, World &world) {}
void TBD_2::physics_spec(World &world) {}
void TBD_2::touch(const P_ptr &nbr, World &world) {}
void TBD_3::physics_spec(World &world) {}
void TBD_3::touch(const P_ptr &nbr, World &world) {}
