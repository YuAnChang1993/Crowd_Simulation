//
// Instructor: Tsung-Yu Tsai
// Email:	tsungyu.cs03g@g2.nctu.edu.tw
//			disneyanduj@gmail.com
//
// Date: 2015/10/15
//Reference : http://www.csie.ntnu.edu.tw/~u91029/

#ifndef __POINT_LINE_COMPUTATION_H_
#define __POINT_LINE_COMPUTATION_H_

#include <vector>
#include <math.h>
#include "../primitives/vector3d.h"

using namespace std;

struct Line {vector3 p1, p2;};
typedef Line Segment;

class POINT_LINE_COMPUTATION {
public :
	POINT_LINE_COMPUTATION() {};

	static bool    intersect(vector3& p, vector3& p1, vector3& p2);
	static bool    intersect(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	static bool    collinear(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	static bool    intersect_crossLine(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	static bool    overlap(vector3& a1, vector3& a2, vector3& b1, vector3& b2);
	static float   cross(vector3& o, vector3& a, vector3& b);
	static float   cross(vector3& v1, vector3& v2);
	static float   dot(vector3& v1, vector3& v2);
	static float   dot(vector3& o, vector3& a, vector3& b);
	static float   distance(vector3& p, Segment& s);
	static float   distance(vector3& p, vector3& p1, vector3&p2);
	static bool    intersectionPoint(vector3& a1, vector3& a2, vector3& b1, vector3& b2, vector3& result);
	static bool    point_in_polygon(vector3& p , vector<vector3>& objPoints);
	static float   cos_v(vector3& v1, vector3& v2);
	static float   sin_v(vector3& v1, vector3& v2);
	static vector3 rotate2D(vector3& v, double degree);

};

#endif
