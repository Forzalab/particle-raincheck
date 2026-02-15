#include "/public/read.h" // IWYU pragma: keep
#include "Game.h"
#include "World.h"
#include <iostream>

int main() {
	Game game;
	game.run();
}

/*
int main() {
	World w(300, 400);
	Air p1(10, 10), p2(20, 20);
	Dust p3(30, 30);

	w.add_particle(std::make_shared<Air>(p1));
	w.add_particle(std::make_shared<Air>(p2));
	w.add_particle(std::make_shared<Dust>(p3));

	P_ptr p4 = w.at(30, 30);

	p4->set_row(40);
	p4->set_col(40);
	p4->set_lifetime(1000);

	std::cout << p4->get_row() << " " << p4->get_col() << std::endl;

	w.add_particle(p4);
	P_ptr p5 = w.at(40, 40);
	std::cout << p5->get_row() << " " << p5->get_col() << std::endl;
		while(true) {
			p4->physics(w);
			std::cout << p4->get_row() << " " << p4->get_col();
			std::cin.get();
		}
	

	p4->touch(std::make_shared<Air>(p2), w);
	p4->touch(p5, w);
	Fire p6(200, 300);
	Water p7(200, 301);
	w.add_particle(std::make_shared<Fire>(p6));
	w.add_particle(std::make_shared<Water>(p7));
	P_ptr pf = w.at(200, 300);

 
       // insidiouds pattern: ptr cpy (not to be
        // confused with shallow cpy or deep cpy!)
	// TRY IT: remove the apostrophe to see
	// what happens.
	P_ptr &pw = w.at(200, 301);

	std::cout << "pf type is " << pf->get_type() << std::endl;
	std::cout << "pw type is " << pw->get_type() << std::endl;

	pf->touch(pw, w);

	std::cout << "pf type is " << pf->get_type() << std::endl;
	std::cout << "pw type is " << pw->get_type() << std::endl;
	
	// Why? shared_ptr is many ptrs to a same obj.
	// Ptr of p4, although a ptr cpy, points to
	// SAME obj of p4. All its copies will, actually.

	// BUT the ptr itself r "different/not references"
	// Internally, touch() modifies ptrs, not obj!
	// It will happily modify a COPY (similar bug
	// to what happens btw pass-by-val & pass-by-ref)

	// So what? ALWAYS PASS A PTR REFERENCE TO TOUCH.
	// Again....
	// ALWAYS PASS A PTR REFERENCE TO TOUCH.
	// The obj of .touch() (aka lhs particle) can stay
	// a cpy of ptr, as the ptr points to obj no matter
	// which copy version it is.
}*/
