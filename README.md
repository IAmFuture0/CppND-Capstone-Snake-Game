# CPPND: Capstone Snake Game Example

This is a repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213) with snakebot controlled by the computer using the A* search algorithm. The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snakebot.gif" width="600" height="600"/>

## New Features

The idea of added features are from the previous project **Build An OpenStreetMap Route Planner** and **Concurrent Traffic Simulation**. The snakebot follow the gray route planned by A* algorithm, and will stop immediately upon detecting a red light at an the intersection. It will move again once the light turns green.

By the way, I kept the code for the player-controlled snake in the program, but I commented it out. If you want to compete with the snakebot, feel free to remove the comments.

Additionally, you can build your own C++ application or extend the work, for example replacing the A* search algorithm with AI approach . This project will demonstrate that you can independently create applications using a wide range of C++ features.

The following diagram is the schematic of my program. The colored blocks indicate the changes I made: light blue for small changes and dark blue for the main changes where I implemented the new features.
<img src="SnakeGame_ProgramSchmetic.png"/>

## Criteria Fullfillment
### Loops, Functions, I/O - meet at least 2 criteria
- [x] The project demonstrates an understanding of C++ functions and control structures.
  * snakebot.h, snakebot.cpp, intersection.h, and intersection.cpp files demonstrate the well-structured code and a variety of control structures.
- [ ] The project reads data from a file and process the data, or the program writes data to a file.
  * NotYet
- [ ] The project accepts user input and processes the input.
  * NotYet
- [x] The project uses data structures and immutable variables.
  * The **delta** variable (in class Astar in *snakebot.h*) and **intersectionLocation** variable (in class Intersection in *Intersection.h*) are both vector with const int element.
  
### Object Oriented Programming - meet at least 3 criteria
- [ ] One or more classes are added to the project with appropriate access specifiers for class members.
- [ ] Class constructors utilize member initialization lists.
- [ ] Classes abstract implementation details from their interfaces.
- [ ] Overloaded functions allow the same function to operate on different parameters.
- [ ] Classes follow an appropriate inheritance hierarchy with virtual and override functions.
- [ ] Templates generalize functions or classes in the project.

### Memory Management - meet at least 3 criteria
- [ ] The project makes use of references in function declarations.
- [ ] The project uses destructors appropriately.
- [ ] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
- [ ] The project follows the Rule of 5.
- [ ] The project uses move semantics to move data instead of copying it, where possible.
- [ ] The project uses smart pointers instead of raw pointers.

### Concurrency - meet at least 2 criteria
- [ ] The project uses multithreading.
- [ ] A promise and future is used in the project.
- [ ] A mutex or lock is used in the project.
- [ ] A condition variable is used in the project.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
