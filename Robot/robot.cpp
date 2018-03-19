// make
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "am_3dShapes.hpp"


class Robot {
private:
	GLfloat legLenght        = 2.0, // É um cilindro
			legDiameter      = 0.3, 
			
			kneeSize         = 0.3, // É uma esfera

			feetLenght       = 0.35, // É um paralelepípedo regular de base retangular
			feetWidth        = 0.15,
			feetHeight       = 0.12,
			
			forearmLenght    = 0.8, // É um cilindro
			armLenght        = 0.9, // É um cilindro
			
			headLenght       = 0.4, // É um cilindro
			headDiameter     = 0.4,
			
			torsoLenght      = 0.35, // É um paralelepípedo regular de base retangular
			torsoWidth       = 0.8,
			torsoHeight      = 1.0;

public:
	GLfloat rightLegFAngle   = 0.0,
			rightLegSAngle   = 0.0,
			leftLegFAngle    = 0.0,
			leftLegSAngle    = 0.0,

			rightKneeAngle   = 0.0,
			leftKneeAngle    = 0.0,

			rightArmFAngle   = 0.0,
			rightArmSAngle   = 0.0,
			leftArmFAngle    = 0.0,
			leftArmSAngle    = 0.0,

			rightElbowFAngle = 0.0,
			rightElbowSAngle = 0.0,
			leftElbowFAngle  = 0.0,
			leftElbowSAngle  = 0.0;

	void draw()
	{
		glPushMatrix();
		{
			// Desenhar torso
			glPushMatrix();
			{
				glColor3f(0.7, 0.5, 0.1);
				SolidCube(torsoWidth, torsoHeight, torsoLenght);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
};

float angle = 0;
float bob = 0;
unsigned int count;
Robot robo;

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
	glClearColor(0.4, 0.4, 0.4, 1.0);
	glLoadIdentity();
 
	gluLookAt(6.0, 2.0, 6.0,
			  0.0, 1.0, 0.0,
			  0.0, 1.0, 0.0);
 
	angle += 0.25;
	if (angle > 360.0) angle = 0;
 
	glRotatef(angle, 0, 1, 0);
 
	glPushMatrix();
		// Chão do mundo
		glColor3f(0.8, 0.4, 0.0);
		glNormal3f(0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glVertex3f(-30.0, 0.0, -30.0);
			glVertex3f(-30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0, -30.0);
		glEnd();

		// Desenhar robô
		glTranslatef(0.0, 1.5, 0.0);
		robo.draw();
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