// put all #include in header file pls

#include "Particle.h"
#include "World.h"
#include <iostream>

using P = Particle;

std::random_device P::rd;
std::mt19937 P::gen(P::rd());
std::binomial_distribution<> P::bd(50, 0.5);

// cstor
P::Particle(const Pc &row, const Pc &col, const Color &r, const Color &g,
			const Color &b, const bool &stationary, const Tick &lifetime,
			const P_Type &type)
	: r(r), g(g), b(b), stationary(stationary), lifetime(lifetime), type(type) {
	set_row(row);
	set_col(col);
	set_stationary(stationary);
	set_lifetime(lifetime);
}

Air::Air(const Pc &row, const Pc &col)
	: Particle(row, col, 255, 255, 255, false, 1000, air) {
	Pc sign_x = (P::bd(P::gen) >= 25) ? 1 : -1;
	Pc sign_y = (P::bd(P::gen) >= 25) ? 1 : -1;
	Pc dx_scale = 0.1, dy_scale = 0.1, bounces = 0;
	set_x_vel(((P::bd(P::gen) * 100) % 3) * dx_scale * sign_x);
	set_y_vel(((P::bd(P::gen) * 100) % 2 + 1) * dy_scale * sign_y);
}

Dust::Dust(const Pc &row, const Pc &col)
	: P_solid(row, col, 120, 120, 120, false, 300, dust) {
	Pc dx_scale = 2, dy_scale = 2;
	set_x_vel(((P::bd(P::gen)) - 24) * dx_scale);
	set_y_vel(((P::bd(P::gen)) - 24) * dy_scale);
}

Fire::Fire(const Pc &row, const Pc &col)
	: Particle(row, col, 255, 32, 16, false, 1500, fire) {}

Water::Water(const Pc &row, const Pc &col)
	: Particle(row, col, 70, 155, 235, false, 20000, water) {}

Earth::Earth(const Pc &row, const Pc &col)
	: P_solid(row, col, 97, 29, 25, true, -1, earth) {}

Dirt::Dirt(const Pc &row, const Pc &col)
	: P_solid(row, col, 138, 52, 26, false, INT32_MAX, dirt) {}

Lightning::Lightning(const Pc &row, const Pc &col)
	: Particle(row, col, 220, 220, 0, false, 75, lightning) {
	int8_t sign_x = (P::bd(P::gen) >= 25) ? 1 : -1;
	int8_t sign_y = (P::bd(P::gen) >= 25) ? 1 : -1;

	Tick lft = 1 * P::bd(P::gen);

	Pc x_grav = ((P::bd(P::gen)) % 3 + 1) * sign_x * 0.1;
	Pc y_grav = ((P::bd(P::gen)) % 3 + 1) * sign_y * 0.1;

	set_x_vel(x_grav);
	set_y_vel(y_grav);
	set_lifetime(lft);
}

TBD_1::TBD_1(const Pc &row, const Pc &col)
	: P_solid(row, col, 255, 255, 255, false, INT32_MAX, tbd_1) {}

Confetti::Confetti(const Pc &row, const Pc &col)
	: P(row, col, 255, 255, 255, false, 100, confetti) {
	this->set_r((P::bd(P::gen)) * 100 % 256);
	this->set_g((P::bd(P::gen)) * 100 % 256);
	this->set_b((P::bd(P::gen)) * 100 % 256);

	// v0
	set_y_vel(-((P::bd(P::gen) * 2 / 75.0f))+0.2);
	set_x_vel((P::bd(P::gen) % 11 - 5) / 40.0);
}

TBD_3::TBD_3(const Pc &row, const Pc &col)
	: P_solid(row, col, 255, 255, 255, false, INT32_MAX, tbd_3) {}

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
		set_lifetime(0);
	// throw std::runtime_error("Row OOB.");
	row = _row;
}

void P::set_col(const Pc &_col) {
	if (_col < 0)
		set_lifetime(0);
	// throw std::runtime_error("Col OOB.");
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

	// Color fade
	if (lifetime <= 30) {
		r *= 0.88;
		g *= 0.88;
		b *= 0.88;
	}
}

void P::set_type(const P_Type &_type) {
	if (!((E_MIN <= _type) && (_type <= E_MAX)))
		throw std::runtime_error("P_Type OOB.");
	type = _type;
}

