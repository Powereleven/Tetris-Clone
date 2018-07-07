#include "Game.h"

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(TABLE_WIDTH * TILE_SIZE, TABLE_HEIGHT * TILE_SIZE), "Tetris Clone");
	sf::Event event;

	Game game(window, event);

	sf::Clock clock;
	float total_time = 0;

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		total_time += dt;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			game.handleEvents();
		}

		if (game.update(total_time)) total_time = 0;

		window.clear();
		game.draw();
		window.display();
	}
}