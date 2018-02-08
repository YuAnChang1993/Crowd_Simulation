//
// Instructor: Tsung-Yu Tsai
// Email:	tsungyu.cs03g@g2.nctu.edu.tw
//			disneyanduj@gmail.com
//
// Date: 2015/10/15

#include "point_line_computation.h"

// 點與線段已確定共線，判斷相交。
bool POINT_LINE_COMPUTATION::intersect(vector3& a1, vector3& a2, vector3& b1, vector3& b2)
{
    return intersect(a1, a2, b1)
        || intersect(a1, a2, b2)
        || intersect(b1, b2, a1)
        || intersect(b1, b2, a2);
}


bool POINT_LINE_COMPUTATION::intersect(vector3& p, vector3& p1, vector3& p2)
{
    // 點與線段共線，而且點要在線段之間。
    return cross(p, p1, p2) == 0
        && dot(p, p1, p2) <= 0;
}


// 兩線段共線，交點無限多
bool POINT_LINE_COMPUTATION::collinear(vector3& a1, vector3& a2, vector3& b1, vector3& b2)
{
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
			if(counter!=1)
			{
				if(a1.x < b1.x && a1.x < b2.x && a2.x < b1.x && a2.x < b2.x)
					return false;
				if(a1.x > b1.x && a1.x > b2.x && a2.x > b1.x && a2.x > b2.x)
					return false;
				if(a1.z < b1.z && a1.z < b2.z && a2.z < b1.z && a2.z < b2.z)
					return false;
				if(a1.z > b1.z && a1.z > b2.z && a2.z > b1.z && a2.z > b2.z)
					return false;
				return true;
			}
				
		}
		else
			return false;// 兩線段平行但不共線
	return false;	
}



// 判斷線段相交，但不能處理端點共線的情況
bool POINT_LINE_COMPUTATION::intersect_crossLine(vector3& a1, vector3& a2, vector3& b1, vector3& b2)
{
    vector3 v1 = b1 - a1;
    vector3 v2 = a2 - b1;
    vector3 v3 = b2 - a2;
    vector3 v4 = a1 - b2;
    double c1 = cross(v1, v2);
    double c2 = cross(v2, v3);
    double c3 = cross(v3, v4);
    double c4 = cross(v4, v1);
 
    // 朝同一方向轉彎。都是順時針，或者都是逆時針。
    if (c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0) return true;
    if (c1 < 0 && c2 < 0 && c3 < 0 && c4 < 0) return true;
    if (c1 == 0 && intersect(a1, a2, b1)) return true;
    if (c2 == 0 && intersect(a1, a2, b2)) return true;
    if (c3 == 0 && intersect(b1, b2, a1)) return true;
    if (c4 == 0 && intersect(b1, b2, a2)) return true;
    return false;
}

////兩線段相交
//bool POINT_LINE_COMPUTATION::intersect_crossLine(vector3& a1, vector3& a2, vector3& b1, vector3& b2)
//{
//    double c1 = cross(a1, a2, b1);
//    double c2 = cross(a1, a2, b2);
//    double c3 = cross(b1, b2, a1);
//    double c4 = cross(b1, b2, a2);
// //cout<<"a";
//    // 端點不共線
//    if (c1 * c2 < 0 && c3 * c4 < 0) return false;
//    // 端點共線
//    if (c1 == 0 && intersect(a1, a2, b1)) return true;
//    if (c2 == 0 && intersect(a1, a2, b2)) return true;
//    if (c3 == 0 && intersect(b1, b2, a1)) return true;
//    if (c4 == 0 && intersect(b1, b2, a2)) return true;
//    return false;
//}

float POINT_LINE_COMPUTATION::cross(vector3& o, vector3& a, vector3& b)
{
    return (a.x-o.x) * (b.z-o.z) - (a.z-o.z) * (b.x-o.x);
}

float POINT_LINE_COMPUTATION::cross(vector3& v1, vector3& v2)
{
    // 沒有除法，儘量避免誤差。
    return v1.x * v2.z - v1.z * v2.x;
}

float POINT_LINE_COMPUTATION::dot(vector3& v1, vector3& v2)
{
    // 沒有除法，儘量避免誤差。
    return v1.x * v2.x + v1.z * v2.z;
}

float POINT_LINE_COMPUTATION::dot(vector3& o, vector3& a, vector3& b)
{
    return (a.x-o.x) * (b.x-o.x) + (a.z-o.z) * (b.z-o.z);
}

float POINT_LINE_COMPUTATION::distance(vector3& p, Segment& s)
{
    // 建立向量求點積，以判斷點在哪一區。
    vector3 v = s.p2 - s.p1;
    vector3 v1 = p - s.p1;
    vector3 v2 = p - s.p2;
 
    // 端點距離區：點到點距離
    // 等於=可以省略
	if (dot(v, v1) <= 0) return v1.Length();
    if (dot(v, v2) >= 0) return v2.Length();
    // 垂直距離區：點到線距離
    return abs(cross(v, v1)) / v.Length();
}

float POINT_LINE_COMPUTATION::distance(vector3& p, vector3& p1, vector3&p2)
{
	Segment s;
	s.p1 = p1;
	s.p2 = p2;
	return POINT_LINE_COMPUTATION::distance(p, s);
}

bool POINT_LINE_COMPUTATION::intersectionPoint(vector3& a1, vector3& a2, vector3& b1, vector3& b2, vector3& result)
{
    vector3 a = a2 - a1, b = b2 - b1, s = b1 - a1;
 
    // 兩線平行，交點不存在。
    // 兩線重疊，交點無限多。
    if (cross(a, b) == 0) return false;
 
    // 計算交點
	result = a1 + a * cross(s, b) / cross(a, b);
    return true;
}

bool POINT_LINE_COMPUTATION::point_in_polygon(vector3& p , vector<vector3>& objPoints)
{
    bool c = false;
	for (size_t i = 0, j = objPoints.size()-1; i < objPoints.size(); j = i++)
        if ((objPoints[i].z > p.z) != (objPoints[j].z > p.z) &&
            p.x < (objPoints[j].x-objPoints[i].x)*(p.z-objPoints[i].z)/(objPoints[j].z-objPoints[i].z)+objPoints[i].x)
            c = !c;
    return c;
}

float POINT_LINE_COMPUTATION::cos_v(vector3& v1, vector3& v2)
{
	return dot(v1, v2) / v1.Length() / v2.Length();
}

float POINT_LINE_COMPUTATION::sin_v(vector3& v1, vector3& v2)
{
	return cross(v1, v2) / v1.Length() / v2.Length();
}

vector3 POINT_LINE_COMPUTATION::rotate2D(vector3& v, double degree)
{
	vector3 tmp;
	tmp.x = static_cast<float>(v.x*cos(degree) - v.z*sin(degree));
	tmp.z = static_cast<float>(v.x*sin(degree) + v.z*cos(degree));
	tmp.y = v.y;
	
	return tmp;
}