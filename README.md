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
- A map (rows x cols in size of chars) which indicates which particle type (or nothing) is at each place in the game world ******* raymond_0865098
- A .size() method which returns how many particles are in the world ******* raymond_0865098
- A .alive_count() method which returns how many particles with lifetime > 0 are in the world ******* raymond_0865098
- A .save() method which saves the state of the world to disk ******* raymond_0865098
- A .load() method which loads the state of the world to disk ******* raymond_0865098
 to save the world to disk
- A .physics() method which runs through the list of Particles and runs .physics() on each one ******* raymond_0865098
- If a particle is now out of bounds (no YOU'RE out of bounds) then delete it from the list ******* raymond_0865098
- If a particle now has a lifetime of exactly 0, delete it from the list ******* raymond_0865098

### Jackson
Working on the actual physics simulation of the particles
------Dirt travels downwards and forms piles when it hits something solid
------Water drips down and if it hits something solid it will slide sideways to find the lowest level. Water touching fire turns into air moving upwards


### Unassigned Work

> ...all the rest of the bullet points...
   
