#include <cmath>
#include <cstddef>
#include <cstdio>
#include <string>
#include <unistd.h>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "render.h"

#include <notcurses/notcurses.h>

int main(int argc, char *argv[]) {
    std::string path = argv[1];
    Mesh mesh(path);

    constexpr size_t H = 512;
    constexpr size_t W = 512;

    notcurses_options nc_opts = {
        NULL,
        NCLOGLEVEL_SILENT,
        // NCLOGLEVEL_DEBUG,
        0, 0, 0, 0, // Margins (top, right, bottom, left)
        0
    };

    notcurses *nc = notcurses_init(&nc_opts, stdout);

    FrameBuffer<W, H> image;

    float theta = 0;
    Vec3<float> C(0, 0, -10);
    Vec3<float> light = C*1.f;
    float f = 1000.f;

    Vec3<float> angles(0, 0, 0);

    Camera cam = Camera::Euler(H, W, f, C, angles);

    float angle_inc = 1.f;
    float trans_inc = 0.3f;

    Vec3<float> dx(trans_inc, 0.f, 0.f);
    Vec3<float> dy(0.f, trans_inc, 0.f);
    Vec3<float> dz(0.f, 0.f, trans_inc);

    Vec3<float> da(angle_inc, 0.f, 0.f);
    Vec3<float> db(0.f, angle_inc, 0.f);
    Vec3<float> dg(0.f, 0.f, angle_inc);

    unsigned char *char_frame = new unsigned char[W*H*4];

    ncinput ni;

    ncplane_options np_opts {
        0, 0, // x, y
        512, 512, // rows, cols
        NULL,
        NULL,
        NULL,
        0, 0, // Margins (bottom, right)
        0
    };

    ncplane *np_std = notcurses_stdplane(nc);
    ncplane *np = ncplane_create(np_std, &np_opts);
    ncvisual_options nv_opts {};
    nv_opts.n = NULL;
    // nv_opts.scaling = NCSCALE_SCALE;
    nv_opts.blitter = NCBLIT_PIXEL;
    // nv_opts.flags = NCVISUAL_OPTION_NODEGRADE;
    // nv_opts.transcolor = 0xFFFFFFFF;
    // notcurses_render(nc);
    ncplane *vplane = nullptr;
    
    uint32_t pressed = 0;
    bool parented = false;
    while(true) {
        image.render(mesh, cam, light, LightSource::PUNCTUAL);

        for(size_t i = 0; i < W*H; ++i) {
            unsigned char gray = (image.img.data[i] * 255.f);

            char_frame[4*i] = gray;
            char_frame[4*i+1] = gray;
            char_frame[4*i+2] = gray;
            char_frame[4*i+3] = image.depth_buffer.data[i] < MAX_DEPTH ? 0xFF : 0x00;
        }

        ncvisual *nv = ncvisual_from_rgba(char_frame, H, 4*W, W);
        vplane = ncvisual_blit(nc, nv, &nv_opts);
        // ncplane_putstr(np, "HI");
        if(!parented) {
            ncplane_reparent(vplane, np);
            parented = true;
        }
        notcurses_render(nc);
        nv_opts.n = vplane;
        // ncpile_render(vplane);
        // ncpile_rasterize(vplane);
        // ncplane_erase(vplane);
        // ncvisual_destroy(nv);

        notcurses_get_blocking(nc, &ni);

        if(ni.id == 'q') break;

        switch(ni.id) {
            case 'w':
                cam.translate(euler(Vec3<float>(0.f, angles.y, 0.f)).T()*dz);
                break;
            case 'a':
                cam.translate(cam.R.T()*-dx);
                break;
            case 's':
                cam.translate(euler(Vec3<float>(0.f, angles.y, 0.f)).T()*-dz);
                break;
            case 'd':
                cam.translate(cam.R.T()*dx);
                break;
            case ' ':
                cam.translate(dy);
                break;
            case 'z':
                cam.translate(-dy);
                break;
            case 'j':
                angles = angles - da;
                cam.set_R(euler(angles));
                break;
            case 'k':
                angles = angles + da;
                cam.set_R(euler(angles));
                break;
            case 'h':
                angles = angles - db;
                cam.set_R(euler(angles));
                break;
            case 'l':
                angles = angles + db;
                cam.set_R(euler(angles));
                break;
        }
        light = cam.C;

    }

    notcurses_stop(nc);
    delete[] char_frame;

    return 0;
}
