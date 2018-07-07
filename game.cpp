#include "Game.h"

extern size_t MAX_TILE_SIDE;
extern bool game_over;

size_t getTableIndex(size_t i, size_t j)
{
	return i * TABLE_HEIGHT + j;
}

Game::Game(sf::RenderWindow &window, sf::Event &event) :
	window(window),
	event(event),
	tile(sf::Vector2f(TILE_SIZE, TILE_SIZE)),
	piece(std::make_unique<Piece>(table)),
	line{ {{0.0f, TILE_SIZE * MAX_TILE_SIDE}},
			{{TILE_SIZE * TABLE_WIDTH, TILE_SIZE * MAX_TILE_SIDE}} }
{

	f.loadFromFile("arial.ttf");
	t.setFont(f);
	t.setString("Game Over!");
	t.setCharacterSize(32);
	t.setStyle(sf::Text::Bold | sf::Text::Italic);
	t.setFillColor(sf::Color::Red);
	t.setPosition(20.0f, 50.0f);
}

void Game::handleEvents()
{
	if (!game_over)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::A) piece->moveLeft();
			if (event.key.code == sf::Keyboard::D) piece->moveRight();
			if (event.key.code == sf::Keyboard::Left) piece->rotateCCW();
			if (event.key.code == sf::Keyboard::Right) piece->rotateCW();
		}
	}
}

bool Game::update(float dt)
{
	if (!game_over)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) delay = 0.01f;
		else delay = 0.5f;

		if (dt >= delay)
		{
			piece->fall();
			if (last_n_pieces != Piece::counter && !game_over)
			{
				last_n_pieces = Piece::counter;
				piece = std::make_unique<Piece>(table);
			}
			return 1;
		}
	}
	return 0;
}

void Game::draw()
{
	window.draw(line, 2, sf::Lines);
	piece->setInTable();
	for (size_t i = 0; i < TABLE_WIDTH; i++)
		for (size_t j = 0; j < TABLE_HEIGHT; j++)
	{
		if (table[getTableIndex(i, j)])
		{
			tile.setPosition(static_cast<float>(i * TILE_SIZE), static_cast<float>(j * TILE_SIZE));
			switch (table[getTableIndex(i, j)])
			{
			case 1:
				tile.setFillColor(sf::Color::Blue);
				break;
			case 2:
				tile.setFillColor(sf::Color::Red);
				break;
			case 3:
				tile.setFillColor(sf::Color::Magenta);
				break;
			case 4:
				tile.setFillColor(sf::Color::Yellow);
				break;
			case 5:
				tile.setFillColor(sf::Color::Green);
				break;
			}
			window.draw(tile);
		}
	}
	piece->removeFromTable();

	if (game_over) window.draw(t);
}