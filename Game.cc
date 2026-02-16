#include <algorithm>
#include <chrono>
#include <cmath>
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
	while(count < 5) {
		show_cursor(false);
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		count++;
		clearscreen();
		if(!world.alive_count()) continue;
		world.physics();
		render();
		std::this_thread::sleep_until(next_frame);
	}
	resetcolor();
	clearscreen();
	show_cursor(true);
	movecursor(0,0);
}

void Game::render() {
	Ps particles = world.getParticles();
	for(const auto& p : particles) {
		movecursor(std::floor(p->get_row()), std::floor(p->get_col()));
		setbgcolor(p->get_r(), p->get_g(), p->get_b());
		std::cout << " ";
		resetcolor();
	}
}
