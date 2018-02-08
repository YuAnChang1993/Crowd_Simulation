//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "agent_generator.h"
#include "../primitives/basic_tools.h"
#include "cs_parameters.h"

using namespace std;
CS_AGENT_GENERATOR::CS_AGENT_GENERATOR( )
{
	resetInternalState( );
}

void CS_AGENT_GENERATOR::setMaxAgentNumber( int v )
{
	mMaxNumOfGeneratedAgents = v;
}

void CS_AGENT_GENERATOR::setMaxGenerationTime( float v )
{
	mMaxGenerationTime = v;
}

void CS_AGENT_GENERATOR::setAgentPathIndex( int v )
{
	mAgentPathIndex = v;
}

void CS_AGENT_GENERATOR::setRadius( float v )
{
	mRadius = v;
}

void CS_AGENT_GENERATOR::setAgentSpeedMin( float v )
{
	mAgentSpeedMin = v;
}

void CS_AGENT_GENERATOR::setAgentSpeedMax( float v )
{
	mAgentSpeedMax = v;
}

void CS_AGENT_GENERATOR::setAgentRadiusRange( float minv, float maxv )
{
	mAgentRadiusMin = minv;
	mAgentRadiusMax = maxv;

}

void CS_AGENT_GENERATOR::set( const CS_AGENT_GENERATOR &g )
{
	mFlgUsed = true;
	mFlgActivated = true;

	mMaxGenerationTime = g.mMaxGenerationTime; 
	mCurGenerationTime = g.mCurGenerationTime;
	mMaxNumOfGeneratedAgents = g.mMaxNumOfGeneratedAgents;
	mCurNumOfGeneratedAgents = g.mCurNumOfGeneratedAgents;

	mClock = g.mClock;
	mObjID = g.mObjID;
	mPosition = g.mPosition;
	mRadius = g.mRadius;
	mNumAgentPerSecond = g.mNumAgentPerSecond;
	mAgentPathIndex = g.mAgentPathIndex;
	mAgentSpeedMin = g.mAgentSpeedMin;
	mAgentSpeedMax = g.mAgentSpeedMax;
	mAgentRadiusMin = g.mAgentRadiusMin;
	mAgentRadiusMax = g.mAgentRadiusMax;

	//
	setGenerationFrequency( mNumAgentPerSecond );
	//
	
	//
	m_bo_ObjType = CS_AGENT_GEN_OBJECT_TYPE_CIRCLE;
	m_bo_ObjState = CS_OBJECT_STATE_NONE;
	//
	mObjID = 0;
}

void CS_AGENT_GENERATOR::setObjType( int objType )
{
	this->m_bo_ObjType = objType;
}

void CS_AGENT_GENERATOR::setObjState( int state )
{
	this->m_bo_ObjState = state;
}

void CS_AGENT_GENERATOR::reset( )
{
	mCurGenerationTime = 0.0;
	mClock = 0.0;
	mCurNumOfGeneratedAgents = 0;
	mGenerationAgentState = 0;
	mGenerationAgentState_MaxTimeForCycle = 0.0;
	mGenerationAgentState_TimeForCycle = 0.0;

}

void CS_AGENT_GENERATOR::resetInternalState( )
{
	mCurNumOfGeneratedAgents = 0;
	mMaxNumOfGeneratedAgents = 1;
	mMaxGenerationTime = -1.0; // infinite time
	mCurGenerationTime = 0.0;
	mClock = 0.0;
	mRadius = 10.0;
	mNumAgentPerSecond = 2.0;
	mAgentPathIndex = CS_AGENT_GEN_PATH_TYPE_CLOSEST;
	mPosition.zero( );
	mAgentSpeedMin = 1.0;
	mAgentSpeedMax = 1.0;
	//
	mAgentRadiusMin = csp_Generator_AgentRadiusMin;
	mAgentRadiusMax = csp_Generator_AgentRadiusMax;


	setGenerationFrequency( mNumAgentPerSecond );
	//
	mGenerationAgentState = 0;
	//mGenerationAgentState_MaxTimeForCycle = 240.0;
	mGenerationAgentState_MaxTimeForCycle = 0.0;
	mGenerationAgentState_TimeForCycle = 0.0;
	//
}

bool CS_AGENT_GENERATOR::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void CS_AGENT_GENERATOR::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}

void CS_AGENT_GENERATOR::bo_setNewPosition_XZ( float x, float z )
{
}
void CS_AGENT_GENERATOR::bo_drawWithHighLight( ) const
{
}

void CS_AGENT_GENERATOR::bo_drawWithHighLight( float r, float g, float b ) const
{
}




void CS_AGENT_GENERATOR::setAgentSpeedRange( float minv, float maxv )
{
	mAgentSpeedMin = minv;
	mAgentSpeedMax = maxv;
}

float CS_AGENT_GENERATOR::getAgentSpeed( ) const
{
	return bt_random_value_interval( mAgentSpeedMin, mAgentSpeedMax );
}

float CS_AGENT_GENERATOR::getAgentRadius( ) const
{
	return bt_random_value_interval( mAgentRadiusMin, mAgentRadiusMax );
}


void CS_AGENT_GENERATOR::setPathIndex( int index )
{
	mAgentPathIndex = index;
}

void CS_AGENT_GENERATOR::setMaxGenerationTime( double t )
{
	mMaxGenerationTime = t;
}

void CS_AGENT_GENERATOR::setGenerationFrequency( float num )
{
	mNumAgentPerSecond = num;
	_mTimeRequireForOneAgent = 1.0/mNumAgentPerSecond;
}