void P::physics(World &world) {
	// general guard for all particles

	Pc x_old = this->get_col();
	Pc y_old = this->get_row();

	// type-specific physics
	physics_spec(world);

	// After ONE particle move
	Pc x_new = this->get_col();
	Pc y_new = this->get_row();

	// ...but the new pos on Map (excluding generated
	// particle, we dont talk about Bruno) HAS the
	// old particle type!!!!!! wtf
	// Map wont be updated DURING this function scope.
	// y_new += (y_vel > 0 ? std::ceil(y_vel) : std::floor(y_vel));
	// x_new += (x_vel > 0 ? std::ceil(x_vel) : std::floor(x_vel));

	P_Type colliding_type = world.atMap(y_new, x_new);
	P_ptr &p = world.atMap_ptr(y_new, x_new);
	// P_ptr &p = world.at(y_new, x_new);

	// No-one? sprint ahead!
	// Has one? ask first.
	// Then, generated p updates the prev map automatically.
	// Axiom: last P in the list gets to APPEAR!
	if (p && colliding_type != none && colliding_type != air &&
		colliding_type != type) {
		if (this->type == lightning)
			std::cerr << std::to_string(int(colliding_type));
		this->touch(p, world);
	}

	// DO NOT INPLMENT ANYTHING IN THIS SPACE
	return;
}

