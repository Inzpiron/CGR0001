// make
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "am_3dShapes.hpp"

float angle = 0;
float bob = 0;
unsigned int count;

void keyboard(unsigned char kb, int x, int y)
{
	switch(kb)
	{
		case 27: exit(0);
		default: break;
	}
}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.08, 0.4, 0.9, 1.0);
	glLoadIdentity();
 
	gluLookAt(20.0, 10.0, 20.0,
			   0.0,  2.0,  0.0,
			   0.0,  1.0,  0.0);
 
	angle += 0.25;
	if (angle > 360.0) angle = 0;
 
	glRotatef(angle, 0, 1, 0);
 
	glPushMatrix();
		// Chão do mundo
		glColor3f(0.1, 0.6, 0.08);
		glBegin(GL_QUADS);
			glNormal3f(  0.0, 1.0,   0.0);
			glVertex3f(-30.0, 0.0, -30.0);
			glVertex3f(-30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0, -30.0);
		glEnd();

		// Chão do castelo
		glColor3f(0.3, 0.3, 0.2);
		glBegin(GL_QUADS);
			glNormal3f( 0.0,  1.0,  0.0);
			glVertex3f(-8.0, 0.01, -8.0);
			glVertex3f(-8.0, 0.01,  8.0);
			glVertex3f( 8.0, 0.01,  8.0);
			glVertex3f( 8.0, 0.01, -8.0);
		glEnd();

		// Torre central
		glPushMatrix();
			glColor3f(0.7, 0.2, 0.05);
			glTranslatef(0.0, 3.25, 0.0);
			glPushMatrix();
				glScaled(2.0, 6.5, 2.0);
				glutSolidCube(1.0);
			glPopMatrix();
			glColor3f(0.5, 0.3, 0.1);
			glTranslatef(0.0, 4.75, 0.0);
			SolidPrismaTriangular(2.3, 3, 2.3);
		glPopMatrix();

		// Cor das torres e paredes
		glColor3f(0.3, 0.3, 0.3);

		// Torre 1
		glPushMatrix();
			glTranslatef(8.0, 0.0, 8.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCylinder(1.0, 6.0, 12, 1);
			glRotated( 90.0, 1, 0, 0);
			glColor3f(0.0, 0.0, 0.0);
			// Telhado
			glColor3f(0.9, 0.5, 0.1);
			glTranslatef( 0.0,  6.0, 0.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCone(1.2, 2.1, 12, 12);
		glPopMatrix();

		// Parede 1-2
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef( 8.0, 1.8, 0.0);
			glScaled(1.0, 3.6, 16.0);
			glutSolidCube(1.0);
		glPopMatrix();

		// Torre 2
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(8.0, 0.0, -8.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCylinder(1.0, 6.0, 12, 1);
			glRotated( 90.0, 1, 0, 0);
			// Telhado
			glColor3f(0.9, 0.5, 0.1);
			glTranslatef( 0.0,  6.0,  0.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCone(1.2, 2.1, 12, 12);
		glPopMatrix();

		// Parede 2-3
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef( 0.0, 1.8, -8.0);
			glScaled(16.0, 3.6, 1.0);
			glutSolidCube(1.0);
		glPopMatrix();
			
		// Torre 3
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(-8.0, 0.0, -8.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCylinder(1.0, 6.0, 12, 1);
			glRotated( 90.0, 1, 0, 0);
			// Telhado
			glColor3f(0.9, 0.5, 0.1);
			glTranslatef( 0.0,  6.0,  0.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCone(1.2, 2.1, 12, 12);
		glPopMatrix();

		// Parede 3-4
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef( -8.0, 1.8, 0.0);
			glScaled(1.0, 3.6, 16.0);
			glutSolidCube(1.0);
		glPopMatrix();

		// Torre 4
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(-8.0, 0.0, 8.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCylinder(1.0, 6.0, 12, 1);
			glRotated( 90.0, 1, 0, 0);
			// Telhado
			glColor3f(0.9, 0.5, 0.1);
			glTranslatef( 0.0, 6.0, 0.0);
			glRotated(-90.0, 1, 0, 0);
			glutSolidCone(1.2, 2.1, 12, 12);
		glPopMatrix();

		// Parede 4-1
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(0.0, 1.8, 8.0);
			glScaled(16.0, 3.6, 1.0);
			glutSolidCube(1.0);
		glPopMatrix();

		// Enable transparency blend
		/* Não vamos usar nesse trabalho
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_BLEND);
		*/
	glPopMatrix();
 
	glFlush();
	glutSwapBuffers();
}

int main (int argc, char **argv)
{
	int width  = 800,
		height = 600;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Castelo");
 
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);

	glViewport(0, 0, width, height);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	GLfloat aspect = (GLfloat) width / height;
	gluPerspective(45, aspect, .01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
 
	GLfloat mat_shininess[]  = { 8.0 };
	GLfloat mat_specular[]   = { 0.75, 0.75, 0.75, 0.75 };
 
	GLfloat light_ambient[]  = { 0.4, 0.4, 0.4, 1.0};
	GLfloat light_diffuse[]  = { 0.8, 0.8, 0.8, 0.9};
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 0.1};
	GLfloat light_position[] = { 6.0, 6.0, 2.0, 0.0};
 
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
 
	glEnable(GL_COLOR_MATERIAL);
 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
 
	glutMainLoop();
 
	return 0;
}