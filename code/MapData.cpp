#include "MapData.hpp"

/**     -----------     **/
/**                     **/
/**        Point        **/
/**                     **/
/**     -----------     **/


Point::Point(int x, int y) {
    Point::x = x;
    Point::y = y;
}

bool operator==(const Point &p1, const Point &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const Point &p1, const Point &p2) {
    return !(p1 == p2);
}

Point::operator bool() const {
    return Point::x == -1 && Point::y == -1;
}



/**     -----------     **/
/**                     **/
/**     Collectible     **/
/**                     **/
/**     -----------     **/

// Collectible::Collectible(): Constructor for Collectible Class
Collectible::Collectible(const Point &p, const int &c) {
    Collectible::pos_ = p;
    Collectible::color_ = c;
    Collectible::state_ = 0;
}

// Getter for Collectible::pos_
Point Collectible::pos() {
    return Collectible::pos_;
}

// Getter for Collectible::color_
int Collectible::color() {
    return Collectible::color_;
}

// Getter for Collectible::state_
int Collectible::state() {
    return static_cast<int>(Collectible::state_);
}

// Setter for Collectible::state_
void Collectible::setState(int s) {
    Collectible::state_ = s;
}

/**     -----------     **/
/**                     **/
/**         Line        **/
/**                     **/
/**     -----------     **/

// Line::Line(): Constructor for Line Class
Line::Line(const Point &p1, const Point &p2) {
    /** Set private vars **/
    Line::p1_ = p1;
    Line::p2_ = p2;
}

// Getter for Line::p1_
Point Line::p1() {
    return Line::p1_;
}

// Getter for Line::p2_
Point Line::p2() {
    return Line::p2_;
}


// Line::intersects():  Test if a line l intersects Line::
//      Input:  Line l
//      Return: intersection Point if existing. Otherwise {-1,-1}
Point Line::intersects(Line &l) {
    // line - line intersection using determinants:
    // source: https://en.wikipedia.ord/wiki/Line%E2%%80%90line_intersection

    // determiant = x1 * y2- x2 * y1
    int L1 = Line::p1().x * Line::p2().y - Line::p2().x * Line::p1().y;
    int L2 = l.p1().x * l.p2().y - l.p2().x * l.p1().y;

    // difs
    int L1xdif = Line::p1().x - Line::p2().x;
    int L1ydif = Line::p1().y - Line::p2().y;
    int L2xdif = l.p1().x - l.p2().x;
    int L2ydif = l.p1().y - l.p2().y;

    // determiant a*d - b*c
    double xnom = L1 * L2xdif - L2 * L1xdif;
    double ynom = L1 * L2ydif - L2 * L2ydif;
    double denom = L1xdif * L2ydif - L2xdif * L1ydif;

    // Lines don't cross
    if (denom == 0) {
        return {-1, -1};
    }
    // return intersection
    return {static_cast<int>(round(xnom / denom)), static_cast<int>(round(ynom / denom))};
}


/**     -----------     **/
/**                     **/
/**         Area        **/
/**                     **/
/**     -----------     **/

// Area::Area(): Constructor for Area class
Area::Area(const std::vector<Point> &p_s) {

    // xmin/ymin = first elem of p_s because there are problems with Infinity
    Area::min_ = {p_s.front().x, p_s.front().y};
    // xmax/ymax = 0;
    Area::max_ = {0, 0};

    Area::Corners_ = p_s;
    Point last_p = p_s.back();
    for (Point p : p_s) {
        Area::Edges_.emplace_back(last_p, p);
        last_p = p;

        // Set boundary box x
        if (p.x < Area::min_.x) {
            Area::min_.x = p.x;
        }
        if (p.x > Area::max_.x) {
            Area::max_.x = p.x;
        }

        // Set boundary box y
        if (p.y < Area::min_.y) {
            Area::min_.y = p.y;
        }
        if (p.y > Area::max_.y) {
            Area::max_.y = p.y;
        }
    }
}

bool Area::isInside(const Point &p) {
    return geometry::isInside(p, *this);
}

std::vector<Point> Area::Corners() {
    return Area::Corners_;
}

std::vector<Line> Area::Edges() {
    return Area::Edges_;
}


/**     -----------     **/
/**                     **/
/**        Field        **/
/**                     **/
/**     -----------     **/



