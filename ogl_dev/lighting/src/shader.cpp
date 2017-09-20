#include "shader.h"

Shader::Shader() {
    m_shaderProg = 0;
}

Shader::~Shader() {
    for (unsigned int &it : m_shaderObjList) {
        glDeleteShader(it);
    }

    if (m_shaderProg != 0) {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}

bool Shader::initialize() {
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0) {
        std::cerr << "Error creating shader program\n";
        return false;
    }

    return true;
}


// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::addShaderFromFile(std::string fileName, GLenum shaderType) {
    std::string shaderText;

    if (!readFile(fileName, shaderText)) {
        std::cerr << "Error reading shader from file " << fileName << " for " << shaderType << std::endl;
        return false;
    }

    return addShaderFromText(shaderText, shaderType);
}

bool Shader::addShaderFromText(std::string &shaderText, GLenum shaderType) {
    GLuint ShaderObj = glCreateShader(shaderType);

    if (ShaderObj == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        return false;
    }

    // Save the shader object - will be deleted in the destructor
    m_shaderObjList.push_back(ShaderObj);

    const GLchar *p[1];
    p[0] = shaderText.c_str();
    GLint Lengths[1] = {(GLint) shaderText.size()};

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, nullptr, InfoLog);
        std::cerr << "Error compiling: " << InfoLog << std::endl;
        return false;
    }

    glAttachShader(m_shaderProg, ShaderObj);

    return true;
}

bool Shader::readFile(std::string &fileName, std::string &output) {
    std::ifstream f(fileName);

    bool ret = false;
    if (f.is_open()) {
        std::string line;
        while (getline(f, line)) {
            output.append(line);
            output.append("\n");
        }

        f.close();

        ret = true;
    } else {
        std::cerr << "Could not open file : " << fileName << std::endl;
    }

    return ret;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::finalize() {
    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), nullptr, ErrorLog);
        std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
        return false;
    }

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), nullptr, ErrorLog);
        std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
        return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (unsigned int &it : m_shaderObjList) {
        glDeleteShader(it);
    }

    m_shaderObjList.clear();

    return true;
}


void Shader::enable() {
    glUseProgram(m_shaderProg);
}


GLint Shader::getUniformLocation(const char *pUniformName) {
    GLint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}
