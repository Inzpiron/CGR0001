#ifndef _AM_3DSHAPES_
#define _AM_3DSHAPES_

/*
 * Coleção de funções para OpenGL
 * Autor: Lucas Litter Mentz
 */

// Todos funcionam mas as sombras ficam um pouco estranhas no SolidCube.

#include <cstdlib>
#include <GL/gl.h>
#include <math.h>


void ObtainTriangleFaceNormal(GLfloat* n, GLfloat v1[], GLfloat v2[], GLfloat v3[])
{
	GLfloat l = 0.0;
	for (int i = 0; i < 3; i++)
		n[i] = (v1[i] + v2[i] + v3[i]) / 3;
	l = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	for (int i = 0; i < 3; i++)
		n[i] /= -l;
}

void ObtainQuadFaceNormal(GLfloat* n, GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[])
{
	GLfloat l = 0.0;
	for (int i = 0; i < 3; i++)
		n[i] = (v1[i] + v2[i] + v3[i] + v4[i]) / 4;
	l = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	for (int i = 0; i < 3; i++)
		n[i] /= -l;
}

// Esse aqui está com problemas no cálculo das normais :(
void SolidCube(GLfloat m, GLfloat h, GLfloat n)
{
	m /= 2; h /= 2; n /= 2;
	GLfloat normal[3];
	GLfloat v[8][3] = {{ -m, -h, -n }, { -m, +h, -n }, { +m, +h, -n },
					   { +m, -h, -n }, { -m, -h, +n }, { -m, +h, +n },
					   { +m, +h, +n }, { +m, -h, +n }};

	// Como fazer um cubo visto de frente
	// Base
	ObtainQuadFaceNormal(normal, v[0], v[3], v[7], v[4]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[0]);
		glVertex3fv(v[3]);
		glVertex3fv(v[7]);
		glVertex3fv(v[4]);
	glEnd();

	// Frente
	ObtainQuadFaceNormal(normal, v[0], v[3], v[2], v[1]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[0]);
		glVertex3fv(v[3]);
		glVertex3fv(v[2]);
		glVertex3fv(v[1]);
	glEnd();

	// Direita
	ObtainQuadFaceNormal(normal, v[3], v[7], v[6], v[2]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[3]);
		glVertex3fv(v[7]);
		glVertex3fv(v[6]);
		glVertex3fv(v[2]);
	glEnd();

	// Esquerda
	ObtainQuadFaceNormal(normal, v[0], v[1], v[5], v[4]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[5]);
		glVertex3fv(v[4]);
	glEnd();

	// Trás
	ObtainQuadFaceNormal(normal, v[4], v[7], v[6], v[5]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[4]);
		glVertex3fv(v[7]);
		glVertex3fv(v[6]);
		glVertex3fv(v[5]);
	glEnd();

	// Topo
	ObtainQuadFaceNormal(normal, v[1], v[2], v[6], v[5]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]);
		glVertex3fv(v[6]);
		glVertex3fv(v[5]);
	glEnd();
}

void SolidPrismaTriangular(GLfloat m, GLfloat h, GLfloat n)
{
	m /= 2; h /= 2; n /= 2;
	GLfloat normal[3];
	GLfloat v[6][3] = {{ -m, -h, -n }, {  0, +h, -n }, { +m, -h, -n },
					   { -m, -h, +n }, {  0, +h, +n }, { +m, -h, +n }};

	// Como fazer um prisma triangular visto de frente
	// Direita
	ObtainQuadFaceNormal(normal, v[1], v[2], v[5], v[4]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]);
		glVertex3fv(v[5]);
		glVertex3fv(v[4]);
	glEnd();

	// Esquerda
	ObtainQuadFaceNormal(normal, v[0], v[1], v[4], v[3]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[4]);
		glVertex3fv(v[3]);
	glEnd();

	// Base
	ObtainQuadFaceNormal(normal, v[0], v[2], v[5], v[3]);
	glNormal3fv(normal);
	glBegin(GL_QUADS);
		glVertex3fv(v[0]);
		glVertex3fv(v[2]);
		glVertex3fv(v[5]);
		glVertex3fv(v[3]);
	glEnd();

	// Frente
	ObtainTriangleFaceNormal(normal, v[0], v[2], v[1]);
	glNormal3fv(normal);
	glBegin(GL_TRIANGLES);
		glVertex3fv(v[0]);
		glVertex3fv(v[2]);
		glVertex3fv(v[1]);
	glEnd();

	// Trás
	ObtainTriangleFaceNormal(normal, v[3], v[4], v[5]);
	glNormal3fv(normal);
	glBegin(GL_TRIANGLES);
		glVertex3fv(v[3]);
		glVertex3fv(v[4]);
		glVertex3fv(v[5]);
	glEnd();
}

void SolidCylinder(GLfloat diameter, GLfloat height, GLint sides)
{
	GLfloat radius = diameter / 2; height /= 2;
	GLfloat x, z, l;

	// Como fazer um cilindro de pé visto de frente
	// Base
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, -height, 0.0);
		for (int i = 0; i <= sides; i++)
		{
			x = cos((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			z = sin((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			glVertex3f(x, -height, z);
		}
	glEnd();

	// Topo
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, height, 0.0);
		for (int i = 0; i <= sides; i++)
		{
			x = cos((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			z = sin((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			glVertex3f(x, height, z);
		}
	glEnd();

	// Corpo
	glBegin(GL_QUAD_STRIP);
		for (int i = 0; i <= sides; i++)
		{
			x = cos((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			z = sin((2.0 * M_PI * i)/(GLdouble)sides) * radius;
			glVertex3f(x, -height, z);
			glVertex3f(x,  height, z);
		}
	glEnd();
}

#endif