#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "libs/PPSettings.hpp"
#include "libs/CommonFunctions.hpp"
#include "libs/ColorRecognition.hpp"
#include "libs/CoSpaceFunctions.hpp"

#include "MapData.hpp"
#include "Pathfinder.hpp"

#include <vector>
#include <ctime>
#include <chrono>
#include <array>


class ObjectLoad {
private:
    std::array<std::vector<Collectible *>, 4> loadedObjects_;
    unsigned int num_;

public:
    ObjectLoad();
    explicit ObjectLoad(const std::vector<Collectible *>&  objects);

    void addObject(Collectible *object);

    bool removeObject(Collectible *object);

    void setLoad(const std::vector<Collectible *>& objects);

    void clearLoad();

    unsigned int getValue();

    unsigned int rgb();

    unsigned int red();

    unsigned int cyan();

    unsigned int black();

    unsigned int super();

    std::array<unsigned int, 4> all();

    const std::array<std::vector<Collectible *>, 4> &loadedObjects();

    [[nodiscard]] unsigned int num() const;
};


/** The robot of the CoSpace-simulator
 *
*/
class Robot {
public:
    Robot(int *_x, int *_y, int *_compass, int *_superObjectX, int *_superObjectY,
          int *_ultraSonicSensorLeft, int *_ultraSonicSensorFront, int *_ultraSonicSensorRight,
          int *_wheelLeft, int *_wheelRight, int *_led, int *_tp, int *_gameTime,
          Field *_map0, Field *_map1);

    //               ______
    //______________/ vars \_____________

    // === Variable pointers to vars updated by the sim ===
    int *posX, *posY;
    int *compass;
    int *superObjectX, *superObjectY;
    std::vector<Collectible *> superObjects;
    HSLColor leftColor, rightColor;
    std::array<int *, 3> ultraSonicSensors;
    int *wheelLeft, *wheelRight;
    int *led;
    int *tp;
    unsigned short level;
    int *gameTime;
    int remainingMapTime;

    /// typedef for time (basically a macro)
    typedef std::chrono::steady_clock timer;

    /// the complete path (contains sub-paths from point to point)
    std::vector<Path> completePath;

    /// controls the robots wheels
    void wheels(int l, int r) const;

    void moveAlongPath(Path &path);

    /// updates the position of the robot mathematically and returns the change
    PVector updatePos();

    int moveToPosition(PVector p);

    /// checks if l, f or r is higher than the us-sensor values. returns a binary-encoded value
    int checkUsSensors(int l, int f, int r);

    void updateLoop();

    /// game loop for first map
    void game0Loop();

    /// game loop for second map
    void game1Loop();

    PVector pos;
    PVector lastPos;

    // === Robot vars ===

    Field *map0, *map1;
    Pathfinder pathfinder0, pathfinder1;
    Pathfinder pathfinder0T, pathfinder1T;

    ObjectLoad loadedObjects;
    unsigned int lastRGBBonus;

    bool huntingSuperObj;

    std::chrono::time_point<timer> lastCycle;              ///< the time the last cycle was executed
    std::chrono::time_point<timer> depositingSince;        ///< the time last depositing has started
    std::chrono::time_point<timer> collectingSince;        ///< the time last collecting has started
    std::chrono::time_point<timer> lastPositionUpdate;     ///< the last time the position was updated

    //               ___________
    //______________/ functions \_____________

    /// gets the current velocity for a certain change in time (in ms)
    [[nodiscard]] PVector getVelocity(long long int dt) const;


    /// decides whether collecting a point is a good idea or not
    bool shouldCollect();

    /// collects a point
    int collect();

    /// decides whether depositing is a good idea or not
    bool shouldDeposit();

    /// deposits
    void deposit();

    /// decides whether teleporting is a good idea or not
    bool shouldTeleport();

    /// teleports
    void teleport();

    /// returns a turning direction if the robot is about to drive off map
    [[nodiscard]] int avoidVoid() const;

    [[nodiscard]] std::array<int, 4> getDesiredLoad() const;

    std::vector<PVector> getPointPath(std::array<int, 4> desiredLoad);
};


#endif //ROBOT_HPP
