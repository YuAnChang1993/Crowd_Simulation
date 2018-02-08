//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CS_ENVIRONMENT_OBJ_H_
#define __CS_ENVIRONMENT_OBJ_H_

#include "../headers.h"
#include <string>
#include <vector>
#include "../openGLbasic/opengl_stuff.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"
#include "base_obj.h"
/*
Rectangle: the lower left and upper right vertices
Polygon : vertices arranged in counter-clockwise
*/
#define CS_ENV_OBJECT_TYPE_NONE			0
#define CS_ENV_OBJECT_TYPE_RECTANGLE	1
#define CS_ENV_OBJECT_TYPE_CIRCLE		2
#define CS_ENV_OBJECT_TYPE_POLYGON		3

#define CS_ENV_OBJECT_GUIDE_DIRECTION_NONE	0
#define CS_ENV_OBJECT_GUIDE_DIRECTION_CCW	1
#define CS_ENV_OBJECT_GUIDE_DIRECTION_CW	2

using namespace std;

#define CS_ENV_OBJ_ACTION_TYPE_NONE		0
#define CS_ENV_OBJ_ACTION_TYPE_ENABLE	1
#define CS_ENV_OBJ_ACTION_TYPE_DISABLE	2

class CS_ENV_OBJ_ACTION 
{
public:
	CS_ENV_OBJ_ACTION( );
	int actionType;
	float time_duration;
};

class CS_ENV_OBJ_ACTION_DATA {
private:
	int mActionIndex;
	float mCurTime;
public:
	
	vector<CS_ENV_OBJ_ACTION> mActionArr; 
	
public:
	CS_ENV_OBJ_ACTION_DATA( );
	void addAction( int actionType, float time_duration );
	void update( float time_step );
	bool getEnabledStatus( ) const;
};

class CS_ENV_OBJECT : public BASE_OBJ {
protected:
	//
	bool mFlgEnabledStatusAsAnObstacle;
	CS_ENV_OBJ_ACTION_DATA mActionData;
	bool mFlgUseDataAction; 	
	//
	void draw_Polygon( ) const;
	void draw_Rect( ) const;
	vector3 mTmpPointForDefiningNewObj;
	void convertRectangleIntoPolygon( );
	//
	void computeExtendedObj( float thickness );
public:
	CS_ENV_OBJECT( );
	int textureID;
	//
	int guideDirection;
	float mCornerThickness_Visual;
	float mCornerThickness_CD;

	//
	// For rectangle: p[0], lower left corner. p[1] upper right corner
	//
	vector<vector3> ptArr;
	//
	vector<CS_LINE_SEGMENT> ptArr_extendedObj;
	//
	vector3 color;
	//
	bool getEnabledStatusAsAnObstacle( ) const;
	void enableActionData( bool flg );
	bool getFlg_UseActionData( ) const;
	//
	void addWorkingCycle( float enabled_duration, float disabled_duration );
	//
	void setObjState( int objState );
	void moveObjPosition_XZ( const vector3 &p );
	void moveSelectedVertexPosition_XZ( const vector3 &p );
	void movePickedItem_XZ( const vector3 &p );
	void setTemporaryPoint( const vector3 & p );
	//
	void addPoint( float x, float y, float z );
	void addPoint( const vector3 & p );
	void setObjType( int objType );
	bool isContained( const vector3 & q, float r ) const;
	bool isContained_Polygon( const vector3 & q0, const vector3 & q1, float r ) const;
	bool isContained_Rect( const vector3 & q0, const vector3 & q1, float r ) const;
	bool isContained( const vector3 & q0, const vector3 & q1, float r ) const;

	//
	void detectProximity_Edges( const vector3 &p, float r, int *edgeArr, int &numEdges );
	//
	vector3 computeSeparationVector_Circle_Polygon( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle_Polygon_Edge( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction, int edgeID ) const;

	vector3 computeSeparationVector_Circle_Rect( const vector3 & q0, const vector3 & q1, float r, float &contact_time, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle_Edge( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, float &contact_time, vector3 &feasible_direction, int edgeID ) const;

	vector3 computeNormalDirection_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flgHit, vector3 &prefer_vec ) const;
	//
	vector3 computeShortestDistancePoint_Rectangle( 
	const vector3 &p,
	float &shortest_distance2 
	);
	vector3 computeShortestDistancePoint( 
	const vector3 &p,
	float &shortest_distance2 
	);
	//
	void computeClosestPointForLineSegment( 
		const vector3 q0, const vector3 q1, 
		vector3 *near_p,
			vector3 *near_q, 
			float *d2Arr
		);
	//
	bool canBePicked_Polygon( const vector3 &p, float hitDistance, float &shortest_distance2 );
	bool canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 );
	//
	void inverseVertexOrder( );
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	void getAABB( vector3 &AABBmin, vector3 &AABBmax ) const;
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	void computeAABB( );
	int getNumPoints( ) const;
	bool isPickedObj( ) const;
	bool isPickedVertex( ) const;
	void deletePickedVertex( );
	//
	void clear( );
	void set( const CS_ENV_OBJECT &obj );
	//
	void update( float time_step );
	//
	void draw( ) const;
	void draw( float zLayer ) const;
	void draw_for_cd( float zLayer, float thickness ) const;
};

#endif