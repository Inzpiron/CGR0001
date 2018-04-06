#include <SFML/OpenGL.hpp>
#include <random>
#include <GL/freeglut.h>
#include "fireworks.hpp"

unsigned int fw_c = 0;
std::vector<FireworkRocket> fw_r;
std::vector<FireworkSpark> fw_s;
std::random_device rd;

void render(sf::Window* window)
{
	window.setActive(true);

	// OpenGL Initialization
	glViewport(0, 0, width, height);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	GLfloat aspect = (GLfloat) window.width / height;
	gluPerspective(45, aspect, .01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char **argv)
{
	int i;
	float x, y, z;

	sf::Window window(sf::VideoMode(800, 600), "Fireworks!", sf::Style::None, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

	window.setActive(false);

	sf::Clock clock;
	sf::Time dt;

	sf::Thread th_render(&render);
	th_render.Launch();

	clock.restart();
	while (window.isOpen())
	{
		dt = clock.getElapsedTime();
		// scanning for window close



		// Physics and game logic
		if ((rd() % 100) < 5 && fw_c < 5)
		{
			// 5% chance to spawn new rocket if there are less than 5 on screen
			fw.push_back(new FireworkRocket());
			fw_c++;
		}

		// Rocket science
		for (i = 0; i < fw_c; i++)
		{
			if (fw_r[i].runTick(dt))
			{
				fw_r.remove(fw_r.begin() + i);
				fw_c--;
				i--;
			}
		}

		// Spark physics
		for (i = 0; i < fw_r; i++)
		{
			if (fw_r[i].runTick(dt))
			{
				fw_r.remove(fw_r.begin() + i);
				fw_c--;
				i--;
			}
		}
		clock.restart();
	}

	return 0;
}