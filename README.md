This is was my fifth attempt at a higher-dimensional game.

About this specific project, it contains a raycaster for voxel grids in 3 dimensions or more, along with a maze generator to produce such grids, along with a display and navigation system.
Despite a few floating-point errors, it is functional.
However, this is not remotely close to being a finished product nor do I intend it to be.
To be specific, I did not implement any GUIs for generating mazes or storing them, and I did not implement storage.
Changing maze generation settings can be done by modifying my test file, `test/basic_maze_display.cpp`.
The only code I might reuse in my current work on this game might be the display and control-related code.

In order to compile and run the project, one needs [Eclipse IDE for C/C++](https://www.eclipse.org/downloads/packages/release/2020-09/r/eclipse-ide-cc-developers).
This was a choice I made because I was still learning C++ and my main language was formerly Java and Eclipse was my main IDE.

I originally made this project in two repos, one for the "core" of the project, including the renderer and everything, and one for the display, which I used for the commit history of this repo, since I combined the two into one repo to push it all to GitHub.
That is why the commit history might look funny.

The prerequisites are [glfw3](https://github.com/glfw/glfw) and [glew](https://github.com/nigels-com/glew). glfw also has a few prerequisites that might not be installed by default.

