#ifndef PARTICLE_H
#define PARTICLE_H

#include <random>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <random>
#include <iostream>
typedef float Pc; // P-coordinates
typedef uint8_t Color;
typedef int32_t Tick;

// type enum. 3 TBDs if feasible, 1 unknown, and as requirement stated.
// init enum defaults to `none` (0).
enum P_Type : int8_t {
	none,
	air,
	dust,
	fire,
	water,
	earth,
	dirt,
	lightning,
	tbd_1, // confetti?
	tbd_2,
	tbd_3,
	OOB, //Checking an out of bounds loc in atMap()
	E_MIN = none, // change as enum list changed
	E_MAX = OOB // change as enum list changed
};

class World; // stub for forward-dependency

// X leftright, Y updown, starting at top left.
class Particle {
	// req needs all var tobe priv!
private:
	// row is y, col is x !!!!
	Pc row{}, col{};		 // coords of P, as P(row, col)
	Pc x_vel{}, y_vel{};	 // velocity
	Color r{}, g{}, b{};	 // color RGB
	bool stationary = false; // self-explanatory
	Tick lifetime{};		 // choosing type int32_t is arbitrary
	P_Type type{};
	// https://stackoverflow.com/questions/7405740/how-can-i-initialize-base-class-member-variables-in-derived-class-constructor
protected:
	static std::random_device rd;
	static std::mt19937 gen;
	static std::binomial_distribution<> bd;

	Particle(const Color &r, const Color &g, const Color &b,
			 const bool &stationary, const Tick &lifetime, const P_Type &type)
		: r(r), g(g), b(b), stationary(stationary), lifetime(lifetime),
		  type(type) {}

public:
	void set_type(const P_Type &_type);
	void set_row(const Pc &_row);
	void set_col(const Pc &_col);
	void set_x_vel(const Pc &_x_vel);
	void set_y_vel(const Pc &_y_vel);

	// if have time, remove cmt & impl multiple particle color in one type
	// maybe fire? conffeti if have time. water waves...

	void set_r(const Color &_r);
	void set_g(const Color &_g);
	void set_b(const Color &_b);

	Pc get_row() const;
	Pc get_col() const;
	Pc get_x_vel() const;
	Pc get_y_vel() const;

	// No setters for color, stationary, lifetime
	// as they are PREdefined/characterized for
	// the corresponding type.
	// A particle changes by its type - touch() will
	// handle it using set_type() in "protected" scope
	Color get_r() const;
	Color get_g() const;
	Color get_b() const;

	P_Type get_type() const;

	bool get_stationary() const;
	void set_stationary(const bool &_stationary);

	Tick get_lifetime() const;
	void set_lifetime(const Tick &_lifetime);

	static bool is_equal(const Pc& lhs, const Pc& rhs);

	virtual bool get_solid() const;

	virtual void physics(World &world) final;

	// These functions below MUST BE IMPLEMENTED in derived classes.
protected:
	virtual void physics_spec(World &world) = 0;

public:
	virtual void touch(
		const std::shared_ptr<Particle> &nbr,
		World &
			world) = 0; // Rationale:
						// https://stackoverflow.com/questions/3644065/how-to-write-an-elegant-collision-handling-mechanism
						// Friend for full acess
						//	friend Particle extractParticle(string s);
};

using P = Particle; // for lazy fuckers like us
typedef std::shared_ptr<P> P_ptr;

// layer 0.5 inheritance
// P_solid: set both constructor AND base class to P_solid
// to mark it as solid
class P_solid : public P {
protected:
	using P::Particle; // re-use stuff from P, which gets passed down
public:
	bool get_solid() const override;
};

// layer 1 inheritance
class Air : public P {
public:
	// https://stackoverflow.com/questions/7405740/how-can-i-initialize-base-class-member-variables-in-derived-class-constructor
	Air(const Pc &row, const Pc &col)
		: Particle(255, 255, 255, false, 1000, air) {
		Pc dx_scale = 5, dy_scale = 5;
		set_x_vel(((P::bd(P::gen) * 100) % 3) * dx_scale);
		set_y_vel(((P::bd(P::gen) * 100) % 2 + 1) * dy_scale);
		set_row(row);
		set_col(col);
		set_lifetime(1000);
	}

	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

// Philosophical question:
// Is a pixel-sized dust a rock, a "tumbleweed", or air?
class Dust : public P_solid {
public:
	Dust(const Pc &row, const Pc &col)
		: P_solid(120, 120, 120, false, 20000, dust) {
		Pc dx_scale = 2;
		Pc dy_scale = 2;
		set_x_vel(((P::bd(P::gen)) - 24) * dx_scale);
		set_y_vel(((P::bd(P::gen)) - 24) * dy_scale);
		set_row(row);
		set_col(col);
	}

	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Fire : public P {
public:
	Fire(const Pc &row, const Pc &col) : Particle(227, 68, 32, true, 15, fire) {
		set_row(row);
		set_col(col);
	}

	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Water : public P {
public:
	Water(const Pc &row, const Pc &col)
		: Particle(70, 155, 235, false, -1, water) {
		set_row(row);
		set_col(col);
	}

	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Earth : public P_solid {
public:
	Earth(const Pc &row, const Pc &col)
		: P_solid(97, 29, 25, true, INT32_MAX, earth) {
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Dirt : public P_solid {
public:
	Dirt(const Pc &row, const Pc &col)
		: P_solid(138, 52, 26, false, INT32_MAX, dirt) {
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Lightning : public P {
public:
	Lightning(const Pc &row, const Pc &col)
		: Particle(255, 255, 0, false, 10, lightning) {
		Pc x_val = (P::bd(P::gen) % 11) / 10.0f;
		Pc y_val = (P::bd(P::gen) % 10 + 1) / 10.0f;

		// From what I understand x could be between -1 and 1
		//  and y could be -1 and 1 but not zero
		set_x_vel(((P::bd(P::gen) % 2 == 1) ? -1 : 1) * x_val);
		set_y_vel(((P::bd(P::gen) % 2 == 1) ? -1 : 1) * y_val);
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class TBD_1 : public P_solid {
public:
	TBD_1(const Pc &row, const Pc &col)
		: P_solid(255, 255, 255, false, INT32_MAX, tbd_1) {
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class TBD_2 : public P {
public:
	TBD_2(const Pc &row, const Pc &col)
		: Particle(255, 255, 255, false, INT32_MAX, tbd_2) {
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class TBD_3 : public P_solid {
public:
	TBD_3(const Pc &row, const Pc &col)
		: P_solid(255, 255, 255, false, INT32_MAX, tbd_3) {
		set_row(row);
		set_col(col);
	}
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

#endif
