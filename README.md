# Termesh
An interactive 3D renderer right in your terminal.
![interactive demo](assets/termesh_demo.mp4)
![cube render](assets/render.png)

## Controls
| Key | Effect |
| --- | ------ |
| W | move forwards |
| A | move left |
| S | move backwards |
| D | move right |
| <space> | move up |
| Z | move down |
| H | turn left |
| J | turn down |
| K | turn up |
| L | turn left |

## Build Instructions
```bash
apt install libnotcurses-dev # install dependencies
mkdir build
cd build
cmake ..
cmake --build .
./termesh your_mesh.ply # runs the built binary
```
