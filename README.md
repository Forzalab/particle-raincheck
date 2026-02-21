# particle-raincheck
## Project #1 for CSCI41

<p align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSgzszyTirXN8liej72-mGnYV0in9wQo7wDUgHHoGtoqw&s=10" alt="discord bad" width="600"/>
  <br/>
  <em>placeholder, ideally replace with console demo</em>
</p>

### Bill
- Make a ColorGrid class and fill it with the colors of the world (or test data)
- Add in a call to bridges.setDataStructure() to tell BRIDGES to render the ColorGrid
- Add in a call to bridges.visualize() to send it off to the web site
- Implement a command to increase frame rate
- Implement a command to decrease frame rate

### Raymond 
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
- A .frame member variable that starts at 0 and counts up by 1 each time physics is run
- Simulation should also automatically pause when world.alive_count is 0
- A .render() method which draws all the particles in the list to the screen using their position and color 
- It must be rendered across the full screen using the color library, using 32 bit
- -A .run() method that starts the game. Your main() function should just be this: Game game; game.run();
- Then go into an infinite loop of: handling user input, running physics, drawing the world, delaying 
- It should delay long enough to guarantee a fixed frame rate of 5 fps (you can adjust this number) 
- Use clock (or std::chrono) to see how long it has been since the last frame began
- Use usleep() to delay execution so you get a guaranteed frame rate of 5 fps or whatever

### Romero
- On launch draw a splash screen (Implementing)
- Lightning travels in a straight line and stops when it hits something solid. If it touches water, the water turns into lightning. If it hits earth it turns into dirt (Implementing)
- Implement a command to increase frame rate (Implemented)
- Implement a command to decrease frame rate (Implemented)
- Implement a command to load the world from disk (Implemented)
- Implement a command to save the world to disk (Implemented)

### Jackson
Working on the actual physics simulation of the particles
------Dirt travels downwards and forms piles when it hits something solid
------Water drips down and if it hits something solid it will slide sideways to find the lowest level. Water touching fire turns into air moving upwards

### Romero
- On launch draw a splash screen ****** romero_1048121
### Unassigned Work

> ...all the rest of the bullet points...
   
