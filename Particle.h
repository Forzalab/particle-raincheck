#ifndef PARTICLE_H
#define PARTICLE_H

#include <random>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <random>
typedef float Pc; // P-coordinates
typedef uint8_t Color;
typedef int32_t Tick;

// type enum. 3 TBDs if feasible, 1 unknown, and as requirement stated.
// init enum defaults to `none` (0).
enum Type {
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
	E_MIN = none, // change as enum list changed
	E_MAX = tbd_3 // change as enum list changed
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
	Type type{};
	// https://stackoverflow.com/questions/7405740/how-can-i-initialize-base-class-member-variables-in-derived-class-constructor
protected:
	static std::random_device rd;
	static std::mt19937 gen;
	static std::binomial_distribution<> bd;
		
	Particle(const Color &r, const Color &g, const Color &b,
			 const bool &stationary, const Tick &lifetime, const Type &type)
		: r(r), g(g), b(b), stationary(stationary), lifetime(lifetime),
		  type(type) {
	}

public:
	void set_type(const Type &_type);
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

	Type get_type() const;

	bool get_stationary() const;
	void set_stationary(const bool &_stationary);

	Tick get_lifetime() const;
	void set_lifetime(const Tick &_lifetime);

	virtual void physics(World &world) final;

	// These functions below MUST BE IMPLEMENTED in derived classes.
	virtual void physics_spec(World &world) = 0;
	virtual void touch(
		Particle &
			nbr) = 0; // Rationale:
					  // https://stackoverflow.com/questions/3644065/how-to-write-an-elegant-collision-handling-mechanism
					  // Friend for full acess
					  //	friend Particle extractParticle(string s);
};

using P = Particle; // for lazy fuckers like us

class Air : public P {
public:
	// https://stackoverflow.com/questions/7405740/how-can-i-initialize-base-class-member-variables-in-derived-class-constructor
	Air() : Particle(255, 255, 255, false, INT32_MAX, air) {
		auto rnd = [&]() -> float {
			return P::bd(P::gen);
		};
		Pc dx_scale = 3, dy_scale = 3;
		set_x_vel(((int)(rnd() * 100) % 3) * dx_scale);
		set_y_vel(((int)(rnd() * 100) % 1 + 1) * dy_scale);
	}

	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Dust : public P {
public:
	Dust() : Particle(120, 120, 120, false, INT32_MAX, dust) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Fire : public P {
public:
	Fire() : Particle(227, 68, 32, false, INT32_MAX, fire) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Water : public P {
public:
	Water() : Particle(70, 155, 235, false, INT32_MAX, water) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Earth : public P {
public:
	Earth() : Particle(97, 29, 25, false, INT32_MAX, earth) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Dirt : public P {
public:
	Dirt() : Particle(138, 52, 26, false, INT32_MAX, dirt) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class Lightning : public P {
public:
	Lightning() : Particle(255, 255, 0, false, INT32_MAX, lightning) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class TBD_1 : public P {
public:
	TBD_1() : Particle(255, 255, 255, false, INT32_MAX, tbd_1) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class TBD_2 : public P {
public:
	TBD_2() : Particle(255, 255, 255, false, INT32_MAX, tbd_2) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

class TBD_3 : public P {
public:
	TBD_3() : Particle(255, 255, 255, false, INT32_MAX, tbd_3) {}
	virtual void physics_spec(World &world) final;
	virtual void touch(Particle &nbr) final;
};

#endif
