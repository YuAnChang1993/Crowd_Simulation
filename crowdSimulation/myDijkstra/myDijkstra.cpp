#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include "myDijkstra.h"

#include <algorithm>
#include <queue>

using namespace boost;

CS_DIJKSTRA::CS_DIJKSTRA(){

}

void CS_DIJKSTRA::addNode(vector3 &p){

	vector3 tmpNode = p;                          
	m_nodes.push_back(tmpNode);
}

void CS_DIJKSTRA::reset(){
	
	m_nodes.clear();
	m_nodes.resize(0);
}

float CS_DIJKSTRA::cross(vector3& o, vector3& a, vector3& b){

	return (a.x - o.x) * (b.z - o.z) - (a.z - o.z) * (b.x - o.x);
}

float CS_DIJKSTRA::cross(vector3& v1, vector3& v2){
	
	return v1.x * v2.z - v1.z * v2.x;
}

float CS_DIJKSTRA::dot(vector3& o, vector3& a, vector3& b){

	return (a.x - o.x) * (b.x - o.x) + (a.z - o.z) * (b.z - o.z);
}

bool CS_DIJKSTRA::compare(const vector3& a, const vector3& b){

	return (a.z < b.z) || (a.z == b.z && a.x < b.x);
}

bool CS_DIJKSTRA::compare_far(vector3& o, vector3& a, vector3& b){

	return length2(o, a) > length2(o, b);
}

bool CS_DIJKSTRA::compare_angle(vector3& a, vector3& b){

	int c = cross(m_nodes[0], a, b);
	return c > 0 || (c == 0 && length2(m_nodes[0], a) < length2(m_nodes[0], b));
}

float CS_DIJKSTRA::length2(vector3& a, vector3& b){

	return (a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z);

}

void CS_DIJKSTRA::convex_hull(){

	start = 0;
	for (int i = 0; i < m_nodes.size(); i++){
		if (compare(m_nodes[i], m_nodes[start]))
			start = i;
	}

	int convex_number = 0;
	m_convex_nodes.push_back(m_nodes[start]);

	for (int m = 1; true; ++m){

		int next = start;
		for (int i = 0; i < m_nodes.size(); i++){
			int c = cross(m_convex_nodes[m - 1], m_nodes[i], m_nodes[next]);
			if (c > 0 || c == 0 && compare_far(m_convex_nodes[m - 1], m_nodes[i], m_nodes[next]))
				next = i;
		}

		if (next == start) break;

		m_convex_nodes.push_back(m_nodes[next]);
	}

	for (int i = 0; i < m_convex_nodes.size(); i++){
		CS_EDGE tmpEdge;
		if (i != m_convex_nodes.size() - 1){
			tmpEdge.first_point = i;
			tmpEdge.second_point = i + 1;
			tmpEdge.weight = sqrt(pow(m_convex_nodes[i].x - m_convex_nodes[i + 1].x, 2) + pow(m_convex_nodes[i].z - m_convex_nodes[i + 1].z, 2));
		}
		else{
			tmpEdge.first_point = i;
			tmpEdge.second_point = 0;
			tmpEdge.weight = sqrt(pow(m_convex_nodes[i].x - m_convex_nodes[0].x, 2) + pow(m_convex_nodes[i].z - m_convex_nodes[0].z, 2));
		}
		m_convex_edges.push_back(tmpEdge);
	}

	//findShortestPath(vector3(0, 0, 0), vector3(0, 0, 0));


	sortByAngle();
	
}

void CS_DIJKSTRA::construct_graph(){

	const int size = m_nodes.size() - 1;
	vector3 neighbor1, neighbor2;
	float dot_value;
	for (int i = 0; i < m_nodes.size(); i++){
		if (i == 0){
			neighbor1 = m_nodes[size];
			neighbor2 = m_nodes[i + 1];
		}
		else if (i == size){
			neighbor1 = m_nodes[size - 1];
			neighbor2 = m_nodes[0];
		}
		else{
			neighbor1 = m_nodes[i - 1];
			neighbor2 = m_nodes[i + 1];
		}
		dot_value = dot(m_nodes[i], neighbor1, neighbor2);
		cout << dot_value << endl;
	}

	for (int i = 0; i < m_nodes.size(); i++){
		CS_EDGE tmpEdge;
		if (i != size){
			tmpEdge.first_point = i;
			tmpEdge.second_point = i + 1;
			tmpEdge.weight = sqrt(pow(m_nodes[i].x - m_nodes[i + 1].x, 2) + pow(m_nodes[i].z - m_nodes[i + 1].z, 2));
		}
		else{
			tmpEdge.first_point = i;
			tmpEdge.second_point = 0;
			tmpEdge.weight = sqrt(pow(m_nodes[i].x - m_nodes[0].x, 2) + pow(m_nodes[i].z - m_nodes[0].z, 2));
		}
		m_edges.push_back(tmpEdge);
	}
}

