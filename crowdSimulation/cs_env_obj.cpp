//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/11
//
#include <iostream>
#include <cmath>
#include "cs_env_obj.h" // environment object

#define MAX_NUM_EDGES 32

CS_ENV_OBJ_ACTION::CS_ENV_OBJ_ACTION( )
{
}

CS_ENV_OBJ_ACTION_DATA::CS_ENV_OBJ_ACTION_DATA( )
{
	mCurTime = 0.0;
	mActionIndex = 0;
}

	void CS_ENV_OBJ_ACTION_DATA::addAction( int actionType, float time_duration )
	{
		CS_ENV_OBJ_ACTION a;
		a.actionType = actionType;
		a.time_duration = time_duration;

		mActionArr.push_back( a );
	}

	void CS_ENV_OBJ_ACTION_DATA::update( float time_step )
	{
		int numActions = mActionArr.size( );
		if ( numActions == 0 ) return;
		if ( numActions == 1 ) return;
		mCurTime += time_step;
		if ( mCurTime >= mActionArr[ mActionIndex ].time_duration ) {
			mCurTime -= mActionArr[ mActionIndex ].time_duration;
			mActionIndex = (mActionIndex+1)%numActions;
		}
	}

	bool CS_ENV_OBJ_ACTION_DATA::getEnabledStatus( ) const	
	{
		if ( mActionArr.size( ) == 0 ) return true;
		if ( mActionArr[mActionIndex].actionType == CS_ENV_OBJ_ACTION_TYPE_ENABLE ) return true;
		return false;
	}

/*
The points of a polygon is arranged in counter-clockwise.
*/
CS_ENV_OBJECT::CS_ENV_OBJECT( )
{
	textureID = -1;
	mFlgEnabledStatusAsAnObstacle = true;
	//
	mFlgUseDataAction = false;
	//
	mObjID = 0;
	ptArr.clear( );
	m_bo_ObjType = CS_ENV_OBJECT_TYPE_NONE;
	m_bo_ObjState = CS_OBJECT_STATE_NONE;

	guideDirection = CS_ENV_OBJECT_GUIDE_DIRECTION_NONE;
	guideDirection = CS_ENV_OBJECT_GUIDE_DIRECTION_CCW;
	//
	color.r = 0.8;
	color.g = 0.8;
	color.b = 0.8;

	mCornerThickness_Visual = 1.5; // 1m
	//mCornerThickness_CD = 0.25; // cm
	mCornerThickness_CD = 1.0;
	//
	m_bo_CurPickedVertexIndex = -1;
	m_bo_CurPickedEdgeIndex = -1;
	m_bo_FlgPicked = false;

}

void CS_ENV_OBJECT::clear( )
{
	ptArr.clear( );
	m_bo_ObjType = CS_ENV_OBJECT_TYPE_NONE;
	m_bo_ObjState = CS_OBJECT_STATE_NONE;
}

void CS_ENV_OBJECT::update( float time_step )
{
	if ( mFlgUseDataAction == false ) return;
	mActionData.update( time_step );
	mFlgEnabledStatusAsAnObstacle = mActionData.getEnabledStatus( );
}

bool CS_ENV_OBJECT::getFlg_UseActionData( ) const
{
	return mFlgEnabledStatusAsAnObstacle;
}

void CS_ENV_OBJECT::addWorkingCycle( float enabled_duration, float disabled_duration )
{
	if ( mActionData.mActionArr.size( ) == 2 ) {
		mActionData.mActionArr[0].time_duration = enabled_duration;
		mActionData.mActionArr[1].time_duration = disabled_duration;

	} else {
		mActionData.addAction( CS_ENV_OBJ_ACTION_TYPE_ENABLE, enabled_duration );
		mActionData.addAction( CS_ENV_OBJ_ACTION_TYPE_DISABLE, disabled_duration );
	}

}

void CS_ENV_OBJECT::enableActionData( bool flg )
{ 
	mFlgUseDataAction = flg;
	if ( mFlgUseDataAction == false ) {
		mFlgEnabledStatusAsAnObstacle = true;
	}
}


bool CS_ENV_OBJECT::getEnabledStatusAsAnObstacle( ) const
{	
	return mFlgEnabledStatusAsAnObstacle;
}

void CS_ENV_OBJECT::setObjState( int objState )
{
	this->m_bo_ObjState = objState;
}

void CS_ENV_OBJECT::setObjType( int objType )
{
	this->m_bo_ObjType = objType;
}

void CS_ENV_OBJECT::convertRectangleIntoPolygon( )
{
	vector3 p0 = ptArr[0]; // lower left
	vector3 p1 = ptArr[1]; // upper right
	m_bo_ObjType = CS_ENV_OBJECT_TYPE_POLYGON;
	ptArr.clear( );
	ptArr.push_back( p0 );
	ptArr.push_back( p0 );
	ptArr.push_back( p1 );
	ptArr.push_back( p1 );

	ptArr[1].x = p1.x;
	ptArr[3].x = p0.x;
}

void CS_ENV_OBJECT::set( const CS_ENV_OBJECT &obj )
{
	textureID = obj.textureID;
	mObjID = obj.mObjID;
	m_bo_ObjType = obj.m_bo_ObjType;
	ptArr = obj.ptArr;
	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_RECTANGLE:
			convertRectangleIntoPolygon( );
			break;
	}
	computeExtendedObj( mCornerThickness_CD );
}

