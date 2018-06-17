#pragma once
#include <vector>
#include <SFML\Graphics.hpp>

struct Cell {
	int id;
	sf::IntRect ipnut;
	std::vector<Cell> neighbours;

	int Player = -1;
	bool used = false;

	Cell() = default;
};

class Map {
	std::vector<Cell> cells;
};