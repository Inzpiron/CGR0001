#include "rt_headers.hpp"
#include "scene.hpp"
#include <iostream>
#include <thread>
#include <mutex>

unsigned W = 640, H = 480;
unsigned rW = W, rH = H;
char outfile[256];
bool toFile = false;
bool rerender = false;
bool die = false;
bool resize = false;
sf::Image image;
sf::Texture texture;
sf::Uint8 *pixels; // 0 - 255 para cada canal (RGBA)
sf::Sprite sprite;
std::mutex mut;
std::mutex rmut;
std::mutex dmut;

void doRender()
{
	sf::Clock clock;
	int frametime;
	while (!die)
	{
		mut.lock();
		if (rerender)
		{
			clock.restart();
			rmut.lock();
			//if (sketch) doSketch(rW, rH, pixels);
			//else render(rW, rH, pixels);
			render(rW, rH, pixels);
			frametime = clock.getElapsedTime().asMilliseconds();
	
			printf("Frametime %1.3fs\n", frametime/1000.0);
			rerender = false;
			rmut.unlock();
		}
		mut.unlock();
	}
}

void consoleReader(sf::RenderWindow *window)
{
	float f_x, f_y, f_z, f_a, f_b, f_c;
	int i_a, i_b;

	std::string line, op;
	std::stringstream command;
	while (window->isOpen())
	{
		rmut.lock();
		// Start interpreter
		printf("> ");
		std::getline(std::cin, line);
		command = std::stringstream(line);
		std::cout << "Command: " << command.str() << std::endl;
		command >> op; ToLowerString(op);
		
		// Select operation
		if (op == "help")
		{
			printf("Commands: \n"
				   "    help  - Show this\n"
				   "    set   - Define certain variables\n"
				   "        campos x y z      - Set camera position\n"
				   "        camangle x y z    - Set camera angle on each axis (radians)\n"
				   "        contrast f        - Set rendering contrast\n"
				   "        depth d           - Set max. ray recursion depth\n"
				   "        res x y           - Set rendering resolution (not screen)\n"
				   "        shadowres a       - Set shadow resolution (sample number\n"
				   "                           for each number)\n"
				   "    add                   - Insert objects to the scene\n"
				   "        sphere x y z r    - Insert a circle\n"
				   "        light x y z r g b - Insert a light source object\n"
				   "        plane x y z d     - Insert a plane at d units from origin\n"
				   "    list objType          - List object list for given type\n"
				   "        objType = spheres|lights|planes\n"
				   "    rem objType index     - Remove certain object from the scene\n"
				   //"    resize x y            - Set screen size (not rendering resolution)\n"
				  );
		} else
		if (op == "set")
		{
			command >> op;
			if (op == "campos")
			{
				command >> f_x >> f_y >> f_z;
				campos.Set(f_x, f_y, f_z);
			} else
			if (op == "camangle")
			{
				command >> f_x >> f_y >> f_z;
				camlook.Set(f_x, f_y, f_z);
			} else
			if (op == "zoom")
			{
				command >> f_a;
				zoom = f_a;
			} else
			if (op == "contrast")
			{
				command >> f_a;
				contrast = f_a;
			} else
			if (op == "depth")
			{
				command >> i_a;
				if (i_a < 1) i_a = 1;
				MAXTRACE = unsigned(i_a);
			} else
			if (op == "res")
			{
				command >> i_a >> i_b;

				dmut.lock();
				rW = unsigned(i_a); rH = unsigned(i_b);
				delete pixels;
				pixels = new sf::Uint8[rW * rH * 4];
				if (!texture.create(rW, rH))
				{
					printf("Error on texture resizing!\n");
					break;
				}
				sprite.setTexture(texture, true);
				float sX = W/float(rW), sY = H/float(rH);
				printf("Nova escala: x=%f, y=%f\n", sX, sY);
				sprite.setScale(sX, sY);
				dmut.unlock();
			} else
			if (op == "shadowres")
			{
				command >> i_a;
				if (i_a < 1) i_a = 1;
				NumArealightVectors = i_a;
				InitArealightVectors();
			}
		} else
		if (op == "add")
		{
			command >> op >> f_x >> f_y >> f_z;
			if (op == "sphere")
			{
				command >> f_a; // Read radius
				Spheres.push_back({{f_x, f_y, f_z}, f_a});
			} else
			if (op == "light")
			{
				command >> f_a >> f_b >> f_c; // Read color
				Lights.push_back({{f_x, f_y, f_z}, {f_a, f_b, f_c}});
			} else
			if (op == "plane")
			{
				command >> f_a; // Read distance
				XYZ dir{{f_x, f_y, f_z}}; dir.Normalize();
				Planes.push_back({dir, f_a});
			}
		} else
		if (op == "list")
		{
			command >> op;
			if (op == "sphere")
			{
				printf("Spheres:\n id |   x   |   y   |   z   | radius\n");
				for (int i = 0; i < Spheres.size(); i++)
				{
					XYZ pos = Spheres[i].center;
					double rad = Spheres[i].radius;
					printf("%3d |%7.2f|%7.2f|%7.2f|%7.2f\n",
					       i, pos[0], pos[1], pos[2], rad);
				}
			}
			if (op == "light")
			{
				printf("Lights:\n id |   x   |   y   |   z   |  r  |  g  |  b\n");
				for (int i = 0; i < Lights.size(); i++)
				{
					XYZ pos = Lights[i].center;
					XYZ rgb = Lights[i].colour;
					printf("%3d |%7.2f|%7.2f|%7.2f|%5.2f|%5.2f|%5.2f\n",
						   i, pos[0], pos[1], pos[2], rgb[0], rgb[1], rgb[2]);
				}
			}
			if (op == "plane")
			{
				printf("Planes:\n id |   x   |   y   |   z   | distance\n");
				for (int i = 0; i < Planes.size(); i++)
				{
					XYZ pos = Planes[i].normal;
					double offset = Planes[i].offset;
					printf("%3d |%7.2f|%7.2f|%7.2f|%7.2f\n",
					       i, pos[0], pos[1], pos[2], offset);
				}
			}
		} else
		if (op == "rem")
		{
			command >> op >> i_a;
			if (op == "sphere")
			{
				if (i_a >= 0 && i_a < Spheres.size())
					Spheres.erase(Spheres.begin() + i_a);
			} else
			if (op == "light")
			{
				if (i_a >= 0 && i_a < Lights.size())
					Lights.erase(Lights.begin() + i_a);
			} else
			if (op == "plane")
			{
				if (i_a >= 0 && i_a < Planes.size())
					Planes.erase(Planes.begin() + i_a);
			}
		} else
		if (op == "resize")
		{
			command >> i_a >> i_b;
			W = unsigned(i_a); H = unsigned(i_b);
			float sX = W/float(rW), sY = H/float(rH);
			printf("Nova escala: x=%f, y=%f\n", sX, sY);
			resize = true;
			sprite.setScale(sX, sY);
			sprite.setTexture(texture, true);
		} else
		if (op == "render")
		{
			rerender = true;
		}
		if (op == "export")
		{
			command >> outfile;
			toFile = true;
		} else
		if (op == "exit")
		{
			die = true;
			break;
		}
		rmut.unlock();
		sleep(50);
	}
}