/** Field::Field(): Constructor for Field class **/
Field::Field(const int &width, const int &height,
             const std::vector<Area> &walls,
             const std::vector<Area> &traps,
             const std::vector<Area> &swamps,
             const std::vector<Area> &waters,
             const std::vector<Point> &wnodes,
             const std::vector<Point> &tnodes,
             const std::vector<Point> &deposits,
             const std::array<std::vector<Collectible>, 3> &collectibles) {
    // Field::size_: Size of the Field
    Field::size_ = {width, height};

    // Field::(MapObjectName)_: Different MapObjects that can be displayed as areas.
    Field::Walls_ = walls;
    Field::Traps_ = traps;
    Field::Swamps_ = swamps;
    Field::Waters_ = waters;

    // Field::(type)Nodes_: Nodes for different MapObjects.
    Field::tNodes_ = tnodes;
    Field::wNodes_ = wnodes;

    // Field::Deposits_: Deposit Areas of the Field
    Field::Deposits_ = deposits;

    // Field::Collectibles: Collectible Points of the Field
    Field::Collectibles_ = collectibles;
}

/** Getter for Field::size **/
Point Field::size() {
    return Field::size_;
}

/** Getter for Field::(MapObjectName)_ **/
std::vector<Area> Field::MapObjects(std::vector<unsigned int> indices) {
    std::vector<Area> return_vector;
    // return after all indices have been checked.
    while (!indices.empty()) {
        unsigned int index = indices.back();
        indices.pop_back();
        switch (index) {
            case 1:
                return_vector.insert(std::end(return_vector), std::begin(Field::Walls_), std::end(Field::Walls_));
            case 2:
                return_vector.insert(std::end(return_vector), std::begin(Field::Traps_), std::end(Field::Traps_));
            case 3:
                return_vector.insert(std::end(return_vector), std::begin(Field::Swamps_), std::end(Field::Swamps_));
            case 5:
                return_vector.insert(std::end(return_vector), std::begin(Field::Waters_), std::end(Field::Waters_));
            default: ERROR_MESSAGE("index out of range/invalid")
        }
    }

    return return_vector;
}

/** Getter for Field::(type)Nodes_ **/
std::vector<Point> Field::Nodes(std::vector<unsigned int> indices) {
    std::vector<Point> return_vector;
    // return after all indices have been checked.
    while (!indices.empty()) {
        unsigned int index = indices.back();
        indices.pop_back();
        switch (index) {
            case 1:
                return_vector.insert(std::end(return_vector), std::begin(Field::wNodes_), std::end(Field::wNodes_));
            case 2:
                return_vector.insert(std::end(return_vector), std::begin(Field::tNodes_), std::end(Field::tNodes_));
            default: ERROR_MESSAGE("index out of range/invalid")
        }
    }

    return return_vector;
}

/** Getter for Field::Collectibles_ **/
std::vector<Point> Field::Deposits() {
    return Field::Deposits_;
}

/** Getter for Field::Collectibles_ **/
std::vector<Collectible> Field::Collectibles(std::vector<unsigned int> indices) {
    std::vector<Collectible> return_vector;
    // return after all indices have been checked.
    while (!indices.empty()) {
        unsigned int index = indices.back();
        indices.pop_back();
        switch (index) {
            case 1:
                return_vector.insert(std::end(return_vector), std::begin(Field::Collectibles_[0]),
                                     std::end(Field::Collectibles_[0]));
            case 2:
                return_vector.insert(std::end(return_vector), std::begin(Field::Collectibles_[1]),
                                     std::end(Field::Collectibles_[1]));
            case 3:
                return_vector.insert(std::end(return_vector), std::begin(Field::Collectibles_[2]),
                                     std::end(Field::Collectibles_[2]));
            default: ERROR_MESSAGE("index out of range")
        }
    }
    return return_vector;
}




/*
 *
 * namespace geometry
 *
 * */


// geometry::onSide():  Test if a point p2 is left/on/right a line through p0 and p1.
//      Input:  Point p0, p1, p2
//      Return: >0 left; =0 on; <0 right
int onSide(Point p0, Point p1, Point p2) {
    return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
}


