#ifndef PPSETTINGS_HPP
#define PPSETTINGS_HPP

/// ----------------------------- ///
///       program settings
/// ----------------------------- ///


/// define COLOR_LOGGING to enable color logging to console
//#define COLOR_LOGGING

/// define DEBUG to enable debug messages
#define DEBUG

/// ---------------------------------
///         SFML settings
/// ---------------------------------

/// define SFML to enable debugging via SFML
#define SFML

/// define DEBUG_PATHFINDER_NODES
//#define DEBUG_PATHFINDER_NODES




/// ----------------------------- ///
///       some debug logic
/// ----------------------------- ///

#ifdef SFML
#undef DEBUG
#endif

#ifdef DEBUG
/// DEBUG is defined so ERROR_MESSAGE prints out the function it is called in joined by a message
#define ERROR_MESSAGE(MESSAGE) {std::cerr << __FUNCTION__ << "\t" << MESSAGE << std::endl;}
#endif

#ifndef DEBUG
/// DEBUG is not defined so ERROR_MESSAGE does nothing
#define ERROR_MESSAGE(message) {}
#endif


/// ----------------------------- ///
///         robot settings
/// ----------------------------- ///

/// The speed of the robot in cm/ms for 1 % of maximum speed (wheels(1, 1))
#define ROBOT_SPEED 0.0005
#define BREAKING_COEFFICIENT 0.01

#define RPOS_ERROR_MARGIN 3

/// The distance between the two powered wheels in cm
#define ROBOT_AXLE_LENGTH 12.6

#define RCOLOR_X -3.6
#define RCOLOR_Y 6

#define LCOLOR_X 3.6
#define LCOLOR_Y 6

/// Revolutions per millisecond at 1% of robot's max wheel speed (1/30 rev/ms)
#define REVPERMS 0.00003333

/// X Distance of the right wheel to the robot's center in cm
#define RWHEEL_X -6.3
/// Y Distance of the right wheel to the robot's center in cm
#define RWHEEL_Y -4.2
/// Radius of the right wheel in cm
#define RWHEEL_RADIUS 3


/// X Distance of the left wheel to the robot's center in cm
#define LWHEEL_X 6.3
/// Y Distance of the left wheel to the robot's center in cm
#define LWHEEL_Y -4.2
/// Radius of the left wheel in cm
#define LWHEEL_RADIUS 3


#define COLOR_SENSOR_ANGLE_OFFSET 49.13072561
#define COLOR_SENSOR_DIST_TO_CORE 6.99714227

#define GAME0_TIME 180
#define GAME1_TIME 300

/// The factor by what speed is reduced in swamps TODO
#define SWAMP_SPEED_PENALITY 10

/// Standard radius of a path in cm
#define PATH_RADIUS 10

/// ----------------------------- ///
///      CoSpace constants
/// ----------------------------- ///

/// Time to deposit in ms
#define DEPOSIT_TIME 6000

/// Time to collect in ms
#define COLLECT_TIME 4000

#endif //PPSETTINGS_HPP
