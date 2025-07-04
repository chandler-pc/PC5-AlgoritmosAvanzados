#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector.h"

class Mesh {

    void calculate_normals() {
        normals.resize(vertices.size(), Vector3(0.0f, 0.0f, 0.0f));

        for (const auto& face : faces) {
            auto& v0 = vertices[face.x];
            auto& v1 = vertices[face.y];
            auto& v2 = vertices[face.z];

            Vector3<float> edge1 = v1 - v0;
            Vector3<float> edge2 = v2 - v0;
            Vector3<float> face_normal = edge1.cross(edge2).normalized();

            normals[face.x] += face_normal;
            normals[face.y] += face_normal;
            normals[face.z] += face_normal;
        }

        for (auto& normal : normals) {
            normal = normal.normalized();
        }
    }
public:
    std::vector<Vector3<float>> vertices;
    std::vector<Vector3<int>> faces;
    std::vector<Vector3<float>> normals;

    bool load_from_obj(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open OBJ file: " << filename << "\n";
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }
            else if (prefix == "f") {
                int v1, v2, v3;
                if (line.find('/') != std::string::npos) {
                    std::replace(line.begin(), line.end(), '/', ' ');
                    std::istringstream iss2(line);
                    iss2 >> prefix >> v1 >> v2 >> v3;
                }
                else {
                    iss >> v1 >> v2 >> v3;
                }
                faces.emplace_back(v1 - 1, v2 - 1,v3 - 1);
			}
			else if (prefix == "vn") {
				float nx, ny, nz;
				iss >> nx >> ny >> nz;
				normals.emplace_back(nx, ny, nz);
			}
        }

        file.close();
        if (normals.empty())
        {
            calculate_normals();
        }
        std::cout << "Loaded OBJ: " << filename << " | Vertices: "
            << vertices.size() << " | Faces: " << faces.size() << "\n";

        return true;
    }

    [[nodiscard]] std::vector<Vector2<int>> get_edges() const {
        std::vector<Vector2<int>> edges;
        for (const auto& face : faces) {
            int v1 = (face.x);
            int v2 = (face.y);
            int v3 = (face.z);

            edges.emplace_back(v1, v2);
            edges.emplace_back(v2, v3);
            edges.emplace_back(v3, v1);
        }
        return edges;
    }
};