void Air::physics_spec(World &world) {
	Pc x = get_col(), y = get_row();
	// if there is nothing solid in front of the moving particle
	if (!P::is_solid(world.atMap(get_y_vel() + y, get_x_vel() + x))) {
		set_row(get_row() + get_y_vel());
		set_col(get_col() + get_x_vel());
	} else {
		// Flips the velocity sign
		if (P::is_solid(world.atMap(y + 1, x)) and
			P::is_solid(world.atMap(y - 1, x)) and
			P::is_solid(world.atMap(y, x + 1)) and
			P::is_solid(world.atMap(y, x - 1))) {
			set_y_vel(0);
			set_x_vel(0);
		} else if (P::is_solid(world.atMap(y + 1, x)) and
				   P::is_solid(world.atMap(y - 1, x))) {
			set_y_vel(0);
		} else if (P::is_solid(world.atMap(y, x + 1)) and
				   P::is_solid(world.atMap(y, x - 1))) {
			set_x_vel(0);
		}
		/*
		//Top Right Corner
		if (P::is_solid(world.atMap(y + 1, x + 1)) and P::is_solid(world.atMap(y
		+ 1, x)) and P::is_solid(world.atMap(y, x + 1))) {
			set_y_vel(-(get_y_vel()));
			set_x_vel(-(get_x_vel()));
		}
		//Bottom Right Corner
		else if (P::is_solid(world.atMap(y - 1, x + 1)) and
		P::is_solid(world.atMap(y - 1, x)) and P::is_solid(world.atMap(y, x +
		1))) { set_y_vel(-(get_y_vel())); set_x_vel(-(get_x_vel()));
		}
		//Top Left Corner
		else if (P::is_solid(world.atMap(y + 1, x - 1)) and
		P::is_solid(world.atMap(y + 1, x)) and P::is_solid(world.atMap(y, x -
		1))) { set_y_vel(-(get_y_vel())); set_x_vel(-(get_x_vel()));
		}
		//Bottom Left Corner
		else if (P::is_solid(world.atMap(y - 1, x - 1)) and
		P::is_solid(world.atMap(y - 1, x)) and P::is_solid(world.atMap(y, x -
		1))) { set_y_vel(-(get_y_vel())); set_x_vel(-(get_x_vel()));
		}
		else if (P::is_solid(world.atMap(y + 1, x)) or P::is_solid(world.atMap(y
		- 1, x))) { set_y_vel(-(get_y_vel()));
		}
		else if (P::is_solid(world.atMap(y, x + 1)) or
		P::is_solid(world.atMap(y, x - 1))) { set_x_vel(-(get_x_vel()));
		}
		*/
		if (P::is_solid(world.atMap(y + 1, x)))
			set_y_vel(-(get_y_vel()));
		if (P::is_solid(world.atMap(y - 1, x)))
			set_y_vel(-(get_y_vel()));
		if (P::is_solid(world.atMap(y, x + 1)))
			set_x_vel(-(get_x_vel()));
		if (P::is_solid(world.atMap(y, x - 1)))
			set_x_vel(-(get_x_vel()));

		// Moves the particle
		set_row(y + get_y_vel());
		set_col(x + get_x_vel());
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

	// color changes for fire
	short clr = P::bd(P::gen);
	if (clr < 15) {
		// red
		this->set_r(190);
		this->set_g(10);
		this->set_b(0);
	} else if (clr < 19) {
		// red
		this->set_r(200);
		this->set_g(20);
		this->set_b(0);
	} else if (clr < 25) {
		// red
		this->set_r(220);
		this->set_g(40);
		this->set_b(0);
	} else {
		// orange
		this->set_r(230);
		this->set_g(60);
		this->set_b(0);
	}

	// Lighting spawn
	bool light = (P::bd(P::gen)) > 30; // P(X >= 33) = 1.6%
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
	bool moving = (std::abs(dx_spawn / speed_scale) >= 1 ||
				   std::abs(dy_spawn / speed_scale) >= 1);

	// moving-to-fire check
	bool mtf = world.atMap((y_spawn + dy_spawn), (x_spawn + dx_spawn)) == fire;

	// assign the lighting particle
	if (light && in_bound_x && in_bound_y && !overlap_itself &&
		!overlap_border && moving && !mtf) {
		Lightning l(y_spawn, x_spawn);
		l.set_x_vel(dx_spawn);
		l.set_y_vel(dy_spawn);
		P_ptr p_l = std::make_shared<Lightning>(l);
		world.add_particle(p_l);
		world.updateMap(p_l);
	}
}

void Fire::touch(const P_ptr &nbr, World &world) {
	if (nbr->get_type() == water) {
		// create new upwards Air
		Air a(nbr->get_row(), nbr->get_col());
		P_ptr p_a = std::make_shared<Air>(a);
		world.add_particle(p_a);
		world.updateMap(p_a);

		world.updateMap(p_a);
		// make it go upwards
		// y starts at 0 and ends with world.height
		// so going up means decreasing y.
		nbr->set_y_vel(std::abs(get_y_vel()) * (-4));
		nbr->set_lifetime(0);

		// delete water? it has been "replaced"
		std::cout << "Water touched\n";
	}
}

void Water::physics_spec(World &world) {
	// (Physics are the same as dirt for
	if (get_stationary()) {
		return;
	}
	// gravity accumulation (new system)
	float new_vel = get_y_vel() + 0.1f;
	int steps = static_cast<int>(new_vel);
	set_y_vel(new_vel - steps);
	set_x_vel(0); // no lateral movement in air

	if (steps == 0)
		return;
	int r = get_row();
	int c = get_col();
	// initial velocity + acceleration due to gravity
	//	set_y_vel(std::clamp(float(get_y_vel() + gravity), 0.0f, 1.0f));

	// if ((world.at(get_row() + 1, get_col())->get_type() == none) &&
	// 	world.at(get_row() + 1, get_col()) == nullptr) {
	// Above code errors. This handles OOB for you. The list that .at() searches
	// is for actual particle types. None is a placeholder type for in bounds
	// but no particle at location.
	if (!P::is_solid(world.atMap(r + 1, c))) {
		set_row(r + 1);
		return;
	}
	// only if something is below it do we now trst where it can slide
	else {
		// if ((world.at(get_row() + 1, get_col() - 1)->get_type() == none) &&
		// 	world.at(get_row() + 1, get_col() - 1) == nullptr) {
		if (world.atMap(r + 1, c - 1) == none) {
			set_row(r + 1);
			set_col(c - 1);
			return;
			// } else if ((world.at(get_row() + 1, get_col() + 1)->get_type() ==
			// 			none) &&
			// 		   world.at(get_row() + 1, get_col() + 1) == nullptr) {
		} else if (world.atMap(r + 1, c + 1) == none) {
			set_row(r + 1);
			set_col(c + 1);
			return;
		}
		// if it can move left and there is something under the new potential
		// position
		else if (world.atMap(r, c - 1) == none &&
				 P::is_solid(world.atMap(r + 1, c - 1))) {
			set_col(c - 1);
			return;
		} else if (world.atMap(r, c + 1) == none &&
				   P::is_solid(world.atMap(r + 1, c + 1))) {
			set_col(c + 1);
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
	if (nbr->get_type() == fire) {
		set_lifetime(0);
	}
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

	// gravity accumulation (new system)
	float new_vel = get_y_vel() + 0.1f;
	int steps = static_cast<int>(new_vel);
	set_y_vel(new_vel - steps);
	set_x_vel(0); // no lateral movement in air

	if (steps == 0)
		return;
	int r = get_row();
	int c = get_col();
	//	set_y_vel(get_y_vel() + gravity);
	//	set_y_vel(std::clamp(float(get_y_vel() + gravity), 0.0f, 1.0f));

	// if ((world.at(get_row() + 1, get_col())->get_type() == none) &&
	// world.at(get_row() + 1, get_col()) == nullptr) {
	// checks below
	if (!P::is_solid(world.atMap(r + 1, c))) {
		set_row(r + 1);
		return;
	}
	// if ((world.at(get_row() + 1, get_col() - 1)->get_type() == none) &&
	// world.at(get_row() + 1, get_col() - 1) == nullptr) {
	// checks below and to the left
	if (!P::is_solid(world.atMap(r + 1, c - 1))) {
		set_row(r + 1);
		set_col(c - 1);
		return;
		// } else if ((world.at(get_row() + 1, get_col() + 1)->get_type() ==
		// none) &&
		// world.at(get_row() + 1, get_col() + 1) == nullptr) {
	}
	// checks below and to the right
	if (!P::is_solid(world.atMap(r + 1, c + 1))) {
		set_row(r + 1);
		set_col(c + 1);
		return;
	}
	// else it cant move
	set_stationary(true);
}

// primitive touch for the time being, can revisit if causes problems
void Dirt::touch(const P_ptr &nbr, World &world) {
	P_ptr p;
	if (nbr->get_type() == water) {
		Dirt d(nbr->get_row(), nbr->get_col());
		p = std::make_shared<Dirt>(d);
		this->set_lifetime(0);
		nbr->set_lifetime(0);
		world.add_particle(p);
		world.updateMap(p);
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

	if (pt == fire || is_solid(pt))
		set_lifetime(1); // leave some tine for solid detection

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

		// A cautionary tale:
		// Once upon a time, a ptr 'p' is born
		// then it was added to his crib
		// but the roon to that crib has a door
		// the door, opened for everyone, is named 'nbr'
		// set_lifetime(0) is a killer:
		// it kills babies on sight
		// door is open, so sadly 'p' was mutilated
		// Life lesson: let the killer mutilates his parents
		// b4 bringing the child into a crib.

		// Particle "delete"
		// the order is improtant, read the fairy tale above
		this->set_lifetime(0);
		nbr->set_lifetime(0);

		world.add_particle(p);
		world.updateMap(p);
	}
}

void TBD_1::physics_spec(World &world) {}
void TBD_1::touch(const P_ptr &nbr, World &world) {}
void Confetti::physics_spec(World &world) {
	Pc x = this->get_col();
	Pc y = this->get_row();
	Pc dx = this->get_x_vel();
	Pc grav = 0.025;
	Pc dy = this->get_y_vel() + grav;
	this->set_y_vel(dy);

	Pc x_dx = (x + dx);
	Pc y_dy = (y + dy);
	P_Type pt = world.atMap(y_dy, x_dx);

	Tick lft = this->get_lifetime();

	if (pt == fire || is_solid(pt))
                set_lifetime(0);

	if (lft % 3) {
		this->set_r((P::bd(P::gen)) * 100 % 256);
		this->set_g((P::bd(P::gen)) * 100 % 256);
		this->set_b((P::bd(P::gen)) * 100 % 256);
	}

	// spawn
/*	if (lft % 60) {
		Dirt d(x, y);
		P_ptr p = std::make_shared<Dirt>(d);

		world.add_particle(p);
		world.updateMap(p);
	}
*/
	if (!get_stationary()) {
		set_col(x_dx);
		set_row(y_dy);
	}
}

void Confetti::touch(const P_ptr &nbr, World &world) {}

void TBD_3::physics_spec(World &world) {}
void TBD_3::touch(const P_ptr &nbr, World &world) {}
