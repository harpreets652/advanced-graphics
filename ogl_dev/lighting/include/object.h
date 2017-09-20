#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "graphics_headers.h"

class Object {
public:
    Object(std::string fileName);

    ~Object();

    void Update(unsigned int dt);

    void Render();

    glm::mat4 GetModel();

private:
    void getVertices(const aiMesh *mesh, std::vector<glm::vec3> &vertices);

    void getFaces(const aiMesh *mesh, std::vector<glm::uvec3> &faces);

    void getNormals(const aiMesh *mesh, std::vector<glm::vec3> &normals);

    void getTextureCoordinates(const aiMesh *mesh, std::vector<glm::vec2> &texCoords);

    void buildGeometry(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> faces);

    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle;
};

#endif /* OBJECT_H */
