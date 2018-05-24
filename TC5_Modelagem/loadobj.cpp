#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <GL/freeglut.h>
#include "OBJ_Loader.h"

#define WW 800
#define WH 600

int main(int argc, char **argv)
{
	sf::Window window(sf::VideoMode(WW, WH),
					  "Modelo do Blender!",
					  sf::Style::Default,
					  sf::ContextSettings(24));

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(144);

	objl::Loader loader;
	sf::Texture texture;

	if (!loader.LoadFile("Modelos/CuboMagico/BlenderExport/magiccube.obj"))
	{
		std::cerr << "Erro ao carregar modelo .obj\n";
		return 1;
	}

	if (!texture.loadFromFile("Modelos/CuboMagico/tex_1024.bmp"))
	{
		std::cerr << "Erro ao carregar textura\n";
		return 1;
	}

	sf::Texture::bind(&texture);

	GLfloat ar = (GLfloat)WH/(GLfloat)WW;

	// Initialize OpenGL
	glViewport (0, 0, WW, WH);						// update the viewport
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);					// update projection
	glLoadIdentity();
	gluPerspective(45, ar, .01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	
	/* Enable Texture Mapping ( NEW ) */
	glEnable( GL_TEXTURE_2D );
	glShadeModel( GL_SMOOTH );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	GLfloat mat_shininess[]  = { 9.6 };
	GLfloat mat_specular[]   = { 0.75, 0.75, 0.75, 0.75 };
 
	GLfloat light_ambient[]  = { 1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[]  = { 0.64, 0.64, 0.64, 0.9};
	GLfloat light_specular[] = { 0.5, 0.5, 0.5, 0.1};
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

	sf::Clock clock;
	sf::Time dt;
	sf::Event event;
	
	GLfloat x, y, z, nx, ny, nz, u, v;

	clock.restart();
	while (window.isOpen())
	{
		clock.restart();
		// scanning for window close
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				ar = (GLfloat)event.size.height/(GLfloat)event.size.width;
				glViewport(0, 0, event.size.width, event.size.height);
				glMatrixMode(GL_PROJECTION);					// update projection
				glLoadIdentity();
				gluPerspective(45, ar, .01f, 100.0f);
				glMatrixMode(GL_MODELVIEW);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		// Drawing
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glLoadIdentity();
	
		gluLookAt( 4.0,  4.0,  4.0,
				   0.0,  0.0,  0.0,
				   0.0,  1.0,  0.0);
		glPushMatrix();
		glColor3f(0.1, 0.6, 0.08);
		glBegin(GL_QUADS);
			glNormal3f(  0.0, 1.0,   0.0);
			glVertex3f(-30.0, 0.0, -30.0);
			glVertex3f(-30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0,  30.0);
			glVertex3f( 30.0, 0.0, -30.0);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		
		sf::Texture::bind(&texture);
		objl::Mesh curMesh;
		for (int i = 0; i < loader.LoadedMeshes.size(); i++)
		{
			curMesh = loader.LoadedMeshes[i];
			glBegin(GL_TRIANGLE_STRIP);
			std::cout << curMesh.Vertices.size() << std::endl;
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				x = curMesh.Vertices[j].Position.X;
				y = curMesh.Vertices[j].Position.Y;
				z = curMesh.Vertices[j].Position.Z;
				nx = curMesh.Vertices[j].Normal.X;
				ny = curMesh.Vertices[j].Normal.Y;
				nz = curMesh.Vertices[j].Normal.Z;
				u = curMesh.Vertices[j].TextureCoordinate.X;
				v = curMesh.Vertices[j].TextureCoordinate.Y;
				//glTexCoord2f(u, v);
				glNormal3f(nx, ny, nz);
				glVertex3f(x, y, z);
				std::cout << "Drawing vertex at " << x << " " << y << " " << z << std::endl;
			}
			glEnd();
		}
		glPopMatrix();

		glFlush();
		window.display();
		printf("FPS=%4d\n", (int)((double)1.0 / (double) dt.asSeconds()));
		dt = clock.getElapsedTime();
	}

	sf::Texture::bind(NULL);

	return 0;
}
