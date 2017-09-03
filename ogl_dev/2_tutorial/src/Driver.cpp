#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

struct Vertex {
    GLfloat vector3[3];
};

GLuint vbo_geometry;

//gl methods
void registerGlutCallbacks();
void render();

//resource management
void initializeGeometry();
void cleanUp();

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 2");

    registerGlutCallbacks();

    GLenum initResult = glewInit();
    if (initResult != GLEW_OK) {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(initResult) << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    initializeGeometry();

    glutMainLoop();
 
    cleanUp();

    return 0;
}

void registerGlutCallbacks() {
    glutDisplayFunc(render);
}

void initializeGeometry() {
    Vertex geometry[] = {{0.0, 0.0, 0.0}};

    glGenBuffers(1, &vbo_geometry);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(geometry), 
                 geometry, 
                 GL_STATIC_DRAW);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, 1);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void cleanUp() {
    // Clean up, Clean up
    glDeleteBuffers(1, &vbo_geometry);
}

