//
// Created by flxwl on 05.06.2020.
//

#include "Robot.hpp"

//====================================
//          Constructor
//====================================
Robot::Robot(int *_x, int *_y, int *_comp, int *_sobj_num, int *_sobj_x, int *_sobj_y,
             int *_rc_r, int *_rc_g, int *_rc_b, int *_lc_r, int *_lc_g, int *_lc_b,
             int *_rus, int *_fus, int *_lus,
             int *_whl_l, int *_whl_r, int *_led, int *_tp, int *_g_time,
             MapData *_map0, MapData *_map1, AStar *_pathfinder0, AStar *_pathfinder1, Speedometer *_speedometer) {

    Robot::x = _x, Robot::y = _y;                                       // robots position
    Robot::comp = _comp;                                                // compass
    Robot::sobj_num = _sobj_num;                                        // super_object_num
    Robot::sobj_x = _sobj_x, Robot::sobj_y = _sobj_y;                   // last super_object_coords
    Robot::rc = {_rc_r, _rc_g, _rc_b};                                  // color sensors right
    Robot::lc = {_lc_r, _lc_g, _lc_b};                                  // color sensors left
    Robot::us = {_rus, _fus, _lus};                                     // ultrasonic sensors
    Robot::whl_l = _whl_l, Robot::whl_r = _whl_r;                       // wheels
    Robot::led = _led;                                                  // led for collect and deposit
    Robot::tp = _tp;                                                    // where to teleport
    Robot::g_time = _g_time;                                            // game time

    Robot::l_x = -1, Robot::l_y = -1;                                   // last xy coords

    Robot::map0 = _map0, Robot::map1 = _map1;
    Robot::pathfinder0 = _pathfinder0, Robot::pathfinder1 = _pathfinder1;
    Robot::speedometer = _speedometer;

    Robot::n_target = {-1, -1};
    Robot::n_target_is_last = false;

    Robot::chasing_sobj_num = 0;                                        // represents how many super_objects are in current paths



    Robot::loaded_objects = {0, 0, 0};
    Robot::loaded_objects_num = 0;


    Robot::collecting_since = Robot::timer::now();                       // time var for collect function
    Robot::depositing_since = Robot::timer::now();                       // time var for deposit function
}

//====================================
//          Private Functions
//====================================

// TODO: update_pos function
void Robot::update_pos() {
    // time difference between last known position and now
    double speed = Robot::speedometer->avg_speed(100);
    double time_dif = std::chrono::duration_cast<std::chrono::milliseconds>(
            Robot::timer::now() - depositing_since).count();

    std::pair<double, double> u_vector = angle2Vector(*Robot::comp);
    u_vector.first *= speed * time_dif, u_vector.second *= speed * time_dif;

    std::cout << "speed: " << str(u_vector) << "\n";

    *Robot::x += u_vector.first, *Robot::y += u_vector.second;
}

// collect functions
bool Robot::should_collect() {

    // if the difference is less or equal to 3 seconds the robot is still collecting;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(timer::now() - collecting_since).count() <= 3000)
        return true;

    // The robot is full; the robot cant collect items anyway
    if (Robot::loaded_objects_num >= 6)
        return false;

    // The objects color is Red
    if (isRed()) {
        // Since super objects count as red objects.
        // Only collect red objects if there's space including the chasing super objects
        return Robot::chasing_sobj_num + Robot::loaded_objects[0] < 2;
    } else if (isCyan()) {
        // nothin' special here
        return Robot::loaded_objects[1] < 2;
    } else if (isBlack()) {
        // nothin' special here
        return Robot::loaded_objects[2] < 2;
    }
    // if there's no object beneath the robot, don't try to collect anything
    return false;
}

