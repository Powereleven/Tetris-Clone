#pragma once
#include "Piece.h"
#include <memory>

// Tile size (in pixels)
constexpr float TILE_SIZE = 10.0f;

class Game
{
public:
	Game(sf::RenderWindow &window, sf::Event &event);
	void handleEvents();
	bool update(float dt);
	void draw();

private:
	sf::RenderWindow &window; sf::Event &event;
	sf::RectangleShape tile;

	sf::Font f;
	sf::Text t;

	std::unique_ptr<Piece> piece;
	sf::Vertex line[2];
	std::array<int, TABLE_WIDTH * TABLE_HEIGHT> table = { 0 };
	unsigned int last_n_pieces = 0;

	float delay;
};