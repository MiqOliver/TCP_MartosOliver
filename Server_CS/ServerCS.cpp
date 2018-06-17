// ServerCS.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>
#include "Receptor.hpp"
#include <SFML\Network.hpp>
#include "../GameLib/Utils.h"
#include "../GameLib/Player.h"

using namespace std;

void main()
{
#pragma region Connection

	//ESTABLECER CONEXION
	sf::TcpListener listener;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	vector<Player*> players;

	//Ponemos el listener a escuchar por el puerto
	if (listener.listen(PORT) != sf::Socket::Done) {
		cout << "No se puede vincular al puerto 50000" << endl;
	}

	//Se conectan todos los jugadores
	for (int i = 1; i <= MAX_PLAYERS; i++) {
		sf::TcpSocket* sock = new sf::TcpSocket();
		if (listener.accept(*sock) != sf::Socket::Done) {
			cout << "No se pudo conectar al socket" << endl;
			delete sock;
		}
		else {
			Player player;
			player.id = i;
			player.nick = "Player " + i;
			player.sock = sock;
			players.push_back(&player);

			sf::Packet pack;
			pack << Utils::PLAYERINFO << player.id;
			sock->send(pack);
			//for each (Player* p in players)
			//{
			//	sf::Packet pack;
			//	pack << Utils::PLAYERINFO << player.id;
			//	p->sock->send(pack);
			//}
		}
	}

	listener.close();

#pragma endregion

#pragma region Loop

	sf::SocketSelector selector;
	sf::Packet pack;

	bool end = false;
	while (!end) {
		if (selector.wait()) {
			for each (Player* p in players)
			{
				sf::Socket::Status status = p->sock->receive(pack);
				if (status != sf::Socket::Done) {
					if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
						for each (Player* _p in players)
						{
							pack.clear();
							pack << (int)Utils::DISCONNECT << p->id;
							_p->sock->send(pack);
						}
						delete p;
						continue;
					}
				}
				else {

				}
			}
		}
	}

#pragma endregion

}