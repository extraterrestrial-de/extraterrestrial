#define CsBot_AI_H //DO NOT delete this line
#define CsBot_AI_C  //DO NOT delete this line
#ifndef CSBOT_REAL

// Libs
#include "libs/PPSettings.hpp"
#include "libs/CoSpaceFunctions.hpp"
#include "libs/CommonFunctions.hpp"
#include "libs/ColorRecognition.hpp"

// Classes
#include "Pathfinder.hpp"
#include "MapData.hpp"
#include "Robot.hpp"

#include <iostream>
#include <fileapi.h>

#endif


#ifdef SFML

#include "SFMLWindow.hpp"

DebugWindow *debugWindow = nullptr;

#endif

Field *GAME0 = nullptr;

Field *GAME1 = nullptr;

Robot *Bot = nullptr;

void Setup() {
	system("cls");
	std::cout << "Setup... ";

	// TODO: Move static objects back to static space so the objects get initialized upon load
	// ----------- Initialisation of static objects -------------------- //

	static Field Game0(270, 180, GAME0WALLS, GAME0TRAPS, GAME0SWAMPS, GAME0WATERS, GAME0DEPOSITS,
	                   GAME0WALLNODES, GAME0TRAPNODES, GAME0SWAMPNODES, GAME0COLLECTIBLES);
	GAME0 = &Game0;

	static Field Game1(360, 270, GAME1WALLS, GAME1TRAPS, GAME1SWAMPS, GAME1WATERS, GAME1DEPOSITS,
	                   GAME1WALLNODES, GAME1TRAPNODES, GAME1SWAMPNODES, GAME1COLLECTIBLES);
	GAME1 = &Game1;

	static Robot bot(INPUT, OUTPUT, {&SuperObj_X, &SuperObj_Y, &SuperObj_Num}, GAME0, GAME1);
	Bot = &bot;

#ifdef SFML
	static DebugWindow window(Bot);
	window.startDebugging(GAME1);
	debugWindow = &window;
#endif
	std::cout << "done" << std::endl;
}

/*
 * ///_________________________________GAME0________________________________________///
 *
 * ///__________________________________________________________________________________///
*/

void Game0() {
	std::cout << "Game0Loop... ";
    //Bot->updateLoop();
    Bot->game0Loop();
#ifdef SFML
	debugWindow->updateLoop();
#endif
	std::cout << "done" << std::endl;


}

/*
 * ///_________________________________GAME1________________________________________///
 *
 * ///__________________________________________________________________________________///
*/




void Game1() {
	std::cout << "Game1Loop... ";
    Bot->updateLoop();
    Bot->game1Loop();
#ifdef SFML
    debugWindow->updateLoop();
#endif
	std::cout << "done" << std::endl;
}