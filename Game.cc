#include "Game.h"
#include <cctype>
#include <cstdlib>
#include <functional>
#include <string>

typedef uint32_t GameTick;

GameTick Game::get_tickrate() const { return tickrate; }

// Declaring here, definition is above render()
std::string unrender(auto &prevPs);

std::string printFPS(const auto &lastFrameStart, Wc rows) {
	std::string s;
	auto diff = std::chrono::duration<double>(std::chrono::steady_clock::now() -
											  lastFrameStart);
	s += movecursor(rows + 5, 0);
	s += std::to_string( std::round(1000.0 / diff.count()) / 1000.0 );
	s += " FPS";
	s += "";
	for (int i = 0; i < 80; i++) {
		s += " ";
	} // Clean up trailing chars from prev frame
	return s;
}

void resetTerminal() {
	std::string s;
	s += resetcolor();
	s += clearscreen();
	s += show_cursor(true);
	s += movecursor(0, 0);
	std::cerr << s;
}

void Game::run() {
	std::string fs;
	fs += show_cursor(false);
	fs += clearscreen();
	std::cerr << fs;
	fs.clear();
	using clock = std::chrono::steady_clock;

	// Placeholder vals. We can change these later.
	Wc rows = 50, cols = 70;
	world.set_rows(rows);
	world.set_cols(cols);
	world.updateVecs();
	world.load("save.JSON");
	{
		// Draw a splash screen here.

		clearscreen();
		system("figlet =======");
		system("figlet Particles");
		system("figlet =======");

		// Options on splash screen- "Start,
		// Load
		// Draw on bridges
		// Quit

		system("figlet =======");
		system("figlet Start");
		system("figlet =======");

		system("figlet =======");
		system("figlet Load");
		system("figlet =======");

		system("figlet =======");
		system("figlet Quit");
		system("figlet =======");

		// Add a time delay for users to see splash screen before game starts
		sleep(2); // Pauses for two seconds
		std::cerr << clearscreen();
	}
	// load(); //Test
	// save(); //Test
	// incr_fps(); //Test
	// dcrs_fps(); //Test

	// start of non-blocking I/O
	set_raw_mode(true);
	/*	while (true) {
			int c = toupper(quick_read());
			if (c == 'L') load();
			if (c == 'S') save();
			if (c == '=') incr_fps();
			if (c == '-') dcrs_fps();
			if (c == 'D') draw();
			if (c == 'P') {
				pause();
				// WHOLE OTHER OPTIONS
			}
		}
	*/
	auto next_frame = clock::now();
	auto prev_frame = clock::now();
	std::vector<pair<Wc, Wc>> prevPs;
	bool paused = false;
	//Stores which P_Type user selects, is none by default. Also contains a member function which will be set as the callback for on mouse down, adding that particle type at the mouse loc
	CallbackHandler ch{world};
	//Creates a callable function wrapper of type function<void(int, int)> with two placeholder params that will be supplied by mousedown events
	auto boundFunc = bind(&CallbackHandler::setRowCol, &ch, placeholders::_1, placeholders::_2);	
	//Sets the function that happens on mouse click down to the created function wrapper
	on_mousedown(boundFunc);
	
	while (frame < 3600) {
		int c = toupper(quick_read());

		if(c == 'P') {
			paused = true;
			std::cerr << set_mouse_mode(true);
		}
		else if (c == 'S') {
			paused = false;
			std::cerr << set_mouse_mode(false);
		}
		//- '0' gets 0-9 in integer form
		// +1 to map to 1(air)-10(TBD3)
		else if(c <= '9' && c >= '0') {
			ch.setPType(static_cast<P_Type>(c - '0' + 1)); 
		}
		fs += printFPS(prev_frame, world.get_rows());
		prev_frame = clock::now();
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		if(!paused) frame += world.physics(); // Physics will always return 1, unless there
											  // are no particles.
		else {
			switch(c) {
				case 'Q':
					resetTerminal();
					return;
				case 'A': //save
					save();
					break;
				case 'L':
					load();
					break;
			}
		}
		fs += unrender(prevPs);
		fs += render();
		std::cerr << fs;
		fs.clear();
		for (const auto &p : world.getParticles()) {
			prevPs.push_back(pair<Wc, Wc>(std::floor(p->get_row()),
										  std::floor(p->get_col())));
		}
		std::this_thread::sleep_until(next_frame);
	}	
	resetTerminal();
}

