#include "mesh.h"
#include "camera.h"
#include "raster.h"
#include "linalg.h"

constexpr int MAX_DEPTH = 500;

enum LightSource { 
    DIRECTIONAL,
    PUNCTUAL
};

template <int W, int H>
class Image {
    public:
        float img[H][W];
        float depth_buffer[H][W];
        
        Image() {
            for(int row = 0; row < H; ++row) {
                for(int col = 0; col < W; ++col) {
                    img[row][col] = 0;
                    depth_buffer[row][col] = MAX_DEPTH;
                }
            }
        }

        void render(Mesh& mesh, Camera& camera, Vec3<float>& light, LightSource light_source) {
            // Initialize for LightSource::DIRECTIONAL
            Vec3<float> light_dir = -light / norm(light);

            for(int i = 0; i < mesh.n_triangles; ++i) {
                Vec3<float> P1 = mesh.vertices[mesh.triangles[i].x];
                Vec3<float> P2 = mesh.vertices[mesh.triangles[i].y];
                Vec3<float> P3 = mesh.vertices[mesh.triangles[i].z];

                Vec3<float> p1d = camera.project(P1);
                Vec3<float> p2d = camera.project(P2);
                Vec3<float> p3d = camera.project(P3);

                Vec2<float> p1(p1d.x, p1d.y);
                Vec2<float> p2(p2d.x, p2d.y);
                Vec2<float> p3(p3d.x, p3d.y);

                float d1 = p1d.z;
                float d2 = p2d.z;
                float d3 = p3d.z;
                float d_min = std::min(std::min(d1,d2), d3);

                TriangleRasterizer rasterizer(p1, p2, p3);

                for(int x = 0; x < W; ++x) {
                    for(int y = 0; y < H; ++y) {
                        // Depth culling
                        if(d_min > depth_buffer[y][x]) continue;
                        Vec2<float> p(x, y);

                        if(rasterizer.inside(p)) {
                            // Visibility check
                            float d = rasterizer.interp(d1, d2, d3, p);
                            if(d > depth_buffer[y][x]) continue;
                            depth_buffer[y][x] = d;

                            // Shading
                            if(light_source == LightSource::PUNCTUAL) {
                                Vec3<float> P = rasterizer.interp(P1, P2, P3, p);
                                light_dir = P - light;
                                light_dir = light_dir/norm(light_dir);
                            }
                            float shading = std::max((float)0, dot(mesh.normals[i], -light_dir));
                            img[y][x] = shading;
                        }
                    }
                }
            }
        }
};