void CS_ENV_OBJECT::addPoint( const vector3 & p )
{
	addPoint( p.x, p.y, p.z );
}

void CS_ENV_OBJECT::addPoint( float x, float y, float z )
{
	ptArr.push_back( vector3( x, y, z) );
	computeExtendedObj( mCornerThickness_CD );
	/*
	if ( numNumPoints >=  maxNumPoints ) {
	return;
	}
	ptArr[ numNumPoints ] = vector3( x, y, z );
	++numNumPoints;
	*/
}

bool CS_ENV_OBJECT::canBePicked_Polygon
( 
 const vector3 &p, 
 float hitDistance,
 float &shortest_distance2 
 )
{
	m_bo_SelectedPoint = p;
	m_bo_FlgPicked = false;
	bool flg = false;
	m_bo_CurPickedVertexIndex = -1;
	m_bo_CurPickedEdgeIndex = -1;
	float hitDistance2 = hitDistance*hitDistance;
	int numPoints = ptArr.size( );
	for ( int i = 0; i < numPoints; ++i ) {
		float d2 = p.distance2( ptArr[i] );
		if ( ( d2 > hitDistance2 ) && ( hitDistance2 > 0 ) ) continue;
		if ( shortest_distance2 < 0 || shortest_distance2 > d2 ) {
			shortest_distance2 = d2;
			m_bo_CurPickedVertexIndex = i;
			//		cout << "shortest_distance2:" << shortest_distance2 << endl;

			flg = true;
		}
	}

	float vertex_distance_threshold = 0.2;

	//cout << "m_bo_CurPickedVertexIndex:" << m_bo_CurPickedVertexIndex << endl;

	if ( flg ) return flg;
	//

	float d2 = -1.0;
	int eIndex = bt_computeClosestPathEdgeIndex_Polygon( p, ptArr, numPoints, d2 );

	if ( ( shortest_distance2 < 0 
		|| 
		shortest_distance2 > d2 )
		&&
		( hitDistance2 > d2 )
		) {
			shortest_distance2 = d2;
			m_bo_CurPickedEdgeIndex = eIndex;
			m_bo_CurPickedVertexIndex = -1;
			flg = true;
	}

	//cout << "m_bo_CurPickedEdgeIndex:" << m_bo_CurPickedEdgeIndex << endl;

	return flg;
}

bool CS_ENV_OBJECT::canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 )
{
	bool flg = false;
	m_bo_SelectedPoint = p;
	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_RECTANGLE:
			break;
		case CS_ENV_OBJECT_TYPE_POLYGON:
			flg = canBePicked_Polygon( p, hitDistance, shortest_distance2 );
			break;
	}
	return flg;
}

/*
Check if q inside the rectangle object extended by r units
*/
bool CS_ENV_OBJECT::isContained( const vector3 & q, float r ) const
{
	vector3 p0 = ptArr[0];
	vector3 p1 = ptArr[1];
	return !( p0.x > q.x + r  
		|| p1.x < q.x - r  
		|| p0.z > q.z + r  
		|| p1.z < q.z - r );
}
bool CS_ENV_OBJECT::isContained( const vector3 & q0, const vector3 & q1, float r ) const
{
	bool flg = false;
	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_POLYGON:
			flg = isContained_Polygon( q0, q1, r );
			break;
	}
	return flg;
}

bool CS_ENV_OBJECT::isContained_Polygon( const vector3 & q0, const vector3 & q1, float r ) const
{
	bool flg = false;
	//cout << ".";
	if ( !insideAABB_lineSegment_XZ( mAABBmin, mAABBmax, q0, q1 ) ) return false;
	flg = true;

	return flg;
}

// Check if the line segment (q0, q1) lies inside the rectangle object.
bool CS_ENV_OBJECT::isContained_Rect( const vector3 & q0, const vector3 & q1, float r ) const
{
	bool flg0 = false;
	bool flg1 = false;

	vector3 p0 = ptArr[0];
	vector3 p1 = ptArr[1];


	flg1 = ( p0.x > q1.x + r  
		|| p1.x < q1.x - r  
		|| p0.z > q1.z + r  
		|| p1.z < q1.z - r ); 
	flg0 = ( p0.x > q0.x + r  
		|| p1.x < q0.x - r  
		|| p0.z > q0.z + r  
		|| p1.z < q0.z - r );

	if ( flg0 == true && flg1 == true ) return false;

	return true;
}

bool isInsideRectBoundary_XZ( const vector3 &q, const vector3 &p0, const vector3 &p1, float r, float delta_d )
{
	vector3 j0 = p0;
	vector3 j1 = p1;

	//printf_vector3( "p0", p0 );
	//printf_vector3( "p1", p1 );

	j0.x -= r + delta_d;
	j0.z -= r + delta_d;
	//
	j1.x += r + delta_d;
	j1.z += r + delta_d;

	//
	//	cout << "r:" << r << endl;
	//	cout << "delta_d:" << delta_d << endl;

	//	printf_vector3( "q", q );
	//	printf_vector3( "j0", j0 );
	//	printf_vector3( "j1", j1 );

	bool flg = ( j0.x > q.x  
		|| j1.x < q.x  
		|| j0.z > q.z  
		|| j1.z < q.z ); 

	if ( flg ) return false;

	return true;

}