void Robot::collect() {

    // the robot is already collecting
    if (std::chrono::duration_cast<std::chrono::milliseconds>(Robot::timer::now() - collecting_since).count() <= 4000) {
        // This is to prevent the robot from moving
        Robot::wheels(0, 0);
        *Robot::led = 1;

    }
        // the robot begins to collect
    else {
        // set collecting_since to now
        Robot::collecting_since = Robot::timer::now();

        // update the loaded_objects vars
        if (isRed() || isSuperObj()) {
            Robot::loaded_objects[0]++;
        } else if (isCyan()) {
            Robot::loaded_objects[1]++;
        } else if (isBlack()) {
            Robot::loaded_objects[2]++;
        }
        Robot::loaded_objects_num++;

        Robot::wheels(0, 0);
        *Robot::led = 1;
    }
}

// deposit functions
bool Robot::should_deposit() {

    // while timer - depositing_since < 6the robot is still depositting
    if (std::chrono::duration_cast<std::chrono::milliseconds>(Robot::timer::now() - depositing_since).count() <= 4000)
        return true;

    // the robot uses a treshhold to determine if it has enough points so that it is worth it to deposit
    // This treshhold should actually not really matter since the robot is supposed to
    // only drive to the deposit area if it is fully loaded

    // basic points;
    int treshhold = Robot::loaded_objects[0] * 10 + Robot::loaded_objects[1] * 15 + Robot::loaded_objects[2] * 20;

    // one rgb-bonus
    if (Robot::loaded_objects[0] > 0 && Robot::loaded_objects[1] > 0 && Robot::loaded_objects[2] > 0) {
        treshhold += 90;
        // second rgb-bonus
        if (Robot::loaded_objects[0] > 1 && Robot::loaded_objects[1] > 1 && Robot::loaded_objects[2] > 1) {
            treshhold += 90;

        }
    }

    // 145 = 2 red + 1 cyan + 1 black | 20 + 15 + 20 + 90
    return treshhold >= 145;
}

void Robot::deposit() {
    // the robot is already depositing
    if (std::chrono::duration_cast<std::chrono::milliseconds>(Robot::timer::now() - depositing_since).count() <= 5000) {
        // This is to prevent the robot from moving
        Robot::wheels(0, 0);
        *Robot::led = 2;

    }
        // the robot begins to deposit
    else {
        // set deposit_since to now
        Robot::depositing_since = Robot::timer::now();

        // update the loaded_objects vars
        Robot::loaded_objects[0] = 0;
        Robot::loaded_objects[1] = 0;
        Robot::loaded_objects[2] = 0;
        Robot::loaded_objects_num = 0;

        Robot::wheels(0, 0);
        *Robot::led = 2;
    }
}

// teleport functions
bool Robot::should_teleport() {

    // while in deposit_area don't teleport
    if (isOrange()) {
        return false;
    }

    // earliest possible teleport after 3min (180sec)
    //      only teleport if it has nothing to lose -> no objects loaded
    if (*Robot::g_time > 180) {
        if (loaded_objects_num == 0) {
            return true;
        }
    }
    // next possible teleport after 3min and 20sec (200sec)
    //      only teleport if points are more valuable then 145 points
    if (*Robot::g_time > 200) {
        if (!should_deposit()) {
            return true;
        }
    }

    // last possible teleport after 4min (240sec)
    //      teleport in any case
    return *Robot::g_time > 240;
}

void Robot::teleport() {
    Robot::loaded_objects_num = 0;
    Robot::loaded_objects[0] = 0;
    Robot::loaded_objects[1] = 0;
    Robot::loaded_objects[2] = 0;

    *Robot::tp = 1;
}

int Robot::avoid_void() {
    if (*Robot::x >= 350 && *Robot::comp > 270 && *Robot::comp <= 360) {
        return -1;
    } else if (*Robot::x >= 350 && *Robot::comp > 180 && *Robot::comp <= 270) {
        return 1;
        //Right END
    } else if (*Robot::x <= 10 && *Robot::comp > 0 && *Robot::comp <= 90) {
        return 1;
    } else if (*Robot::x <= 10 && *Robot::comp > 90 && *Robot::comp <= 180) {
        return -1;
        //TOP END
    } else if (*Robot::y >= 260 && *Robot::comp > 270 && *Robot::comp <= 360) {
        return 1;
    } else if (*Robot::y >= 260 && *Robot::comp >= 0 && *Robot::comp <= 90) {
        return -1;
        //BOTTOM END
    } else if (*Robot::y <= 10 && *Robot::comp > 180 && *Robot::comp <= 270) {
        return -1;
    } else if (*Robot::y <= 10 && *Robot::comp < 180 && *Robot::comp >= 90) {
        return 1;
    }
    return 0;
}

