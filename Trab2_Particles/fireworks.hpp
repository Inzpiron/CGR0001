#ifndef _M_FIREWORKS_
#define _M_FIREWORKS_

#include <SFML/System.hpp>
#include <cstdio>
#include <vector>
#include <cmath>
#include <random>

class FireworkRocket
{
private:
	static std::random_device rd;
	float grav = -0.981f;
	float vx, vy, vz;
	sf::Time life;

public:
	float x, y, z;

	FireworkRocket()
	{
		this->life = sf::Time.asSeconds(0);
		x = ((rd() % 2001) - 1000) / 1000.0f;
		z = ((rd() % 2001) - 1000) / 1000.0f;
		y = -0.01f;
		vx = ((rd() % 1001) - 500) / 1000.0f;
		vz = ((rd() % 1001) - 500) / 1000.0f;
		vy = 1.5f + ((rd() % 1000) - 400) / 1000.0f;
	}

	int runTick(sf::Time t)
	{
		life += t;
		float dt = t.asSeconds();
		x += dt * vx;
		z += dt * vz;
		y += (dt * vy) / 2;
		vy += dt * grav;
		y += (dt * vy) / 2;

		if (life > 2.0)
		{
			if (rd() % 100 < 30)
				return 0;
		}
	}
}

class FireworkSpark
{
private:
	static std::random_device rd;
	float grav = -0.8f;
	float xz_decay = 0.97f;
	float vx, vy, vz;
	sf::Time life;

public:
	float x, y, z;

	FireworkSpark(float x, float y, float z)
	{
		this->life = sf::Time.asSeconds(0);
		this->x = x;
		this->y = y;
		this->z = z;
		vx = ((rd() % 2001) - 1000) / 1000.0f;
		vy = ((rd() % 2001) -  800) / 1000.0f;
		vz = ((rd() % 2001) - 1000) / 1000.0f;
	}

	int runTick(sf::Time t)
	{
		life += t;
		float dt = t.asSeconds();

		x  += dt * vx;
		vx *= xz_decay;
		z  += dt * vz;
		vz *= xz_decay;
		y  += (dt * vy) / 2;
		vy += dt * grav;
		y  += (dt * vy) / 2;

		if (life > 2.0)
		{
			if (rd() % 100 < 30)
				return 0;
		}
		return 1;
	}
}

#endif