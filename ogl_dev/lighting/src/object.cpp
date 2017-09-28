#include "object.h"

Object::~Object() {
    Vertices.clear();
    Indices.clear();
}

Object::Object(std::string fileName) {
    Assimp::Importer importer;
    //Note~ ASSIMP expects a newline at the end of the object file...otherwise it will not read the last face
    const aiScene *scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | //make triangles
                                                               aiProcess_JoinIdenticalVertices |
                                                               aiProcess_GenUVCoords |  //generate Texture coordinates
                                                               aiProcess_GenSmoothNormals); //normals

    if (scene == nullptr) {
        std::cerr << "Unable to load object file " << fileName << ". Error: \n"
                  << importer.GetErrorString() << std::endl;
        return;
    }

    //todo: loading objects using assimp could be a separate class entirely
    if (scene->mNumMeshes > 1) {
        std::cerr << "More than one mesh in object file " << fileName << std::endl;
        return;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;

    for (int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[i];

        getVertices(mesh, vertices);
        getFaces(mesh, faces);
        getNormals(mesh, normals);
        getTextureCoordinates(mesh, textureCoordinates);

        buildGeometry(vertices, faces, normals, textureCoordinates);
    }

    angle = 0.0f;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void Object::buildGeometry(std::vector<glm::vec3> &vertices,
                           std::vector<glm::uvec3> &faces,
                           std::vector<glm::vec3> &normals,
                           std::vector<glm::vec2> &texCoordinates) {
    Vertex temp(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0));

    for (int i = 0; i < vertices.size(); ++i) {
        temp.vertex = vertices[i];

        if (!normals.empty()) {
            temp.normal = normals[i];
        }

        if (!texCoordinates.empty()) {
            temp.textureCoordinates = texCoordinates[i];
        }

        temp.color = glm::vec3(float(rand() % 100) / 100.0f,
                               float(rand() % 100) / 100.0f,
                               float(rand() % 100) / 100.0f);

        Vertices.push_back(temp);
    }

    //each face contains indices; first index needs to be 0 so subtracting by one
    for (auto face : faces) {
        Indices.push_back(face.x);
        Indices.push_back(face.y);
        Indices.push_back(face.z);
    }
}

void Object::getVertices(const aiMesh *mesh, std::vector<glm::vec3> &vertices) {
    glm::vec3 vertex;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D temp_vertex = mesh->mVertices[i];
        vertex.x = temp_vertex.x;
        vertex.y = temp_vertex.y;
        vertex.z = temp_vertex.z;

        vertices.push_back(vertex);
    }
}

void Object::getFaces(const aiMesh *mesh, std::vector<glm::uvec3> &faces) {
    glm::uvec3 tempFace;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        tempFace.x = face.mIndices[0];
        tempFace.y = face.mIndices[1];
        tempFace.z = face.mIndices[2];

        faces.push_back(tempFace);
    }
}

void Object::getNormals(const aiMesh *mesh, std::vector<glm::vec3> &normals) {
    glm::vec3 normal;

    if (mesh->HasNormals()) {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D temp_vertex = mesh->mNormals[i];
            normal.x = temp_vertex.x;
            normal.y = temp_vertex.y;
            normal.z = temp_vertex.z;

            normals.push_back(normal);
        }
    }
}

void Object::getTextureCoordinates(const aiMesh *mesh, std::vector<glm::vec2> &texCoords) {
    glm::vec2 texCoord;

    if (mesh->HasTextureCoords(0)) {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D temp_vertex = mesh->mTextureCoords[0][i];
            texCoord.x = temp_vertex.x;
            texCoord.y = temp_vertex.y;

            texCoords.push_back(texCoord);
        }
    }
}

void Object::Update(unsigned int dt) {
    angle += dt * M_PI / 10000;
//    model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Object::GetModel() {
    return model;
}

void Object::Render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    TextureManager::getInstance()->setTextureUnit(0);
//    TextureManager::getInstance()->enableTexture("chessboard", GL_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, textureCoordinates));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
//    TextureManager::getInstance()->disableTexture("chessboard");
}

