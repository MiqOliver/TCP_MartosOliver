#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include "Utils.h"

using namespace utils;
using namespace std;


struct Receptor {
public:
	vector<sf::TcpSocket*>* sock;
	sf::SocketSelector selector;
	vector<pair<string, int>>* aMsj;
	pair<string, int> message;
	char receivedText[MAX_LENGTH];
	size_t receivedLength;

	Receptor(vector<sf::TcpSocket*>* sock, vector<pair<string, int>>* aMsj) :	sock(sock),
																				aMsj(aMsj) {
		for each (sf::TcpSocket* s in *sock) {
			selector.add(*s);
		}
	}

	void operator() () {
		for (int i = 0; i < MAX_LENGTH; i++)
			receivedText[i] = ' ';
		while (sock->size() != 0) {
			if(selector.wait()) {
				for(int i = 0; i < sock->size(); i++){
					if (selector.isReady(*sock->at(i))) {
						sf::Socket::Status status = sock->at(i)->receive(receivedText, MAX_LENGTH, receivedLength);
						if (status == sf::Socket::Done) {
							message = { receivedText, i };
							mu.lock();
							aMsj->push_back(message);
							for (int i = 0; i < MAX_LENGTH; i++)
								receivedText[i] = ' ';
							if (aMsj->size() > 25) {
								aMsj->erase(aMsj->begin(), aMsj->begin() + 1);
							}
							mu.unlock();
						}
						else if (status == sf::Socket::Disconnected) {
							selector.remove(*sock->at(i));
							sock->at(i)->disconnect();
							sock->erase(sock->begin() + i);
							std::cout << "Elimino el socket que se ha desconectado" << endl;
							break;
						} 
						else{
							cout << "Error al recibir mensaje" << endl;
						}
					}
				}
			}
		}
	}
};