vector3 CS_ENV_OBJECT::computeNormalDirection_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, vector3 &prefer_vec ) const
{
	float delta_d = 0.05;
	flgHit = false;
	vector3 n;
	n.zero( );
	prefer_vec.zero( );

	vector3 p0 = ptArr[0];
	vector3 p1 = ptArr[1];
	//
	bool flg0 = isInsideRectBoundary_XZ( q0, p0, p1, r, delta_d );
	bool flg1 = isInsideRectBoundary_XZ( q1, p0, p1, r, delta_d );
	//

	if ( flg0 == false ) return n;
	//
	flgHit = true;
	float d[4];
	d[0] = q0.z - p1.z; // top;

	d[1] = p0.z - q0.z; // bottom;

	d[2] = q0.x - p1.x; // right;

	d[3] = p0.x - q0.x; // left;

	float abs_d[4];
	bool flg_outside = false;
	for ( int i = 0; i < 4; ++i ) {
		abs_d[i] = d[i];
		if ( abs_d[i] < 0.0 ) {
			abs_d[i] = -abs_d[i];
		} else {
			flg_outside = true;
		}
	}

	bool c[4] = {0, 0, 0, 0};
	// BEGIN: MISTAKE
	// Does not handle the case that 
	// q lying inside the rectangle.
	// simpliy using thresholding is not good.
	vector3 q = q0;
	//
	// adjust q so that q lying outside or on boundary
	// 



	if ( flg_outside == false ) {
		float dz = abs_d[0];
		if ( abs_d[1] > abs_d[0] ) {
		} else {
			dz = -abs_d[1];
		}

		float dx = abs_d[2];
		if ( abs_d[3] > abs_d[2] ) {
		} else {
			dx = -abs_d[3];
		}
		q.x += dx;
		q.z += dz;


	}

	delta_d = 0.052;

	if ( q.z > p1.z - delta_d ) { // top
		n += vector3( 0.0, 0.0, 1.0);
		//
		prefer_vec += vector3( -1.0, 0.0, 0.0);
		c[0] = 1;
	}

	if ( q.z < p0.z + delta_d ) { // bottom
		n += vector3( 0.0, 0.0, -1.0);
		//
		prefer_vec += vector3( 1.0, 0.0, 0.0);

		c[1] = 1;

	}

	if ( q.x > p1.x - delta_d ) { // right
		n += vector3( 1.0, 0.0, 0.0);
		prefer_vec += vector3( 0.0, 0.0, 1.0);

		c[2] = 1;

	}

	if ( q.x < p0.x + delta_d ) { // left
		n += vector3( -1.0, 0.0, 0.0);
		prefer_vec += vector3( 0.0, 0.0, -1.0);

		c[3] = 1;

	}

	prefer_vec.Normalized( );
	n.Normalize( );
	if ( n.x != n.x ) {
		cout << "NAN" << endl;
		printf_vector3("p0:", p0 );
		printf_vector3("p1:", p1 );

		cout << "q0:" << q0.x << "\t" << q0.y << "\t" << q0.z << endl;
		cout << "c:" << c[0] << "\t" << c[1] << "\t" << c[2] << "\t" << c[3] << endl;
		cout << "q outside:" << flg_outside << endl;
		system("pause");
	}
	return n;
	/*
	if ( q0.z > p1.z && q0.x > p0.x && p1.x > q0.x ) {
	n = vector3( 0.0, 0.0, 1.0);
	return n;
	}

	if ( q0.z < p0.z && q0.x > p0.x && p1.x > q0.x ) {
	n = vector3( 0.0, 0.0, -1.0);
	return n;
	}
	//
	if ( q0.x > p1.x && q0.z > p0.z && p1.z > q0.z ) {
	n = vector3( 1.0, 0.0, 0.0);
	return n;
	}

	if ( q0.x > p1.x && q0.z > p0.z && p1.z > q0.z ) {
	n = vector3( -1.0, 0.0, 1.0);
	return n;
	}
	*/

	//
	float collideTime[4];
	float ct;
	bool flg_handled = false;

	if ( q1.x - q0.x != 0.0 ) {
		collideTime[0] = ( p0.x - q0.x - r )/(q1.x - q0.x);
		collideTime[1] = ( p1.x - q0.x + r )/(q1.x - q0.x);
	} else {
		collideTime[0] = collideTime[1] = 2.0;
	}
	//
	if ( q1.z - q0.z != 0.0 ) {
		collideTime[2] = ( p0.z - q0.z - r )/(q1.z - q0.z);
		collideTime[3] = ( p1.z - q0.z + r )/(q1.z - q0.z);

	} else {
		collideTime[2] = collideTime[3] = 2.0;
	}

	int index = -1;

	float min_ct = 0.0;

	//cout << "----" << endl;
	for ( int i = 0; i < 4; ++i ) {
		float ct = collideTime[i];
		//cout << "ct:" << ct << endl;
		if ( 0.0 > ct || ct > 1.0 ) {
			if ( flg0 && flg1 ) {
				if ( 0.0 > ct ) {
				}
			} else {
				continue;
			}
		}
		vector3 q = q0 + ct*( q1 - q0 );
		if ( isInsideRectBoundary_XZ( q, p0, p1, r, delta_d ) ) {
			if ( index == -1 || min_ct > ct ) {
				index = i;
				min_ct = ct;
				flgHit = true;
			}
		}
	}

	if ( index >= 0 ) {
		switch ( index ) {
			case 0:
				n = vector3(-1.0, 0.0, 0.0);
				break;
			case 1:
				n = vector3(1.0, 0.0, 0.0);
				break;
			case 2:
				n = vector3(0.0, 0.0, -1.0);
				break;
			case 3:
				n = vector3(0.0, 0.0, 1.0);
				break;
		}
	}

	if ( ( flg0 && flg1 ) ) {
		if ( flgHit == false ) {
			//cout << "error" << endl;
		}
	}
	return n;
}


