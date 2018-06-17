#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "Direction.h"
#include "Utils.h"
#include "Receptor.hpp"

using namespace utils;

void main() {
	Direction dir[MAX_PLAYERS - 1];
	vector<sf::TcpSocket*> sock;
	sf::TcpSocket serverSock;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpListener listener;

	std::cout << "Enter your NickName:" << endl;
	std::cin >> nick;

#pragma region Connection
	//Connect to server
	if (serverSock.connect(ip, PORT) != sf::Socket::Done)
		cout << "No se puede conectar al servidor" << endl;
	else {
		serverSock.send(nick.c_str(), nick.length());

		sf::Packet peers;
		cout << "Esperando Peers" << endl;
		if(serverSock.receive(peers) != sf::Socket::Done)
			cout << "No se recibieron los otros peers" << endl;
		else {
			unsigned short myPort = serverSock.getLocalPort();
			serverSock.disconnect();
			cout << "Peers recibidos" << endl;
			int players;
			peers >> players;
			for (int i = 0; i < players; i++) {
				sf::TcpSocket* auxSock = new sf::TcpSocket();
				peers >> dir[i].nick >> dir[i].ip >> dir[i].port;
				if (auxSock->connect(ip, dir[i].port) != sf::Socket::Done) {
					cout << "Imposible conectar al peer: " << i << endl;
				}
				else {
					cout << "Conexion con exito al peer: " << i << endl;
					cout << "Socket " << i << ": IP - " << auxSock->getRemoteAddress().toString() << " PORT - " << auxSock->getRemotePort() << endl;
					sock.push_back(auxSock);
				}
			}
			if (listener.listen(myPort) != sf::Socket::Done) {
				cout << "No se puede vincular al puerto 5000" << endl;
			}
			for (int i = 0; i < MAX_PLAYERS - 1 - players; i++) {
				sf::TcpSocket* auxSock = new sf::TcpSocket();
				if (listener.accept(*auxSock) == sf::Socket::Done) {
					sock.push_back(auxSock);
					cout << "Conexion con exito al peer: " << players + i << endl;
					cout << "Socket " << i << ": IP - " << auxSock->getRemoteAddress().toString() << " PORT - " << auxSock->getRemotePort() << endl;
				}
			}
			listener.close();
		}
	}
#pragma endregion

#pragma region ScreenDisplay
	sf::Vector2i screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	sf::RenderWindow window;
	string windowName = "Chat " + nick;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), windowName);

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) {
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje = "";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);
#pragma endregion

	string sendText;
	size_t received = 0;

	vector<pair<string, int>> aMensajes;
	pair<string, int> message;
	Receptor r(&sock, &aMensajes);
	thread t(r); 

#pragma region InteractionLoop
	while (window.isOpen()) {
		if (sock.size() == 0)
			window.close();
		sf::Event evento;
		while (window.pollEvent(evento)) {
			switch (evento.type) {
			case sf::Event::Closed:
				sendText = nick + " has disconnected";
				for each (sf::TcpSocket* s in sock) {
					s->send(sendText.c_str(), sendText.length());
				}
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape) {
					sendText = nick + " has disconnected";
					for each (sf::TcpSocket* s in sock) {
						s->send(sendText.c_str(), sendText.length());
					}
					window.close();
				}
				else if (evento.key.code == sf::Keyboard::Return) {
					if (mensaje == "exit") {
						sendText = nick + " has disconnected";
						for each (sf::TcpSocket* s in sock) {
							s->send(sendText.c_str(), sendText.length());
						}
						window.close();
					}
					else {
						mensaje = nick + ": " + mensaje;
						message = { mensaje, -1 };
						mu.lock();
						aMensajes.push_back(message);
						if (aMensajes.size() > 25) {
							aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
						}
						mu.unlock();
						//SEND
						sendText = mensaje;
						for (int i = 0; i < sock.size(); i++) {
							sf::Socket::Status status = sock[i]->send(sendText.c_str(), sendText.length());
							if (status == sf::Socket::Error) {
								cout << "Ha fallado el envio de datos" << endl;
							}
							else if (status == sf::Socket::Disconnected) {
								cout << "Se ha desconectado " << sock[i]->getRemotePort() << endl;
								sock[i]->disconnect();
							}
						}
						sendText = "";
						mensaje = "";
					}
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
				break;
			}
		}
		window.draw(separator);
#pragma endregion

#pragma region DrawMessages
		for (size_t i = 0; i < aMensajes.size(); i++) {
			string chatting = aMensajes[i].first;
			chattingText.setPosition(sf::Vector2f(0, 20 * (float)i));
			chattingText.setString(chatting);
			switch (aMensajes[i].second) {
			case 0:
				chattingText.setFillColor(COLOR_0);
				break;
			case 1:
				chattingText.setFillColor(COLOR_1);
				break;
			case 2:
				chattingText.setFillColor(COLOR_2);
				break;
			default:
				chattingText.setFillColor(MINE_COLOR);
				break;
			}
			window.draw(chattingText);
		}
		string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window.draw(text);
#pragma endregion

		window.display();
		window.clear();
	}

	for each (sf::TcpSocket* s in sock) {
		s->disconnect();
	}

	sock.clear();

	t.join();
}