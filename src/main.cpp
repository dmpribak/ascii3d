// #include <Magick++/Color.h>
// #include <Magick++/Geometry.h>
// #include <Magick++/Include.h>
// #include <MagickCore/magick-type.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <ostream>
#include <unistd.h>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "render.h"

// #include<Magick++.h>
#include<sixel.h>

int sixel_write(char *data, int size, void *priv) {
    return fwrite(data, 1, size, (FILE *)priv);
}

int main(int argc, char *argv[]) {
    // Full rendering test
    // std::string palette = " .,~+*:;!?&#%@";
    std::string palette[] = {u8" ", u8"'", u8"`", u8".", u8",", u8";", u8":", u8"~", u8"-", u8"=", u8"+", u8"*", u8"a", u8"o", u8"O", u8"0", u8"@"};
    // palette = " .'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    int n_palette = 15;

    std::string path = argv[1];
    // Mesh mesh("/home/dillan/ascii3d/data/cube.ply");
    Mesh mesh(path);
    const size_t H = 512;
    const size_t W = 512;


    sixel_output_t *output;
    sixel_output_new(&output, sixel_write, stdout, NULL);
    sixel_dither_t *dither = sixel_dither_get(SIXEL_BUILTIN_G8);
    sixel_dither_set_pixelformat(dither, SIXEL_PIXELFORMAT_G8);
    BufMat<unsigned char, W, H> char_frame;
    
    FrameBuffer<W, H> image;

    for(float theta = 0; theta < 360; theta += 1) {
        Vec3<float> C(0, 0, -10);
        Mat3x3<float> R = rot_y(theta*0.3456) * rot_x(theta) * rot_y(30) * rot_x(70);
        C = R.T()*C;
        Vec3<float> light = C*(float)1;

        Camera cam(H, W, 1000.0, C, R);


        image.render(mesh, cam, light, LightSource::PUNCTUAL);

        // Magick::Image my_image(W, H, "I", Magick::FloatPixel, image.img.data);

        for(size_t i = 0; i < W*H; ++i) {
            char_frame.data[i] = (unsigned char)(image.img.data[i] * 256.);
        }

        // std::ofstream file("render.txt", std::ios::binary);
        // for(size_t row = 0; row < H; ++row) {
        //     for(size_t col = 0; col < W; ++col) {
        //         float idx_float = image.img[H-row-1][col];
        //         size_t idx = 0;
        //         if(idx_float > 0) {
        //             idx = (size_t)(idx_float * (float)(n_palette-2) + (float)1);
        //         }
        //         file << palette[idx] << palette[idx];
        //     }
        //     file << "\n";
        // }
        // file << std::flush;
        // file.close();
        usleep(15000);

        // my_image.magick("sixel");
        std::cout << "\033[H";
        // sixel_dither_initialize(dither, char_frame.data, W, H, SIXEL_PIXELFORMAT_G1, SIXEL_LARGE_AUTO, SIXEL_REP_AUTO, SIXEL_QUALITY_HIGH);
        sixel_encode(char_frame.data, W, H, 0, dither, output);
        // my_image.write("sixel:-");
        std::cout << std::flush;

    }

    return 0;
}
