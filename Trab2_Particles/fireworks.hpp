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
	float grav = -0.981f;
	// float vx, vy, vz;
	float vx, vy;

public:
	// float x, y, z;
	float x, y;
	float color[3];
	sf::Time life;

	FireworkRocket(sf::Time t)
	{
		srand (t.asMicroseconds());
		this->life = sf::seconds(0);
		x = ((rand() % 2001) - 1000) / 1.0f;
		//z = ((rand() % 2001) - 1000) / 1000.0f;
		y = -200.0f;
		vx = ((rand() % 1001) - 500) / 1000.0f;
		//vz = ((rand() % 1001) - 500) / 1000.0f;
		vy = 40.0f + ((rand() % 2000)) / 100.0f;
		color[0] = (rand() % 256) / 255.0;
		color[1] = (rand() % 256) / 255.0;
		color[2] = (rand() % 256) / 255.0;
	}

	int runTick(sf::Time t)
	{
		life += sf::seconds(t.asSeconds()*10000);
		long long dt = t.asMilliseconds();

		x += dt * vx;
		//z += dt * vz;
		y += (dt * vy) / 2;
		vy += dt * grav;
		y += (dt * vy) / 2;

		if (life.asSeconds() > 2.0)
		{
			if (rand() % 100 < 30)
				return 1;
		}
		return 0;
	}
};

class FireworkSpark
{
private:
	float grav = -07.f;
	float xz_decay = 0.72f;
	// float vx, vy, vz;
	float vx, vy;
	sf::Time life;

public:
	// float x, y, z;
	float x, y;
	float color[3];

	// FireworkSpark(float x, float y, float z)
	FireworkSpark(float x, float y, sf::Time t)
	{
		srand (t.asMicroseconds());
		this->life = sf::seconds(0);
		this->x = x;
		this->y = y;
		// this->z = z;
		vx = ((rand() % 2001) - 1000) / 10.0f;
		vy = ((rand() % 2001) -  800) / 10.0f;
		// vz = ((rand() % 2001) - 1000) / 1000.0f;
		color[0] = (rand() % 256) / 255.0;
		color[1] = (rand() % 256) / 255.0;
		color[2] = (rand() % 256) / 255.0;
	}

	int runTick(sf::Time t)
	{
		life += sf::seconds(t.asSeconds()*10000);
		long long dt = t.asMicroseconds();

		x  += dt * vx;
		vx *= xz_decay;
		// z  += dt * vz;
		// vz *= xz_decay;
		y  += (dt * vy) / 2;
		vy += dt * grav;
		y  += (dt * vy) / 2;

		if (life.asSeconds() > 2.0)
		{
			if (rand() % 100 < 30)
				return 1;
		}
		return 0;
	}
};

#endif
