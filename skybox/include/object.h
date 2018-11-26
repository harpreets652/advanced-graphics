#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "graphics_headers.h"
#include "TextureManager.h"

class Object {
public:
    Object(std::string fileName);

    ~Object();

    void setColorTextureId(std::string textId);

    void setNormalMapTextureId(std::string textId);

    void Update(unsigned int dt);

    void Render();

    void SimpleRender();

    void ShadowRender();

    glm::mat4 GetModel();

    void SetModel(glm::mat4 newModel);

private:
    void getVertices(const aiMesh *mesh, std::vector<glm::vec3> &vertices);

    void getFaces(const aiMesh *mesh, std::vector<glm::uvec3> &faces);

    void getNormals(const aiMesh *mesh, std::vector<glm::vec3> &normals);

    void getTangents(const aiMesh *mesh, std::vector<glm::vec3> &tangents);

    void getTextureCoordinates(const aiMesh *mesh, std::vector<glm::vec2> &texCoords);

    void buildGeometry(std::vector<glm::vec3> &vertices, std::vector<glm::uvec3> &faces,
                       std::vector<glm::vec3> &normals, std::vector<glm::vec2> &texCoordinates,
                       std::vector<glm::vec3> &tangents);

    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    std::string colorTextureId;
    std::string normalMapTextureId;

    float angle;
};

#endif /* OBJECT_H */
