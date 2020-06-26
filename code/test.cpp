#include"Point.hpp"
#include"MapData.hpp"
#include"Robot.hpp"

#include<iostream>
#include<vector>
#include<algorithm>

//TODO add coordinates get method for robot class

std::vector<Point> predefine_path(const int &posx, const int &posy){
    //TODO update point choose method
    std::vector<Point> _path;
    int i = 0;
    std::vector<Point*> points = MapData::getAllPoints;
    _path.push_back(points.front());
    for(auto _point : points){
        if(i>0){
            if (_point->calculate_dist_to_point(posx, posy) < points.front()->calculate_dist_to_point(posx, posy)){
                points.front() = _point;
            }
        }
        i++;
    }
    Point* temp;
    std::array<int,3> obj;
    obj = Robot::get_loaded_objects();
    int num = Robot::get_inventory_number();
    while(num < 6){
        if(obj[2] < 2){
            temp = _path.back()->get_closest_point(3);
            _path.push_back(temp);
        }
        if(obj[1] < 2){
            temp = _path.back()->get_closest_point(3);
            _path.push_back(temp);
        }
        if(obj[0] < 2){
            temp = _path.back()->get_closest_point(3);
            _path.push_back(temp);
        }
    }

    return _path;
}