void CS_DIJKSTRA::sortByAngle(){

	for (int i = 0; i < m_nodes.size(); i++){
		for (int j = 0; j < m_nodes.size(); j++){
			if (compare_angle(m_nodes[i], m_nodes[j]))
				swap(m_nodes[i], m_nodes[j]);
		}
	}
}

bool CS_DIJKSTRA::intersect_collinear(vector3& p, vector3& p1, vector3& p2){

	return p.x >= min(p1.x, p2.x)
		&& p.x <= max(p1.x, p2.x)
		&& p.z >= min(p1.z, p2.z)
		&& p.z <= max(p1.z, p2.z);

}
// 兩線段共線，交點無限多
bool CS_DIJKSTRA::collinear(vector3& a1, vector3& a2, vector3& b1, vector3& b2){
	vector3 a = a2 - a1, b = b2 - b1, s = b1 - a1;
	double c1 = cross(a, b), c2 = cross(s, b);
	vector3 t = a1 - b1;
	double c3 = cross(b, a), c4 = cross(t, a);

	// 調整一下正負號，方便判斷
	if (c1 < 0) c1 = -c1, c2 = -c2;
	if (c3 < 0) c3 = -c3, c4 = -c4;

	if (c1 == 0)
	if (c2 == 0)
	{
		int counter = 0;
		// 確定交點後，剩下的線段端點必須位於交點的不同側。
		if (a1 == b1 && dot(a1, a2, b2) <= 0) counter++;
		if (a1 == b2 && dot(a1, a2, b1) <= 0) counter++;
		if (a2 == b1 && dot(a2, a1, b2) <= 0) counter++;
		if (a2 == b2 && dot(a2, a1, b1) <= 0) counter++;

		// 交點無限多、交點不存在。
		if (counter != 1)
		{
			if (a1.x < b1.x && a1.x < b2.x && a2.x < b1.x && a2.x < b2.x)
				return false;
			if (a1.x > b1.x && a1.x > b2.x && a2.x > b1.x && a2.x > b2.x)
				return false;
			if (a1.z < b1.z && a1.z < b2.z && a2.z < b1.z && a2.z < b2.z)
				return false;
			if (a1.z > b1.z && a1.z > b2.z && a2.z > b1.z && a2.z > b2.z)
				return false;
			return true;
		}

	}
	else
		return false;// 兩線段平行但不共線
	return false;
}

bool CS_DIJKSTRA::intersect(vector3& p, vector3& p1, vector3& p2){
	// 點與線段共線，而且點要在線段之間。
	return cross(p, p1, p2) == 0 && dot(p, p1, p2) <= 0;

}
// 點與線段已確定共線，判斷相交。
bool CS_DIJKSTRA::intersect(vector3& a1, vector3& a2, vector3& b1, vector3& b2){
	return intersect(a1, a2, b1)
		|| intersect(a1, a2, b2)
		|| intersect(b1, b2, a1)
		|| intersect(b1, b2, a2);
}
// 判斷線段相交，但不能處理端點共線的情況
bool CS_DIJKSTRA::intersect_crossLine(vector3& a1, vector3& a2, vector3& b1, vector3& b2){

	vector3 v1 = b1 - a1;
	vector3 v2 = a2 - b1;
	vector3 v3 = b2 - a2;
	vector3 v4 = a1 - b2;
	double c1 = cross(v1, v2);
	double c2 = cross(v2, v3);
	double c3 = cross(v3, v4);
	double c4 = cross(v4, v1);

	double L1 = cross(b1 - a1, a2 - a1);
	double L2 = cross(b2 - a1, a2 - a1);
	double L3 = cross(a1 - b1, b2 - b1);
	double L4 = cross(a2 - b1, b2 - b1);

	if (c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0) return true;
	if (c1 < 0 && c2 < 0 && c3 < 0 && c4 < 0) return true;
	//if (L1 * L2 < 0 && L3 * L4 < 0) return true;
	if (c1 == 0 && intersect(a1, a2, b1)) return true;
	if (c2 == 0 && intersect(a1, a2, b2)) return true;
	if (c3 == 0 && intersect(b1, b2, a1)) return true;
	if (c4 == 0 && intersect(b1, b2, a2)) return true;
	/*if (L1 == 0 && intersect(a1, a2, b1)) return true;
	if (L2 == 0 && intersect(a1, a2, b2)) return true;
	if (L3 == 0 && intersect(b1, b2, a1)) return true;
	if (L4 == 0 && intersect(b1, b2, a2)) return true;*/
	return false;

}

