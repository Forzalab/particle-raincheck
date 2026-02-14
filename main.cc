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
	World w(300,400);
	Air p1(10,10), p2(20,20);
	Dust p3(30,30);

	w.add_particle(std::make_shared<Air>(p1));
	w.add_particle(std::make_shared<Air>(p2));
	w.add_particle(std::make_shared<Dust>(p3));
	P_ptr p4 = w.at(30,30);
//	std::cout << p4->get_type();
	p4->set_row(40); p4->set_col(40);
	p4->set_lifetime(1000);

	std::cout << p4->get_row() << " " << p4->get_col() << std::endl;
/*	w.add_particle(p4);
	P_ptr p5 = w.at(40,40);
	std::cout << p5->get_row() << " " << p5->get_col() << std::endl;*/
	while(true) {
		p4->physics(w);
		std::cout << p4->get_row() << " " << p4->get_col();
		std::cin.get();
	}

}
