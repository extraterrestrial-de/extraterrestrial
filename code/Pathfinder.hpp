#ifndef CSBOT_PATHFINDER_HPP
#define CSBOT_PATHFINDER_HPP

#include "MapData.hpp"
#include "libs/CommonFunctions.hpp"
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <cmath>

/// The factor by what speed is reduced in swamps
#define SWAMP_SPEED_PENALITY 10

/// Standard radius of a path
#define PATH_RADIUS 10

/** A point in a 2D word used by Pathfinders
 *
 *  @ingroup Points
 *
 *  @tparam pos   Position of Node in a 2D world.
 *  @tparam field Field-Object a Node is working on.
 *
 *  A %Node can be described as anchor point in a 2D grid.
 *  It offers functions for quick visibility checks for other
 *  Nodes, which can even be in other 2D grids, variables to keep
 *  track of their status in the current pathfinding problem and
 *  a vector containing all visible nodes in the same grid together
 *  with respective their costs.
*/
class Node {
public:
	Node(PVector& pos, Field* field);

	/** Booleans that indicate if this Node is in a special list
	 *  used by the A*Pathfinding algorithm
	 *
	 *  @note These variables aren't updated automatically
	*/
	bool isClosed, isOpen;

	/** Doubles that are used in the A*Pathfinding algorithm
	 *
	 *  @note These variables aren't updated automatically
	*/
	double g, f;

	/** A pointer to the last visited Node. This is used
	 *  by the A*Pathfinding algorithm
	 *
	 *  @note This variable isn't updated automatically
	*/
	Node* previous;

	/** This Method calculates a cost to a Node
	 *
	 *  @param node
	 *
	 *  @details This function takes visibility into account(for that it uses Node::canSee()).
	 *  If this Node can't see the other Node the cost will be -1.
	 *  Other wise the cost is calculated also taking swamps into account.
	*/
	double calculateCost(Node& node);

	/** This Method checks if this Node can see a certain other Node
	 *
	 *  @param node
	 *  @param ObstaclesStructs A vector containing all structures that are counted
	 *  as Obstacles
	*/
	bool canSee(Node& node, const std::vector<Area>& ObstaclesStructs);

	/** This Method gets every neighbour and calculates the cost.
	 *
	 *  @param Nodes A vector of pointers to Nodes
	 *  @param ObstaclesStructs A vector containing all structures that are counted
	 *  as Obstacles
	 *
	 *  @return the number of existing neighbours
	 *
	 *  @note Every Node in %Nodes has to be initialized before executing this Method
	*/
	int findNeighbours(const std::vector<Node>& Nodes, const std::vector<Area>& ObstacleStructs);

	/// Getter for Node::pos_
	const PVector& getPos() const;

	/// Getter for Node::Field_
	Field* getField() const;

	/// Getter for Node::neighbours_
	const std::vector<std::pair<Node*, double>>& getNeighbours() const;

private:

	/** A PVector struct that stores the position of this node
	 *
	 *  @note This variable could be constant and is not meant to change.
	 *  However for usability reasons it's not constant.
	*/
	PVector pos_{};

	/** A pointer to the Field Object this Node is stored in
	 *
	 *  @details This pointer can be used to find neighbours or obstacles.
	 *  It is important that a node knows in which Field it is operating in.
	 *
	 *  @note This variable could be constant and is not meant to change.
	 *  However for usability reasons it's not constant.
	 *
	 *  @attention Maybe theres a better solution to this
	*/
	Field* Field_;

	/** A vector that stores all visible neighbour nodes with their
	 *  respective costs.
	 *
	 *  @details To optimise both speed and memory usage the A*Pathfinding
	 *  works on a precalculated environment. This vector keeps track of
	 *  neighbours and distances/costs.
	*/
	std::vector<std::pair<Node*, double>> neighbours_; // Node / cost

};


/** A container class for a path generated by the Pathfinder class
 *
 *  @tparam points PVector vector containing all points in the path
 *  @tparam r the radius of the path
*/
class Path {
public:
	Path(std::vector<PVector> points, double r);

	PVector getClosestNormalPoint(PVector p, double d);

	void addPoint(PVector p);
	void removeLast();

	double getR() const;
	void setR(double r);

	bool isOnPath(PVector p);

	std::vector<PVector> getPoints() const;

	bool isEmpty() {
		return points_.empty();
	};
	unsigned int length() {
		return points_.size();
	};

private:
	std::vector<PVector> points_;
	double r_;
};

/** A Pathfinder class that operates on a specific maps using nodes
 *
 * @tparam MAP A Field-object representing a map
 * @tparam trap_sensitive A bool that determines whether the pathfinder
 * watches for traps or not
 *
 * @details At this point there's only one pathfinding algorithm (AStar)
 * This works fine but may be extended later
 */
class Pathfinder {
public:
	Pathfinder(Field& MAP, bool trap_sensitive);

	// pathfinding algorithm
	Path AStar(PVector& start, PVector& end);

	std::vector<Node> map;

private:

	bool trap_sensitive_;
	Field* Field_;

	struct PRIORITY {
		bool operator()(Node* child, Node* parent) const {
			return parent->f < child->f;
		}
	};

	/// heuristic function
	static double heuristic(const PVector& cur, const PVector& end);

	// convert previous pointers of nodes to path
	static Path traverse(Node* end);

	// convert nodepath to pair
	static std::vector<PVector> to_point(const std::vector<Node>& p);
};

namespace helper {
	bool compare(const std::pair<PVector, double>& p, const std::pair<PVector, double>& q);
}

#endif // CSBOT_PATHFINDER_HPP