bool isInside(const Point &p, Area &area) {
    // Point in Polygon(PIP) using the winding number algorithm:
    // source: https://en.wikipedia.ord/wiki/Point_in_polygon

    int wn = 0;    // the  winding number counter
    int n = area.Corners().size(); // The number of corners


    std::vector<Point> poly = area.Corners();      // vector with all corners + V[0] at V[n+1]
    poly.push_back(poly.front());

    // loop through all edges of the polygon
    for (unsigned int i = 0; i < n; i++) {     // edge from V[i] to  V[i+1]
        if (poly[i].y <= p.y) {                                 // start y <= P.y
            if (poly[i + 1].y > p.y)                            // an upward crossing
                if (onSide(poly[i], poly[i + 1], p) > 0)        // P left of  edge
                    ++wn;                                       // have  a valid up intersect
        } else {                                                // start y > P.y (no test needed)
            if (poly[i + 1].y <= p.y)                           // a downward crossing
                if (onSide(poly[i], poly[i + 1], p) < 0)        // P right of  edge
                    --wn;                                       // have  a valid down intersect
        }
    }
    return wn != 0;
}

Point intersects(Line &l1, Line &l2) {
    // line - line intersection using determinants:
    // source: https://en.wikipedia.ord/wiki/Line%E2%%80%90line_intersection

    // determiant = x1 * y2- x2 * y1
    int L1 = l1.p1().x * l1.p2().y - l1.p2().x * l1.p1().y;
    int L2 = l2.p1().x * l2.p2().y - l2.p2().x * l2.p1().y;

    // difs
    int L1xdif = l1.p1().x - l1.p2().x;
    int L1ydif = l1.p1().y - l1.p2().y;
    int L2xdif = l2.p1().x - l2.p2().x;
    int L2ydif = l2.p1().y - l2.p2().y;

    // determiant a*d - b*c
    double xnom = L1 * L2xdif - L2 * L1xdif;
    double ynom = L1 * L2ydif - L2 * L2ydif;
    double denom = L1xdif * L2ydif - L2xdif * L1ydif;

    // Lines don't cross
    if (denom == 0) {
        return {-1, -1};
    }
    // return intersection
    return {static_cast<int>(round(xnom / denom)), static_cast<int>(round(ynom / denom))};
}

//   _______                _____          __
//  |   |   |.---.-..-----.|     \ .---.-.|  |_ .---.-.
//  |       ||  _  ||  _  ||  --  ||  _  ||   _||  _  |
//  |__|_|__||___._||   __||_____/ |___._||____||___._|
//                  |__|



//------------- Game0_Objects --------------//

		/*walls*/
std::vector<Area>GAME0WALLS = {
	Area({{126,309},{353,309},{353,271},{126,271}}),
	Area({{154,191},{187,191},{187,190},{235,143},{235,142},{244,142},{244,143},{292,190},{292,191},{325,191},{325,190},{327,188},{328,188},{328,155},{327,155},{274,101},{273,101},{273,89},{272,89},{272,86},{271,86},{271,84},{270,84},{270,82},{269,82},{267,80},{267,79},{265,79},{265,78},{263,78},{263,77},{262,77},{262,76},{260,76},{260,75},{259,75},{259,1},{220,1},{220,75},{219,75},{219,76},{217,76},{217,77},{216,77},{216,78},{214,78},{214,79},{213,79},{213,80},{211,80},{211,81},{210,82},{209,82},{209,84},{208,84},{208,86},{207,86},{207,89},{206,89},{206,101},{205,101},{152,155},{151,155},{151,188},{152,188},{154,190}}),
	Area({{356,105},{404,105},{404,56},{356,56}}),
	Area({{75,103},{123,103},{123,55},{75,55}})};
		/*traps*/
std::vector<Area>GAME0TRAPS = {
	Area({{77,277},{125,277},{125,270},{135,270},{135,220},{77,220}}),
	Area({{354,277},{404,277},{404,220},{346,220},{346,270},{354,270}})};
		/*swamps*/
std::vector<Area>GAME0SWAMPS = {}
		/*Water*/
std::vector<Area>GAME0WATERS = {
	Area({{390,121},{479,121},{479,1},{339,1},{339,70},{355,70},{355,55},{405,55},{405,106},{390,106}}),
	Area({{0,120},{89,120},{89,104},{74,104},{74,54},{124,54},{124,70},{140,70},{140,1},{0,1}})};
		/*deposit*/
std::vector<Point>GAME0DEPOSITS = {{357,257},{121,258}};

