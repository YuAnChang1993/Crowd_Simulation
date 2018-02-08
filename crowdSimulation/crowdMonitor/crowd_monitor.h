#ifndef __CROWD_MONITOR_H__
#define __CROWD_MONITOR_H__

#include "../agent.h"
class CROWD_MONITOR : public BASE_OBJ
{
protected:
	vector3 mPosition;
	float mRadius;
	//
	double mNumOfRecords;
	vector3 mAccuPoint_Obstacle;
	vector3 mPotentialObstacle;
	int mMaxCycles;
	int mNumCycles;
public:
	CROWD_MONITOR( );
	float getReportValue( ) const;
	void setPosition( const vector3 &p );
	void set( const vector3 &p, float r );
	void addHistoryRecord( const vector3 &p, float w );
	void resetHistory( );
	void analyze( );
	int hasNoRecord( ) const;
	int isProblemSignificant( );
	void set( const CROWD_MONITOR m);
	vector3 getPosition( ) const;
	void getInfo( vector3 &p, float &r );
	void checkAgent( CS_AGENT *g );
	void clear( );
	void computeAABB( );
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	void addPoint( const vector3 &p );
	bool canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 );
	//----------------------------------------------------
	virtual bool bo_isPicked_XZPlane( float x, float z ) { return false; }
	virtual void bo_setNewPosition_XZ( float x, float z ){ }
	virtual void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 ) { }
	virtual void bo_drawWithHighLight( ) const { }
	virtual void bo_drawWithHighLight( float r, float g, float b ) const { }
	//----------------------------------------------------
	void draw( ) const;
};

#endif