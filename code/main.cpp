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

#endif


#ifdef SFML
#include <SFML/Graphics.hpp>

sf::RenderWindow *CC = nullptr;
sf::Font *unbutton = nullptr;
#endif


void updateHSL() {

    hueR = rgb2h(CSRight_R, CSRight_G, CSRight_B);
    hueL = rgb2h(CSLeft_R, CSLeft_G, CSLeft_B);

    satR = rgb2s(CSRight_R, CSRight_G, CSRight_B);
    satL = rgb2s(CSLeft_R, CSLeft_G, CSLeft_B);

    lumR = rgb2l(CSRight_R, CSRight_G, CSRight_B);
    lumL = rgb2l(CSLeft_R, CSLeft_G, CSLeft_B);
}

Field *GAME0 = nullptr;

Field *GAME1 = nullptr;

Robot *Bot = nullptr;

void Setup() {
    system("cls");

    // ----------- Initialisation of static objects -------------------- //

    static Field Game0(270, 180, GAME0WALLS, GAME0TRAPS, GAME0SWAMPS, GAME0WATERS, GAME0DEPOSITS, GAME0WALLNODES,
                       GAME0TRAPNODES,
                       GAME0COLLECTIBLES);
    GAME0 = &Game0;

    static Field Game1(360, 270, GAME1WALLS, GAME1TRAPS, GAME1SWAMPS, GAME1WATERS, GAME1DEPOSITS, GAME1WALLNODES,
                       GAME1TRAPNODES,
                       GAME1COLLECTIBLES);
    GAME1 = &Game1;

    static Robot bot(&PositionX, &PositionY, &Compass, &SuperObj_Num, &SuperObj_X, &SuperObj_Y,
                     &CSRight_R, &CSRight_G, &CSRight_B, &CSLeft_R, &CSLeft_G, &CSLeft_B,
                     &US_Left, &US_Front, &US_Right,
                     &WheelLeft, &WheelRight, &LED_1, &Teleport, &Time, GAME0, GAME1);
    Bot = &bot;


#ifdef SFML
    static sf::RenderWindow window(sf::VideoMode(1080, 840), "Debug_Console");
    CC = &window;
    CC->setFramerateLimit(10);

    static sf::Font font;
    if (font.loadFromFile("../fonts/unbutton.ttf")) {
        unbutton = &font;
    }
    else if (font.loadFromFile("unbutton.ttf")) {
        unbutton = &font;
    }
    else if (font.loadFromFile(
            "C:/Users/flxwly/Microsoft Robotics Dev Studio 4/CS.C/User/RSC/extraterrestrial/unbutton.ttf")) {
        unbutton = &font;
    }

    if (unbutton != nullptr) {
        std::cout << "loaded font" << std::endl;
    }
#endif

    updateHSL();

}

/*
 * ///_________________________________GAME0________________________________________///
 *
 * ///__________________________________________________________________________________///
*/

void Game0() {
    updateHSL();
    Bot->game0Loop();
}

/*
 * ///_________________________________GAME1________________________________________///
 *
 * ///__________________________________________________________________________________///
*/

void Game1Debug() {

#ifdef SFML
    //##########//
    //  Setup   //
    //##########//

    // check nullptr to prevent crash.
    if (unbutton == nullptr || CC == nullptr) {
        ERROR_MESSAGE("Something went wrong. Window or font is not initialized correctly.")
        return;
    }

    // Event handling
    sf::Event event;
    while (CC->pollEvent(event)) {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            CC->close();
    }

    // clear screen
    CC->clear(sf::Color::Black);

    // Vars for drawing on map
    sf::Vector2u size = CC->getSize();
    sf::Vector2f scale = {static_cast<float>(size.x) / static_cast<float>(GAME1->getSize().x),
                          static_cast<float>(size.y) / static_cast<float>(GAME1->getSize().y)};
    sf::RectangleShape block(sf::Vector2f(scale.x, scale.y));

    //##########//
    //  print   //
    //##########//

    // Map label
    sf::Text label("Map", *unbutton);
    label.setPosition(10, 5);
    label.setCharacterSize(30);
    label.setFillColor(sf::Color::Black);
    label.setStyle(sf::Text::Regular);
    CC->draw(label);

    // MapData
    //GAME1.print(CC);

    // Path
    block.setSize(sf::Vector2f(scale.x * 3, scale.y * 3));
    block.setFillColor({80, 0, 255});        // purpleish
    sf::VertexArray path_lines;
    for (const auto &path : Bot->completePath) {
        for (auto point : path.getPoints()) {
            path_lines.append({sf::Vector2f(static_cast<float> (point.x) * scale.x,
                                            static_cast<float> (GAME1->getSize().y - 1 - point.y) * scale.y),
                               sf::Color::Red});
            block.setPosition(static_cast<float> (point.x) * scale.y,
                              static_cast<float> (GAME1->getSize().x - 1 - point.y) * scale.y);
            CC->draw(block);
        }
        block.setFillColor({160, 0, 255});       // purple
    }

    CC->draw(path_lines);

    // Position
    block.setSize(sf::Vector2f(scale.x * 4, scale.y * 4));
    block.setFillColor({ 140, 30, 0 });             // dark red / brown
    block.setPosition(static_cast<float> (PositionX) * scale.x,
        static_cast<float> (GAME1->getSize().y - 1 - PositionY) * scale.y);
    CC->draw(block);

    CC->display();
#endif
}


void Game1() {
    updateHSL();

    Bot->game1Loop();
    //Game1Debug();
}