//
// moving from q0 to q1
//

vector3 CS_ENV_OBJECT::computeSeparationVector_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction ) const
{

	vector3 s;
	if ( mFlgEnabledStatusAsAnObstacle == false ) return s;

	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_POLYGON:
			return computeSeparationVector_Circle_Polygon( q0, q1, r, flgHit, contact_time, feasible_direction );
			break;
	}
	return s;
}

vector3 CS_ENV_OBJECT::computeSeparationVector_Circle_Edge( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction, int edgeID ) const
{

	vector3 s;
	if ( mFlgEnabledStatusAsAnObstacle == false ) return s;

	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_POLYGON:
			return computeSeparationVector_Circle_Polygon_Edge( q0, q1, r, flgHit, contact_time, feasible_direction, edgeID );
			break;
	}
	return s;
}

//
//
//
vector3 CS_ENV_OBJECT::computeSeparationVector_Circle_Polygon( 
	const vector3 & p0,		//starting point
	const vector3 & p1,		//ending point
	float obj_r,				//radius
	bool &flgHit, 
	float &contact_time, 
	vector3 &feasible_direction ) const
{
	vector3 s;

	flgHit = false;
	float e = 0.10;
	float e2 = e*e;

	/*
	compute the closest line segment for q0
	*/
	contact_time = -1.0;
	float closest_d = -1.0;
	vector3 closest_n;
	int closestLineSegmentIndex = -1;
	// Line segment
	for ( int i = 0; i < ptArr_extendedObj.size( ); ++i ) {
		float d0, d1;
		vector3 env_q0 = ptArr_extendedObj[i].p0;
		vector3 env_q1 = ptArr_extendedObj[i].p1;
		vector3 n = ptArr_extendedObj[i].n;
		//
		vector3 q0 = env_q0 + n*obj_r;
		vector3 q1 = env_q1 + n*obj_r;

		//
		d0 = n.dot( p0 - q0 );
		d1 = n.dot( p1 - q0 );
		if ( (contact_time < 0.0) && d0 > 0.0 && d1 > 0.0 ) {
			float dd2 = bt_computeDistance2_LineSegment( p1, q0, q1 );
			if ( e2 > dd2 ) {
				if ( closest_d < 0.0 || closest_d > dd2) {
					//cout << "case 1: dd2" << dd2 << endl;
					closest_d = dd2;
					closest_n = n;
					contact_time = 2.0;
				}
				flgHit = true;
			}
			continue; // outside, not collide
		}
		if ( (contact_time < 0.0) && d0 < 0.0 ) {
			if ( d1 > d0 ) {
				if ( d1 < 0.0 ) d1 = -d1;
				float dd2 = bt_computeDistance2_LineSegment( p1, q0, q1 );

				if ( e2 > dd2 ) {
					if ( closest_d < 0.0 || closest_d > dd2) {
						//cout << "case 2: dd2" << dd2 << endl;
						closest_d = dd2;
						contact_time = 2.0;
						closest_n = n;
					}
					flgHit = true;
				}
				continue; // move from interior to outside
			}
		}
		float ct = bt_computeContactTime_LineSegment( p0, p1, q0, q1, n );

		if ( 
			( contact_time < 0.0 && ct >=0.0 ) 
			|| (ct >=0.0 && contact_time > ct) 
			)
		{
			contact_time = ct;
			closestLineSegmentIndex = i;
			closest_n = n;

		}
	}

	// for points
	float vertex_cd_r = mCornerThickness_CD + obj_r;
	float vertex_cd_r2 = vertex_cd_r*vertex_cd_r;
	int closestVertexIndex = -1;
	float vertex_d2 = -1.0;
	for ( int i = 0; i < ptArr.size( ); ++i ) {
		vector3 q = ptArr[i];
		vector3 n;

		float d2 = p0.distance2( q );
		if ( vertex_cd_r2 + e2 >= d2 ) {
			//p0 inside
			if ( closestLineSegmentIndex < 0 ) {
				n = (p0 - q);
				n.Normalize( );
				closest_n = n;
				closest_d = 0.0;
				//s = p0 - p1;
				flgHit = true;
			}
			/*
			if ( 0 > closestVertexIndex ) {
			n = (p0 - q);
			n.Normalize( );
			closest_n = n;
			closest_d = 0.0;
			flgHit = true;
			}
			*/
			continue; // p0 
		}
		/*
		if ( ) {
		if ( e2 > dd2 ) {
		if ( closest_d < 0.0 || closest_d > dd2) {
		cout << "case 1: dd2" << dd2 << endl;
		closest_d = dd2;
		closest_n = n;
		contact_time = 2.0;
		}
		flgHit = true;
		}
		}
		*/		
		float ct = bt_computeContactTime_PointCircle( p0, p1, q, vertex_cd_r );
		if ( 
			( contact_time < 0.0 && ct >=0.0 ) 
			|| (ct >=0.0 && contact_time > ct) 
			)
		{
			contact_time = ct;
			closestVertexIndex = i;
			//closest_n = n;
			closestLineSegmentIndex = -1;

		}
	}
	//

	feasible_direction = closest_n;
	//cout << "contact_time:" << contact_time << endl;
	//cout << "closestLineSegmentIndex:" << closestLineSegmentIndex << endl;
	//cout << "closestVertexIndex:" << closestVertexIndex << endl;

	if ( closestLineSegmentIndex < 0 && closestVertexIndex < 0 ) {
		if ( flgHit ) {
			//s += closest_n*0.01;
			s = -(p1 - p0)/1.01 + closest_n*0.01;
		}
		return s;
	}
	//
	vector3 n;
	vector3 q0, q1;
	if ( closestLineSegmentIndex >= 0 ) {
		vector3 env_q0 = ptArr_extendedObj[closestLineSegmentIndex].p0;
		vector3 env_q1 = ptArr_extendedObj[closestLineSegmentIndex].p1;
		n = ptArr_extendedObj[closestLineSegmentIndex].n;
		//
		q0 = env_q0 + n*obj_r;
		q1 = env_q1 + n*obj_r;

		//
		if ( n.Length( ) == 0.0 ) {
			cout << "Error! n.Length( ) == 0.0" << endl;
			system("pause");
		}
		//check if q0 inside the object
		vector3 pt = p0 + contact_time*(p1 - p0);
		vector3 p0p0 = p0 - q0;
		vector3 ptp0 = pt - q0;
		s = pt - p1;
		float d = n.dot( ptp0 );
		if ( d >= 0.0 ) {
		} else {
			// inside
			//s = -d*n;
			//s.y = 0.0;
			s = pt - p1;
		}
	} else if ( closestVertexIndex >= 0 ) {
		vector3 pt = p0 + contact_time*(p1 - p0);
		q0 = ptArr[closestVertexIndex];
		s = pt - p1;
		n = (pt - q0);
		n.Normalize( );
	}

	s += 0.05*n; // n is a unit vector; normalized normal vector.

	closest_d = 0.0;
	flgHit = true;
	return s;
}

