// gcc snowman.cpp -lglut -lGL -lGLU -o  snowman && ./snowman
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
 
float angle = 300.0;

void keyboard(unsigned char kb, int x, int y)
{
	switch(kb)
	{
		case 27: exit(0);
		default: break;
	}
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glLoadIdentity();
 
	gluLookAt(4.2, 1.4, 0.0,
			  0.0, 0.4, 0.0,
			  0.0, 1.0, 0.0);
 
	angle += 0.5;
	if (angle > 360.0) angle = 0;
 
	glRotatef(angle, 0, 1, 0);
	glColor3f(0.8, 0.8, 0.8);
 
	glPushMatrix();
		glTranslatef(0.0, 0.2, 0.0);
		glPushMatrix();
			glTranslatef(0.0, -0.298, 0.0);
			glRotatef(90, 1, 0, 0);
			glutSolidCylinder(1.13, 0.002, 11, 1);
		glPopMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidSphere(0.45, 11, 11);
		glTranslatef(0.0, 0.45, 0.0);
		glutSolidSphere(0.3, 11, 11);
		glTranslatef(0.0, 0.35, 0.0);
		glutSolidSphere(0.25, 11, 11);

		// Olhos
		glColor3f(0.4, 0.38, 0.36);
		// Direito
		glPushMatrix();
			glRotatef( 26.0, 0, 1, 0);
			glTranslatef(0.21, 0.08, 0.0);
			glColor3f(0.6, 0.4, 0.1);
			glutSolidSphere(0.04, 8, 8);
		glPopMatrix();

		// Esquerdo
		glPushMatrix();
			glRotatef(-26.0, 0, 1, 0);
			glTranslatef(0.21, 0.08, 0.0);
			glutSolidSphere(0.04, 8, 8);
		glPopMatrix();
 
		// Nariz
		glPushMatrix();
			glTranslatef(0.22, -0.01, 0.0);
			glRotatef(90, 0, 1, 0);
			glColor3f(0.7, 0.4, 0.2);
			glutSolidCone(0.03, 0.18, 8, 6);
		glPopMatrix();
		
		// Base do globo de neve
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
			glTranslatef(0.0, -1.1, 0.0);
			glColor4f(0.12, 0.1, 0.1, 1.0);
			glRotatef(90, 1, 0, 0);
			glutSolidCylinder(1.23, 0.4, 11, 1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -1.13, 0.0);
			glColor4f(0.7, 0.0, 0.0, 1.0);
			glPushMatrix();
				glRotatef(90, 1, 0, 0);
				glutSolidCylinder(1.3, 0.4, 11, 1);
			glPopMatrix();
			glTranslatef(0.0, -0.08, 0.0);
			glColor4f(0.0, 0.8, 0.0, 1.0);
			glPushMatrix();
				glRotatef(90, 1, 0, 0);
				glutSolidCylinder(1.308, 0.24, 11, 1);
			glPopMatrix();
		glPopMatrix();
 
		// Globo de neve
		glPushMatrix();
			glColor4f(0.2, 0.2, 0.2, 0.2);
			glTranslatef(0.0, -0.5, 0.0);
			glRotatef(90.f, 1, 0, 0);
			glutSolidSphere(1.3, 11, 11);
		glPopMatrix();
		glDisable(GL_BLEND);
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
	glutCreateWindow("Boneco de Neve");
 
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
 
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspect = (GLfloat) width / height;
	gluPerspective(45, aspect, .01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
 
	GLfloat mat_shininess[] =  { 15.0 };
	GLfloat mat_specular[] =    { 0.75, 0.75, 0.75, 0.75 };
 
	GLfloat light_ambient[] =  {   0.6, 0.6, 0.6, 1.0 };
	GLfloat light_diffuse[] =  {   0.8, 0.8, 0.8, 0.8 };
	GLfloat light_specular[] = {   1.0, 1.0, 1.0, 0.3 };
	GLfloat light_position[] = {   6.0, 6.0, 2.0, 0.0 };
 
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
