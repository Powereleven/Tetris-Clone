#include "Piece.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

static size_t N_COLORS = 5;

size_t MAX_TILE_SIDE = 0;
bool game_over = 0;

size_t Piece::counter = 0;
size_t getTableIndex(size_t i, size_t j);

size_t Piece::getPieceIndex(size_t i, size_t j)
{
	return i * TILE_SIDE + j;
}

void Piece::transpose()
{
	auto copy = piece;
	for (size_t x = 0; x < TILE_SIDE; x++)
		for (size_t y = 0; y < TILE_SIDE; y++)
		{
			piece[getPieceIndex(x, y)] = copy[getPieceIndex(y, x)];
		}
}

void Piece::mirrorXAxis()
{
	auto copy = piece;
	for (size_t x = 0; x < TILE_SIDE; x++)
		for (size_t y = 0; y < TILE_SIDE; y++)
		{
			piece[getPieceIndex(x, y)] = copy[getPieceIndex(TILE_SIDE - 1 - x, y)];
		}
}

void Piece::mirrorYAxis()
{
	auto copy = piece;
	for (size_t x = 0; x < TILE_SIDE; x++)
		for (size_t y = 0; y < TILE_SIDE; y++)
		{
			piece[getPieceIndex(x, y)] = copy[getPieceIndex(x, TILE_SIDE - 1 - y)];
		}
}

void Piece::piecesReader(std::string file_name)
{
	// it's important to have 2 extra lines in the end of the Pieces.txt file
	std::ifstream file(file_name);
	std::string line;
	bool b, // reads each boolean
	aux = 1; // resizes the vector only once (after reading the first line of the piece)
	size_t line_side = 0;
	std::vector<bool> this_piece;
	std::vector<bool> booleans;

	while (std::getline(file, line))
	{
		std::stringstream stream(line);

		if (!line.empty()) // means we are reading data still from the same piece
		{
			if (aux)
			{
				while (stream >> b)
				{
					line_side++;
					booleans.push_back(b);
				}
				this_piece.resize(line_side * line_side);
				aux = 0;
			}
			else while (stream >> b) booleans.push_back(b);
		}
		else // finished reading a piece
		{
			N_DIFFERENT_TILES++;
			TILE_SIDE = line_side;
			MAX_TILE_SIDE = std::max(TILE_SIDE, MAX_TILE_SIDE); // stores the biggest piece's side (used to set the map delimiter at the top)

			size_t i = 0;
			for (size_t y = 0; y < line_side; y++)
				for (size_t x = 0; x < line_side; x++)
					this_piece[getPieceIndex(x, y)] = booleans[i++];

			pieces.push_back(this_piece);
			// ready to read next piece
			aux = 1; line_side = 0;
			this_piece.clear();
			booleans.clear();
		}
	}
}

Piece::Piece(std::array<int, TABLE_WIDTH * TABLE_HEIGHT> &table) :
	table(table),
	coord(0, 0),
	color(rand() % N_COLORS + 1)
{
	piecesReader("Pieces.txt");

	size_t n = rand() % N_DIFFERENT_TILES;
	piece = pieces[n];
	TILE_SIDE = sqrt(piece.size());
}

void Piece::fall()
{
	coord.y++;
	if (isCollide()) // piece hit the ground
	{
		coord.y--;
		for (size_t i = 0; i < TILE_SIDE; i++)
			for (size_t j = 0; j < TILE_SIDE; j++)
			{
				if (piece[getPieceIndex(i, j)]) table[getTableIndex(i + coord.x, j + coord.y)] = color;
				if (j + coord.y < MAX_TILE_SIDE) game_over = 1;
			}
		counter++;

		// check for completed rows or cols
		checkLines();
	}
}

void Piece::moveLeft()
{
	coord.x--;
	if (isCollide()) coord.x++;
}

void Piece::moveRight()
{
	coord.x++;
	if (isCollide()) coord.x--;
}

void Piece::rotateCW()
{
	transpose();
	mirrorXAxis();
	if (isCollide())
	{
		mirrorXAxis();
		transpose();
	}
}

void Piece::rotateCCW()
{
	transpose();
	mirrorYAxis();
	if (isCollide())
	{
		mirrorYAxis();
		transpose();
	}
}

bool Piece::isCollide()
{
	for (int i = 0; i < TILE_SIDE; i++)
		for (int j = 0; j < TILE_SIDE; j++)
			if (piece[getPieceIndex(i, j)] && (i + coord.x == TABLE_WIDTH || i + coord.x == -1 || 
				j + coord.y == TABLE_HEIGHT || table[getTableIndex(i + coord.x, j + coord.y)])) return 1;
	return 0;
}

void Piece::checkLines()
{
	bool score;
	size_t x, y;
	// Row
	for (y = 0; y < TABLE_HEIGHT; y++)
	{
		score = 1;
		for (x = 0; x < TABLE_WIDTH; x++)
		{
			if (!table[getTableIndex(x, y)])
			{
				score = 0; break;
			}
		}
		if (score)
		{
			for (x = 0; x < TABLE_WIDTH; x++)
				table[getTableIndex(x, y)] = 0;
		}
	}
	// Col
	for (x = 0; x < TABLE_WIDTH; x++)
	{
		score = 1;
		for (y = MAX_TILE_SIDE; y < TABLE_HEIGHT; y++)
		{
			if (!table[getTableIndex(x, y)])
			{
				score = 0; break;
			}
		}
		if (score)
		{
			for (y = 0; y < TABLE_HEIGHT; y++)
				table[getTableIndex(x, y)] = 0;
		}
	}
}

void Piece::setInTable()
{
	for (size_t i = 0; i < TILE_SIDE; i++)
		for (size_t j = 0; j < TILE_SIDE; j++)
			if (piece[getPieceIndex(i, j)]) table[getTableIndex(i + coord.x, j + coord.y)] = color;
}

void Piece::removeFromTable()
{
	for (size_t i = 0; i < TILE_SIDE; i++)
		for (size_t j = 0; j < TILE_SIDE; j++)
			if (piece[getPieceIndex(i, j)]) table[getTableIndex(i + coord.x, j + coord.y)] = 0;
}