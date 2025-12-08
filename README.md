# Termesh
An interactive 3D renderer right in your terminal.

[demo](https://github.com/user-attachments/assets/abc066dc-a92e-4925-a706-75ba97cd51ec)

[demo2](https://github.com/user-attachments/assets/60c21a90-e111-44a1-aaa9-daea903abc52)

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
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./termesh your_mesh.ply # runs the built binary
```
