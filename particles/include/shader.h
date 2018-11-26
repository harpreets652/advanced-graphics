#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
#include <fstream>

#include "graphics_headers.h"

class Shader {
public:
    Shader();

    ~Shader();

    bool initialize();

    bool addShaderFromFile(std::string fileName, GLenum shaderType);

    bool addShaderFromText(std::string &shaderText, GLenum shaderType);

    void enable();

    bool finalize();

    GLint getUniformLocation(const char *pUniformName);

    GLuint getShaderProgram();

private:
    bool readFile(std::string &fileName, std::string &output);

    GLuint m_shaderProg;
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