std::vector<std::pair<int, int>> Robot::get_points(MapData &mapData) {

    // point array which which will later be returned
    std::vector<std::pair<int, int>> point_path;
    point_path.emplace_back(*Robot::x, *Robot::y);

    // point finding algorithm. (See Idea 4 on https://stackoverflow.com/questions/62179174)
    //      This might be an almost optimal solution

    double b_overall_dist = -1;

    for (auto deposit_area : mapData.getDepositAreas()) {
        // temporary array to be able to compare
        std::vector<std::pair<int, int>> cur_path;
        cur_path.emplace_back(*Robot::x, *Robot::y);

        // c_points keeps track of which object_types are needed/free to find
        std::array<int, 3> c_points = {Robot::loaded_objects[0], Robot::loaded_objects[1], Robot::loaded_objects[2]};

        // p_dist is used to decide which route to which deposit_area to pick
        double p_dist = 0;

        // add upto 6 points to cur_path;
        for (int i = Robot::loaded_objects_num; i < 6; ++i) {

            std::array<int, 3> b_point{0, 0, 0};
            double b_dist = -1;

            // getAllPoints() returns a vector<array<int, 3>>:
            //      get the point with the lowest f
            for (auto point : mapData.getAllPoints()) {

                // check if the object_type is chosen less or equal to 2 times
                if (c_points[point[2]] + 1 <= 2) {

                    // g_dist is the dist from the last chosen point to the current point
                    double g_dist = dist(point[0], cur_path.end()->first, point[1], cur_path.end()->second);
                    // h_dist is the distance to the deposit area
                    double h_dist = dist(point[0], deposit_area.first, point[1], deposit_area.second);

                    // if f_cost is lower set f_cost to cur_cost
                    if (g_dist + h_dist < b_dist || b_dist == -1) {
                        b_dist = g_dist + h_dist;
                        b_point = {point[0], point[1], point[2]};
                    }
                }
            }
            c_points[b_point[2]]++;
            // The best point is added to the cur_path alongside with it's distance
            p_dist += dist(b_point[0], cur_path.end()->first, b_point[1], cur_path.end()->second);
            cur_path.emplace_back(b_point[0], b_point[1]);
        }
        // add the dist from the last point to the deposit_area
        p_dist += dist(deposit_area.first, cur_path.end()->first, deposit_area.second, cur_path.end()->second);

        // if the overall path length is shorter then before -> set best to cur
        if (p_dist < b_overall_dist || b_overall_dist == -1) {
            b_overall_dist = p_dist;

            // this wont overfill because point_path gets set to cur_path and therefore forgets it's earlier content
            point_path = cur_path;
            point_path.push_back(deposit_area);
        }
    }

    return point_path;
}

//====================================
//          Public Functions
//====================================
void Robot::wheels(int l, int r) {
    *Robot::whl_l = l, *Robot::whl_r = r;
}