bool CS_DIJKSTRA::point_int_polygon(vector3& p){
	bool c = false;
	for (size_t i = 0, j = m_nodes.size() - 1; i < m_nodes.size(); j = i++)
	if ((m_nodes[i].z > p.z) != (m_nodes[j].z > p.z) &&
		p.x < (m_nodes[j].x - m_nodes[i].x)*(p.z - m_nodes[i].z) / (m_nodes[j].z - m_nodes[i].z) + m_nodes[i].x)
		c = !c;
	return c;
}

float CS_DIJKSTRA::findShortestPath(vector3& start, vector3& goal, vector<int> start_visibilityNode, vector<int> goal_visiblityNode, bool concave){

	typedef adjacency_list < listS, vecS, undirectedS,
		no_property, property < edge_weight_t, float > > graph_t;
	typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
	typedef std::pair<int, int> Edge;

	// obstacle's nodes + start node + goal node
	const int num_nodes = m_convex_nodes.size() + 2; 
	Edge *edge_array;
	float *weights;
	if (!concave){
		edge_array = new Edge[m_convex_edges.size() + start_visibilityNode.size() + goal_visiblityNode.size()];
		weights = new float[m_convex_edges.size() + start_visibilityNode.size() + goal_visiblityNode.size()];
	}
	if (concave){
		edge_array = new Edge[m_convex_edges.size() + start_visibilityNode.size() + goal_visiblityNode.size()];
		weights = new float[m_convex_edges.size() + start_visibilityNode.size() + goal_visiblityNode.size()];
	}
	for (int i = 0; i < m_convex_edges.size(); i++){
		edge_array[i] = Edge(m_convex_edges[i].first_point, m_convex_edges[i].second_point);
		weights[i] = (float)m_convex_edges[i].weight;
	}
	/*
	add the segment which connect with start node
	*/
	for (int i = 0; i < start_visibilityNode.size(); i++){
		int node_index = start_visibilityNode.at(i);
		edge_array[i + m_convex_edges.size()] = Edge(START_NODE, start_visibilityNode.at(i));
		weights[i + m_convex_edges.size()] =
			(float)sqrt(pow(start.x - m_convex_nodes[node_index].x, 2)
			+ pow(start.z - m_convex_nodes[node_index].z, 2));
	}
	/*
	add the segment which connect with goal node
	*/
	for (int i = 0; i < goal_visiblityNode.size(); i++){
		int node_index = goal_visiblityNode.at(i);
		//cout << node_index << endl;
		edge_array[i + m_convex_edges.size() + start_visibilityNode.size()] =
			Edge(goal_visiblityNode.at(i), GOAL_NODE);
		weights[i + m_convex_edges.size() + start_visibilityNode.size()] =
			(float)sqrt(pow(goal.x - m_convex_nodes[node_index].x, 2)
			+ pow(goal.z - m_convex_nodes[node_index].z, 2));
	}

	int num_arcs = m_convex_edges.size() + start_visibilityNode.size() + goal_visiblityNode.size();
	graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	vector<vertex_descriptor> p(num_vertices(g));
	vector<float> d(num_vertices(g));
	vertex_descriptor s = vertex(START_NODE, g); // point START_NODE is the start

	dijkstra_shortest_paths(g, s,
		predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
		distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

	int currentNode = GOAL_NODE;  //從終點出發
	m_shortestPath.clear();

	while (true)
	{
		if (p[currentNode] == currentNode){
			break;
		}
		if (p[currentNode] == START_NODE) //起點
		{
			m_shortestPath.push_back(currentNode);
			m_shortestPath.push_back(START_NODE);
			break;
		}
		m_shortestPath.push_back(currentNode);
		currentNode = p[currentNode];
	}

	float shortest_dis = 0;
	if (m_shortestPath.size() != 0){
		for (int i = 0; i < m_shortestPath.size() - 1; i++){
			if (m_shortestPath.at(i) == GOAL_NODE)
				shortest_dis +=
				sqrt(pow(goal.x - m_convex_nodes[m_shortestPath[i + 1]].x, 2) +
				pow(goal.z - m_convex_nodes[m_shortestPath[i + 1]].z, 2));
			else if (m_shortestPath.at(i + 1) == START_NODE)
				shortest_dis +=
				sqrt(pow(start.x - m_convex_nodes[m_shortestPath[i]].x, 2) +
				pow(start.z - m_convex_nodes[m_shortestPath[i]].z, 2));
			else
				shortest_dis +=
				sqrt(pow(m_convex_nodes[m_shortestPath[i]].x - m_convex_nodes[m_shortestPath[i + 1]].x, 2) +
				pow(m_convex_nodes[m_shortestPath[i]].x - m_convex_nodes[m_shortestPath[i + 1]].z, 2));
		}
	}

	delete[] edge_array;
	delete[] weights;

	return shortest_dis;
}

float CS_DIJKSTRA::findShortestPath_(vector3& start, vector3& goal, vector<int> start_visibilityNode, vector<int> goal_visiblityNode){

	int size = start_visibilityNode.size() + goal_visiblityNode.size() + 2;
	bool *visited = new bool[size];
	float *dist = new float[size];
	float **graph = new float*[size];
	for (int i = 0; i < size; i++){
		dist[i] = INFINITY;
		visited[i] = false;
		graph[i] = new float[size];
	}

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (i == j)
				graph[i][j] = 0;
			else{
				int connect_node;
				if (i == 0){
					for (int k = 0; k < start_visibilityNode.size(); k++){
						connect_node = start_visibilityNode[k] + 1;
						//graph[i][connect_node] = start.distance(vector3())
					}
				}
				else if (i == size - 1){

				}
				else{

				}
			}
		}
	}

	priority_queue<node_distance, vector<node_distance>, compare_dist> pq;

	node_distance first = { 0, 0 };
	pq.push(first);

	while (!pq.empty()){
		node_distance temp = pq.top();
		pq.pop();
		int node = temp.node;
		for (int i = 0; i < size; i++){
			if (graph[node][i] != 0){
				if (dist[i] >(dist[node] + graph[node][i]))
					dist[i] = dist[node] + graph[node][i];

				if (!visited[i]){
					node_distance new_node;
					new_node.node = i;
					new_node.distance = dist[i];
					pq.push(new_node);
					visited[i] = true;
				}
			}
		}
	}
}

