#pragma once
#include <string>

namespace Utils {
#define PORT 50000
#define MAX_PLAYERS 4

#define WIDTH 780
#define HEIGHT 933

	enum Protocol {
		DISCONNECT,
		POSITION,
		PLAYERINFO,
		START,
		END,
		TURN
	};

	std::string background = "../rsc/bg.pgn";
	std::string shpaes[4] = {

	};
}