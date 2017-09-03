#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <cassert>

using namespace std;

struct Vertex {
    GLfloat vector3[3];
};

GLuint vbo_geometry; //vertex buffer object
GLuint program; //The GLSL program handle
GLuint gScaleLocation;

//gl methods
void registerGlutCallbacks();
void render();
void update();

//resource management
void initializeGeometry();
void initializeShaders();
void addShader(GLuint ShaderProgram, const char* pShaderText, 
               GLenum ShaderType);
bool readFile(string pFileName, string &output);
void cleanUp();

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 4");

    registerGlutCallbacks();

    GLenum initResult = glewInit();
    if (initResult != GLEW_OK) {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(initResult) << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    initializeGeometry();
    initializeShaders();

    glutMainLoop();
 
    cleanUp();

    return 0;
}

void registerGlutCallbacks() {
    glutDisplayFunc(render);
    glutIdleFunc(update);
}

void initializeGeometry() {
    Vertex geometry[] = {{-1.0f, -1.0f, 0.0f},
                         {1.0f, -1.0f, 0.0f},
                         {0.0f, 1.0f, 0.0f}};

    glGenBuffers(1, &vbo_geometry);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(geometry), 
                 geometry, 
                 GL_STATIC_DRAW);
}

void initializeShaders() {
    program = glCreateProgram();
    if (program == 0) {
        std::cerr <<  "Error creating shader program" << std::endl;
        exit(1);
    }

    string vertexShader;
    string fragmentShader;

    if (!readFile("../src/vertexShader.vs", vertexShader)) {
        exit(1);
    };

    if (!readFile("../src/fragmentShader.fs", fragmentShader)) {
        exit(1);
    };
     
    addShader(program, vertexShader.c_str(), GL_VERTEX_SHADER);
    addShader(program, fragmentShader.c_str(), GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
        cerr << "Shader program failed to link: " << errorLog << std::endl;
        exit(1);
    }

    //could call this before every draw call
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
        cerr << "Invalid shader program: " << errorLog << endl;
        exit(1);
    }

    glUseProgram(program);

    GLuint gScaleLocation = glGetUniformLocation(program, "gScale");
    assert (gScaleLocation != 0xFFFFFFFF);
}

void addShader(GLuint pProgram, 
               const char* pShaderText, 
               GLenum pShaderType) {
    GLuint shaderObj = glCreateShader(pShaderType);

    if (shaderObj == 0) {
        std::cerr << "Error creating shader type " << pShaderType << std::endl;
        exit(1);
    }

    glShaderSource(shaderObj, 1, &pShaderText, NULL);
    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
        cerr << "Error compiling shader " << pShaderType
             << ". Reason: " << infoLog << endl;
        exit(1);
    }

    glAttachShader(pProgram, shaderObj);
}

bool readFile(string pFileName, string &output) {
    ifstream f(pFileName);

    bool ret = false;
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            output.append(line);
            output.append("\n");
        }
        
        f.close();
        
        ret = true;
    } else {
        cerr << "Could not open file : " << pFileName << endl;
    }
    
    return ret;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void update() {
    static float scale = 0.0;
    scale += 0.001;
    
    glUniform1f(gScaleLocation, sinf(scale));

    glutPostRedisplay();
}


void cleanUp() {
    // Clean up, Clean up
    glDeleteBuffers(1, &vbo_geometry);
}

