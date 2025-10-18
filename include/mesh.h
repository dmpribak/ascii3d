#pragma once

#include "linalg.h"
#include <string>

class Mesh {
    public:
        Vec3<float> *vertices;
        size_t n_vertices;
        Vec3<size_t> *triangles;
        size_t n_triangles;
        Vec3<float> *normals;
        
        Mesh(std::string filename);
        ~Mesh();
        void compute_normals();
};
