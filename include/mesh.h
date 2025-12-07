#pragma once

#include "linalg.h"
#include <string>

class Mesh {
    public:
        Vector3f *vertices;
        size_t n_vertices;
        Vec3<size_t> *triangles;
        size_t n_triangles;
        Vector3f *normals;
        
        Mesh(std::string filename);
        ~Mesh();
        void compute_normals();
};