void CS_DIJKSTRA::set_Obstacles(){
	//setPolygonObstacle1();
	//setPolygonObstacle2();
	//setRectObstacle1();
	setRectObstacle2();
}

void CS_DIJKSTRA::setPolygonObstacle1(){
	addNode(vector3(6, 0, 8));
	addNode(vector3(17, 0, 5));
	addNode(vector3(25, 0, 18));
	addNode(vector3(27, 0, 19));
	addNode(vector3(35, 0, 6));
	addNode(vector3(47, 0, 20));
	addNode(vector3(36, 0, 45));
	addNode(vector3(10, 0, 15));
	addNode(vector3(12, 0, 12));
	addNode(vector3(11, 0, 13));
	addNode(vector3(10, 0, 10));
	addNode(vector3(7, 0, 11));
}

void CS_DIJKSTRA::setPolygonObstacle2(){
	addNode(vector3(10, 0, 15));
	addNode(vector3(10, 0, 35));
	addNode(vector3(45, 0, 35));
	addNode(vector3(30, 0, 25));
	addNode(vector3(45, 0, 15));
}

void CS_DIJKSTRA::setRectObstacle1(){
	addNode(vector3(25, 0, 10));
	addNode(vector3(36, 0, 10));
	addNode(vector3(36, 0, 28));
	addNode(vector3(25, 0, 28));
}

void CS_DIJKSTRA::setRectObstacle2(){
	addNode(vector3(48, 0, 5));
	addNode(vector3(30, 0, 5));
	addNode(vector3(30, 0, 15));
	addNode(vector3(48, 0, 15));
}