vector3 CS_ENV_OBJECT::computeSeparationVector_Circle_Polygon_Edge( 
	const vector3 & p0,		//starting point
	const vector3 & p1,		//ending point
	float obj_r,				//radius
	bool &flgHit, 
	float &contact_time, 
	vector3 &feasible_direction, int edgeID ) const
{
	vector3 s;

	flgHit = false;
	float e = 0.10;
	float e2 = e*e;

	/*
	compute the closest line segment for q0
	*/
	contact_time = -1.0;
	float closest_d = -1.0;
	vector3 closest_n;
	int closestLineSegmentIndex = -1;
	// Line segment
	//for ( int i = 0; i < ptArr_extendedObj.size( ); ++i ) {
	//cout << "edgeID:" << edgeID << endl;
	{
		int i = edgeID;
		float d0, d1;
		vector3 env_q0 = ptArr_extendedObj[i].p0;
		vector3 env_q1 = ptArr_extendedObj[i].p1;
		vector3 n = ptArr_extendedObj[i].n;
		//
		vector3 q0 = env_q0 + n*obj_r;
		vector3 q1 = env_q1 + n*obj_r;

		//
		d0 = n.dot( p0 - q0 );
		d1 = n.dot( p1 - q0 );
		if ( (contact_time < 0.0) && d0 > 0.0 && d1 > 0.0 ) {
			float dd2 = bt_computeDistance2_LineSegment( p1, q0, q1 );
			if ( e2 > dd2 ) {
				if ( closest_d < 0.0 || closest_d > dd2) {
					//cout << "case 1: dd2" << dd2 << endl;
					closest_d = dd2;
					closest_n = n;
					contact_time = 2.0;
				}
				flgHit = true;
			}
			//continue; // outside, not collide
		}
		if ( (contact_time < 0.0) && d0 < 0.0 ) {
			if ( d1 > d0 ) {
				if ( d1 < 0.0 ) d1 = -d1;
				float dd2 = bt_computeDistance2_LineSegment( p1, q0, q1 );

				if ( e2 > dd2 ) {
					if ( closest_d < 0.0 || closest_d > dd2) {
						//cout << "case 2: dd2" << dd2 << endl;
						closest_d = dd2;
						contact_time = 2.0;
						closest_n = n;
					}
					flgHit = true;
				}
				//continue; // move from interior to outside
			}
		}
		float ct = bt_computeContactTime_LineSegment( p0, p1, q0, q1, n );

		if ( 
			( contact_time < 0.0 && ct >=0.0 ) 
			|| (ct >=0.0 && contact_time > ct) 
			)
		{
			contact_time = ct;
			closestLineSegmentIndex = i;
			closest_n = n;

		}
	}

	// for points
	float vertex_cd_r = mCornerThickness_CD + obj_r;
	float vertex_cd_r2 = vertex_cd_r*vertex_cd_r;
	int closestVertexIndex = -1;
	float vertex_d2 = -1.0;
	//for ( int i = 0; i < ptArr.size( ); ++i ) {
	//
	int numVertices = ptArr.size( );
	for ( int j = edgeID; j < edgeID + 2; ++j ) {
		int i = (j)%numVertices;
		vector3 q = ptArr[i];
		vector3 n;

		float d2 = p0.distance2( q );
		if ( vertex_cd_r2 + e2 >= d2 ) {
			//p0 inside
			if ( closestLineSegmentIndex < 0 ) {
				n = (p0 - q);
				n.Normalize( );
				closest_n = n;
				closest_d = 0.0;
				//s = p0 - p1;
				flgHit = true;
			}
			/*
			if ( 0 > closestVertexIndex ) {
			n = (p0 - q);
			n.Normalize( );
			closest_n = n;
			closest_d = 0.0;
			flgHit = true;
			}
			*/
			continue; // p0 
		}
		/*
		if ( ) {
		if ( e2 > dd2 ) {
		if ( closest_d < 0.0 || closest_d > dd2) {
		cout << "case 1: dd2" << dd2 << endl;
		closest_d = dd2;
		closest_n = n;
		contact_time = 2.0;
		}
		flgHit = true;
		}
		}
		*/		
		float ct = bt_computeContactTime_PointCircle( p0, p1, q, vertex_cd_r );
		if ( 
			( contact_time < 0.0 && ct >=0.0 ) 
			|| (ct >=0.0 && contact_time > ct) 
			)
		{
			contact_time = ct;
			closestVertexIndex = i;
			//closest_n = n;
			closestLineSegmentIndex = -1;

		}
	}
	//

	feasible_direction = closest_n;
	//cout << "contact_time:" << contact_time << endl;
	//cout << "closestLineSegmentIndex:" << closestLineSegmentIndex << endl;
	//cout << "closestVertexIndex:" << closestVertexIndex << endl;

	if ( closestLineSegmentIndex < 0 && closestVertexIndex < 0 ) {
		if ( flgHit ) {
			//s += closest_n*0.01;
			s = -(p1 - p0)/1.01 + closest_n*0.01;
		}
		return s;
	}
	//
	vector3 n;
	vector3 q0, q1;
	if ( closestLineSegmentIndex >= 0 ) {
		vector3 env_q0 = ptArr_extendedObj[closestLineSegmentIndex].p0;
		vector3 env_q1 = ptArr_extendedObj[closestLineSegmentIndex].p1;
		n = ptArr_extendedObj[closestLineSegmentIndex].n;
		//
		q0 = env_q0 + n*obj_r;
		q1 = env_q1 + n*obj_r;

		//
		if ( n.Length( ) == 0.0 ) {
			cout << "Error! n.Length( ) == 0.0" << endl;
			system("pause");
		}
		//check if q0 inside the object
		vector3 pt = p0 + contact_time*(p1 - p0);
		vector3 p0p0 = p0 - q0;
		vector3 ptp0 = pt - q0;
		s = pt - p1;
		float d = n.dot( ptp0 );
		if ( d >= 0.0 ) {
		} else {
			// inside
			//s = -d*n;
			//s.y = 0.0;
			s = pt - p1;
		}
	} else if ( closestVertexIndex >= 0 ) {
		vector3 pt = p0 + contact_time*(p1 - p0);
		q0 = ptArr[closestVertexIndex];
		s = pt - p1;
		n = (pt - q0);
		n.Normalize( );
	}

	s += 0.05*n; // n is a unit vector; normalized normal vector.

	closest_d = 0.0;
	flgHit = true;
	return s;
}

