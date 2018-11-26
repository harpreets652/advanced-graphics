#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

//GLUT callback functions
void render();

int main (int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("test window");
    
    glutDisplayFunc(render);

    glClearColor(0.0f, 0.5f, 0.0f, 0.0f);

    glutMainLoop();

    return 0;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}