//------------- Game1_Objects --------------//

		/*walls*/
std::vector<Area>GAME1WALLS = {
	Area({{337,480},{381,480},{381,479},{386,479},{386,478},{390,478},{390,477},{393,477},{393,476},{395,476},{395,475},{397,475},{397,474},{399,474},{399,473},{401,473},{401,472},{404,470},{404,469},{406,469},{406,468},{411,463},{412,463},{412,461},{413,461},{415,458},{416,458},{416,456},{417,456},{417,453},{418,453},{418,451},{419,451},{419,449},{420,449},{420,446},{421,446},{421,442},{422,442},{422,433},{423,433},{423,399},{422,399},{422,391},{421,391},{421,387},{420,387},{420,384},{419,384},{419,381},{418,381},{418,379},{417,379},{417,377},{416,377},{416,375},{415,375},{413,372},{412,372},{412,370},{411,370},{410,368},{409,368},{409,366},{407,366},{407,365},{405,364},{405,363},{403,363},{403,362},{401,361},{401,360},{399,360},{399,359},{398,359},{398,358},{395,358},{395,357},{393,357},{393,356},{390,356},{390,355},{387,355},{387,354},{382,354},{382,353},{376,353},{376,352},{342,352},{342,353},{336,353},{336,354},{331,354},{331,355},{328,355},{328,356},{325,356},{325,357},{323,357},{323,358},{320,358},{320,359},{319,359},{319,360},{317,360},{317,361},{315,362},{315,363},{313,363},{313,364},{305,372},{304,372},{304,374},{303,374},{303,375},{302,375},{302,377},{301,377},{301,379},{300,379},{300,382},{299,382},{299,384},{298,384},{298,387},{297,387},{297,391},{296,391},{296,395},{295,395},{295,436},{296,436},{296,442},{297,442},{297,445},{298,445},{298,449},{299,449},{299,451},{300,451},{300,453},{301,453},{301,456},{302,456},{302,457},{303,457},{303,459},{304,459},{305,461},{306,461},{306,463},{307,463},{312,468},{312,469},{314,469},{314,470},{317,472},{317,473},{319,473},{319,474},{321,474},{321,475},{323,475},{323,476},{325,476},{325,477},{328,477},{328,478},{332,478},{332,479},{337,479}}),
	Area({{206,349},{254,349},{254,240},{206,240}}),
	Area({{464,349},{512,349},{512,241},{464,241}}),
	Area({{305,188},{413,188},{413,142},{305,142}})};
		/*traps*/
std::vector<Area>GAME1TRAPS = {
	Area({{169,403},{231,403},{231,350},{205,350},{205,342},{169,342}}),
	Area({{488,403},{550,403},{550,342},{513,342},{513,350},{488,350}}),
	Area({{328,69},{390,69},{390,8},{328,8}})};
		/*swamps*/
std::vector<Area>GAME1SWAMPS = {
	Area({{239,416},{294,416},{294,394},{295,394},{295,390},{296,390},{296,386},{297,386},{297,383},{298,383},{298,381},{299,381},{299,378},{300,378},{300,376},{301,376},{301,374},{302,374},{302,373},{303,373},{303,371},{304,371},{312,363},{312,362},{314,362},{314,361},{316,360},{316,359},{318,359},{318,358},{319,358},{319,357},{322,357},{322,356},{324,356},{324,355},{327,355},{327,354},{330,354},{330,353},{335,353},{335,352},{341,352},{341,351},{377,351},{377,352},{383,352},{383,353},{388,353},{388,354},{391,354},{391,355},{394,355},{394,356},{396,356},{396,357},{399,357},{399,358},{400,358},{400,359},{402,359},{402,360},{404,361},{404,362},{406,362},{406,363},{408,364},{408,365},{410,365},{410,367},{411,367},{412,369},{413,369},{413,371},{414,371},{416,374},{417,374},{417,376},{418,376},{418,378},{419,378},{419,380},{420,380},{420,383},{421,383},{421,386},{422,386},{422,390},{423,390},{423,398},{424,398},{424,416},{479,416},{479,350},{463,350},{463,336},{255,336},{255,350},{239,350}})};
		/*Water*/
