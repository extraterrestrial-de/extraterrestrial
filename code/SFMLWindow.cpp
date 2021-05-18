#include "SFMLWindow.hpp"

#ifdef SFML

DebugWindow::DebugWindow(Robot *robot) : Bot(robot) {

    Font.loadFromFile("C:/CoSpaceRobot Studio/RCJ-2021/User/RSC/extraterrestrial/fonts/arial.ttf");

}

void DebugWindow::GameDebug(unsigned int ID) {

    Field *map = maps[ID];

    sf::RenderWindow window(sf::VideoMode(1080, 840), "Map" + std::to_string(ID));
    sf::View view = window.getDefaultView();
    view.setSize(1080, -840);
    window.setView(view);
    runningWindows.push_back(&window);

    long long int index = 0;

    while (isRunning[ID]) {

        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
                isRunning[ID] = false;
                return;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            continue;
        }

        // clear screen
        window.clear(sf::Color::Black);

        // Vars for drawing on map
        sf::Vector2u size = window.getSize();
        sf::Vector2f scale = {static_cast<float>(size.x) / static_cast<float>(map->getSize().x),
                              static_cast<float>(size.y) / static_cast<float>(map->getSize().y)};
        sf::RectangleShape block(sf::Vector2f(scale.x, scale.y));

        //##########//
        //  print   //
        //##########//

        // Map label
        sf::Text label("Map", Font);
        label.setPosition(10, 5);
        label.setCharacterSize(30);
        label.setFillColor(sf::Color::Black);
        label.setStyle(sf::Text::Regular);
        window.draw(label);

        // Map:
        for (auto &walls : map->getMapObjects({0, 1, 2})) {
            for (auto &edge : walls.getEdges()) {
                sf::Vector2f p1(static_cast<float>(edge.p1.x * scale.x),
                                static_cast<float>(edge.p1.y * scale.y));
                sf::Vector2f p2(static_cast<float>(edge.p2.x * scale.x),
                                static_cast<float>(edge.p2.y * scale.y));

                sf::Vertex area[] = {{p1, sf::Color(0, 255, 0, 80)},
                                     {p2, sf::Color(0, 255, 0, 80)}};

                window.draw(area, 2, sf::Lines);
            }
        }

        block.setSize({scale.x * 2, scale.y * 2});
        block.setOrigin(scale.x, scale.y);
        for (auto &collectible : map->getCollectibles({0, 1, 2})) {
            sf::Color color = getCollectibleSFMLColor(collectible);
            block.setPosition(scale.x * collectible->pos.x, scale.y * collectible->pos.y);
            block.setFillColor(color);
            window.draw(block);
        }
#ifdef DEBUG_PATHFINDER_NODES

        int nodeIndex = static_cast<int>(std::floor(static_cast<float>(index) / 500)) %
                        static_cast<int>(Bot->pathfinder1T.map.size());

        block.setFillColor({255, 0, 0});
        auto node = Bot->pathfinder1T.map[nodeIndex];

        sf::Vector2f p1(static_cast<float>(node.pos.x * scale.x),
                        static_cast<float>(node.pos.y * scale.y));
        block.setPosition(p1);

        for (auto neighbour : node.neighbours) {

            sf::Vector2f p2(static_cast<float>(neighbour.first->pos.x * scale.x),
                            static_cast<float>(neighbour.first->pos.y * scale.y));

            int x = (neighbour.second <= 255) ? 255 - static_cast<int> (std::round(neighbour.second)) : 0;

            sf::Color color = sf::Color(255, x, x);

            sf::Vertex line[] = {{p1, color},
                                 {p2, color}};

            window.draw(line, 2, sf::Lines);
        }


        window.draw(block);

#endif



        // Path
        block.setSize(sf::Vector2f(scale.x * 3, scale.y * 3));
        block.setOrigin(scale.x * 1.5f, scale.y * 1.5f);
        sf::VertexArray path_lines(sf::LineStrip);
        for (unsigned int i = 0; i < Bot->completePath.size(); i++) {

            Path path = Bot->completePath[i].first;


            sf::Uint8 rval = i * 80 + 80;
            block.setFillColor({rval, 0, 255});


            for (unsigned int j = 0; j < path.points.size(); j++) {


                PVector point = path.points[j];

                path_lines.append({sf::Vector2f(static_cast<float> (point.x) * scale.x,
                                                static_cast<float> (point.y) * scale.y),
                                   sf::Color::Red});
                block.setPosition(static_cast<float> (point.x) * scale.x,
                                  static_cast<float> (point.y) * scale.y);

                if (j == path.points.size() - 1) {

                    if (Bot->completePath[i].second) {
                        block.setSize(sf::Vector2f(scale.x * 5, scale.y * 5));
                        block.setOrigin(scale.x * 2.5f, scale.y * 2.5f);
                        block.setFillColor(getCollectibleSFMLColor(
                                static_cast<Collectible *>(Bot->completePath[i].second)));
                    } else {
                        block.setFillColor({200, 100, 0});
                    }

                    sf::Text text;
                    text.setString(std::to_string(i));
                    text.setFillColor(sf::Color::White);
                    text.setCharacterSize(30);
                    text.setFont(Font);

                    text.setPosition(static_cast<float> (point.x) * scale.x,
                                     static_cast<float> (point.y) * scale.y + 10);
                    text.rotate(M_PI/2);
                    window.draw(text);
                }

                window.draw(block);
            }
            block.setFillColor({160, 0, 255});       // purple
        }

        window.draw(path_lines);

        // Position
        block.setSize(sf::Vector2f(scale.x * 4, scale.y * 4));
        block.setOrigin(scale.x * 2, scale.y * 2);
        block.setFillColor({140, 30, 0});             // dark red / brown
        block.setPosition(static_cast<float> (Bot->pos.x) * scale.x,
                          static_cast<float> (Bot->pos.y) * scale.y);
        window.draw(block);

        window.display();

        index++;
    }

    window.close();
}