void CS_AGENT_GENERATOR::update( float time_step )
{
	mClock += time_step;
	mCurGenerationTime += time_step;
	//
	mGenerationAgentState_TimeForCycle += time_step;
	if ( 
		mGenerationAgentState_TimeForCycle >= mGenerationAgentState_MaxTimeForCycle ) {
		mGenerationAgentState_TimeForCycle -= mGenerationAgentState_MaxTimeForCycle;
		//mGenerationAgentState = ( 1 + mGenerationAgentState )%2;
		if ( mGenerationAgentState_MaxTimeForCycle > 0.0 ) mClock = 0.0;

	}
}

void CS_AGENT_GENERATOR::setGenerationRegion( const vector3 &p, float radius )
{
	mPosition = p;
	mRadius = radius;
}



vector3 CS_AGENT_GENERATOR::getPosition( ) const
{
	return mPosition;
}



void CS_AGENT_GENERATOR::getGenerationPosition( vector3 &p, float &r ) const
{
	p = mPosition;
	r = mRadius;
}



int CS_AGENT_GENERATOR::getAssignedAgentPathID( ) const
{
	return mAgentPathIndex;
}

//
// mMaxNumOfGeneratedAgents < 0: can generate unlimited number of agents
//
bool CS_AGENT_GENERATOR::generatePosition( vector3 &p ) const
{
	if ( mMaxGenerationTime >= 0.0 && mCurGenerationTime >= mMaxGenerationTime )
	{
		return false;
	}

	if ( 
		mGenerationAgentState_MaxTimeForCycle > 0.0
		&&
		(mGenerationAgentState_TimeForCycle*3.0
		> 
		mGenerationAgentState_MaxTimeForCycle) )
	{
		return false;
	}

	float k = 1.0;

	switch ( mGenerationAgentState ) {
		case 0:
		k = 1.0;
		break;
		case 1:
		k = 20.0;
		break;
	}

	if ( k*_mTimeRequireForOneAgent > mClock ) {
		return false;
	}
	if ( mMaxNumOfGeneratedAgents >= 0 
		&& 
		(mCurNumOfGeneratedAgents >= mMaxNumOfGeneratedAgents) ) return false;

	p = bt_random_sample_circle_XZPlane( mPosition, mRadius );
	

	mClock -= _mTimeRequireForOneAgent*k;

	


	++mCurNumOfGeneratedAgents;
	return true;
}

void CS_AGENT_GENERATOR::printf_Data( ) const
{
	cout << "BEGIN CS_AGENT_GENERATOR::printf_Data( )" << endl;
	
	cout << "mAgentPathIndex\t" << mAgentPathIndex << endl;
	cout << "Position:" << mPosition.x << "\t" << mPosition.y << "\t" << mPosition.z << endl;
	cout << "mMaxGenerationTime\t" << mMaxGenerationTime << endl;
	cout << "mCurGenerationTime\t" << mCurGenerationTime << endl;
	cout << "mRadius\t" << mRadius << endl;
	cout << "mNumAgentPerSecond\t" << mNumAgentPerSecond << endl;
	cout << "mTimeRequireForOneAgent\t" << _mTimeRequireForOneAgent << endl;
	
	cout << "END CS_AGENT_GENERATOR::printf_Data( )" << endl;

}

bool CS_AGENT_GENERATOR::canBePicked( const vector3 &p, float hitDistance, float &shortest_distance2 )
{
	bool flg = false;
	float hitDistance2 = hitDistance*hitDistance;
	float d2 = p.distance2( mPosition );
	m_bo_SelectedPoint = p;
	if ( shortest_distance2 < 0 || shortest_distance2 > d2 ) {
			shortest_distance2 = d2;
			flg = true;
	}

	return flg;
}

void CS_AGENT_GENERATOR::setObjPosition_XZ( const vector3 &p )
{
	mPosition = p;

}

void CS_AGENT_GENERATOR::addPoint( const vector3 &p )
{
	mPosition = p;
}

void CS_AGENT_GENERATOR::clear( )
{
	m_bo_ObjState = CS_OBJECT_STATE_NONE;
}

void CS_AGENT_GENERATOR::movePickedItem_XZ( const vector3 &p)
{
	vector3 translated_vector = p - m_bo_SelectedPoint;
	mPosition += translated_vector;
	m_bo_SelectedPoint = p;
}

void CS_AGENT_GENERATOR::setTemporaryPoint( const vector3 &p )
{
	mPosition = p;
}

float CS_AGENT_GENERATOR::getRadius( ) const
{
	return mRadius;
}

void CS_AGENT_GENERATOR::getAgentRadiusRange( float &minRadius, float &maxRadius ) const
{
	minRadius = mAgentRadiusMin;
	maxRadius = mAgentRadiusMax;
}

	void CS_AGENT_GENERATOR::getAgentSpeedRange( float &minSpeed, float &maxSpeed ) const{
		minSpeed = mAgentSpeedMin;
		maxSpeed = mAgentSpeedMax;

}

	int	CS_AGENT_GENERATOR::getPathType( ) const{
		return mAgentPathIndex;
}

	float CS_AGENT_GENERATOR::getNumAgentPerSecond( ) const{
		return mNumAgentPerSecond;
}

	float CS_AGENT_GENERATOR::getMaxGenerationTime( ) const
	{
		return mMaxGenerationTime;
	}

	int CS_AGENT_GENERATOR::getMaxNumOfGeneratedAgents( ) const
	{
		return mMaxNumOfGeneratedAgents;
	}

	void CS_AGENT_GENERATOR::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
	{
		computeAABB( );
		AABBmin = mAABBmin;
		AABBmax = mAABBmax;

	}

	void CS_AGENT_GENERATOR::computeAABB( )
	{
		mAABBmin = mPosition - vector3( mRadius, 0.0, mRadius );
		mAABBmax = mPosition + vector3( mRadius, 0.0, mRadius );
	}
