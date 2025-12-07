#define EIGEN_USE_BLAS

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <string>
#include <unistd.h>

#include "Eigen/Core"
#include "mesh.h"
#include "camera.h"
#include "render.h"

#include <notcurses/notcurses.h>
#include <notcurses/direct.h>
#include <Eigen/Dense>

using Eigen::Matrix3f;
using Eigen::Vector3f;

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
    Vector3f C(0, 0, -10);
    Vector3f light = C*1.f;
    float f = 1000.f;

    Vector3f angles(0, 0, 0);

    Camera cam = Camera::Euler(H, W, f, C, angles);

    float angle_inc = 0.65f;
    float trans_inc = 0.1f;

    Vector3f dx(trans_inc, 0.f, 0.f);
    Vector3f dy(0.f, trans_inc, 0.f);
    Vector3f dz(0.f, 0.f, trans_inc);

    Vector3f da(angle_inc, 0.f, 0.f);
    Vector3f db(0.f, angle_inc, 0.f);
    Vector3f dg(0.f, 0.f, angle_inc);

    unsigned char *char_frame = new unsigned char[W*H*4];

    ncinput ni;

    ncplane_options np_opts {
        0, 0, // x, y
        1, 1, // rows, cols
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
    ncplane *vplane = nullptr;
    
    uint32_t pressed = 0;
    bool parented = false;

    bool wpressed = false;
    bool apressed = false;
    bool spressed = false;
    bool dpressed = false;
    bool zpressed = false;
    bool spacepressed = false;
    bool hpressed = false;
    bool jpressed = false;
    bool kpressed = false;
    bool lpressed = false;

    while(true) {
        // usleep(3200);
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
        if(!parented) {
            ncplane_reparent(vplane, np);
            parented = true;
        }
        notcurses_render(nc);
        nv_opts.n = vplane;

        notcurses_get_nblock(nc, &ni);

        if(ni.id == 'q') break;

        switch(ni.id) {
            case 'w':
                if(ni.evtype == NCTYPE_PRESS) {
                    wpressed = true;
                } else wpressed = false;
                break;
            case 'a':
                if(ni.evtype == NCTYPE_PRESS) {
                    apressed = true;
                } else apressed = false;
                break;
            case 's':
                if(ni.evtype == NCTYPE_PRESS) {
                    spressed = true;
                } else spressed = false;
                break;
            case 'd':
                if(ni.evtype == NCTYPE_PRESS) {
                    dpressed = true;
                } else dpressed = false;
                break;
            case ' ':
                if(ni.evtype == NCTYPE_PRESS) {
                    spacepressed = true;
                } else spacepressed = false;
                break;
            case 'z':
                if(ni.evtype == NCTYPE_PRESS) {
                    zpressed = true;
                } else zpressed = false;
                break;
            case 'j':
                if(ni.evtype == NCTYPE_PRESS) {
                    jpressed = true;
                } else jpressed = false;
                break;
            case 'k':
                if(ni.evtype == NCTYPE_PRESS) {
                    kpressed = true;
                } else kpressed = false;
                break;
            case 'h':
                if(ni.evtype == NCTYPE_PRESS) {
                    hpressed = true;
                } else hpressed = false;
                break;
            case 'l':
                if(ni.evtype == NCTYPE_PRESS) {
                    lpressed = true;
                } else lpressed = false;
                break;
        }

        if(wpressed) {
            cam.translate(euler(Vector3f(0.f, angles.y(), 0.f)).transpose()*dz);
        }
        if(apressed) {
            cam.translate(cam.R.transpose()*-dx);
        }
        if(spressed) {
            cam.translate(euler(Vector3f(0.f, angles.y(), 0.f)).transpose()*-dz);
        }
        if(dpressed) {
            cam.translate(cam.R.transpose()*dx);
        }
        if(spacepressed) {
            cam.translate(dy);
        }
        if(zpressed) {
            cam.translate(-dy);
        }
        if(jpressed) {
            angles = angles - da;
            cam.set_R(euler(angles));
        }
        if(kpressed) {
            angles = angles + da;
            cam.set_R(euler(angles));
        }
        if(hpressed) {
            angles = angles - db;
            cam.set_R(euler(angles));
        }
        if(lpressed) {
            angles = angles + db;
            cam.set_R(euler(angles));
        }

        light = cam.C;

    }

    notcurses_stop(nc);
    delete[] char_frame;
    
    return 0;
}