//treat corner as a circle
vector3 CS_ENV_OBJECT::computeSeparationVector_Circle_Rect( const vector3 & q0, const vector3 & q1, float r, float &contact_time, vector3 &feasible_direction ) const
{
	vector3 s;
	bool flg0 = false; // outside the rectangle?
	bool flg1 = false; // outside the rectangle?

	vector3 p0, p1;
	p0 = ptArr[0];
	p1 = ptArr[1];

	flg0 = ( p0.x > q0.x + r  
		|| p1.x < q0.x - r  
		|| p0.z > q0.z + r  
		|| p1.z < q0.z - r );

	flg1 = ( p0.x > q1.x + r  
		|| p1.x < q1.x - r  
		|| p0.z > q1.z + r  
		|| p1.z < q1.z - r ); 

	//cout << "flg0:" << flg0 << endl;
	//cout << "flg1:" << flg1 << endl;

	if ( flg0 == true && flg1 == true ) return s;

	if ( flg1 == true ) return s;

	float r0 = 0.0;
	flg0 = ( p0.x > q0.x + r0  
		|| p1.x < q0.x - r0  
		|| p0.z > q0.z + r0  
		|| p1.z < q0.z - r0 );

	float d_minx = q1.x - p0.x;
	float d_maxx = p1.x - q1.x;
	float d_minz = q1.x - p0.z;
	float d_maxz = p1.z - q1.z;

	bool flg0_minx = false; // outside it
	bool flg0_maxx = false;
	bool flg0_minz = false;
	bool flg0_maxz = false;

	//
	//     +-------------+
	//     |             |
	// q0  |             |
	//     |             |
	//     +-------------+
	//           q0
	if ( flg0 == false ) {

	} else {
		// q0 outside of the rectangle.
		if ( q0.x < p0.x ) flg0_minx = true;
		if ( q0.x > p1.x ) flg0_maxx = true;
		if ( q0.z < p0.z ) flg0_minz = true;
		if ( q0.z > p1.z ) flg0_maxz = true;

		float collideTime[4];

		if ( q1.x - q0.x != 0.0 ) {
			collideTime[0] = ( p0.x - q0.x - r )/(q1.x - q0.x);
			collideTime[1] = ( p1.x - q0.x +r )/(q1.x - q0.x);
			if ( (q0.x >= q1.x )) { collideTime[0] = 2.0; } // consider moving direction
			if ( (q1.x >= q0.x )) { collideTime[1] = 2.0; } // consider moving direction
		} else {
			collideTime[0] = collideTime[1] = 2.0;
		}
		//
		if ( q1.z - q0.z != 0.0 ) {
			collideTime[2] = ( p0.z - q0.z - r )/(q1.z - q0.z); 
			collideTime[3] = ( p1.z - q0.z + r )/(q1.z - q0.z);
			if ( (q0.z >= q1.z )) { collideTime[2] = 2.0; } // consider moving direction, moving away
			if ( (q1.z >= q0.z )) { collideTime[3] = 2.0; } // consider moving direction


		} else {
			collideTime[2] = collideTime[3] = 2.0;
		}

		/*
		for ( int i = 0; i < 4; ++i ) {
		cout << "collideTime(i):" << i << "\tt:" << collideTime[i] << endl;
		}
		*/
		int case0 = -1;
		float low_b = -0.5;
		float t = collideTime[0];
		if ( t >= low_b && 1.0 >= t && isContained( q0 + t*(q1-q0), r*1.05 ) ) {
			case0 = 0;
		} else {
			t = 2.0;
		}


		//cout << "case0:" << case0 << endl;
		for ( int i = 1; i < 4; ++i ) {
			//cout << "-----" << endl;
			float ct = collideTime[i];
			if ( t>=low_b ) {
				if ( t > ct &&ct >= low_b && 1.0 >= ct ) {

					bool flg = isContained( q0 + ct*(q1-q0), r*1.05 );
					//cout << "flg:" << flg << endl;
					//cout << "ct:" << ct << endl;
					if ( flg == true ) {
						case0 = i;
						t = collideTime[i];
					}
				}
			} else if ( ct >= low_b && 1.0 >= ct ) {
				if ( isContained( q0 + ct*(q1-q0), r*1.05 ) ) {
					case0 = i;
					t = ct;
				}
			}
			//cout << "i:" << i << endl;
			//cout << "case0:" << case0 << endl;
			//cout << "t:" << t << endl;

		}
		if ( t >= low_b && 1.0 >= t ) {
			if ( case0 >= 0 ) {
				vector3 new_q;
				new_q = q0 + t*(q1-q0);
				s = new_q - q1;
				//cout << "t:" << t << endl;
				//cout << "computeSeparationVector_Circle contact time:" << t << endl;
			}
		}
	}
	return s;
}

