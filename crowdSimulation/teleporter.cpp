#include "teleporter.h"

TELEPORTER::TELEPORTER( )
{

}

bool TELEPORTER::reachTeleporter( const CS_AGENT *g ) const
{
	bool flg = false;
	float R2 = mSourceRadius*mSourceRadius;
	if ( g->p.distance2( mSourcePoint ) > R2 ) return flg;
	return true;
}

// true: register successfully
bool TELEPORTER::registerAgent( CS_AGENT *g )
{
	bool flg = false;
	if ( reachTeleporter( g ) == false ) return flg;

	mAgentQueue.push_back( g );
	return true;
	}

CS_AGENT *TELEPORTER::teleportAgent( vector3 &p )
{
	CS_AGENT *g = 0;
	if ( mAgentQueue.size( ) == 0 ) return g;
	g = mAgentQueue.back( );
	mAgentQueue.pop_back( );
	p = mTargetPoint;
	return g;
}

void TELEPORTER::clear( )
{
}

void TELEPORTER::setTemporaryPoint( const vector3 & p )
{
}

void TELEPORTER::addPoint( float x, float y, float z )
{
}

void TELEPORTER::addPoint( const vector3 & p )
{
}

bool TELEPORTER::canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 )
{
	bool flg = false;
	return false;
}

void TELEPORTER::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
}

	void TELEPORTER::computeAABB( )
	{
	}


	bool TELEPORTER::bo_isPicked_XZPlane( float x, float z )
	{
		return false;
	}
	void TELEPORTER::bo_setNewPosition_XZ( float x, float z )
	{
	}
	void TELEPORTER::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
	{
	}
