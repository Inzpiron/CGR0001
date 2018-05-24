#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/freeglut.h>
#include <thread>
#include <mutex>
#include "fireworks.hpp"
#include "am_3dShapes.hpp"

#define WW 800
#define WH 600

int spawn = 0, spark = 0;
std::mutex mut_r, mut_s;
std::vector<FireworkRocket> fw_r;
std::vector<FireworkSpark> fw_s;
GLfloat angle = 0.0;

void physics(sf::Window *window)
{
	int i, j, np = 0;
	double x, y;
	std::random_device rd;
	std::mt19937 mt(rd());
	sf::Clock clock;
	sf::Time dt;

	while (window->isOpen())
	{
		clock.restart();
		// Physics and game logic
		if ((spawn && fw_r.size() < 15) || ((mt() % 1000) < 8 && fw_r.size() < 4))
		{
			// 5% chance to spawn new rocket if there are less than 10 on screen
			// or spawn one if user pressed 'space'
			fw_r.push_back(FireworkRocket(mt));
			spawn = 0;
		}

		// Rocket science
		mut_r.lock();
		for (i = 0; i < fw_r.size(); i++)
		{
			if (fw_r[i].runTick(dt))
			{
				spark = 1;
				x = fw_r[i].x;
				y = fw_r[i].y;
				np = 8 + (mt() % 550);
				for (j = 0; j < np; j++)
				{
					fw_s.push_back(FireworkSpark(fw_r[i], mt));
				}

				fw_r.erase(fw_r.begin() + i);
				i--;
			}
		}
		mut_r.unlock();

		// Spark physics
		mut_s.lock();
		for (i = 0; i < fw_s.size(); i++)
		{
			if (fw_s[i].runTick(dt))
			{
				fw_s.erase(fw_s.begin() + i);
				i--;
			}
		}
		mut_s.unlock();

		sf::sleep(sf::milliseconds(10));
		dt = clock.getElapsedTime();
	}

}

int main(int argc, char **argv)
{
	int i, j;
	float x, y, z;

	sf::Window window(sf::VideoMode(WW, WH),
					  "Fireworks!",
					  sf::Style::Default,
					  sf::ContextSettings(24));

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(180);

	std::thread phsx_thr(physics, &window);

	GLfloat ar = (GLfloat)WH/(GLfloat)WW;

	// Initialize OpenGL
	glViewport (0, 0, WW, WH);						// update the viewport
	glMatrixMode(GL_PROJECTION);					// update projection
	glLoadIdentity();
	gluOrtho2D(-WW/2.0, WW/2.0, -WW*ar/2.0, WW*ar/2.0);	// map screen size to viewport
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
			else if (event.type == sf::Event::Resized)
			{
				ar = (GLfloat)event.size.height/(GLfloat)event.size.width;
				glViewport(0, 0, event.size.width, event.size.height);
				glMatrixMode(GL_PROJECTION);					// update projection
				glLoadIdentity();
				gluOrtho2D(-WW/2.0, WW/2.0, -WW*ar/2.0, WW*ar/2.0);	// map screen size to viewport
				glMatrixMode(GL_MODELVIEW);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				spawn = 1;
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
			glClearColor(BG_R, BG_G, BG_B, 1.0);
		glLoadIdentity();

		// Rocket science
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mut_r.lock();
		for (i = 0; i < fw_r.size(); i++)
		{
			glPushMatrix();
				glTranslatef(fw_r[i].x, fw_r[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor4fv(fw_r[i].color); //rainbow
				amCircle(15.0, 360);
			glPopMatrix();
		}
		mut_r.unlock();
		glDisable(GL_BLEND);

		// Spark science
		mut_s.lock();
		for (i = 0; i < fw_s.size(); i++)
		{
			glPushMatrix();
				glTranslatef(fw_s[i].x, fw_s[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3fv(fw_s[i].color); //rainbow
				amCircle(6.0 * (1.55 - fw_s[i].life.asSeconds()), fw_s[i].sides);
			glPopMatrix();
		}
		mut_s.unlock();

		//glFlush();
		window.display();
		printf("FPS=%4d\n", (int)((double)1.0 / (double) dt.asSeconds()));
		dt = clock.getElapsedTime();
	}

	phsx_thr.join();

	return 0;
}
