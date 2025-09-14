#include <cmath>
#include <iostream>
#include <fstream>

#include "mesh.h"
#include "camera.h"
#include "linalg.h"
#include "raster.h"

int main() {
    Mesh mesh("/home/dillan/ascii3d/data/cube.ply");
    Vec3<float> point(0, -10, 0);
    Vec3<float> C(0, 0.5, -10);
    Mat3x3<float> R = rot_y(60);
    C = R.T()*C;
    Vec3<float> light(5, 15, -20);
    light = light / norm(light);
    int H = 128;
    int W = 128;
    Camera cam(H, W, 200.0, C, R);

    int img[H][W];
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            img[row][col] = 0;
        }
    }

    int DEPTH_MAX = 500;
    int depth_buffer[H][W];
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            depth_buffer[row][col] = 500;
        }
    }

    for(int i = 0; i < mesh.n_triangles; ++i) {
        Vec3<float> p1d = cam.project(mesh.vertices[mesh.triangles[i].x]);
        Vec3<float> p2d = cam.project(mesh.vertices[mesh.triangles[i].y]);
        Vec3<float> p3d = cam.project(mesh.vertices[mesh.triangles[i].z]);

        Vec2<float> p1(p1d.x, p1d.y);
        Vec2<float> p2(p2d.x, p2d.y);
        Vec2<float> p3(p3d.x, p3d.y);

        float d1 = p1d.z;
        float d2 = p2d.z;
        float d3 = p3d.z;
        float d = std::min(std::min(d1,d2), d3);
        
        TriangleRasterizer rasterizer(p1, p2, p3);

        for(int row = 0; row < H; ++row) {
            for(int col = 0; col < W; ++col) {
                if(d > depth_buffer[row][col]) continue;
                if(rasterizer.inside(Vec2<float>(row, col))) {
                    depth_buffer[row][col] = d;
                    // std::cout << mesh.normals[i] << std::endl;
                    // float shading = std::max((float)0, dot(mesh.normals[i], light));
                    float shading = fabsf(dot(mesh.normals[i], light));
                    if(shading > img[row][col]) {
                        std::cout << shading << std::endl;
                        img[row][col] = (int)(shading * 7);
                    }
                }
            }
        }
    }

    std::string palette = " .,:;?#@";
    std::ofstream file("render.txt");
    for(int row = 0; row < H; ++row) {
        for(int col = 0; col < W; ++col) {
            file << palette[img[row][col]] << palette[img[row][col]];
            // if(img[row][col] > 0) file << "@@";
            // else file << "  ";
        }
        file << "\n";
    }
}
