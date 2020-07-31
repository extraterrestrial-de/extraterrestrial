#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <vector>
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Point.hpp"

class MapData {
public:
	int MapX;
	int MapY;
	std::vector<std::vector<int>> Map;
	MapData(int mapX, int mapY,
            std::vector<std::pair<int, int>> *redPoints,
            std::vector<std::pair<int, int>> *greenPoints,
            std::vector<std::pair<int, int>> *blackPoints,
            std::vector<std::pair<int, int>> *deposits,
            std::vector<std::pair<int, int>> *walls,
            std::vector<std::pair<int, int>> *traps,
            std::vector<std::pair<int, int>> *swamps);

    Point *find_point(std::pair<int, int> pos, int pColor);

    std::vector<Point *> getAllPoints();
    Point *getClosestPoint(std::pair<int, int> pos);
    std::vector<std::pair<int, int>> getDepositAreas();

    int markPoint(std::pair<int, int> pos, int state, int c);

    std::pair<std::vector<Point *>, std::pair<int, int>> get_path(std::array<int, 3> loaded_objects, int loaded_objects_num, std::pair<int, int> pos);

    void print(sf::RenderWindow *window);

private:
    std::vector<Point> *get_ptr_to_arr(const int &_color);

    std::vector<std::pair<int, int>> DepositAreas;
    std::vector<Point> RedPoints;
    std::vector<Point> GreenPoints;
    std::vector<Point> BlackPoints;
    std::vector<Point *> AllPoints;
};

/*walls*/ extern std::vector<std::pair<int, int>> GAME0WALLS;
/*traps*/ extern std::vector<std::pair<int, int>> GAME0TRAPS;
/*swamps*/ extern std::vector<std::pair<int, int>> GAME0SWAMPS;
/*deposit*/ extern std::vector<std::pair<int, int>> GAME0DEPOSITAREAS;

extern std::vector<std::pair<int, int>> GAME0REDPOINTS;
extern std::vector<std::pair<int, int>> GAME0GREENPOINTS;
extern std::vector<std::pair<int, int>> GAME0BLACKPOINTS;


/*walls*/ extern std::vector<std::pair<int, int>> GAME1WALLS;
/*traps*/ extern std::vector<std::pair<int, int>> GAME1TRAPS;
/*swamps*/ extern std::vector<std::pair<int, int>> GAME1SWAMPS;
/*deposit*/ extern std::vector<std::pair<int, int>> GAME1DEPOSITAREAS;

extern std::vector<std::pair<int, int>> GAME1REDPOINTS;
extern std::vector<std::pair<int, int>> GAME1GREENPOINTS;
extern std::vector<std::pair<int, int>> GAME1BLACKPOINTS;

#endif // !MAPDATA_HPP