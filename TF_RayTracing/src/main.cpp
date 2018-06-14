#include "rt_headers.hpp"
#include <iostream>
#include <thread>
#include <mutex>
//#include <semaphore>

unsigned W = 640, H = 480;
unsigned rW = W, rH = H;
char outfile[256];
bool toFile = false;
bool rerender = false;
bool die = false;
sf::Image image;
sf::Texture texture;
sf::Uint8 *pixels; // 0 - 255 para cada canal (RGBA)
sf::Sprite sprite;
std::mutex mut;
std::mutex rmut;
//std::semaphore sem;

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
	float f_x, f_y, f_z, f_a, f_b;
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
				   "    help\n"
				   "    set \n"
				   "        campos x y z    - Set camera position\n"
				   "        camangle x y z  - Set camera aiming direction\n"
				   "        contrast f      - Set rendering contrast\n"
				   "        depth d         - Set max. ray recursion depth\n"
				   //"        res x y         - Set rendering resolution (not screen)\n"
				   //"    resize x y          - Set screen size (not rendering resolution)\n"
				  );
		} else
		if (op == "set")
		{
			command >> op;
			if (op == "campos")
			{
				command >> f_x >> f_y >> f_z;
				campos.Set(f_x, f_y, f_z);
				printf("Setting camera position to (%f, %f, %f)\n", f_x, f_y, f_z);
			} else
			if (op == "camangle")
			{
				command >> f_x >> f_y >> f_z;
				camlook.Set(f_x, f_y, f_z);
				//printf("Setting camera position to (%f, %f, %f)\n", f_x, f_y, f_z);
			} else
			if (op == "contrast")
			{
				command >> f_a;
				contrast = f_a;
			} else
			if (op == "depth")
			{
				command >> i_a;
				MAXTRACE = unsigned(i_a);
			} else
			if (op == "res")
			{
				command >> i_a >> i_b;
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
			} else
			if (op == "shadowres")
			{
				command >> i_a;
				NumArealightVectors = i_a;
				InitArealightVectors();
			}
		} else
		/*
		if (op == "resize")
		{
			command >> i_a >> i_b;
			W = unsigned(i_a); H = unsigned(i_b);
			window.setSize(sf::Vector2u(W, H));
			float sX = W/float(rW), sY = H/float(rH);
			//printf("Nova escala: x=%f, y=%f\n", sX, sY);
			//sprite.setTexture(texture, true);
			//sprite.setScale(sX, sY);
		} else
		*/
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
	window.clear();
	window.display();

	//sf::Image image;
	image.create(W, H, sf::Color::Black);
	//sf::Texture texture;
	if (!texture.create(W, H))
		printf("Erro ao criar textura\n");

	//sf::Sprite sprite;

	//sf::Uint8 *pixels = new sf::Uint8[rW * rH * 4];
	pixels = new sf::Uint8[rW * rH * 4];
	//for (unsigned i=0; i < rW*rH*4; i++) pixels[i] = 0;

	texture.update(pixels);
	sprite.setTexture(texture);

	InitArealightVectors();

	//double zoom = 46.0, zoomdelta = 0.99;
	zoom = 2.2; //zoomdelta = 0.99;
	contrast = 3.2; //contrast_offset = -0.12;
	// Redefine camera parameters
	campos.Set(0.0, 0.0, -180.0);
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
	window.setActive(false);
	//std::thread windowThread(updateScreen, &window, &texture, &sprite, pixels);
	//sf::Thread windowThread(&updateScreen, &window);
	//windowThread.launch();
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
			window.clear();
			//window.display();
			// Atualizar textura
			texture.update(pixels);
			sprite.setTexture(texture, true);
			// Apresentar resultado
			window.draw(sprite);
			window.display();
		}

		if (die)
			break;
	}
	window.close();
	renderThread.join();
	consoleThread.join();

	return 0;
}
