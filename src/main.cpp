#include <Magick++/Color.h>
#include <Magick++/Geometry.h>
#include <Magick++/Include.h>
#include <MagickCore/magick-type.h>
#include <cmath>
#include <fstream>
#include <ostream>
#include <unistd.h>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "render.h"

#include<Magick++.h>

int main() {
    // Full rendering test
    // std::string palette = " .,~+*:;!?&#%@";
    std::string palette[] = {u8" ", u8"'", u8"`", u8".", u8",", u8";", u8":", u8"~", u8"-", u8"=", u8"+", u8"*", u8"a", u8"o", u8"O", u8"0", u8"@"};
    // palette = " .'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    int n_palette = 15;

    Mesh mesh("/home/dillan/ascii3d/data/cube.ply");
    const size_t H = 2048;
    const size_t W = 2048;

    for(float theta = 0; theta < 1; theta += 0.1) {
        Vec3<float> C(0, 0, -10);
        Mat3x3<float> R = rot_y(theta*0.3456) * rot_x(theta) * rot_y(30) * rot_x(70);
        C = R.T()*C;
        Vec3<float> light = C*(float)1;

        Camera cam(H, W, 6000.0, C, R);

        FrameBuffer<W, H> image;

        image.render(mesh, cam, light, LightSource::PUNCTUAL);

        std::ofstream file("render.txt", std::ios::binary);
        for(size_t row = 0; row < H; ++row) {
            for(size_t col = 0; col < W; ++col) {
                float idx_float = image.img[H-row-1][col];
                size_t idx = 0;
                if(idx_float > 0) {
                    idx = (size_t)(idx_float * (float)(n_palette-2) + (float)1);
                }
                file << palette[idx] << palette[idx];
            }
            file << "\n";
        }
        file << std::flush;
        file.close();
        usleep(15000);

        Magick::Image my_image(Magick::Geometry(W, H), "white");

        for(size_t y = 0; y < H; ++y) {
            for(size_t x = 0; x < W; ++x) {
                float intensity = image.img[y][x];
                // float intensity = ((float)x+(float)y)/2.0/W;
                my_image.pixelColor(x,y,Magick::Color(intensity*65536.0, intensity*65536.0, intensity*65536.0, 65536.0));
            }
        }
        my_image.magick("png");
        my_image.write("render.png");
        break;

    }

    return 0;
}
