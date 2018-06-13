#include "rt_headers.hpp"

/* MAIN PROGRAM */
int main(int argc, char **argv)
{
	bool toFile = false;
	char outfile[256];
	unsigned W = 320, H = 240;
	MAXTRACE = 3;

	if (argc > 1) { sscanf(argv[1], "%u", &W); sscanf(argv[2], "%u", &H); }
	if (argc > 3) { sscanf(argv[3], "%u", &MAXTRACE); }
	if (argc > 4) { toFile = true; sscanf(argv[4], "%s", outfile); }

	double AR = double(W)/double(H);

	sf::RenderWindow window(sf::VideoMode(W, H), "MultiThreaded SFML PathTracing"/*, sf::Style::None*/);
	if (toFile) window.setVisible(false);
	else window.setVerticalSyncEnabled(true);

	sf::Image image;
	image.create(W, H, sf::Color::Black);
	sf::Texture texture;
	if (!texture.create(W, H))
		printf("Erro ao criar textura\n");

	sf::Sprite sprite;

	sf::Uint8 *pixels = new sf::Uint8[W * H * 4];
	for (unsigned i=0; i < W*H*4; i++) pixels[i] = 0;

	texture.update(pixels);
	sprite.setTexture(texture);

	InitArealightVectors();

	//double zoom = 46.0, zoomdelta = 0.99;
	zoom = 3.0; zoomdelta = 0.99;
	contrast = 6.2; contrast_offset = -0.12;
	// Redefine camera parameters
	campos.Set(0.0, 10.0, 7.0);
	camangle.Set(0,0,0);
	camangledelta.Set(-.005, -.011, -.017);
	camlook.Set(0,0,0);
	camlookdelta.Set(-.001, .005, .004);

	double delay;
	clock_t before;

	sf::Event event;
	for(unsigned frameno=0; frameno<9300; ++frameno)
	{
		fprintf(stderr, "Frame %u; Contrast %g, offset %g; ",
			frameno,contrast,contrast_offset);

		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		before = clock();

		render(W, H, pixels);

		if (toFile)
		{
			image.create(W, H, pixels);
			image.saveToFile(outfile);
			break;
		}
		else
		{
			// Atualizar textura
			texture.update(pixels);
			// Apresentar resultado
			window.draw(sprite);
			window.display();
		}

		delay = (clock() - before) / double(CLOCKS_PER_SEC);
		printf("FrameTime %.4fs\n", delay);
	}

	return 0;
}
