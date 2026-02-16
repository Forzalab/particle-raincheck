#include <algorithm>
#include <chrono>
#include <thread>
#include "Game.h"
#include "/public/colors.h"

void Game::run() {
	using clock = std::chrono::steady_clock;

	//Placeholder vals. We can change these later.
	Wc rows = 400, cols = 300;
	world.set_rows(rows);
	world.set_cols(cols);

	world.load();
	{
	//Draw a splash screen here.
	}
	auto next_frame = clock::now();
	int count = 0;
	while(count < 5) {
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		count++;
		if(!world.alive_count()) continue;
		world.physics();
		// render();
		std::this_thread::sleep_until(next_frame);
	}
}

void Game::render() {
	Ps particles = world.getParticles();
	clearscreen();	
	for(const auto& p : particles) {
		setbgcolor(p->get_r(), p->get_g(), p->get_b());
		movecursor(p->get_row(), p->get_col());
		cout << " ";
		resetcolor();
	}
}
