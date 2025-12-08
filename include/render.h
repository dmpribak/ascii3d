#include "mesh.h"
#include "camera.h"
#include "raster.h"
#include "linalg.h"
#include <cassert>
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

        FrameBuffer();

        void clear();

        void render(Mesh& mesh, Camera& camera, Vector3f light, LightSource light_source);

        bool render_pixel(
                size_t x, size_t y, 
                Vector3f P1, Vector3f P2, Vector3f P3, 
                float d1, float d2, float d3, float d_min,
                Vector3f face_normal, 
                Vector3f light, 
                LightSource light_source,
                TriangleRasterizer &rasterizer,
                RasterIterator &ri);
};

template <size_t W, size_t H>
FrameBuffer<W, H>::FrameBuffer() {
    clear();
}

template <size_t W, size_t H>
void FrameBuffer<W, H>::clear() {
    std::fill(img.data, img.data + H*W, 0.f);
    std::fill(depth_buffer.data, depth_buffer.data + H*W, MAX_DEPTH);
}

template <size_t W, size_t H>
void FrameBuffer<W, H>::render(Mesh& mesh, Camera& camera, Vector3f light, LightSource light_source) {
    clear();

    for(size_t i = 0; i < mesh.n_triangles; ++i) {
        Vector3f P1 = mesh.vertices[mesh.triangles[i].x];
        Vector3f P2 = mesh.vertices[mesh.triangles[i].y];
        Vector3f P3 = mesh.vertices[mesh.triangles[i].z];

        Vector3f p1d = camera.project(P1);
        Vector3f p2d = camera.project(P2);
        Vector3f p3d = camera.project(P3);

        float d1 = p1d.z();
        float d2 = p2d.z();
        float d3 = p3d.z();
        float d_min = std::min(std::min(d1,d2), d3);
        
        // Bounding box
        float x_min = std::min(std::min(p1d.x(), p2d.x()), p3d.x());
        x_min = std::max(0.f, x_min);
        float x_max = std::max(std::max(p1d.x(), p2d.x()), p3d.x());
        x_max = std::min((float)(W-1), x_max);
        float y_min = std::min(std::min(p1d.y(), p2d.y()), p3d.y());
        y_min = std::max(0.f, y_min);
        float y_max = std::max(std::max(p1d.y(), p2d.y()), p3d.y());
        y_max = std::min((float)(H-1), y_max);
        TriangleRasterizer rasterizer(p1d, p2d, p3d);

        size_t x_mid = (x_min+x_max)/2;

        for(size_t y = y_min; y <= y_max; ++y) {
            bool inside_found = false;

            Vector2f iter_start1(x_mid, y);
            RasterIterator ri = rasterizer.xiter(iter_start1);
            for(size_t x = x_mid; x <= x_max && x >= x_min; ++x) {
                Vector2f curr(x, y);
                // Depth culling and inside check
                bool inside = *ri++;
                if(!inside && inside_found) break;
                if(!inside || d_min > depth_buffer[H-y-1][x]) continue;

                bool rendered = render_pixel(x, y, P1, P2, P3, d1, d2, d3, d_min, mesh.normals[i], light, light_source, rasterizer, ri);
                if(!rendered && inside_found) break;
                else if(rendered) inside_found = true;
            }

            Vector2f iter_start2(x_mid, y);
            ri = rasterizer.xiter(iter_start2);
            for(size_t x = x_mid; x >= x_min && x <= x_max; --x) {
                Vector2f curr(x, y);
                // Depth culling and inside check
                bool inside = *ri--;
                if(!inside && inside_found) break;
                if(!inside || d_min > depth_buffer[H-y-1][x]) continue;

                bool rendered = render_pixel(x, y, P1, P2, P3, d1, d2, d3, d_min, mesh.normals[i], light, light_source, rasterizer, ri);
                if(!rendered && inside_found) break;
                else if(rendered) inside_found = true;
            }
        }
    }
}

template <size_t W, size_t H>
bool FrameBuffer<W, H>::render_pixel(
        size_t x, size_t y, 
        Vector3f P1, Vector3f P2, Vector3f P3, 
        float d1, float d2, float d3, float d_min,
        Vector3f face_normal, 
        Vector3f light, 
        LightSource light_source,
        TriangleRasterizer &rasterizer,
        RasterIterator &ri) {

        Vector2f p(x, y);

        // Visibility check
        float d = rasterizer.interp(d1, d2, d3, p);
        if(d > depth_buffer[H-y-1][x]) return false;
        depth_buffer[H-y-1][x] = d;

        // Shading
        Vector3f light_dir;
        switch(light_source) {
            case LightSource::PUNCTUAL: {
                    Vector3f P = rasterizer.interp(P1, P2, P3, p);
                    Vector3f light_dir_long = P - light;
                                            
                    light_dir = light_dir_long/light_dir_long.norm();
                }
                break;
            case LightSource::DIRECTIONAL:
                light_dir = -light / light.norm();
                break;
        }
        float shading = std::max(0.f, face_normal.dot(-light_dir));
        img[H-y-1][x] = shading;

        return true;
}
