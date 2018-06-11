#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <GL/freeglut.h>
#include "OBJ_Loader.h"

#define WW 800
#define WH 600

int main(int argc, char **argv)
{
	sf::RenderWindow window(sf::VideoMode(WW, WH),
					  "Modelo do Blender!",
					  sf::Style::Default,
					  sf::ContextSettings(24));

	window.setVerticalSyncEnabled(true);

	objl::Loader loader;
	sf::Image img;

	if (!loader.LoadFile("Modelos/CuboMagico/BlenderExport/CuboMagico.obj"))
	{
		std::cerr << "Erro ao carregar modelo .obj\n";
		return 1;
	}

	if (!img.loadFromFile("Modelos/CuboMagico/tex_1024.bmp"))
	{
		std::cerr << "Erro ao carregar textura\n";
		return 1;
	}
	
	glEnable( GL_TEXTURE_2D );

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLfloat ar = (GLfloat)WH/(GLfloat)WW;

	// Initialize OpenGL
	glViewport (0, 0, WW, WH);						// update the viewport
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);					// update projection
	glLoadIdentity();
	gluPerspective(60, ar, .01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	
	/* Enable Texture Mapping ( NEW ) */
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

	sf::Clock clock, uclock;
	sf::Time dt;
	sf::Event event;
	
	GLfloat x, y, z, nx, ny, nz, u, v;

	clock.restart();
	uclock.restart();
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
				gluPerspective(60, ar, .01f, 100.0f);
				glMatrixMode(GL_MODELVIEW);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		// Drawing
		glClear(GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		window.clear(sf::Color::Black);
	
		glBindTexture(GL_TEXTURE_2D, tex);
		//sf::Texture::bind(&texture);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( 3.0,  3.0,  3.0,
				   0.0,  0.0,  0.0,
				   0.0,  1.0,  0.0);
	
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		
		objl::Mesh curMesh;
		glRotatef(uclock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f);
		glRotatef(uclock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f);
		for (int i = 0; i < loader.LoadedMeshes.size(); i++)
		{
			curMesh = loader.LoadedMeshes[i];
			glBegin(GL_TRIANGLES);
			//std::cout << curMesh.Vertices.size() << std::endl;
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				x  = curMesh.Vertices[j].Position.X;
				y  = curMesh.Vertices[j].Position.Y;
				z  = curMesh.Vertices[j].Position.Z;
				nx = curMesh.Vertices[j].Normal.X;
				ny = curMesh.Vertices[j].Normal.Y;
				nz = curMesh.Vertices[j].Normal.Z;
				u  = curMesh.Vertices[j].TextureCoordinate.X;
				v  = curMesh.Vertices[j].TextureCoordinate.Y;
				glTexCoord2f(v, u);
				//glNormal3f(nx, ny, nz);
				glVertex3f(x, y, z);
				std::cout << "Drawing vertex at " << x << " " << y << " " << z << std::endl;
			}
			glEnd();
		}
		
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		//sf::Texture::bind(NULL);

		window.display();
		printf("FPS = %d\n", (int)((double)1.0 / (double) dt.asSeconds()));
		dt = clock.getElapsedTime();
	}

	sf::Texture::bind(NULL);

	return 0;
}
