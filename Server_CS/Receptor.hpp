#pragma once
#include <vector>
#include <SFML\Network.hpp>
#include "../GameLib/Player.h"
#include "../GameLib/Utils.h"

using namespace std;

class Receptor {
public:
	vector<Player*> players;
	sf::SocketSelector selector;
	sf::Packet pack;

	Receptor() = default;
	Receptor(vector<Player*> players) : players(players) {
	}

	void operator() () {
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
	}
};