std::vector<Area>GAME1WATERS = {
	Area({{255,335},{463,335},{463,254},{400,254},{400,253},{399,253},{399,189},{319,189},{319,253},{318,253},{318,254},{255,254}}),
	Area({{49,134},{65,134},{65,130},{66,130},{66,129},{72,129},{72,128},{78,128},{78,127},{81,127},{81,126},{85,126},{85,125},{89,125},{89,124},{92,124},{92,123},{93,123},{93,122},{96,122},{96,121},{98,121},{98,120},{101,120},{101,119},{103,119},{103,118},{105,118},{105,117},{107,117},{107,116},{109,116},{109,115},{111,115},{111,114},{112,114},{112,113},{114,113},{114,112},{115,112},{115,111},{117,111},{117,110},{119,110},{119,109},{120,109},{120,108},{122,108},{122,107},{125,105},{125,104},{127,104},{127,103},{130,101},{130,100},{132,100},{132,99},{142,89},{143,89},{143,87},{144,87},{146,84},{147,84},{147,82},{148,82},{149,80},{150,80},{150,78},{151,78},{152,76},{153,76},{153,74},{154,74},{154,73},{155,73},{155,71},{156,71},{156,69},{157,69},{157,68},{158,68},{158,66},{159,66},{159,64},{160,64},{160,62},{161,62},{161,60},{162,60},{162,58},{163,58},{163,55},{164,55},{164,53},{165,53},{165,51},{166,51},{166,48},{167,48},{167,46},{168,46},{168,43},{169,43},{169,39},{170,39},{170,35},{171,35},{171,30},{172,30},{172,24},{173,24},{173,14},{174,14},{174,1},{0,1},{0,131},{49,131}}),
	Area({{667,127},{719,127},{719,1},{549,1},{549,9},{550,9},{550,19},{551,19},{551,25},{552,25},{552,31},{553,31},{553,34},{554,34},{554,39},{555,39},{555,41},{556,41},{556,44},{557,44},{557,46},{558,46},{558,49},{559,49},{559,51},{560,51},{560,53},{561,53},{561,56},{562,56},{562,58},{563,58},{563,60},{564,60},{564,62},{565,62},{565,63},{566,63},{566,65},{567,65},{567,67},{568,67},{568,68},{569,68},{569,70},{570,70},{570,71},{571,71},{571,73},{572,73},{572,74},{573,74},{573,76},{574,76},{575,78},{576,78},{576,80},{577,80},{579,83},{580,83},{580,85},{581,85},{591,95},{591,96},{593,96},{593,97},{595,98},{595,99},{597,99},{597,100},{601,103},{601,104},{603,104},{603,105},{605,105},{605,106},{606,106},{606,107},{608,107},{608,108},{609,108},{609,109},{611,109},{611,110},{612,110},{612,111},{614,111},{614,112},{616,112},{616,113},{618,113},{618,114},{620,114},{620,115},{623,115},{623,116},{625,116},{625,117},{627,117},{627,118},{630,118},{630,119},{631,119},{631,120},{634,120},{634,121},{638,121},{638,122},{642,122},{642,123},{646,123},{646,124},{651,124},{651,125},{657,125},{657,126},{667,126}})};
		/*deposit*/
std::vector<Point>GAME1DEPOSITS = {{534,235},{183,236},{358,428}};




//   _______                _____          __
//  |   |   |.---.-..-----.|     \ .---.-.|  |_ .---.-.
//  |       ||  _  ||  _  ||  --  ||  _  ||   _||  _  |
//  |__|_|__||___._||   __||_____/ |___._||____||___._|
//                  |__|



//------------- Game0_Objects --------------//

		/*walls*/
std::vector<Area>GAME0WALLS = {
	Area({{126,309},{353,309},{353,271},{126,271}}),
	Area({{154,191},{187,191},{187,190},{235,143},{235,142},{244,142},{244,143},{292,190},{292,191},{325,191},{325,190},{327,188},{328,188},{328,155},{327,155},{274,101},{273,101},{273,89},{272,89},{272,86},{271,86},{271,84},{270,84},{270,82},{269,82},{267,80},{267,79},{265,79},{265,78},{263,78},{263,77},{262,77},{262,76},{260,76},{260,75},{259,75},{259,1},{220,1},{220,75},{219,75},{219,76},{217,76},{217,77},{216,77},{216,78},{214,78},{214,79},{213,79},{213,80},{211,80},{211,81},{210,82},{209,82},{209,84},{208,84},{208,86},{207,86},{207,89},{206,89},{206,101},{205,101},{152,155},{151,155},{151,188},{152,188},{154,190}}),
	Area({{356,105},{404,105},{404,56},{356,56}}),
	Area({{75,103},{123,103},{123,55},{75,55}})};
		/*traps*/
