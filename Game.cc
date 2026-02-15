#include <algorithm>
#include <chrono>
#include <thread>
#include "Game.h"


void Game::run() {
	using clock = std::chrono::steady_clock;

	//Placeholder vals. We can change these later.
	Wc rows = 400, cols = 300;
	world.set_rows(rows);
	world.set_cols(cols);

	{
	//Draw a splash screen here.
	}
	auto next_frame = clock::now();
	int count = 0;
	while(count < 5) {
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		std::cout << tickrate << endl;
		count++;
		std::this_thread::sleep_until(next_frame);
	}
}
