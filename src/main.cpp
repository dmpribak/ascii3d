#include <cmath>
#include <fstream>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "render.h"

int main() {
    // Full rendering test
    std::string palette = " .,~+*:;!?&#%@";
    int n_palette = 13;

    Mesh mesh("/home/dillan/ascii3d/data/cube.ply");
    // Vec3<float> C(1.0, 0.5, -10);
    Vec3<float> C(0, 0, -10);
    Mat3x3<float> R = rot_x(30) * rot_y(60);
    // Mat3x3<float> R;
    C = R.T()*C;
    Vec3<float> light(5, 15, -20);
    light = C;
    constexpr int H = 128;
    constexpr int W = 128;
    Camera cam(H, W, 150.0, C, R);

    Image<W, H> image;

    image.render(mesh, cam, light, LightSource::PUNCTUAL);

    std::ofstream file("render.txt");
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            int idx = (int)(image.img[row][col] * (n_palette-1));
            file << palette[idx] << palette[idx];
        }
        file << "\n";
    }

    return 0;
}