// Janky way to clear screen without iterating over every pixel or flickering
// the screen with clearscreen();
std::string unrender(auto &prevPs) {
	std::string s;
	s += movecursor(0, 0);
	s += setbgcolor(0, 0, 0); // std::cerr << s;
	for (const auto &p : prevPs) {
		s += movecursor(p.first, p.second) + " ";
	}
	prevPs.clear();
	s += resetcolor();
	return s;
}

std::string Game::render() {
	std::string s;
	Ps particles = world.getParticles();
	s += movecursor(0,0);
	s += resetcolor();
	for (const auto &p : particles) {
		Wc row = std::round(p->get_row());
		Wc col = std::round(p->get_col());
		if (col < 0 || col > world.get_cols() || row > world.get_rows() ||
			row < 0)
			continue; // Do not print particles that are OOB and not yet culled
					  // by world::physics()
		s += movecursor(std::round(row), std::round(col));
		s += setbgcolor(p->get_r(), p->get_g(), p->get_b());
		s += " ";
		s += resetcolor();
	}
	return s;
}

void Game::incr_fps() {
	GameTick input = 0;
	cin >> input;
	std::cerr << clearscreen();
	if (!cin || input < 3 || input > 60) { // If input is bad
		cin.clear();
		GameTick s = 0; // New variable
		cin >> s;		// Stores the new input into new variable
		cout << "BAD INPUT!\n";
		sleep(1); // Gives time to read message
	} else if (input + get_tickrate() < 3 ||
			   input + get_tickrate() > 60) { // If new tickrate is out of range
		cout << "INPUT OUT OF RANGE!!! (Keep FPS in between 3-60)\n";
		sleep(1); // Gives time to read message
	} else {
		tickrate = get_tickrate() + input;
	}
}
void Game::dcrs_fps() {
	GameTick input = 0;
	cin >> input;
	std::cerr << clearscreen();
	if (!cin || input < 3 || input > 60) { // If input is bad
		cin.clear();
		GameTick s = 0; // New variable
		cin >> s;		// Stores the new input into new variable
		cout << "BAD INPUT!\n";
		sleep(1); // Gives time to read message
	} else if (input + get_tickrate() < 3 ||
			   input + get_tickrate() > 60) { // If new tickrate is out of range
		cout << "INPUT OUT OF RANGE!!! (Keep FPS in between 3-60)\n";
		sleep(1); // Gives time to read message
	} else {
		tickrate = get_tickrate() - input;
	}
}

void Game::load() {
	cout << "Please enter name of file without the file extension: ";
	std::string filename;
	cin >> filename;
	filename += ".JSON";
	cout << filename << endl; // Test
	world.load(filename);
}

void Game::save() {
	cout << "Please enter name of file without the file extension: ";
	std::string filename;
	cin >> filename;
	filename += ".JSON";
	cout << filename << endl; // Test
	world.save(filename);
}

P_ptr CallbackHandler::generateParticle() {
	P_ptr pt;			
	switch(type) {
		case air:
			pt = make_shared<Air>(row, col);
			break;
		case dust:
			pt = make_shared<Dust>(row, col);
			break;
		case dirt:
			pt = make_shared<Dirt>(row, col);
			break;
		case fire:
			pt = make_shared<Fire>(row, col);
			break;
		case water:
			pt = make_shared<Water>(row, col);
			break;
		case lightning:
			pt = make_shared<Lightning>(row, col);
			break;
		case earth:
			pt = make_shared<Earth>(row, col);
	}
	return pt;
}
