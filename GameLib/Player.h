#pragma once
#include <string>
#include <SFML\Graphics.hpp>

class Player {
public:
	int id;
	std::string nick;
	sf::Texture shape;
	sf::TcpSocket* sock;

	Player() = default;
};