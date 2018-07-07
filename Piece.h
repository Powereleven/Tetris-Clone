#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

// Settings
constexpr size_t TABLE_WIDTH = 30;
constexpr size_t TABLE_HEIGHT = 60;

class Piece
{
public:
	Piece(std::array<int, TABLE_WIDTH * TABLE_HEIGHT> &table);
	static size_t counter;

	void fall();
	void moveLeft();
	void moveRight();

	void rotateCW();
	void rotateCCW();

	void setInTable();
	void removeFromTable();

private:
	std::array<int, TABLE_WIDTH * TABLE_HEIGHT> &table;

	sf::Vector2i coord;

	// each peace has its own color
	int color;

	// side of the square that is the piece's bounding box
	size_t TILE_SIDE;

	size_t N_DIFFERENT_TILES = 0;

	std::vector<bool> piece;
	size_t getPieceIndex(size_t i, size_t j);

	bool isCollide();
	void checkLines();

	// functions to do the piece's bounding box rotation
	void transpose();
	void mirrorXAxis();
	void mirrorYAxis();

	// reads from file each peace and saves in the vector of vectors
	void piecesReader(std::string file_name);
	std::vector<std::vector<bool>> pieces;
};