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

    std::string path = argv[1];
    Mesh mesh(path);
    constexpr size_t H = 512;
    constexpr size_t W = 512;

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
        Vec3<float> light = C*1.f;

        Camera cam(H, W, 1000.f, C, R);

        image.render(mesh, cam, light, LightSource::PUNCTUAL);

        // Magick::Image my_image(W, H, "I", Magick::FloatPixel, image.img.data);

        for(size_t i = 0; i < W*H; ++i) {
            char_frame.data[i] = (unsigned char)(image.img.data[i] * 256.f);
        }

        usleep(15000);

        // my_image.magick("png");
        std::cout << "\033[H";
        sixel_encode(char_frame.data, W, H, 0, dither, output);
        // my_image.write("render.png");
        std::cout << std::flush;

    }

    return 0;
}
