//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/10/12
//
#ifndef __TELEPORTER_H_
#define __TELEPORTER_H_

#include "../headers.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"
#include "agent.h"
//
//move from source to target
//
class TELEPORTER : public BASE_OBJ
{
protected:
	vector3 mSourcePoint;
	float mSourceRadius;
	//
	vector3 mTargetPoint;
	float mTargetRadius;

	vector<CS_AGENT *> mAgentQueue;
public:
	TELEPORTER( );
	bool reachTeleporter( const CS_AGENT *g ) const;
	//
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	void computeAABB( );
	void clear( );
	//
	bool canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 );
	//
	void addPoint( const vector3 & p );
	void setTemporaryPoint( const vector3 & p );
	void addPoint( float x, float y, float z );
	//
	bool registerAgent( CS_AGENT *g );
	CS_AGENT *teleportAgent( vector3 &p );
	//
	void draw( ) const;
	//
		bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
};

#endif