int Robot::move_to(int _x, int _y, bool safety) {
    // an angle should be created that represent the difference between the point to 0;
    // It should range from -180 to 180 instead of 0 tp 360;
    int angle = vector2Angle(_x - *Robot::x, _y - *Robot::y);

    // Difference between compass
    angle -= *Robot::comp;

    // If the angle is higher then 180 the point is on the other side
    if (abs(angle) > 180) {
        //          -> get the same angle but with another prefix
        angle = (angle + ((angle > 0) ? -360 : 360)) % 360;
    }

    switch (Robot::check_us_sensors(10, 8, 10)) {
        // case 0 means check_us_sensors has detected no near obstacles
        //      -> the robot can move freely
        case 0:
            // the angle to x, y is small so there's no correction of it needed
            //      -> drive straight
            if (abs(angle) < 10) {
                wheels(5, 5);
            }
                // the angle is a bit bigger so the robot needs to make a small correction
            else if (abs(angle) < 30) {
                if (angle < 0) {
                    if (!safety || dist(*Robot::x, _x, *Robot::y, _y) < 15) {
                        wheels(2, 1);
                    } else {
                        wheels(5, 4);
                    }
                } else {
                    if (!safety || dist(*Robot::x, _x, *Robot::y, _y) < 15) {
                        wheels(1, 2);
                    } else {
                        wheels(4, 5);
                    }
                }
            }
                // the angle is quite big; now a bigger correction is needed
            else if (abs(angle) < 60) {
                if (angle < 0) {
                    if (!safety || dist(*Robot::x, _x, *Robot::y, _y) < 15) {
                        wheels(2, -1);
                    } else {
                        wheels(5, 2);
                    }
                } else {
                    if (!safety || dist(*Robot::x, _x, *Robot::y, _y) < 15) {
                        wheels(-1, 2);
                    } else {
                        wheels(2, 5);
                    }
                }
            }
                // the angle is very big; a huge correction is needed
                //      -> no forward momentum; spinning
            else {
                if (angle < 0) {
                    wheels(3, -3);
                } else {
                    wheels(-3, 3);
                }
            }
            return 0;

        case 1: // obstacle left
            wheels(-2, -5);
            return 1;
        case 2: // obstacle front
            wheels(-5, -5);
            return 2;
        case 3: // obstacles left & front
            wheels(-2, -5);
            return 3;
        case 4: // obstacle right
            wheels(-5, -2);
            return 4;
        case 5: // obstacles left & right; turning would be fatal; just drive forward
            wheels(3, 3);
            return 5;
        case 6: // obstacles front & right
            wheels(-5, -2);
            return 6;
        case 7: // all sensors see an obstacle -> dead end; Just spinn
            wheels(-5, 5);
            return 7;
        default:
            return -1;
    }
}
int Robot::move_to(std::pair<int, int> p, bool safety) {
    return Robot::move_to(p.first, p.second, safety);
}

int Robot::check_us_sensors(int l, int f, int r) {
    int sum = 0;
    if (*Robot::us[0] < l)
        sum += 1;
    if (*Robot::us[1] < f)
        sum += 2;
    if (*Robot::us[2] < r)
        sum += 4;
    return sum;
}

void Robot::game_0_loop() {
    if (Robot::should_deposit() && (isOrangeLeft() || isOrangeRight())) {
        if (isOrange()) {
            Robot::deposit();
        } else if (isOrangeRight()) {
            Robot::wheels(3, 0);
        } else {
            Robot::wheels(0, 3);
        }

    } else if (Robot::should_collect()) {
        Robot::collect();
    } else {
        // avoid trap on the right if objects are loaded
        if (isYellowRight() && Robot::loaded_objects_num > 0) {
            wheels(0, 5);
        }
            // avoid trap on the left
        else if (isYellowLeft() && Robot::loaded_objects_num > 0) {
            wheels(5, 0);
        } else {
            switch (Robot::check_us_sensors(8, 12, 8)) {
                // no obstacle
                case 0:
                    // 4 | 4 is standard movement speed in w1
                    wheels(4, 4);
                    break;
                case 1: // obstacle left
                    wheels(4, 0);
                    break;
                case 2: // obstacle front
                    wheels(-3, -5);
                    break;
                case 3: // obstacles left & front
                    wheels(-1, -5);
                    break;
                case 4: // obstacle right
                    wheels(0, 4);
                    break;
                case 5: // obstacles left & right; turning would be fatal; just drive forward
                    wheels(3, 3);
                    break;
                case 6: // obstacles front & right
                    wheels(-5, -1);
                    break;
                case 7: // all sensors see an obstacle -> dead end; Just spinn
                    wheels(-5, 5);
                    break;
                default:
                    break;
            }
            *Robot::led = 0;
        }
    }
    // Teleport
    if (Robot::should_teleport()) {
        Robot::teleport();
    }
}