/*

*/
vector3 CS_ENV_OBJECT::computeShortestDistancePoint_Rectangle( 
	const vector3 &p,
	float &shortest_distance2 
	)
{
	vector3 out_p;
	vector<vector3> pArr;
	vector3 p0, p1, p2, p3;
	p0 = ptArr[ 0 ];
	p1 = ptArr[ 0 ];
	p2 = ptArr[ 1 ];
	p3 = ptArr[ 1 ];
	p1.x = p2.x;
	p3.x = p0.x;

	pArr.push_back( p0 );
	pArr.push_back( p1 );
	pArr.push_back( p2 );
	pArr.push_back( p3 );

	out_p = bt_computeShortestDistancePoint_Polygon( p, ptArr, shortest_distance2 );

	return out_p;
}



/*
Compute the point closest to point p.
out_direction:
return: 
the point closest to point p
shortest_distance2: the distance2
*/
vector3 CS_ENV_OBJECT::computeShortestDistancePoint( 
	const vector3 &p,
	float &shortest_distance2 
	)
{
	vector3 out_p;
	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_RECTANGLE:
			out_p = computeShortestDistancePoint_Rectangle( p, shortest_distance2 );
			break;
		case CS_ENV_OBJECT_TYPE_POLYGON:
			out_p = bt_computeShortestDistancePoint_Polygon( p, ptArr, shortest_distance2 );
			break;
	}
	return out_p;
}

void CS_ENV_OBJECT::inverseVertexOrder( )
{
	int num = ptArr.size( );
	for ( int i = 0; i < num/2; ++i ) {
		vector3 p = ptArr[ i ];
		vector3 q = ptArr[ num - i - 1 ];
		//
		ptArr[ i ] = q;
		ptArr[ num - i - 1 ] = p;
	}
}

//////////////////////////////////////
bool CS_ENV_OBJECT::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void CS_ENV_OBJECT::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}

void CS_ENV_OBJECT::bo_setNewPosition_XZ( float x, float z )
{
}
void CS_ENV_OBJECT::bo_drawWithHighLight( ) const
{
}

void CS_ENV_OBJECT::bo_drawWithHighLight( float r, float g, float b ) const
{
}

//////////////////////////////////////
void CS_ENV_OBJECT::getAABB( vector3 &AABBmin, vector3 &AABBmax ) const
{
	AABBmin = mAABBmin;
	AABBmax = mAABBmax;
}

void CS_ENV_OBJECT::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	computeAABB( );
	AABBmin = mAABBmin;
	AABBmax = mAABBmax;
}

void CS_ENV_OBJECT::computeAABB( )
{
	if ( ptArr.size( ) == 0 ) {
		mAABBmin = mAABBmax = vector3(0.0, 0.0, 0.0);
		return;
	}
	float r = mCornerThickness_Visual;
	mAABBmin = ptArr[0] - vector3( r, 0.0, r );
	mAABBmax = ptArr[0] + vector3( r, 0.0, r );

	for ( int i = 0; i < ptArr.size( ); ++i ) {
		vector3 p = ptArr[i];
		if ( p.x - r < mAABBmin.x ) {
			mAABBmin.x = p.x - r;
		} else if ( p.x + r > mAABBmax.x ) {
			mAABBmax.x = p.x + r;
		} 

		if ( p.y - r < mAABBmin.y ) {
			mAABBmin.y = p.y - r;
		} else if ( p.y + r > mAABBmax.y ) {
			mAABBmax.y = p.y + r;
		} 

		if ( p.z -r < mAABBmin.z ) {
			mAABBmin.z = p.z - r;
		} else if ( p.z + r > mAABBmax.z ) {
			mAABBmax.z = p.z + r;
		} 
	}
	computeExtendedObj( mCornerThickness_CD );
}

