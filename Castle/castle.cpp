//gcc castle.c -lglut -lGL -lGLU -lm -o castle && ./castle
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

void drawScene(void);
void reshape(GLsizei width, GLsizei height);
float angle = 0;
float bob = 0;
unsigned int count;


        /* Como fazer um cubo
        glBegin(GL_QUADS);
            // Base
            glVertex3f(-0.2,  0.0, -0.2);
            glVertex3f(-0.2,  0.0,  0.2);
            glVertex3f( 0.2,  0.0,  0.2);
            glVertex3f( 0.2,  0.0, -0.2);
 
            // Lado 1
            glVertex3f( 0.2,  0.0, -0.2);
            glVertex3f( 0.2,  0.4, -0.2);
            glVertex3f(-0.2,  0.4, -0.2);
            glVertex3f(-0.2,  0.0, -0.2);
 
            // Lado 2 ...
        glEnd();
        */

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(800,600);
	glutCreateWindow("Boneco de Neve");

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	//glutReshapeFunc(reshape);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspect = (GLfloat) 800 / 600;
	gluPerspective(45, aspect, 2.0f, 15.0f);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel( GL_SMOOTH );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();

	return 0;
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 5.0, 1.0,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	//glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();
	glClearColor(0.2, 0.2, 0.2, 1.0);
	
	angle += 1;
	if(angle >= 360)
		angle = 0;
	bob = sin(count++ / (M_PI * 12)) / 16.0;

	glRotated(angle, 0, 1, 0);
	//glTranslated(0.0, bob, 0.0);

	glPushMatrix();
	{
		//Chão
		glColor4f(0.1, 0.8, 0.08, 1);
		glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			glTranslated(0.0, 0.0, 0.5);
			glutSolidCylinder(0.8, 0.1, 8, 1);
		glPopMatrix();

		/*
		//Corpo e cabeça
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
		*/
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}