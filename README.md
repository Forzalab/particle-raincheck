# particle-raincheck

<p align="center">
  <img src="https://i.ibb.co/ns8GJqpc/Screenshot-20260227-204003-Termux.png" alt="discord bad" width="600"/>
  <br/>
  <em>Game splash screen (made with lolcat)</em>
</p>

## What is it?
Our group made a C++-written, console-based particle simulator simulating 9 particle types. Complete with RGB color rendering, mouse input, and BRIDGES visualization.
### How to run?
```bash
git clone https://github.com/Forzalab/particle-raincheck.git && \
cd particle-raincheck && \
make && \
./a.out
```
### Which account to use?
`raymond_0865098`

## Who does what?
### Raymond
#### Particle class
- No two Particles can be on the same location
- Game of Life particle (extra credit)
#### World class
- A size (.rows .cols int) that is the boundary of the world.
    - Any point which leaves the boundary must be deleted
- A map (rows x cols in size of chars) which indicates which particle type (or nothing) is at each place in the game world
- A list (must be std::list) of Particles which holds all particles in the world
- .at(row,col) method that returns a pointer to the Particle at the location row x col in the world (or nullptr if there is no particle there)
- A .size() method which returns how many particles are in the world 
- A .alive_count() method which returns how many particles with lifetime > 0 are in the world
- A .save() method which saves the state of the world to disk 
- A .load() method which loads the state of the world to disk to save the world to disk
- A .physics() method which runs through the list of Particles and runs .physics() on each one
    - If a particle is now out of bounds (no YOU'RE out of bounds) then delete it from the list 
    - If a particle now has a lifetime of exactly 0, delete it from the list
    - Any point which leaves the boundary must be deleted 
   - Physics (see below) will decrement it each frame by 1 to a minimum of 0.
    - If a particle is now out of bounds (no YOU'RE out of bounds) then delete it from the list
    - It has the world (.world World) as a member variable
#### Game class
- A .frame member variable that starts at 0 and counts up by 1 each time physics is run
- Simulation should also automatically pause when world.alive_count is 0
- A .render() method which draws all the particles in the list to the screen using their position and color 
- It must be rendered across the full screen using the color library, using 32 bit
- A .run() method that starts the game. Your main() function should just be this: Game game; game.run();
    - Then go into an infinite loop of: handling user input, running physics, drawing the world, delaying 
    - It should delay long enough to guarantee a fixed frame rate of 5 fps (you can adjust this number) 
- Use clock (or std::chrono) to see how long it has been since the last frame began
- Use usleep() to delay execution so you get a guaranteed frame rate of 5 fps or whatever
- Implement a command to pause simulating the world
- Implement a command to quit
- The user must be able to edit the game world by adding and deleting particles while the world is paused
- It must work with mouse input

### Romero
#### Game class
- Implement a command to draw the world on BRIDGES
- On launch draw a splash screen
- Below the world you should display the frame count and show the commands possible (L)oad S)ave P)ause etc)
- Implement a command to increase frame rate
- Implement a command to decrease frame rate
- Implement a command to load the world from disk
- Implement a command to save the world to disk
#### Particle class
- Lightning travels in a straight line and stops when it hits something solid. If it touches water, the water turns into lightning. If it hits earth it turns into dirt.
- Air moves in a straight line (ignoring gravity) bouncing off solid

### Jackson
#### Particle class
- Each type has a different physics movement type:
    - Dirt travels downwards and forms piles when it hits something solid
    - Water drips down and if it hits something solid it will slide sideways to find the lowest level. Water touching fire turns into air moving upwards 
    - So if you turn stationary true on water it will not move even if a spot opens up beneath it
- Earth is always stationary and solid
- A .touch() method that gets called when another particle touches it. 
- Between .physics and .touch you can handle all interactions (fire x water, water x lightning, etc.)
### Game class
- It has to handle all input from mouse and keyboard using non-blocking I/O
- Implement a command to start simulating the world

### Ho
#### Particle class
- A position (.row, .col float) which indicates where it is on the screen
- A velocity (.x_vel and .y_vel float) indicating how many cells per tick it moves
- A .type which must be an enum which indicates if it is air, dust, fire, water, earth, dirt, lightning
- Each type has a different physics movement type:
    - Air moves in a straight line (ignoring gravity) bouncing off solid
    - Dust has a small amount of gravity and randomly moves left and right every frame
    - Fire is stationary and shoots sparks of lightning in different directions over time
    - Earth is always stationary and solid
- Color (.r .g .b uint8_t) that should be set initially by its type (fire should be red) but can change
- A boolean named .stationary that if true means it is solid and does not move/simulate physics
- A .lifetime variable which holds how many frames more it will exist for
- Fire and lightning should have relatively short lifetimes, stone should have a lifetime of -1.
- A lifetime of -1 means infinite lifetime (it will not go away over time)
- Member variables must be in the private section, methods in the public section
    - Make the usual constructors, getters/setters, and so forth. You should use the Rule of 0
- A .physics() method that gets the world map passed in by reference, that runs physics for the particle and updates the world map if it moved. If it runs into another particle, it calls .touch() on the other particle.
    - Physics also decrements .lifetime by one each frame
- A .touch() method that gets called when another particle touches it.
- Confetti particle (extra credit)
#### BRIDGES (through `Bifrost.h`)
- Read the BRIDGES documentation: https://bridgesuncc.github.io/ and comprehend it
- Add in your username and API key
- Make a ColorGrid class and fill it with the colors of the world (or test data)
- Add in a call to bridges.setDataStructure() to tell BRIDGES
to render the ColorGrid
- Add in a call to bridges.visualize() to send it off to the web site
    - This will give you a URL that you can use to visualize your world in a browser
