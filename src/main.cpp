#include <cmath>
#include <iostream>
#include <fstream>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "raster.h"

int main() {
    // Full rendering test
    Mesh mesh("/home/dillan/ascii3d/data/cube.ply");
    Vec3<float> C(1.0, 0.5, -10);
    // Vec3<float> C(0, 0, -8);
    Mat3x3<float> R = rot_x(30) * rot_y(60);
    // Mat3x3<float> R;
    C = R.T()*C;
    Vec3<float> light(5, 15, -20);
    // Vec3<float> light(0, 0, -20);
    // light = light / norm(light);
    light = C / norm(C);
    int H = 128;
    int W = 128;
    Camera cam(H, W, 150.0, C, R);

    int img[H][W];
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            img[row][col] = 0;
        }
    }

    int DEPTH_MAX = 5000;
    int depth_buffer[H][W];
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            depth_buffer[row][col] = DEPTH_MAX;
        }
    }

    for(int i = 0; i < mesh.n_triangles; ++i) {
        // if(!(i == 6 || i == 7)) continue;
        // i = 6, i = 7 are closest faces

        Vec3<float> p1d = cam.project(mesh.vertices[mesh.triangles[i].x]);
        Vec3<float> p2d = cam.project(mesh.vertices[mesh.triangles[i].y]);
        Vec3<float> p3d = cam.project(mesh.vertices[mesh.triangles[i].z]);

        Vec2<float> p1(p1d.x, p1d.y);
        Vec2<float> p2(p2d.x, p2d.y);
        Vec2<float> p3(p3d.x, p3d.y);

        float d1 = p1d.z;
        float d2 = p2d.z;
        float d3 = p3d.z;
        float d_min = std::min(std::min(d1,d2), d3);
        Vec3<float> depths(d1, d2, d3);
        
        TriangleRasterizer rasterizer(p1, p2, p3);

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {

                if(d_min > depth_buffer[x][y]) continue;
                float d = rasterizer.interp(depths, Vec2<float>(x, y));

                if(rasterizer.inside(Vec2<float>(x, y))) {
                    if(d > depth_buffer[x][y]) continue;
                    depth_buffer[x][y] = d;
                    float shading = std::max((float)0, dot(mesh.normals[i], light));
                    if(shading > img[x][y] || true) {
                        if(shading == 0) img[x][y] = 0;
                        else {
                            img[x][y] = (int)((shading) * 6);
                        }
                    }
                }
            }
        }
    }

    // 47 -> 83
    std::string palette = " .,:;?#@";
    std::ofstream file("render.txt");
    for(int y = 0; y < H; ++y) {
        for(int x = 0; x < W; ++x) {
            file << palette[img[y][x]] << palette[img[y][x]];
        }
        file << "\n";
    }

    return 0;
}
