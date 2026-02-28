// for the love of god, PLEASE DONT WRITE FUNCTION AND CSTOR
// IMPLEMENTATIONS IN *.h files!!!! declarations is a-ok :))

#ifndef PARTICLE_H
#define PARTICLE_H

#include <algorithm>
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
enum P_Type : uint8_t {
	none,
	air,
	dust,
	fire,
	water,
	earth,
	dirt,
	lightning,
	life,
	confetti,
	tbd_3,
	OOB,		  // Checking an out of bounds loc in atMap()
	E_MIN = none, // change as enum list changed
	E_MAX = OOB	  // change as enum list changed
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

	Particle(const Pc &row, const Pc &col, const Color &r, const Color &g,
			 const Color &b, const bool &stationary, const Tick &lifetime,
			 const P_Type &type);

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

	static bool is_equal(const Pc &lhs, const Pc &rhs);
	static bool is_solid(const P_Type &pt);
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
	Air(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

// Philosophical question:
// Is a pixel-sized dust a rock, a "tumbleweed", or air?
class Dust : public P_solid {
public:
	Dust(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Fire : public P {
public:
	Fire(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Water : public P {
public:
	Water(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Earth : public P_solid {
public:
	Earth(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Dirt : public P_solid {
public:
	Dirt(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Lightning : public P {
public:
	Lightning(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Life : public P_solid {
private:
	int mutate = 0;
public:
	Life(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class Confetti : public P {
public:
	Confetti(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

class TBD_3 : public P_solid {
public:
	TBD_3(const Pc &row, const Pc &col);
	void physics_spec(World &world) final;
	void touch(const P_ptr &nbr, World &world) final;
};

#endif