std::vector<Area>GAME0TRAPS = {
	Area({{77,277},{125,277},{125,270},{135,270},{135,220},{77,220}}),
	Area({{354,277},{404,277},{404,220},{346,220},{346,270},{354,270}})};
		/*swamps*/
std::vector<Area>GAME0SWAMPS = {}
		/*Water*/
std::vector<Area>GAME0WATERS = {
	Area({{390,121},{479,121},{479,1},{339,1},{339,70},{355,70},{355,55},{405,55},{405,106},{390,106}}),
	Area({{0,120},{89,120},{89,104},{74,104},{74,54},{124,54},{124,70},{140,70},{140,1},{0,1}})};
		/*deposit*/
std::vector<Point>GAME0DEPOSITS = {{357,257},{121,258}};

//------------- Game1_Objects --------------//

		/*walls*/
std::vector<Area>GAME1WALLS = {
	Area({{337,480},{381,480},{381,479},{386,479},{386,478},{390,478},{390,477},{393,477},{393,476},{395,476},{395,475},{397,475},{397,474},{399,474},{399,473},{401,473},{401,472},{404,470},{404,469},{406,469},{406,468},{411,463},{412,463},{412,461},{413,461},{415,458},{416,458},{416,456},{417,456},{417,453},{418,453},{418,451},{419,451},{419,449},{420,449},{420,446},{421,446},{421,442},{422,442},{422,433},{423,433},{423,399},{422,399},{422,391},{421,391},{421,387},{420,387},{420,384},{419,384},{419,381},{418,381},{418,379},{417,379},{417,377},{416,377},{416,375},{415,375},{413,372},{412,372},{412,370},{411,370},{410,368},{409,368},{409,366},{407,366},{407,365},{405,364},{405,363},{403,363},{403,362},{401,361},{401,360},{399,360},{399,359},{398,359},{398,358},{395,358},{395,357},{393,357},{393,356},{390,356},{390,355},{387,355},{387,354},{382,354},{382,353},{376,353},{376,352},{342,352},{342,353},{336,353},{336,354},{331,354},{331,355},{328,355},{328,356},{325,356},{325,357},{323,357},{323,358},{320,358},{320,359},{319,359},{319,360},{317,360},{317,361},{315,362},{315,363},{313,363},{313,364},{305,372},{304,372},{304,374},{303,374},{303,375},{302,375},{302,377},{301,377},{301,379},{300,379},{300,382},{299,382},{299,384},{298,384},{298,387},{297,387},{297,391},{296,391},{296,395},{295,395},{295,436},{296,436},{296,442},{297,442},{297,445},{298,445},{298,449},{299,449},{299,451},{300,451},{300,453},{301,453},{301,456},{302,456},{302,457},{303,457},{303,459},{304,459},{305,461},{306,461},{306,463},{307,463},{312,468},{312,469},{314,469},{314,470},{317,472},{317,473},{319,473},{319,474},{321,474},{321,475},{323,475},{323,476},{325,476},{325,477},{328,477},{328,478},{332,478},{332,479},{337,479}}),
	Area({{206,349},{254,349},{254,240},{206,240}}),
	Area({{464,349},{512,349},{512,241},{464,241}}),
	Area({{305,188},{413,188},{413,142},{305,142}})};
		/*traps*/
std::vector<Area>GAME1TRAPS = {
	Area({{169,403},{231,403},{231,350},{205,350},{205,342},{169,342}}),
	Area({{488,403},{550,403},{550,342},{513,342},{513,350},{488,350}}),
	Area({{328,69},{390,69},{390,8},{328,8}})};
		/*swamps*/