sf::Color DebugWindow::getCollectibleSFMLColor(Collectible *collectible) {
    sf::Color color = sf::Color(255, 0, 0);
    switch (collectible->color) {
        case 0:
            break;
        case 1:
            color.r = 0;
            color.b = 255;
            break;
        default:
            color.r = 0;
            color.g = 255;
            break;
    }


    switch (collectible->state) {
        case 0:
            break;
        case 1:
            color.a = 60;
            break;
        default:
            color.a = 30;
            break;
    }
    return color;
}

void DebugWindow::startDebugging(Field *map) {

    maps.push_back(map);
    isRunning.push_back(true);

    runningLoops.emplace_back(&DebugWindow::GameDebug, this, IDs.size());
    IDs.push_back(IDs.size());
}

void DebugWindow::stopDebugging(unsigned int ID) {

    unsigned int index = std::find(IDs.begin(), IDs.end(), ID) - IDs.begin();

    if (index < IDs.size()) {

        // make sure the window gets closed
        isRunning[index] = false;
        runningLoops[index].join();


        // remove it from the vectors
        IDs.erase(IDs.begin() + index);
        maps.erase(maps.begin() + index);
        runningLoops.erase(runningLoops.begin() + index);
        runningWindows.erase(runningWindows.begin() + index);
        isRunning.erase(isRunning.begin() + index);
    }
}

void DebugWindow::updateLoop() {

    std::vector<unsigned int> canBeDeleted;

    for (unsigned int i = 0; i < IDs.size(); i++) {
        if (!isRunning[i]) {
            canBeDeleted.push_back(IDs[i]);
        }
    }

    for (auto ID : canBeDeleted) {
        stopDebugging(ID);
    }

}

void DebugWindow::stopAll() {
    for (unsigned int i = 0; i < IDs.size(); i++) {
        // make sure the window gets closed
        isRunning[i] = false;
        runningLoops[i].join();


        // remove it from the vectors
        IDs.erase(IDs.begin() + i);
        maps.erase(maps.begin() + i);
        runningLoops.erase(runningLoops.begin() + i);
        runningWindows.erase(runningWindows.begin() + i);
        isRunning.erase(isRunning.begin() + i);
    }
}

#endif