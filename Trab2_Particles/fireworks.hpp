#ifndef _M_FIREWORKS_
#define _M_FIREWORKS_

#include <SFML/System.hpp>
#include <cstdio>
#include <vector>
#include <cmath>
#include <random>
#include <cstdlib>

class FireworkRocket
{
private:
	float grav = -9.81f;
	float vx, vy;

public:
	float x, y;
	float color[3];
	sf::Time life;

	FireworkRocket(std::mt19937 mt)
	{
		srand(mt());
		this->life = sf::seconds(0);
		x = (int) ((rand() % 600) - 300) / 1.0f;
		y = -320.0f;
		vx = ((rand() % 1001) - 500) / 10.0f;
		vy = 200.0f + ((rand() % 2000)) / 15.0f;
		color[0] = (rand() % 256) / 255.0;
		color[1] = (rand() % 256) / 255.0;
		color[2] = (rand() % 256) / 255.0;
	}

	int runTick(sf::Time t)
	{
		life += t;
		double dt = t.asSeconds();

		x += dt * vx;
		y += (dt * vy) / 2;
		vy += dt * grav;
		y += (dt * vy) / 2;

		if (life.asSeconds() > 1.0)
		{
			if (rand() % 100 < 3)
				return 1;
		}
		return 0;
	}
};

class FireworkSpark
{
private:
	float grav = -7.0f;
	float v_decay = 0.989f;
	float dx, dy, v;

public:
	float x, y;
	float color[3];
	sf::Time life;

	FireworkSpark(float x, float y, float color[], std::mt19937 &mt)
	{
		srand(mt());
		this->life = sf::seconds(0);
		this->x = x;
		this->y = y;
		dx = ((rand() % 2001) - 1000) / 1000.0f;
		dy = ((rand() % 2001) - 1000) / 1000.0f;
		v = 20.0f + (rand() % 370) / 1.0f;
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
	}

	int runTick(sf::Time t)
	{
		life += t;
		double dt = t.asSeconds();

		x  += dt * v * dx;
		v *= v_decay;
		// y  += (dt * v * dy) / 2.0;
		y += dt * v * dy;
		// dy += dy * grav;
		// y  += (dt * v * dy) / 2.0;

		if (life.asSeconds() > 1.5)
		{
			if (rand() % 100 < 15)
				return 1;
		}
		return 0;
	}
};

#endif
