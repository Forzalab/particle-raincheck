# particle-raincheck
## Project #1 for CSCI41

<p align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSgzszyTirXN8liej72-mGnYV0in9wQo7wDUgHHoGtoqw&s=10" alt="discord bad" width="600"/>
  <br/>
  <em>placeholder, ideally replace with console demo</em>
</p>

## Raymond
### World class 
- A map (rows x cols in size of chars) which indicates which particle type (or nothing) is at each place in the game world 
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
### Game class
- A .frame member variable that starts at 0 and counts up by 1 each time physics is run
- Simulation should also automatically pause when world.alive_count is 0
- A .render() method which draws all the particles in the list to the screen using their position and color 
- It must be rendered across the full screen using the color library, using 32 bit
- -A .run() method that starts the game. Your main() function should just be this: Game game; game.run();
- Then go into an infinite loop of: handling user input, running physics, drawing the world, delaying 
- It should delay long enough to guarantee a fixed frame rate of 5 fps (you can adjust this number) 
- Use clock (or std::chrono) to see how long it has been since the last frame began
- Use usleep() to delay execution so you get a guaranteed frame rate of 5 fps or whatever

## Romero
### Game class
- On launch draw a splash screen (Implementing)
### Particle class
- Lightning travels in a straight line and stops when it hits something solid. If it touches water, the water turns into lightning. If it hits earth it turns into dirt (Implementing)
- Implement a command to increase frame rate (Implemented)
- Implement a command to decrease frame rate (Implemented)
- Implement a command to load the world from disk (Implemented)
- Implement a command to save the world to disk (Implemented)

## Jackson
### Particle class
- Working on the actual physics simulation of the particles
    - Dirt travels downwards and forms piles when it hits something solid
    - Water drips down and if it hits something solid it will slide sideways to find the lowest level. Water touching fire turns into air moving upwards

## Ho
### Particle class
- A position (.row, .col float) which indicates where it is on
 the screen
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
- A .touch() method that gets called when another particle tou
ches it.
