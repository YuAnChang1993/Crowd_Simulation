#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "agent_generator_manager.h"
#include "agent_generator_data_format.h"
#include "./cs_parameters.h"

AGENT_GENERATOR_MANAGER *AGENT_GENERATOR_MANAGER::__instance = 0;

AGENT_GENERATOR_MANAGER::AGENT_GENERATOR_MANAGER( )
{
	//mCurNumOfAgentGenerators = 0;
	mMaxNumOfAgentGenerators = csp_maxNum_AgentGenerators;
	mAgentGenProvider.init( mMaxNumOfAgentGenerators );
	//
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	//mTmpObject; // for adding a new object
	mCurPickedObj = 0;
	mCurHitObj = 0;
	mLockPickedObject = false;
	mFlgNewlyPickedObj = false;
}

void AGENT_GENERATOR_MANAGER::setAgentGeneratorTemplate( const CS_AGENT_GENERATOR &g )
{
	mTmpObject.set( g );
}

/*
CS_AGENT_GENERATOR *AGENT_GENERATOR_MANAGER::getGenerators( int &num, int **UIObjArr ) const
{
	num = mCurNumOfAgentGenerators;
	*UIObjArr = mUIObjArr;
	return mAgentGeneratorArr;
}
*/

void AGENT_GENERATOR_MANAGER::set( const AGENT_GENERATOR_MANAGER *m )
{
	mAgentGenProvider.reset( );

	m->mAgentGenProvider.resetIterator( );

	CS_AGENT_GENERATOR *obj;
	while ( obj = m->mAgentGenProvider.getNext( ) ) {
		addAgentGenerator( obj );
	}
}

void AGENT_GENERATOR_MANAGER::addAgentGenerator( const CS_AGENT_GENERATOR *g )
{
	int objIndex = mAgentGenProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_AGENT_GENERATOR *obj = mAgentGenProvider.getEle( objIndex );
	
	obj->set( *g );
	//BUG, the idea is incorrect
	mAgentGenProvider.autoSetUniqueObjID( obj );
}

void AGENT_GENERATOR_MANAGER::setObjAction( int action )
{
	m_bo_CurAction = action;

	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			mTmpObject.setObjState( CS_OBJECT_STATE_NONE );
			mTmpObject.clear( );
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			begin_DefineObj(CS_AGENT_GEN_OBJECT_TYPE_CIRCLE);
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			mTmpObject.setObjState( CS_OBJECT_STATE_NONE );
			mTmpObject.clear( );
			break;
	}
}

void AGENT_GENERATOR_MANAGER::save( const string &fileName ) const
{
	ofstream *fp;
		

		fp = new std::ofstream(fileName.data(), ios::out | ios::binary);

		if ( fp == 0 ) {
			cout << "Cannot create file:" << fileName << endl;
			return;
		}

		*fp << AGENT_GEN_DF_TAG_MAIN << endl << endl;

		mAgentGenProvider.resetIterator( );
		CS_AGENT_GENERATOR *obj;
		while ( obj = mAgentGenProvider.getNext( ) ) {
			obj->save( fp );
			*fp << endl;
		}

		fp->close( );
}

void AGENT_GENERATOR_MANAGER::resetIterator( )
{
	mAgentGenProvider.resetIterator( );
}

CS_AGENT_GENERATOR *AGENT_GENERATOR_MANAGER::getNext( )
{
	return mAgentGenProvider.getNext( );
}


void AGENT_GENERATOR_MANAGER::reset( )
{
	mAgentGenProvider.resetIterator( );
		CS_AGENT_GENERATOR *obj;
		while ( obj = mAgentGenProvider.getNext( ) ) {
			obj->reset( );
		}
}


bool AGENT_GENERATOR_MANAGER::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void AGENT_GENERATOR_MANAGER::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}

void AGENT_GENERATOR_MANAGER::bo_setNewPosition_XZ( float x, float z )
{
}
void AGENT_GENERATOR_MANAGER::bo_drawWithHighLight( ) const
{
}

void AGENT_GENERATOR_MANAGER::bo_drawWithHighLight( float r, float g, float b ) const
{
}

void AGENT_GENERATOR_MANAGER::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	mAgentGenProvider.resetIterator( );

	CS_AGENT_GENERATOR *agentObj;

	bool flgFirst = true;
	vector3 aabbMin, aabbMax;
	while ( agentObj = mAgentGenProvider.getNext( ) ) {
		agentObj->computeAABB( aabbMin, aabbMax );
		if ( flgFirst ) { 
			AABBmin = aabbMin;
			AABBmax = aabbMax;
			flgFirst = false;
		} else {
			mergeAABB( AABBmin, AABBmax, aabbMin, aabbMax );
		}
	}

}
