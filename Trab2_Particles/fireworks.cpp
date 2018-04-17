#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/freeglut.h>
#include "fireworks.hpp"
#include "am_3dShapes.hpp"

#define WW 1280
#define WH 600

unsigned int fw_cr = 0, fw_cs = 0;
std::vector<FireworkRocket> fw_r;
std::vector<FireworkSpark> fw_s;
GLfloat angle = 0.0;

void render(sf::Window *window)
{
	window->setActive(true);

	window->setVerticalSyncEnabled(true);

	int i, j;

	while (window->isOpen())
	{
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glLoadIdentity();

		// Rocket science
		printf("\n\nfw_cr=%d\n");
		for (i = 0; i < fw_cr; i++)
		{
			printf("i=%d,x=%f,y=%f\n", i, fw_r[i].x, fw_r[i].y);
			glPushMatrix();
				glTranslatef(fw_r[i].x, fw_r[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3f(((rand() % 255) / 255), ((rand() % 255) / 255), ((rand() % 255) / 255)); //rainbow
				amCircle(60.0, 6);
			glPopMatrix();
		}

		glFlush();
		window->display();
	}

}

int main(int argc, char **argv)
{
	int i, j;
	float x, y, z;
	std::random_device rd;

	sf::Window window(sf::VideoMode(WW, WH), "Fireworks!", sf::Style::None, sf::ContextSettings(24));
	window.setVerticalSyncEnabled(true);

	//window.setActive(false);

	// Initialize OpenGL
	glViewport (0, 0, WW, WH);						// update the viewport
    glMatrixMode(GL_PROJECTION);					// update projection
    glLoadIdentity();
    gluOrtho2D(-WW/2.0, WW/2.0, -WH/2.0, WH/2.0);	// map screen size to viewport
    glMatrixMode(GL_MODELVIEW);

	sf::Clock clock, rc;
	sf::Time dt;
    sf::Event event;

	//sf::Thread th_render(&render, &window);
	//th_render.launch();

	clock.restart();
	while (window.isOpen())
	{
		dt = clock.getElapsedTime();
		//printf("fps = %lf\n", 1000.0/dt.asMilliseconds());
		// scanning for window close
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

		// Physics and game logic
		if ((rd() % 100) < 5 && fw_cr < 10)
		{
			// 5% chance to spawn new rocket if there are less than 10 on screen
			fw_r.push_back(FireworkRocket(rc.getElapsedTime()));
			fw_cr++;
		}

		// Rocket science
		for (i = 0; i < fw_cr; i++)
		{
			if (fw_r[i].runTick(dt))
			{
				x = fw_r[i].x;
				y = fw_r[i].y;
				//z = fw_r[i].z;
				for (j = 0; j < 100; j++)
				{
					// fw_s.push_back(FireworkSpark(x, y, z));
					fw_s.push_back(FireworkSpark(x, y, rc.getElapsedTime()));
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

		// Drawing
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.03, 0.01, 0.04, 1.0);
		glLoadIdentity();

		// Rocket science
		printf("\nfw_cr=%d\n");
		for (i = 0; i < fw_cr; i++)
		{
			printf("i=%d,x=%f,y=%f,life=%f\n", i, fw_r[i].x, fw_r[i].y, fw_r[i].life.asSeconds());
			glPushMatrix();
				glTranslatef(fw_r[i].x, fw_r[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3fv(fw_r[i].color); //rainbow
				amCircle(60.0, 9);
			glPopMatrix();
		}

		// Spark science
		for (i = 0; i < fw_cs; i++)
		{
			//printf("i=%d,x=%f,y=%f,life=%f\n", i, fw_r[i].x, fw_r[i].y, fw_r[i].life.asSeconds());
			glPushMatrix();
				glTranslatef(fw_s[i].x, fw_r[i].y, 0.0);
				glRotatef((rand() % 360), 0, 0, 1);
				glColor3fv(fw_s[i].color); //rainbow
				amCircle(7.0, 3);
			glPopMatrix();
		}

		glFlush();
		window.display();
		sf::sleep(sf::microseconds(1000));
		clock.restart();
	}

	//th_render.join();

	return 0;
}
