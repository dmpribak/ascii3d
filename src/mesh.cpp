#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "mesh.h"
#include "linalg.h"

Mesh::Mesh(std::string filename) {
    std::ifstream file(filename);
    if(!file) std::cout << "Failed to open file " << filename << std::endl;

    std::string line;
    std::getline(file, line);
    if(line != "ply") throw std::runtime_error("Invalid PLY file!");
    std::getline(file, line);
    while(std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::string word;
        line_stream >> word;
        if(word == "element") {
            line_stream >> word;
            if(word == "vertex") {
                line_stream >> word;
                n_vertices = std::stoi(word);
            } else if(word == "face") {
                line_stream >> word;
                n_triangles = std::stoi(word);
            }
        } else if(word == "end_header") break;
    }

    // Reserve memory for data
    vertices = new Vector3f[n_vertices];
    triangles = new Vec3<size_t>[n_triangles];
    normals = new Vector3f[n_triangles];

    // Read vertex coordinates
    for(size_t i = 0; i < n_vertices; ++i) {
        std::getline(file, line);
        std::istringstream line_stream(line);
        std::string word;
        float coords[3];
        for(size_t j = 0; j < 3; ++j) {
            line_stream >> word;
            coords[j] = std::stof(word);
        }
        vertices[i] = Vector3f(coords[0], coords[1], coords[2]);
    }

    // Read triangle indices
    for(size_t i = 0; i < n_triangles; ++i) {
        std::getline(file, line);
        std::istringstream line_stream(line);
        std::string word;
        line_stream >> word; // Absorb the first number (number of vertices in face)
        size_t num_vertices = std::stoi(word);
        float indices[3];
        for(size_t j = 0; j < 3; ++j) {
            line_stream >> word;
            indices[j] = std::stof(word);
        }
        triangles[i] = Vec3<size_t>(indices[0], indices[1], indices[2]);
    }

    // Compute face normals
    compute_normals();
}

Mesh::~Mesh() {
    delete[] vertices;
    delete[] triangles;
}

void Mesh::compute_normals() {
    for(size_t i = 0; i < n_triangles; ++i) {
        Vector3f v1 = vertices[triangles[i].x];
        Vector3f v2 = vertices[triangles[i].y];
        Vector3f v3 = vertices[triangles[i].z];

        Vector3f e1 = v2 - v1;
        Vector3f e2 = v3 - v1;
        Vector3f e3 = e1.cross(e2);
        Vector3f normal = e3 / e3.norm();
        normals[i] = normal;
    }
}
