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
Type P::get_type() const { return type; }
Tick P::get_lifetime() const { return lifetime; }
// float P::rnd() { return Particle::bd(Particle::gen); }

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
	stationary = _stationary;
	if (stationary == true) {
		set_x_vel(0);
		set_y_vel(0);
		set_lifetime(-1);
	}
}

void P::set_lifetime(const Tick &_lifetime) {
	if (_lifetime < -1)
		throw std::runtime_error("Lifetime OOB.");
	lifetime = _lifetime;
}

void P::set_type(const Type &_type) {
	if (!((E_MIN <= _type) && (_type <= E_MAX)))
		throw std::runtime_error("Type OOB.");
	type = _type;
}

void P::physics(World &world) {
	Wc wrow = (Wc)get_row(), wcol = (Wc)get_col();
	const P_ptr _p = world.at(wrow, wcol);

	if (_p != nullptr) {
		// general guard for all particles
		/* here */

		// lifetime expiry check
		if (get_lifetime() == 0) {
			world.erase(wrow, wcol);
			return;
		}

		// type-specific physics
		physics_spec(world);

		// post specific-physics
		set_lifetime(get_lifetime() - 1);
	}

	// DO NOT INPLMENT ANYTHING IN THIS SPACE
	return;
}

// TODO: add constructor WITH MEM-INIT-LIST
//       (r,g,b, stationary, lifetime) to definitions
void Air::physics_spec(World &world) {

	// if not (not st.) = stay still
	// ==> change nothing about pos
	if (!get_stationary()) {
		set_row(get_row() + (Wc)get_y_vel());
		set_col(get_col() + (Wc)get_x_vel());
	}

	return;
}

void Air::touch(P_ptr nbr) {}

void Dust::physics_spec(World &world) {
	Pc gravity = 0.2;

	Pc dx_scale = 20 * ((P::bd(P::gen)) > 27) ? -1 : 1;
	Pc dy_scale = 30 * ((P::bd(P::gen)) > 27) ? -1 : 1;

	set_x_vel(((P::bd(P::gen)) > 27) ? -1 : 1 * dx_scale);
	set_y_vel(((P::bd(P::gen)) > 27) ? -1 : 1 * dy_scale);

	if (!get_stationary()) {
		set_row(get_row() + get_y_vel());
		set_col(get_col() + get_x_vel());
	}

	return;
}

void Dust::touch(P_ptr nbr) {
	
}
void Fire::physics_spec(World &world) {}
void Fire::touch(P_ptr nbr) {}
void Water::physics_spec(World &world) {}
void Water::touch(P_ptr nbr) {}

void Earth::physics_spec(World &world) {
	// nothing 2 impl.
	// Earth is stationary!
}

void Earth::touch(P_ptr nbr) {}
void Dirt::physics_spec(World &world) {}
void Dirt::touch(P_ptr nbr) {}
void Lightning::physics_spec(World &world) {}
void Lightning::touch(P_ptr nbr) {}
void TBD_1::physics_spec(World &world) {}
void TBD_1::touch(P_ptr nbr) {}
void TBD_2::physics_spec(World &world) {}
void TBD_2::touch(P_ptr nbr) {}
void TBD_3::physics_spec(World &world) {}
void TBD_3::touch(P_ptr nbr) {}
