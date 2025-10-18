#include "mesh.h"
#include "camera.h"
#include "raster.h"
#include "linalg.h"
#include <cstddef>

constexpr float MAX_DEPTH = 500;

enum LightSource { 
    DIRECTIONAL,
    PUNCTUAL
};

template <typename T, size_t W, size_t H>
class BufMat {
    public:
        T *data;

        BufMat() { data = new T[H*W]; }

        ~BufMat() { delete[] data; }

        T *operator[](size_t i) { return data + i * W; }
};

template <size_t W, size_t H>
class FrameBuffer {
    public:
        BufMat<float, W, H> img;
        BufMat<float, W, H> depth_buffer;

        FrameBuffer() {
            clear();
        }

        void clear() {
            std::fill(img.data, img.data + H*W, 0.f);
            std::fill(depth_buffer.data, depth_buffer.data + H*W, MAX_DEPTH);
        }

        void render(Mesh& mesh, Camera& camera, Vec3<float>& light, LightSource light_source) {
            clear();
            // Initialize for LightSource::DIRECTIONAL
            Vec3<float> light_dir = -light / norm(light);

            for(size_t i = 0; i < mesh.n_triangles; ++i) {
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

                // Bounding box
                float x_min = std::min(std::min(p1.x, p2.x), p3.x);
                x_min = std::max((float)0, x_min);
                float x_max = std::max(std::max(p1.x, p2.x), p3.x);
                x_max = std::min((float)(W-1), x_max);
                float y_min = std::min(std::min(p1.y, p2.y), p3.y);
                y_min = std::max((float)0, y_min);
                float y_max = std::max(std::max(p1.y, p2.y), p3.y);
                y_max = std::min((float)(H-1), y_max);
                TriangleRasterizer rasterizer(p1d, p2d, p3d);

                for(size_t x = x_min; x <= x_max; ++x) {
                    for(size_t y = y_min; y <= y_max; ++y) {
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
                                Vec3<float> light_dir_long = P - light;
                                light_dir = light_dir_long/norm(light_dir_long);
                            }
                            float shading = std::max((float)0, dot(mesh.normals[i], -light_dir));
                            img[y][x] = shading;
                        }
                    }
                }
            }
        }
};
