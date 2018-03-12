//gcc snowman.c -lglut -lGL -lGLU -o snowman && ./snowman

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>

void drawScene(void);
float angle = 0;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Teste");

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    //glDepthFunc(GL_LEQUAL);

    glutMainLoop();

    return 0;
}

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //glClearColor(0.3, 0.3, 0.3, 0.3);
    angle += 1;
    //if(angle >= 360.0)
    //    angle = 0.0;

    glRotated(angle, 0, 1, 0);

    glPushMatrix();
        //Corpo e cabeça
        glColor4f(0.5, 0.5, 0.5, 1);
        glTranslated(0.0, -0.3, 0);
        glutSolidSphere(0.45, 10, 10);
        glTranslated(0.0, 0.4, 0);
        glutSolidSphere(0.30, 10, 10);
        glTranslated(0.0, 0.35, 0);
        glutSolidSphere(0.25, 10, 10);

        //Detalhes da cabeça
        //Nariz
        glColor4f(0.8, 0.5, 0, 1);
        glTranslated(0.0, 0.0, 0.235);
        glutSolidCone(0.065, 0.25, 10, 10);

        glColor4f(101/255.0, 69/255.0, 26/255.0, 1); // Cor marrom

        //Olho esquerdo
        glTranslated(-0.15, 0.065, -0.075);
        glutSolidSphere(0.055, 10, 10);

        //Olho direito
        glTranslated(0.30, 0, 0);
        glutSolidSphere(0.055, 10, 10);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}
