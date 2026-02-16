#include <algorithm>
#include <chrono>
#include <thread>
#include "Game.h"
#include "/public/colors.h"

void Game::run() {
	using clock = std::chrono::steady_clock;

	//Placeholder vals. We can change these later.
	Wc rows = 50, cols = 70;
	world.set_rows(rows);
	world.set_cols(cols);

	world.load();
	{
	//Draw a splash screen here.
	}
	auto next_frame = clock::now();
	int count = 0;
	// while(count < 5) {
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		count++;
		// if(!world.alive_count()) continue;
		world.physics();
		render();
		std::this_thread::sleep_until(next_frame);
	// }
		resetcolor();
}

void Game::render() {
	clearscreen();
	Ps particles = world.getParticles();
	for(const auto& p : particles) {
		setbgcolor(p->get_r(), p->get_g(), p->get_b());
		std::cout << " ";
		movecursor(p->get_row(), p->get_col());
		// resetcolor();
	}
}
