//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __AGENT_H_
#define __AGENT_H_

#include "../headers.h"
#include <string>
#include "../openGLbasic/opengl_stuff.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"
#include "base_obj.h"
//
#define CS_AGENT_STRATEGY_NONE		0
#define CS_AGENT_STRATEGY_AGGRESIVE	1

#define CS_AGENT_SIM_TYPE_NONE				0
#define CS_AGENT_SIM_TYPE_POINT_TO_POINT	1
#define CS_AGENT_SIM_TYPE_ALONG_PATH		2


// unit: 1.0 = 1 meter
class CS_AGENT : public BASE_OBJ {
public:
	
	//
	int group_id;
	//
	bool flgHit;
	vector3 hitNormal;
	vector3 preferred_dir;
	//
	bool mInvalidPosition;

	bool flgReachedDestination;
	//
	bool flgInitPosition;
	int simType;
	vector3 p;
	vector3 v; // velocity
	vector3 destP; // destination position
	vector3 sv; // step velocity, that's v*time_step
	//
	vector3 desired_v;
	//
	float r; // radius
	float destR; // destination radius
	float destR2; // destR*destR
	float cur_desired_speed; // current desired speed
	float desired_speed; // desired speed
	float radial_obstacle[8];
	int strategy;
	//
	vector3 perceived_v;	// the other agents can observe this velocity
	float perceived_s;	// the other agents can observe this velocity
	float curAccumulatedResponseTime;
	float maxResponseTime;
	//
	float max_slow_down_counter;
	float slow_down_counter;
	//
	vector3 prev_p;
	vector3 prev_v;

	vector3 predict_p;
	//
	vector3 mFeasibleDirection;
	//
	vector3 init_p;
	vector3 init_v;
	//
	double travelTime;
	double travelLength;
	//
	int curPathEdgeIndex;
	int curPathPointIndex;
	int destPathPointIndex;
	int maxNumOfPathPoints;
	int mNumOfPathPoints;
	CS_PATH_POINT *mPathPointArr;
	//
	const CS_PATH *external_path;

	//------------------------
	//BEGIN for spatial scheme
	CS_AGENT *_next;
	CS_AGENT *_prev;
	int _key;
	//------------------------
	//------------------------
protected:
	bool computeDesireVelocity_AlongEdge(const vector3 &q0, const vector3 &q1, float time_step );

	bool computeDesireVelocity_AlongPath_ParallelPathEdge_CornerAware( 
		int numPoints,
		CS_PATH_POINT *pointArr,
		float time_step
	);

	bool computeDesireVelocity_AlongPath_ParallelPathEdge( 
		int numPoints,
		CS_PATH_POINT *pointArr,
		float time_step
		);
	bool computeDesireVelocity_AlongPath_VoronoiRegions( 
		int numPoints,
		CS_PATH_POINT *pointArr,
		float time_step
		);
	bool computeDesireVelocity_AlongPath_PoinToPoint( 
		int numPoints,
		CS_PATH_POINT *pointArr,
		float time_step );
	bool computeDesireVelocity_PointToPoint( 
		int numPoints,
		CS_PATH_POINT *pointArr,
		float time_step);
	bool CS_AGENT::computeDesireVelocity_AlongPath( 
		int numPoints,
		CS_PATH_POINT *pointArr, 
		int edgeIndex,
		float time_step );
	//
	void prepareBasedonSimulationType(
		int numOfPoints,
		CS_PATH_POINT *pathPointsArr);
public:
	CS_AGENT( );
	void setSimulationType( int type );
	//
	void setRadius( float r );
	//
	void reset( );
	void save( );
	void setPosition( const vector3 &p );
	void setPosition( float x, float y, float z );
	void setDestinationPosition( float x, float y, float z );
	void computeDesireVelocity( float time_step );
	void pre_update( );
	void post_update( float time_step );

	float computeDistance2_XZ( const vector3 & p ) const; //square

	void updatePosition( float time_step );
	void chooseStrategy( );
	//
	bool isAtDestination( ) const;
	//
	virtual float getRadius2( ) const;

	//
	// dest_p: destination position
	// dest_r: destination ratius
	void setDestinationPosition( const vector3 &dest_p, float dest_r );
	void setDestinationPosition( float x, float y, float z, float dest_r );

	// for simulation
	void prepareBasedonSimulationType( );
	//
	// for path settting
	void begin_pathPoints( );
	void addPathPoint( const vector3 &p, float path_radius );
	void end_pathPoints( );
	void setPath( const CS_PATH &path );
	//
	void setExternalPath( const CS_PATH *path );
	//
	void addPathLoad( );
	void addProjectedDistanceToPath( );
	// for drawing
	void draw( const vector3 agent_color = vector3(0.0, 0.0, 1.0), bool used_own_color = true ) const;
	void draw_debug( const vector3 agent_color = vector3(0.0, 0.0, 1.0), bool used_own_color = true ) const;

	//void printf( ) const;
	void printf_Data( ) const;
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
};

class CS_AGENT_INFORMATION
{
public:
	CS_AGENT_INFORMATION( ) {
	}
	void clear( ) {
		leader.clear( );
		neighbor.clear( );

	}
	void draw( ) const;
	vector<CS_AGENT *> leader;
	vector<CS_AGENT *> neighbor;
};

extern bool isAgentsColliding( const CS_AGENT *agi, const CS_AGENT *agj );
extern float moveAgentsApart( CS_AGENT *agi, CS_AGENT *agj ); // separation distance
extern float moveAgentsApart_VelocityBased( CS_AGENT *agi, CS_AGENT *agj ); // separation distance
extern bool adjustAgentVelocity_CollisionAvoidance_Overlapped( CS_AGENT *agi, CS_AGENT *agj );
extern bool adjustAgentVelocity_CollisionAvoidance( CS_AGENT *agi, CS_AGENT *agj );
extern bool adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased( CS_AGENT *agi, CS_AGENT *agj );
extern bool adjustAgentVelocity_CollisionAvoidance_VelocityBased( CS_AGENT *agi, CS_AGENT *agj );
extern bool adjustAgentVelocity_CA_Overlapped_VelocityBased_Perceived( CS_AGENT *agi, CS_AGENT *agj );
extern bool adjustAgentVelocity_CA_VelocityBased_Perceived( CS_AGENT *agi, CS_AGENT *agj );

#endif