void Robot::game_1_loop() {
    // check if robot is in signal lost zone
    if (*Robot::x == 0 && *Robot::y == 0) {

        // set normal coords to last coords and update with function
        *Robot::x = Robot::l_x, *Robot::y = Robot::l_y;
        Robot::update_pos();
    }

    Robot::speedometer->log_speed(dist(*Robot::x, Robot::l_x, *Robot::y, Robot::l_y),
                                  (*Robot::whl_l + *Robot::whl_r) / 2);
    std::cout << Robot::speedometer->avg_speed(10) << "\n";
    // set last coords to normal coords (last coords wont get overwritten by the sim)
    Robot::l_x = *Robot::x, Robot::l_y = *Robot::y;

    //#####################
    // -- PATHFINDING --
    //#####################

    // There's no path to follow
    if (Robot::complete_path.empty()) {
        // get a path of points
        std::vector<std::pair<int, int>> point_path = Robot::get_points(*Robot::map1);
        std::pair<int, int> start = {-1, -1};

        // calculate a path from one point to the next
        for (auto end : point_path) {
            if (start.first != -1 && start.second != -1) {
                if (Robot::pathfinder1->findPath(start, end, Robot::loaded_objects_num > 0)) {

                    // add the path to the complete path
                    // the first path is at the front of the vector
                    Robot::complete_path.push_back(Robot::pathfinder1->pathToPair());
                    std::cout << "added Path\n";
                }
            }
            std::cout << "Path from: " << str(start) << " to " << str(end) << std::endl;
            start = end;
        }
        int i = 0;
        for (const auto &path : Robot::complete_path) {
            std::cout << "Path No. " << std::to_string(i) << std::endl;
            for (auto point : path) {
                std::cout << "\t" << str(point) << std::endl;
            }
            i++;
        }
    }


    // get the next target
    if (!Robot::complete_path.empty()) {

        if (Robot::n_target.first == -1 && Robot::n_target.second == -1) {

            // set n_target to the last element of the first path in complete_path
            //      the paths in complete_path are reversed; The end of the path is the first element

            Robot::n_target_is_last = Robot::complete_path.front().size() == 1;
            Robot::n_target = Robot::complete_path.front().back();

            std::cout << "new target: " << str(Robot::n_target) << std::endl;

            // if it's the last element remove the path entirely
            if (Robot::n_target_is_last) {
                Robot::complete_path.erase(Robot::complete_path.begin());
            }
                // Otherwise just remove it
            else {
                Robot::complete_path.front().pop_back();//erase(Robot::complete_path.front().begin());
            }
        }
    }


    /*--------------------
     * Priority Structure
     * -------------------
     * Deposit
     * Collect
     * dodge traps and out of bounds <- only for safety.
     * Pathfind using active collision avoidance
     *
     * */


    if (Robot::should_deposit() && (isOrangeLeft() || isOrangeRight())) {
        if (isOrange()) {
            Robot::deposit();
        } else if (isOrangeRight()) {
            Robot::wheels(3, 0);
        } else {
            Robot::wheels(0, 3);
        }

    } else if (Robot::should_collect()) {
        Robot::collect();
    } else {

        Robot::move_to(Robot::n_target, Robot::n_target_is_last);
        std::cout << "Is at: " << str(*Robot::x, *Robot::y) << "\tmoving to: " << str(Robot::n_target) << std::endl;

        // if the distance is very small the target has been reached
        if (dist(Robot::n_target.first, *Robot::x, Robot::n_target.second, *Robot::y) < 5) {
            std::cout << "reached Object" << std::endl;
            Robot::n_target = {-1, -1};
        }

        // avoid the void by driving left || avoid trap on the right if objects are loaded
        if (Robot::avoid_void() == -1 || (isYellowRight() && Robot::loaded_objects_num > 0)) {
            wheels(0, 5);
        }
            // avoid the void by driving right || avoid trap on the left
        else if (Robot::avoid_void() == 1 || (isYellowLeft() && Robot::loaded_objects_num > 0)) {
            wheels(5, 0);
        }
    }
}
