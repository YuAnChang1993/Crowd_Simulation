//
// Instructor: Yu-An Chang
// Email:	andy123654.cs04g@g2.nctu.edu.tw
//			j60508888@gmail.com
//
// Date: 2017/4/20
//Reference : http://www.csie.ntnu.edu.tw/~u91029/

#ifndef _CS_MYDIJKSTRA_
#define _CS_MYDIJKSTRA_
#include <iostream>
#include <utility>
#include <vector>
#include "../../headers.h"
#include "../openGLbasic/opengl_stuff.h"
#include "../primitives/vector3d.h"
#include "../cs_basic_structs.h"
#include "../agent.h"
#include "../environment.h"
#include "../myCellularAutomata/cellular_automata.h"
/*
obstacle's node number cannot bigger than 30, if it does,
change the START_NODE and GOAL_NODE's number 
*/
#define START_NODE 31
#define GOAL_NODE 32
#define CONCAVE 800
#define CONVEX 801

class NODE{
public:
	NODE(){

	}
	vector3 pos;
	vector3 left, right;
	int type;//CONCAVE or CONVEX
};

class node_distance{
public:
	int node;
	float distance;
};

class compare_dist{
public:
	bool operator()(node_distance& n1, node_distance& n2){
		if (n1.distance > n2.distance)
			return true;
		return false;
	}
};

class CS_DIJKSTRA{
public:
	CS_DIJKSTRA();
	void addNode(vector3 &p);
	void reset();
	void convex_hull();
	void construct_graph();
	void set_Obstacles();
	bool intersect_collinear(vector3& p, vector3& p1, vector3& p2);
	bool intersect_crossLine(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	bool intersect(vector3&, vector3&, vector3&);
	bool intersect_length(vector3&, vector3&, vector3&);
	bool intersect(vector3&, vector3&, vector3&, vector3&);
	bool compare_far(vector3& o, vector3& a, vector3& b);
	bool compare_angle(vector3& a, vector3& b);
	bool compare(const vector3& a, const vector3& b);
	bool collinear(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	bool point_int_polygon(vector3&);
	float cross(vector3& o, vector3& a, vector3& b);
	float cross(vector3&, vector3&);
	float dot(vector3&, vector3&, vector3&);
	float findShortestPath(vector3& start, vector3& goal, vector<int> start_visibilityNode, vector<int> goal_visiblityNode, bool concave);
	float findShortestPath_(vector3& start, vector3& goal, vector<int> start_visibilityNode, vector<int> goal_visiblityNode);
	float length2(vector3& a, vector3& b);
	friend class CS_CELLULAR_AUTOMATA;
private:
	vector<vector3> m_nodes;
	vector<vector3> m_convex_nodes;
	vector<NODE> nodes;
	vector<CS_EDGE> m_edges;
	vector<CS_EDGE> m_convex_edges;
	vector<int> m_shortestPath;
	void sortByAngle();
	void setPolygonObstacle1();
	void setPolygonObstacle2();
	void setRectObstacle1();
	void setRectObstacle2();
	int start;
};

#endif