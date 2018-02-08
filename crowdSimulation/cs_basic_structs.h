//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/31
//
#ifndef __CS_BASIC_STRUCTS_H_
#define __CS_BASIC_STRUCTS_H_

#include <vector>
#include "../primitives/vector3d.h"
#include "base_obj.h"

struct CS_EDGE{
	int first_point;
	int second_point;
	float weight;
};

struct CS_NODE{
	int neighborEdgeID1;
	int neighborEdgeID2;
	vector3 position;
	vector3 bisector_out;
	int nodeType;
};

class CS_LINE_SEGMENT {
public:
	vector3 p0, p1;
	vector3 n;
	CS_LINE_SEGMENT() {
	}
};

class CS_PATH_POINT : public BASE_OBJ
{
public:
	vector3 p;
	float r;
public:
	CS_PATH_POINT( )
	{
		r = 0.0;
	}
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
};

class CS_PATH_AFFECT_DATA
{
public:
	CS_PATH_AFFECT_DATA( ) { }
	vector3 *affectingSelfPointArr[4]; // affecting points
	vector3 *affectingPointArr[4]; // affecting points
	float *affectingPointD2[4]; // square of
	float *maxProjectedDistance;
	float *trendOfProjectedDistance;
};

class CS_PATH : public BASE_OBJ {
public:
	CS_PATH( );
	~CS_PATH( );
	//
	
	int numPoints;
	int maxPoints;
	float r; // radius
	vector3 pathColor;
	CS_PATH_POINT *ptArr;
	int mEdgeLoadIndex;
	float *mEdgeLoadArr[2];
	float *mEdgeLoadDiffArr;
	float *mExpectedPathWidth;
	//
	vector3 *affectingSelfPointArr[4]; // affecting points
	vector3 *affectingPointArr[4]; // affecting points
	float *affectingPointD2[4]; // square of distance 
	float *maxProjectedDistance;
	float *trendOfProjectedDistance;
	float *projectedDistanceRecords[5];
	int mRecordIndexOfProjectedDistance;
	//
	vector3 *mWorsePositionArr;
	float *mWorseValueArr;
	//
	void set( const CS_PATH &path );
	void clear( );
	void addPoint( const vector3 &p, float r );
	void addPoint( float x, float y, float z );
	void setRadius( float r );
	//
	void computeExpectedPathWidth( );
	float computeExpectedPathWidth( int edgeIndex );
	void resetWorsePosition( int node_index, float time_step  );
	bool setWorseValue( int node_index, float v, const vector3 &p );
	vector3 getWorsePosition( int node_index, float &worseValue ) const;
	//
	float *getExpectedPathWidth( ) const;


	void resetNearestAffectingPointsForEdge( );
	void setNearestAffectingPointsForEdge( int node_index, vector3 *near_p, vector3 *near_q, float *d2 );
	void getAffectingInfo( CS_PATH_AFFECT_DATA &g ) const;
	//
	void setPickedPathNodeRadius( float r );
	float getPickedPathNodeRadius( ) const;
	//
	void setPathNodePosition( const vector3 &p, int node_index );
	vector3 getFirstPointPosition( ) const;
	vector3 getPathNodePosition( int node_index ) const;
	vector3 getEdgeDirectionBasedOnPathNode( int node_index ) const;
	BASE_OBJ *pickPathPoint_XZPlane( float x, float z ) const;
	void computeClosestPointsOnTwoEdges( const vector3 &p, int node_index, vector3 &q0, vector3 &q1 );
	//
	bool canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 );
	void moveObjPosition_XZ( const vector3 &p );
	void moveSelectedVertexPosition_XZ( const vector3 &p );
	//
	CS_PATH_POINT *getVertArr( ) const;
	int getNumPoints( ) const;
	//float *getRadiusArr( ) const;
	bool isPickedObj( ) const;
	bool isPickedVertex( ) const;
	void deletePickedVertex( );
	//
	void showInfo( ) const;

	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	void computeAABB( );
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	//
	void printf_Data( ) const;
	//
	void getPathPoints( int edgeIndex, vector3 &p0, vector3 &p1 ) const;
	//
	void resetLoad( );
	void addLoadRecord( int edgeIndex, float w );
	void addMaxProjectedDistance( int edgeIndex, float d_to_next_pathPoint, float proj_d );
	//
	void drawAffectintPoints( ) const;
	void draw( ) const;
};

extern void mergeAABB( vector3 &AABBmin, vector3 &AABBmax, const vector3 &aabbMin, const vector3 &aabbMax );
extern bool insideAABB_lineSegment_XZ( const vector3 &mAABBmin, const vector3 &mAABBmax, const vector3 &q0, const vector3 &q1 );
#endif