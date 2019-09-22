# Terminal3D

A very simple C++ 3D graphics engine with minimal dependencies. Written from scratch as a personal excersice in 3D projection and rasterization. Apart from the C++ standard lib, only the [ncurses library](https://invisible-island.net/ncurses/announce.html) is required for proper terminal output.

![Screenshot](/img/screenshot_cube.png?raw=true "Impress your friends with these world-class graphics straight in the terminal")

#### Main features
- Displays a nice little rotating cube
- Simple arrow key menu for toggling settings
- Toggleable wireframe/mesh and fill drawing
- Back-face culling
- Viewport size and aspect ratio -adapting projection; displays nicely in a terminal of any size and shape

#### Usage
No precompiled binaries or makefile provided, build from source. On Linux:
- Make sure gcc is installed
- Make sure ncurses is installed:
  `sudo apt-get install libncurses5-dev`/`sudo yum install ncurses-devel`
- Compile with the `-lncurses` flag, e.g.
  `c++ main.cpp -o terminal3d -lncurses`
- Run: `./terminal3d`
