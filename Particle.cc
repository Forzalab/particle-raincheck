
/*
the due date you need to have:
1. A Particle class. You must use 'proper' class design. Each
part
icle has:
--A position (.row, .col float) which indicates where it is on
 the
 screen
----No two Particles can be on the same location
--A velocity (.x_vel and .y_vel float) indicating how many cel
ls p
er tick it moves
--A .type which must be an enum which indicates if it is air,
dust
, fire, water, earth, dirt, lightning
----Each type has a different physics movement type:
------Air moves in a straight line (ignoring gravity) bouncing
 off
 solid
------Dust has a small amount of gravity and randomly moves le
ft a
nd right every frame
------Fire is stationary and shoots sparks of lightning in dif
fere
nt directions over time
------Water drips down and if it hits something solid it will
slid
e sideways to find the lowest level. Water touching fire turns
 int
o air moving upwards
------Earth is always stationary and solid
------Lightning travels in a straight line and stops when it h
its
something solid. If it touches water, the water turns into lig
htni
ng. If it hits earth it turns into dirt
------Dirt travels downwards and forms piles when it hits some
thin
g solid
------You can add your own types as well for extra credit
--Color (.r .g .b uint8_t) that should be set initially by its
 typ
e (fire should be red) but can change
--A boolean named .stationary that if true means it is solid a
nd d
oes not move/simulate physics
----So if you turn stationary true on water it will not move e
ven
if a spot opens up beneath it
----Stationary Particles can still be destroyed or transformed
 by
other Particles
----Stationary particles have a lifetime of -1 (see below)
--A .lifetime variable which holds how many frames more it wil
l ex
ist for
----Physics (see below) will decrement it each frame by 1 to a
 min
imum of 0.
*/

#include "Particle.h"

using P = Particle;

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

// setters
void P::set_row(const Pc& _row) { row = _row; }
void P::set_col(const Pc& _col) { col = _col; }
void P::set_x_vel(const Pc& _x_vel) { x_vel = _x_vel; }
void P::set_y_vel(const Pc& _y_vel) { y_vel = _y_vel; }
void P::set_type(const Type& _type) { type = ((E_MIN <= _type)&&(_type <= E_MAX)) ? _type : none;  }

// TODO: add constructor WITH MEM-INIT-LIST
//       (r,g,b, stationary, lifetime) to definitions

void Air::physics(World& world) {}
void Air::touch(P& nbr) {}
void Dust::physics(World& world) {}
void Dust::touch(P& nbr) {}
void Fire::physics(World& world) {}
void Fire::touch(P& nbr) {}
void Water::physics(World& world) {}
void Water::touch(P& nbr) {}
void Earth::physics(World& world) {}
void Earth::touch(P& nbr) {}
void Dirt::physics(World& world) {}
void Dirt::touch(P& nbr) {}
void Lightning::physics(World& world) {}
void Lightning::touch(P& nbr) {}
void TBD_1::physics(World& world) {}
void TBD_1::touch(P& nbr) {}
void TBD_2::physics(World& world) {}
void TBD_2::touch(P& nbr) {}
void TBD_3::physics(World& world) {}
void TBD_3::touch(P& nbr) {}

