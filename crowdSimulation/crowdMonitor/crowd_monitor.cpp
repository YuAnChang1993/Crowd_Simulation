#include "crowd_monitor.h"


CROWD_MONITOR::CROWD_MONITOR( )
{
	mMaxCycles = 5;
	mNumCycles = 0;
}

void CROWD_MONITOR::setPosition( const vector3 &p )
{
	mPosition = p;
}

void CROWD_MONITOR::set( const CROWD_MONITOR m)
{
	mPosition = m.mPosition;
	mRadius = m.mRadius;
}

void CROWD_MONITOR::set( const vector3 &p, float r )
{
	mPosition = p;
	mRadius = r;
}

void CROWD_MONITOR::getInfo( vector3 &p, float &r )
{
	p = mPosition;
	r = mRadius;
}

vector3 CROWD_MONITOR::getPosition( ) const
{
	return mPosition;
}

int CROWD_MONITOR::hasNoRecord( ) const
{
	if ( mNumOfRecords == 0)  return true;
	return false;
}

int CROWD_MONITOR::isProblemSignificant( )
{
	++mNumCycles;
	if ( mNumCycles < mMaxCycles ) {
		return 0;
	}	
	mNumCycles = 0;
	if ( mNumOfRecords < 1 ) return -1;
	return 1;
}

void CROWD_MONITOR::resetHistory( )
{
	if ( mNumCycles != 0 ) return;

	mNumOfRecords = 0;
	mAccuPoint_Obstacle.zero( );
	mPotentialObstacle.zero( );
}

void CROWD_MONITOR::addHistoryRecord( const vector3 &p, float w )
{
	mAccuPoint_Obstacle += p;
	mNumOfRecords += w;
}

void CROWD_MONITOR::analyze( )
{
	mPotentialObstacle = mAccuPoint_Obstacle/mNumOfRecords;
}

float CROWD_MONITOR::getReportValue( ) const
{
	return mNumOfRecords;
}

void CROWD_MONITOR::clear( )
{
}

void CROWD_MONITOR::checkAgent( CS_AGENT *g )
{
}

void CROWD_MONITOR::addPoint( const vector3 &p )
{
}

void CROWD_MONITOR::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
}

void CROWD_MONITOR::computeAABB( )
{
}

bool CROWD_MONITOR::canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 )
{
	bool flg = false;
	return flg;
}
