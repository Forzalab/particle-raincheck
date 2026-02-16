#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>
#include "Game.h"
#include "/public/colors.h"

//Declaring here, definition is above render()
void unrender(auto &prevPs);

void printFPS(const auto &lastFrameStart, Wc rows) {
	auto diff = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - lastFrameStart);
	movecursor(rows + 5, 0);
	if(diff.count() > 0.05) std::cout << 1.0 / diff.count() << " / 60FPS"; 
}

void resetTerminal() {
	resetcolor();
	clearscreen();
	show_cursor(true);
	movecursor(0,0);
}

void Game::run() {
	show_cursor(false);
	clearscreen();
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
	auto prev_frame = clock::now();
	int count = 0;
	std::vector<pair<Wc, Wc>> prevPs;
	while(count < 100) {
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		printFPS(prev_frame, world.get_rows());
		prev_frame = next_frame;
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		count++;
		world.physics();
		unrender(prevPs);
		render();
		for(const auto &p : world.getParticles()) {
			prevPs.push_back(pair<Wc, Wc>(std::floor(p->get_row()), std::floor(p->get_col())));
		}
		std::this_thread::sleep_until(next_frame);
	}
	resetTerminal();
}

//Janky way to clear screen without iterating over every pixel or flickering the screen with clearscreen();
void unrender(auto &prevPs) {
	for(const auto &p : prevPs) {
		movecursor(p.first, p.second);
		std::cout << " ";
	}
	prevPs.clear();
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