std::vector<Area>GAME1SWAMPS = {
	Area({{239,416},{294,416},{294,394},{295,394},{295,390},{296,390},{296,386},{297,386},{297,383},{298,383},{298,381},{299,381},{299,378},{300,378},{300,376},{301,376},{301,374},{302,374},{302,373},{303,373},{303,371},{304,371},{312,363},{312,362},{314,362},{314,361},{316,360},{316,359},{318,359},{318,358},{319,358},{319,357},{322,357},{322,356},{324,356},{324,355},{327,355},{327,354},{330,354},{330,353},{335,353},{335,352},{341,352},{341,351},{377,351},{377,352},{383,352},{383,353},{388,353},{388,354},{391,354},{391,355},{394,355},{394,356},{396,356},{396,357},{399,357},{399,358},{400,358},{400,359},{402,359},{402,360},{404,361},{404,362},{406,362},{406,363},{408,364},{408,365},{410,365},{410,367},{411,367},{412,369},{413,369},{413,371},{414,371},{416,374},{417,374},{417,376},{418,376},{418,378},{419,378},{419,380},{420,380},{420,383},{421,383},{421,386},{422,386},{422,390},{423,390},{423,398},{424,398},{424,416},{479,416},{479,350},{463,350},{463,336},{255,336},{255,350},{239,350}})};
		/*Water*/
std::vector<Area>GAME1WATERS = {
	Area({{255,335},{463,335},{463,254},{400,254},{400,253},{399,253},{399,189},{319,189},{319,253},{318,253},{318,254},{255,254}}),
	Area({{49,134},{65,134},{65,130},{66,130},{66,129},{72,129},{72,128},{78,128},{78,127},{81,127},{81,126},{85,126},{85,125},{89,125},{89,124},{92,124},{92,123},{93,123},{93,122},{96,122},{96,121},{98,121},{98,120},{101,120},{101,119},{103,119},{103,118},{105,118},{105,117},{107,117},{107,116},{109,116},{109,115},{111,115},{111,114},{112,114},{112,113},{114,113},{114,112},{115,112},{115,111},{117,111},{117,110},{119,110},{119,109},{120,109},{120,108},{122,108},{122,107},{125,105},{125,104},{127,104},{127,103},{130,101},{130,100},{132,100},{132,99},{142,89},{143,89},{143,87},{144,87},{146,84},{147,84},{147,82},{148,82},{149,80},{150,80},{150,78},{151,78},{152,76},{153,76},{153,74},{154,74},{154,73},{155,73},{155,71},{156,71},{156,69},{157,69},{157,68},{158,68},{158,66},{159,66},{159,64},{160,64},{160,62},{161,62},{161,60},{162,60},{162,58},{163,58},{163,55},{164,55},{164,53},{165,53},{165,51},{166,51},{166,48},{167,48},{167,46},{168,46},{168,43},{169,43},{169,39},{170,39},{170,35},{171,35},{171,30},{172,30},{172,24},{173,24},{173,14},{174,14},{174,1},{0,1},{0,131},{49,131}}),
	Area({{667,127},{719,127},{719,1},{549,1},{549,9},{550,9},{550,19},{551,19},{551,25},{552,25},{552,31},{553,31},{553,34},{554,34},{554,39},{555,39},{555,41},{556,41},{556,44},{557,44},{557,46},{558,46},{558,49},{559,49},{559,51},{560,51},{560,53},{561,53},{561,56},{562,56},{562,58},{563,58},{563,60},{564,60},{564,62},{565,62},{565,63},{566,63},{566,65},{567,65},{567,67},{568,67},{568,68},{569,68},{569,70},{570,70},{570,71},{571,71},{571,73},{572,73},{572,74},{573,74},{573,76},{574,76},{575,78},{576,78},{576,80},{577,80},{579,83},{580,83},{580,85},{581,85},{591,95},{591,96},{593,96},{593,97},{595,98},{595,99},{597,99},{597,100},{601,103},{601,104},{603,104},{603,105},{605,105},{605,106},{606,106},{606,107},{608,107},{608,108},{609,108},{609,109},{611,109},{611,110},{612,110},{612,111},{614,111},{614,112},{616,112},{616,113},{618,113},{618,114},{620,114},{620,115},{623,115},{623,116},{625,116},{625,117},{627,117},{627,118},{630,118},{630,119},{631,119},{631,120},{634,120},{634,121},{638,121},{638,122},{642,122},{642,123},{646,123},{646,124},{651,124},{651,125},{657,125},{657,126},{667,126}})};
		/*deposit*/
std::vector<Point>GAME1DEPOSITS = {{534,235},{183,236},{358,428}};

