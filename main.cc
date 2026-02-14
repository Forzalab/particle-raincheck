#include "/public/read.h" // IWYU pragma: keep
#include "World.h"
#include <iostream>

int main() {
	//TEST CODE FOR World::Save() and World::Load()
	//As far as I can tell, these work. I cannot thoroughly test Load, as Particle is still unimplemented.
	// World w;
	// w.load();
	// Particle p1{};
	// Particle p2{};
	// Particle p3{};
	// Particle p4{};
	// w.add_particle(p1);
	// w.add_particle(p2);
	// w.add_particle(p3);
	// w.add_particle(p4);
	// w.save();
	World w(3,4);
	Air p1, p2;
	Dust p3;
	w.add_particle(std::make_shared<Air>(p1));
	w.add_particle(std::make_shared<Air>(p2));
	w.add_particle(std::make_shared<Dust>(p3));
	P_ptr p4 = w.at(0,2);
	std::cout << p4->get_type();
}
