// ClientCS.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <SFML\Network.hpp>
#include <iostream>
#include "../GameLib/Utils.h"
#include "../GameLib/Player.h"
#include "../GameLib/Renderer.h"


void main()
{
	Player me;
	bool myTurn = false;

#pragma region Connection

	//ESTABLECER CONEXION
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;

	sf::Socket::Status status = socket.connect(ip, PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Error de conexion\n";
		exit(0);
	}
	else {
		std::cout << "Conexion establecida\n";
	}

#pragma endregion

	int prot;
	sf::Packet receivedData;
	sf::Socket::Status stat = socket.receive(receivedData);
	if (stat != sf::Socket::Done) {
		//ERROR
	}
	else {
		receivedData >> prot;
		if (prot == Utils::PLAYERINFO) {
			receivedData >> me.id;
			std::cout << me.id << std::endl;
		}
	}

	//R.CreateWindow(WIDTH, HEIGHT, "Connect 3");
	//R.SetBackground(Utils::background);

#pragma region GameLoop

	bool end = false;
	while (!end) {
		if (myTurn) {

		}
		else {
			receivedData.clear();
			stat = socket.receive(receivedData);
		}

		//R.Draw();
	}

#pragma endregion

}