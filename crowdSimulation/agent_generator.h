//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __AGENT_GENRATOR_H_
#define __AGENT_GENRATOR_H_

#include "../headers.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"

#define CS_AGENT_GEN_OBJECT_TYPE_NONE	0
#define CS_AGENT_GEN_OBJECT_TYPE_CIRCLE	1

#define CS_AGENT_GEN_PATH_TYPE_CLOSEST	-1
#define CS_AGENT_GEN_PATH_TYPE_RANDOM	-2

class CS_AGENT_GENERATOR : public BASE_OBJ {
protected:
	//

	//
	vector3 mPosition;
	mutable double mClock;
	double mMaxGenerationTime;
	double mCurGenerationTime;
	int mGenerationAgentState;
	double mGenerationAgentState_TimeForCycle;
	double mGenerationAgentState_MaxTimeForCycle;
	int mMaxNumOfGeneratedAgents;
	mutable int mCurNumOfGeneratedAgents;
	float mRadius;
	double mNumAgentPerSecond;
	double _mTimeRequireForOneAgent; // derived value
	float mAgentSpeedMin, mAgentSpeedMax;
	float mAgentRadiusMin, mAgentRadiusMax;
	// mAgentPathIndex
	// dedicated path : 0, 1, 2, ...
	// closest: -1
	// random: -2
	int mAgentPathIndex;	
public:
	CS_AGENT_GENERATOR( );

	virtual void setObjType( int objType );
	virtual void setObjState( int state );
	virtual bool canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 );
	virtual void resetInternalState( );
	virtual void setGenerationFrequency( float num );
	virtual void update( float time_step );
	virtual void setGenerationRegion( const vector3 &p, float radius );
	virtual bool generatePosition( vector3 &p ) const;
	virtual int getAssignedAgentPathID( ) const;
	virtual void getGenerationPosition( vector3 &p, float &r ) const;
	virtual vector3 getPosition( ) const;
	virtual void reset( );
	virtual void setMaxGenerationTime( double t );
	virtual void set( const CS_AGENT_GENERATOR &g );
	virtual void setPathIndex( int index );
	virtual void setAgentSpeedRange( float minv, float maxv );
	virtual void setAgentRadiusRange( float minv, float maxv );

	virtual float getAgentSpeed( ) const;
	virtual float getAgentRadius( ) const;

	virtual void printf_Data( ) const;
	//
	float getRadius( ) const;
	void getAgentSpeedRange( float &minSpeed, float &maxSpeed ) const;
	void getAgentRadiusRange( float &minRadius, float &maxRadius ) const;
	int	getPathType( ) const;
	float getNumAgentPerSecond( ) const;
	float getMaxGenerationTime( ) const;
	int getMaxNumOfGeneratedAgents( ) const;
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	virtual void setObjPosition_XZ( const vector3 &p );
	virtual void movePickedItem_XZ( const vector3 &p );
	virtual void setTemporaryPoint( const vector3 &p );
	virtual void clear( );
	virtual void addPoint( const vector3 &p );
	//
	void save( ofstream *fp ) const;
	//
	void setMaxGenerationTime( float v );
	void setMaxAgentNumber( int v );
	void setRadius( float v );

	void setAgentPathIndex( int v );
	void setAgentSpeedMin( float v );
	void setAgentSpeedMax( float v );
	
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	void computeAABB( );

};


//extern void read_Generator_DataFile( const char *fileName );
//extern CS_AGENT_GENERATOR **getAgentGenerator( int &num );

#endif