int CS_ENV_OBJECT::getNumPoints( ) const
{
	return ptArr.size( );
}

bool CS_ENV_OBJECT::isPickedObj( ) const
{
	return m_bo_CurPickedEdgeIndex >= 0;
}


bool CS_ENV_OBJECT::isPickedVertex( ) const
{
	return m_bo_CurPickedVertexIndex >= 0;
}

void CS_ENV_OBJECT::deletePickedVertex( )
{
	bool flg = false;
	if ( m_bo_CurPickedVertexIndex < 0 ) return;
	int j = 0;
	int num = ptArr.size( );
	for ( int i = 0; i < num; ++i ) {
		if ( m_bo_CurPickedVertexIndex == i ) {
			flg = true;
			continue;
		}
		ptArr[j] = ptArr[i];
		++j;
	}

	if ( flg ) ptArr.resize( --num );
}

void CS_ENV_OBJECT::computeExtendedObj( float thickness )
{
	ptArr_extendedObj.clear( );

	int numPoints = ptArr.size( );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p0 = ptArr[i];
		vector3 p1 = ptArr[(i+1)%numPoints];
		vector3 q0 = vector3( p0.x, 0.0, p0.z );
		vector3 q1 = vector3( p1.x, 0.0, p1.z );
		//
		vector3 pp = p1 - p0;
		pp.Normalize( );
		vector3 n = pp.cross( vector3(0.0, -1.0, 0.0) );
		float e = n.Normalize( );
		if ( 0.001 > e  ) {
			cout << "error. CS_ENV_OBJECT::computeExtendedObj. too short segment:" << e << endl;
			continue;
		}
		q0 = q0 + thickness*n;
		q1 = q1 + thickness*n;
		CS_LINE_SEGMENT lineSeg;
		lineSeg.p0 = q0;
		lineSeg.p1 = q1;
		lineSeg.n = n;
		ptArr_extendedObj.push_back( lineSeg );
	}
}

//
// mark = 0001b	: positive near to q0
// mark = 0010b	: positive near to q1
// mark = 0100b	: negative near to q0
// mark = 1000b	: negative near to q1
//
// near_q[0] = near_pos_q0
// near_q[1] = near_pos_q1
// near_q[2] = near_neg_q0
// near_q[3] = near_neg_q1
//
// near_p on the line segment (p0, p1)
//
void CS_ENV_OBJECT::computeClosestPointForLineSegment( 
		const vector3 p0, const vector3 p1, 
		vector3 *near_p,
			vector3 *near_q, 
			float *d2
		)
{
	d2[0] = -1.0;
	d2[1] = -1.0;
	d2[2] = -1.0;
	d2[3] = -1.0;

	vector3 p, q;
	int numPoints = ptArr.size( );
	for ( int i = 0; i < numPoints; ++i ) {
		int j = ( i + 1 )%numPoints;
		vector3 q0 = ptArr[ i ];
		vector3 q1 = ptArr[ j ];
		vector3 tmp_p, tmp_q;
		bt_computeNearestPointOfTwoLineSegments( p0, p1, q0, q1, tmp_p, tmp_q );

		vector3 pp = p1 - p0;
		float d2_pq = tmp_p.distance2( tmp_q );
		float Lp = pp.Normalize( );
		float d_p0 = (tmp_p - p0).dot( pp );

		float ratio = d_p0/Lp;
		float y = - (pp.cross( tmp_q - p0 )).y;
		if ( y > 0.0 ) {
			if ( ratio < 0.5 ) {
				if ( d2[0] > d2_pq || d2[0] < 0.0 ) {
					d2[0] = d2_pq;
					near_p[0] = tmp_p;

					near_q[0] = tmp_q;
				}
			} else {
				if ( d2[1] > d2_pq || d2[1] < 0.0 ) {
					d2[1] = d2_pq;
					near_p[1] = tmp_p;
					near_q[1] = tmp_q;
				}
			}

		} else {
			if ( ratio < 0.5 ) {
				if ( d2[2] > d2_pq || d2[2] < 0.0 ) {
					d2[2] = d2_pq;
					near_p[2] = tmp_p;
					near_q[2] = tmp_q;
				}
			} else {
				if ( d2[3] > d2_pq || d2[3] < 0.0 ) {
					d2[3] = d2_pq;
					near_p[3] = tmp_p;
					near_q[3] = tmp_q;
				}
			}
		}

	}
}

void CS_ENV_OBJECT::detectProximity_Edges( const vector3 &p, float r, int *edgeArr, int &numEdges )
{
	numEdges = 0;
	float r2 = r*r;
	int numPoints = ptArr.size( );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 q0 = ptArr[i];
		vector3 q1 = ptArr[(i+1)%numPoints];

		vector3 near_p = bt_computeNearestPoint_LineSegment( p, q0, q1 );

		float d2 = near_p.distance2( p );
		if ( r2 >= d2 ) {
			if ( numEdges >= MAX_NUM_EDGES ) {
			} else {
				edgeArr[ numEdges ] = i;
				++numEdges;
			}
		}
	}
}
