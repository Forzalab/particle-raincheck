// put all #include in header file pls

#include "Game.h"

typedef uint32_t GameTick;

const std::string SAVEFILE = "save.JSON";
const uint16_t ROWS_DFT = 50;
const uint16_t COLS_DFT = 70;

Game::Game() : world(ROWS_DFT, COLS_DFT) { world.load(SAVEFILE); }

Game::Game(const uint16_t &rows, const uint16_t &cols)
	: rows(rows), cols(cols), world(rows, cols) {
	world.load(SAVEFILE);
}

GameTick Game::get_tickrate() const { return tickrate; }

// Used for boundary checking fps when increasing and decreasing
GameTick const fps_min = 3, fps_max = 60;

// Declaring here, definition is above render()
std::string unrender(auto &prevPs);

std::string printFPS(const auto &lastFrameStart, Wc rows, bool paused) {
	std::string s;
	int size = 0;
	auto diff = std::chrono::duration<double>(std::chrono::steady_clock::now() -
											  lastFrameStart);
	s += movecursor(rows + 5, 0);
	s += std::to_string(int(1000.0 / diff.count()) / 1000.0);
	s += " FPS";
	s += "";
	size = s.size();
	for (int i = 0; i < 20 - size; i++) {
		s += " ";
	} // Clean up trailing chars from prev frame

	if (paused == false) {
		s += "(P):Pause (+):Increase_FPS (-):Decrease_FPS";
		s += movecursor(rows + 6, size - 6);
		for (int i = 0; i < 71; i++) {
			s += " ";
		} // Clean up trailing chars from prev frame
	} else {
		size = s.size();
		//s += movecursor(rows + 5, size);
		s += "(S):Unpause (Q):Quit (A):Save (L):Load";
		for (int i = 0; i < 30; i++) {
			s += " ";
		} // Clean up trailing chars from prev frame
		s += movecursor(rows + 6, size - 6);
		s += "(0):Air (1):Dust (2):Fire (3):Water (4):Earth (5):Dirt "
			 "(6):Lightning";
	}

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
	// Default of 5. Tickrate is directly proportional to framerate. 60 tickrate
	// -> 1 / tickrate = 60fps.
	tickrate = 30;

	std::string fs;
	fs += show_cursor(false);
	fs += clearscreen();
	std::cerr << fs;
	fs.clear();
	using clock = std::chrono::steady_clock;

	// Placeholder vals. We can change these later.
	{
		// Draw a splash screen here.
		fs += clearscreen();
		std::cerr << fs;
		fs.clear();
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
	// Stores which P_Type user selects, is none by default. Also contains a
	// member function which will be set as the callback for on mouse down,
	// adding that particle type at the mouse loc
	CallbackHandler ch{world};
	// Creates a callable function wrapper of type function<void(int, int)> with
	// two placeholder params that will be supplied by mousedown events
	auto boundFunc = bind(&CallbackHandler::setRowCol, &ch, placeholders::_1,
						  placeholders::_2);
	// Sets the function that happens on mouse click down to the created
	// function wrapper
	on_mousedown(boundFunc);

	while (true) {
		int c = toupper(quick_read());
		if (c == 'P') {
			paused = true;
			std::cerr << set_mouse_mode(true);
		} else if (c == 'S') {
			paused = false;
			std::cerr << set_mouse_mode(false);
		}
		//- '0' gets 0-9 in integer form
		// +1 to map to 1(air)-10(TBD3)
		else if (c <= '9' && c >= '0') {
			ch.setPType(static_cast<P_Type>(c - '0' + 1));
		} else if (c == '-') {
			dcrs_fps();
		} else if (c == '+') {
			incr_fps();
		}
		fs += printFPS(prev_frame, world.get_rows(), paused);
		;
		prev_frame = clock::now();
		auto tickDur = std::chrono::duration<double>(1.0 / double(tickrate));
		next_frame += std::chrono::duration_cast<clock::duration>(tickDur);
		if (!paused)
			frame += world.physics(); // Physics will always return 1, unless
									  // there are no particles.
		else {
			switch (c) {
			case 'Q':
				resetTerminal();
				return;
			case 'A': // save
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
			prevPs.push_back(
				pair<Wc, Wc>(int(p->get_row()), int(p->get_col())));
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
	s += movecursor(0, 0);
	s += resetcolor();
	for (const auto &p : particles) {
		Wc row = int(p->get_row());
		Wc col = int(p->get_col());
		if (col < 0 || col > world.get_cols() || row > world.get_rows() ||
			row < 0)
			continue; // Do not print particles that are OOB and not yet culled
					  // by world::physics()
		s += movecursor(int(row), int(col));
		s += setbgcolor(p->get_r(), p->get_g(), p->get_b());
		s += " ";
		s += resetcolor();
	}
	return s;
}

void Game::incr_fps() {
	// New change that makes input increment/decrement tickrate
	tickrate++;
	tickrate = std::clamp(
		tickrate, fps_min,
		fps_max); // Makes sure tickrate doesnt leave its set boundary
				  /*
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
				  */
}
void Game::dcrs_fps() {
	tickrate--;
	tickrate = std::clamp(tickrate, fps_min, fps_max);
}

void Game::load() {
	std::cerr << resetcolor();
	std::cerr << movecursor(0, 0);
	cout << "Please enter name of file without the file extension: ";
	std::string filename;
	set_raw_mode(false);
	cin >> filename;
	set_raw_mode(true);
	filename += ".JSON";
	int err = world.load(filename);
	if (err == -1) {
		std::cerr << "File: " << filename << " does not exist.";
	} else {
		std::cerr << clearscreen();
	}
	// set_raw_mode(true);
}

void Game::save() {
	std::cerr << movecursor(0, 0);
	cout << "Please enter name of file without the file extension: ";
	std::string filename;
	set_raw_mode(false);
	cin >> filename;
	set_raw_mode(true);
	filename += ".JSON";
	world.save(filename);
	std::cerr << clearscreen();
}

P_ptr CallbackHandler::generateParticle() {
	P_ptr pt;
	switch (type) {
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

using CH = CallbackHandler;

CH::CallbackHandler(World &inworld) : world(inworld) {}

void CH::setRowCol(int inrow, int incol) {
	if (type == none)
		return;
	row = inrow;
	col = incol;
	if (world.atMap(row, col) == none) { // any value other than none will not
										 // allow particle generation. Prevents>
		P_ptr pt = generateParticle();
		if (pt != nullptr)
			pt->set_lifetime(1000); // arbitrary for testing
		if (pt != nullptr)
			world.add_particle(pt); // Ensure it doesnt generate a nullptr
	}
} // inrow and incol are previously verified in bounds. This is the function
  // called on mousedown

void CH::setPType(P_Type inType) { type = inType; }
