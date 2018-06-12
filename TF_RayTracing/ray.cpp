#include "rt_headers.hpp"

/* MAIN PROGRAM */
int main(int argc, char **argv) // <<<<----- boa pratica
{
	bool toFile = false;
	char outfile[256];
	unsigned W = 320, H = 240;
	MAXTRACE = 3;

	if (argc > 1) { sscanf(argv[1], "%u", &W); sscanf(argv[2], "%u", &H); }
	if (argc > 3) { sscanf(argv[3], "%u", &MAXTRACE); }
	if (argc > 4) { toFile = true; sscanf(argv[4], "%s", outfile); }

	double AR = double(W)/double(H);

	sf::RenderWindow window(sf::VideoMode(W, H), "MultiThreaded SFML PathTracing", sf::Style::None);
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
	//texture.update(image);
	sprite.setTexture(texture);

	InitArealightVectors();

	//double zoom = 46.0, zoomdelta = 0.99;
	zoom = 3.0; zoomdelta = 0.99;
	contrast = 6.2; contrast_offset = -0.12;

	sf::Event event;
	for(unsigned frameno=0; frameno<9300; ++frameno)
	{
		fprintf(stderr, "Begins frame %u; contrast %g, contrast offset %g\n",
			frameno,contrast,contrast_offset);

		// Put camera between the central sphere and the walls
		XYZ campos = { { 0.0, -3.0, 16.0} };
		// Rotate it around the center
		Matrix camrotatematrix, camlookmatrix;
		camrotatematrix.InitRotate(camangle);
		camrotatematrix.Transform(campos);
		camlookmatrix.InitRotate(camlook);

		// Determine the contrast ratio for this frame's pixels
		double thisframe_min = 100;
		double thisframe_max = -100;

		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

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

		printf("Terminado frame %4d\n", frameno);
		
		// Tweak coordinates / camera parameters for the next frame
		double much = 1.0;

		// In the beginning, do some camera action (play with zoom)
		if(zoom <= 1.1)
			zoom = 1.1;
		else
		{
			if(zoom > 40) { if(zoomdelta > 0.95) zoomdelta -= 0.001; }
			else if(zoom < 3) { if(zoomdelta < 0.99) zoomdelta += 0.001; }
			zoom *= zoomdelta;
			much = 1.1 / pow(zoom/1.1, 3);
		}

		// Update the rotation angle
		//camlook  += camlookdelta * much;
		//camangle += camangledelta * much;

		// Dynamically readjust the contrast based on the contents
		// of the last frame
		double middle = (thisframe_min + thisframe_max) * 0.5;
		double span   = (thisframe_max - thisframe_min);
		thisframe_min = middle - span*0.60; // Avoid dark tones
		thisframe_max = middle + span*0.37; // Emphasize bright tones
		double new_contrast_offset = -thisframe_min;
		double new_contrast		= 1 / (thisframe_max - thisframe_min);
		// Avoid too abrupt changes, though
		double l = 0.85;
		if(frameno == 0) l = 0.7;
		contrast_offset = (contrast_offset*l + new_contrast_offset*(1.0-l));
		contrast		= (contrast*l + new_contrast*(1.0-l));
	}

	return 0;
}
