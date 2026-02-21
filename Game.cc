#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <thread>
#include "Game.h"
#include "/public/colors.h"

typedef uint32_t GameTick;

GameTick Game::get_tickrate() const { return tickrate; }

//Declaring here, definition is above render()
void unrender(auto &prevPs);

void printFPS(const auto &lastFrameStart, Wc rows) {
	auto diff = std::chrono::duration<double>(std::chrono::steady_clock::now() - lastFrameStart);
	movecursor(rows + 5, 0);
	std::cout << 1.0 / diff.count() <<" FPS" << ""; 
	for(int i = 0; i < 80; i++) { std::cout << " "; } //Clean up trailing chars from prev frame
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
	world.updateVecs();
	world.load();
	{
	//Draw a splash screen here.
		
		clearscreen();
		system("figlet =======");
		system("figlet Particles");
		system("figlet =======");
	

		//Options on splash screen- "Start, 
		//Load
		//Draw on bridges
		//Quit

		//Add a time delay for users to see splash screen before game starts
		sleep(2); //Pauses for two seconds
		clearscreen();
		
	}
	auto next_frame = clock::now();
	auto prev_frame = clock::now();
	std::vector<pair<Wc, Wc>> prevPs;
	while(frame < 3600) {
		printFPS(prev_frame, world.get_rows());
		prev_frame = clock::now();
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		frame += world.physics(); //Physics will always return 1, unless there are no particles.
		unrender(prevPs);
		render();
		for(const auto &p : world.getParticles()) {
			movecursor(0,0);
			prevPs.push_back(pair<Wc, Wc>(std::round(p->get_row()), std::round(p->get_col())));
		}
		std::this_thread::sleep_until(next_frame);
	}
	resetTerminal();
}

//Janky way to clear screen without iterating over every pixel or flickering the screen with clearscreen();
void unrender(auto &prevPs) {
	movecursor(0,0);
	setbgcolor(0,0,0);
	for(const auto &p : prevPs) {
		movecursor(p.first, p.second);
		std::cout << " ";
	}
	prevPs.clear();
	resetcolor();
}

void Game::render() {
	Ps particles = world.getParticles();
	for(const auto& p : particles) {
		Wc row = std::round(p->get_row());
		Wc col = std::round(p->get_col());
		if(col < 0 || col > world.get_cols() || row > world.get_rows() || row < 0) continue; // Do not print particles that are OOB and not yet culled by world::physics()
		movecursor(std::round(row), std::round(col));
		setbgcolor(p->get_r(), p->get_g(), p->get_b());
		std::cout << " ";
		resetcolor();
	}
}

void Game::incr_fps() {
	GameTick input = 0;
	cin >> input;
	if (!cin || input < 3 || input > 60) { //If input is bad
		cin.clear();
		GameTick s = 0; //New variable
		cin >> s; //Stores the new input into new variable
		cout << "BAD INPUT!\n";
		sleep(1); //Gives time to read message
	}
	else if (input + get_tickrate() < 3 || input + get_tickrate() > 60) { //If new tickrate is out of range
		cout << "INPUT OUT OF RANGE!!! (Keep FPS in between 3-60)\n";
		sleep(1); //Gives time to read message
	}
	else {
		tickrate = get_tickrate() + input;
	}
}
void Game::dcrs_fps() {
	GameTick input = 0;
	cin >> input;
	if (!cin || input < 3 || input > 60) { //If input is bad
		cin.clear();
		GameTick s = 0; //New variable
		cin >> s; //Stores the new input into new variable
		cout << "BAD INPUT!\n";
		sleep(1); //Gives time to read message
	}
	else if (input + get_tickrate() < 3 || input + get_tickrate() > 60) { //If new tickrate is out of range
		cout << "INPUT OUT OF RANGE!!! (Keep FPS in between 3-60)\n";
		sleep(1); //Gives time to read message
	}
	else {
		tickrate = get_tickrate() - input;
	}
}
