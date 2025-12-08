# Termesh
An interactive 3D renderer right in your terminal.

[demo](https://github.com/user-attachments/assets/abc066dc-a92e-4925-a706-75ba97cd51ec)

<img src="assets/render.png" width=50%>

## Controls
| Key | Effect |
| --- | ------ |
| `W` | move forwards |
| `A` | move left |
| `S` | move backwards |
| `D` | move right |
| `<space>` | move up |
| `Z` | move down |
| `H` | turn left |
| `J` | turn down |




| `K` | turn up |
| `L` | turn left |

## Build Instructions
```bash
apt install libnotcurses-dev # install dependencies
mkdir build
cd build
cmake ..
cmake --build .
./termesh your_mesh.ply # runs the built binary
```