/* MAIN PROGRAM */
int main(int argc, char **argv)
{
	MAXTRACE = 3;

	if (argc > 1) { sscanf(argv[1], "%u", &W); sscanf(argv[2], "%u", &H); }
	if (argc > 3) { sscanf(argv[3], "%u", &MAXTRACE); }
	if (argc > 4) { toFile = true; sscanf(argv[4], "%s", outfile); }

	rW = W; rH = H;

	sf::RenderWindow window(sf::VideoMode(W, H), "MultiThreaded SFML RayTracing", sf::Style::Titlebar);
	window.setVerticalSyncEnabled(true);
	window.clear();
	window.display();

	image.create(W, H, sf::Color::Black);
	if (!texture.create(W, H))
		printf("Erro ao criar textura\n");

	pixels = new sf::Uint8[rW * rH * 4];

	texture.update(pixels);
	sprite.setTexture(texture);

	InitArealightVectors();
	InitDefaultScene();

	zoom = 1.0; //zoomdelta = 0.99;
	contrast = 3.2; //contrast_offset = -0.12;
	campos.Set(0.0, 0.0, -60.0);
	camangle.Set(0,0,0);
	camangledelta.Set(-.005, -.011, -.017);
	camlook.Set(0,0,0);
	camlookdelta.Set(-.001, .005, .004);

	std::stringstream command;
	std::string line, op;

	float f_x, f_y, f_z, f_a, f_b;
	int i_a, i_b, i_iw, i_ih;
	bool redraw = true, sketch = false;

	int frametime;
	sf::Clock clock;
	sf::Event event;
	std::thread renderThread(doRender);

	std::thread consoleThread(consoleReader, &window);
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}
	
		if (toFile)
		{
			image.create(rW, rH, pixels);
			image.saveToFile(outfile);
			toFile = false;
		}
		else
		{
			dmut.lock();
			window.clear();
			//window.display();
			if (resize) window.setSize(sf::Vector2u(W, H));
			// Atualizar textura
			texture.update(pixels);
			sprite.setTexture(texture, true);
			// Apresentar resultado
			window.draw(sprite);
			window.display();
			dmut.unlock();
		}

		if (die)
			break;
	}
	window.close();
	renderThread.join();
	consoleThread.join();

	return 0;
}
