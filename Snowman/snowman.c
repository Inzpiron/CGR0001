//gcc snowman.c -lglut -lGL -lGLU -lm -o snowman && ./snowman
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

void drawScene(void);
float angle = 0;
float bob = 0;
unsigned int count;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Teste");

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glDepthFunc(GL_LEQUAL);

    glutMainLoop();

    return 0;
}

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glClearColor(0.25, 0.25, 0.25, 0.3);
    angle += 1;
    if(angle >= 360)
        angle = 0;
    bob = sin(count++ / (M_PI * 12)) / 16.0;

    glRotated(angle, 0, 1, 0);
    glTranslated(0.0, bob, 0.0);

    glPushMatrix();
        //Corpo e cabeça
        glColor4f(0.75, 0.75, 0.78, 1);
        glTranslated(0.0, -0.25, 0);
        glutSolidSphere(0.45, 12, 12);
        glTranslated(0.0, 0.4, 0);
        glutSolidSphere(0.30, 12, 12);
        glTranslated(0.0, 0.35, 0);
        glutSolidSphere(0.25, 12, 12);

        //Detalhes da cabeça
        //Nariz
        glColor4f(0.8, 0.5, 0, 1);
        glTranslated(0.0, 0.0, 0.235);
        glutSolidCone(0.065, 0.25, 12, 12);

        glColor4f(101/255.0, 69/255.0, 26/255.0, 1); // Cor marrom

        //Olho esquerdo
        glTranslated(-0.15, 0.065, -0.075);
        glutSolidSphere(0.055, 12, 12);

        //Olho direito
        glTranslated(0.30, 0, 0);
        glutSolidSphere(0.055, 12, 12);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}
