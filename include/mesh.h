#pragma once

#include "linalg.h"
#include <string>

class Mesh {
    public:
        Vec3<float> *vertices;
        int n_vertices;
        Vec3<int> *triangles;
        int n_triangles;
        Vec3<float> *normals;
        
        Mesh(std::string filename);
        ~Mesh();
        void compute_normals();
};
