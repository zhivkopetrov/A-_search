Graphical representation of A* search algorithm in 2D grid.
List of developed heuristics: 
- Manhattan distance;
- Diagonal distance;
- Euclidean distance;

The project uses:

- SDL2 external library for graphics;
- cmake for build tool;

In order to run the project you need to have the listed libraries installed:
libsdl2-dev;
libsdl2-ttf-dev;
libsdl2-image-dev;
libsdl2-image-mixer;
cmake;

1) To auto-generate na GNU makefile step inside the build directory and run "cmake .." in the terminal; The cmake should generate a GNU makefile.
2) After generation is complete run "make";
3) When compilation has completed run the binary with "./game";

NOTE: the binary takes argument - "isDiagonalMovementAllowed";
Run with "./game true" to allowed diagonal movements in the grid;

TODOs:
- implement an "obstacle loader" that uses the "UP" and "DOWN" arrow keys to load a preset of obstacle levels (level with no obstacles, and 2 more)
- when animation is finished all "non-preset" walls should be removed
- move the "isDiagonalMovement allowed to an option"
- Implement a "popping up" menu (in the upper right corner). It will have the ticks "isDiagonalMovementAllowed"
 to a checkbox and up-down arrows for levels (predefined walls)
 - Try to change the AStar algorithm with the one with std::priority_queue<>