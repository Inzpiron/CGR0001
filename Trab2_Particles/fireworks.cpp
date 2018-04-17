#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/freeglut.h>
#include <thread>
#include "fireworks.hpp"
#include "am_3dShapes.hpp"

#define WW 800
#define WH 600

int mut = 0, spark = 0;
unsigned int fw_cr = 0, fw_cs = 0;
std::vector<FireworkRocket> fw_r;
std::vector<FireworkSpark> fw_s;
GLfloat angle = 0.0;

void physics(sf::Window *window)
{
	int i, j;
	double x, y;
	std::random_device rd;
	std::mt19937 mt(rd());
	sf::Clock clock;
	sf::Time dt;

	while (window->isOpen())
	{
		clock.restart();
		// Physics and game logic
		if (mut || ((mt() % 1000) < 8 && fw_cr < 4))
		{
			// 5% chance to spawn new rocket if there are less than 10 on screen
			// or spawn one if user pressed 'space'
			fw_r.push_back(FireworkRocket(mt));
			fw_cr++;
			mut = 0;
		}

		// Rocket science
		for (i = 0; i < fw_cr; i++)
		{
			if (fw_r[i].runTick(dt))
			{
				spark = 1;
				x = fw_r[i].x;
				y = fw_r[i].y;
				for (j = 0; j < 100; j++)
				{
					fw_s.push_back(FireworkSpark(x, y, fw_r[i].color, mt));
				}

				fw_cs += 100;

				fw_r.erase(fw_r.begin() + i);
				fw_cr--;
				i--;
			}
		}

		// Spark physics
		for (i = 0; i < fw_cs; i++)
		{
			if (fw_s[i].runTick(dt))
			{
				fw_s.erase(fw_s.begin() + i);
				fw_cs--;
				i--;
			}
		}
		sf::sleep(sf::milliseconds(10));
		dt = clock.getElapsedTime();
	}

}

int main(int argc, char **argv)
{
	int i, j;
	float x, y, z;

	sf::Window window(sf::VideoMode(WW, WH), "Fireworks!", sf::Style::Default, sf::ContextSettings(24));
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(144);

	std::thread phsx_thr(physics, &window);

	// Initialize OpenGL
	glViewport (0, 0, WW, WH);						// update the viewport
    glMatrixMode(GL_PROJECTION);					// update projection
    glLoadIdentity();
    gluOrtho2D(-WW/2.0, WW/2.0, -WH/2.0, WH/2.0);	// map screen size to viewport
    glMatrixMode(GL_MODELVIEW);

	sf::Clock clock;
	sf::Time dt;
    sf::Event event;

	clock.restart();
	while (window.isOpen())
	{
		clock.restart();
		// scanning for window close
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				mut = 1;
        }

		// Drawing
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (spark)
		{
			glClearColor(0.9, 0.9, 0.7, 1.0);
			spark = 0;
		}
		else
			glClearColor(0.03, 0.01, 0.04, 1.0);
		glLoadIdentity();

		// Rocket science
		printf("fw_cr=%d\n", fw_cr);
		for (i = 0; i < fw_cr; i++)
		{
			//printf("r=%d,x=%f,y=%f,life=%f\n", i, fw_r[i].x, fw_r[i].y, fw_r[i].life.asSeconds());
			glPushMatrix();
				glTranslatef(fw_r[i].x, fw_r[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3fv(fw_r[i].color); //rainbow
				amCircle(20.0, 9);
			glPopMatrix();
		}

		// Spark science
		for (i = 0; i < fw_cs; i++)
		{
			//printf("s=%d,x=%f,y=%f,life=%f\n", i, fw_r[i].x, fw_r[i].y, fw_r[i].life.asSeconds());
			glPushMatrix();
				glTranslatef(fw_s[i].x, fw_s[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3fv(fw_s[i].color); //rainbow
				amCircle(7.0 * (1.5 - fw_s[i].life.asSeconds()), 3);
			glPopMatrix();
		}

		glFlush();
		window.display();
		printf("FPS=%9.2lf\n", (double)1.0 / (double) dt.asSeconds());
		dt = clock.getElapsedTime();
	}

	phsx_thr.join();

	return 0;
}
