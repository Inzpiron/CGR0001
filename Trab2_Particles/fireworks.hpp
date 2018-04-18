#ifndef _M_FIREWORKS_
#define _M_FIREWORKS_

#include <SFML/System.hpp>
#include <cstdio>
#include <vector>
#include <cmath>
#include <random>
#include <cstdlib>

#define BG_R 0.015
#define BG_G 0.005
#define BG_B 0.015

class FireworkRocket
{
private:
	float grav = -9.81f;

public:
	float x, y;
	float vx, vy, ma;
	float color[4], def[4];
	sf::Time life;

	FireworkRocket(std::mt19937 mt)
	{
		srand(mt());
		this->life = sf::seconds(0);
		x = (int) ((rand() % 600) - 300) / 1.0f;
		y = -400.0f;
		vx = ((rand() % 1001) - 500) / 10.0f;
		vy = 200.0f + ((rand() % 2000)) / 15.0f;
		ma = (((rand() % 1000) + 200) / 1000.0f);
		color[0] = (rand() % 256) / 255.0;
		color[1] = (rand() % 256) / 255.0;
		color[2] = (rand() % 256) / 255.0;
		color[3] = 0.6;
		def[0] = color[0];
		def[1] = color[1];
		def[2] = color[2];
		def[3] = 1.0;
	}

	int runTick(sf::Time t)
	{
		life += t;
		double dt = t.asSeconds();

		x += dt * vx;
		y += (dt * vy) / 2;
		vy += dt * grav;
		y += (dt * vy) / 2;
		color[3] = 0.6 - life.asSeconds() * 0.3;

		if (life.asSeconds() > 1.0)
		{
			if (rand() % 100 < 3)
			{
				color[0] = def[0];
				color[1] = def[1];
				color[2] = def[2];
				color[3] = def[3];
				return 1;
			}
		}
		return 0;
	}
};

class FireworkSpark
{
private:
	float grav = -0.0981f;
	float v_decay = 0.980f;
	float dx, dy, v;

public:
	float x, y;
	float color[3];
	sf::Time life;

	FireworkSpark(FireworkRocket pai, std::mt19937 &mt)
	{
		float dir;
		srand(mt());
		this->life = sf::seconds(0);
		this->x = pai.x;
		this->y = pai.y;
		float l = sqrt(pai.vx*pai.vx + pai.vy*pai.vy);
		dir = (rand() % 360) / M_PI;
		dx = cos(2*dir);
		dy = sin(2*dir);
		float mv = (float) (rand() % 10);
		v = (9.0f + mv + (rand() % 370) / 1.0f) * pai.ma;
		//v_decay += ((rand() % 1000) / 30000.0f);
		this->color[0] = pai.color[0];
		this->color[1] = pai.color[1];
		this->color[2] = pai.color[2];
	}

	int runTick(sf::Time t)
	{
		life += t;
		double dt = t.asSeconds();

		x  += dt * v * dx;
		v *= v_decay;
		// y  += (dt * v * dy) / 2.0;
		y += dt * v * dy;
		//dy += dy * grav;
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
