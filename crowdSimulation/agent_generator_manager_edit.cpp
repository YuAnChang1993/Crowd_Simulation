#include <iostream>
#include "agent_generator_manager.h"

using namespace std;

void AGENT_GENERATOR_MANAGER::toggleLockPickedObject( )
{
	if ( mCurPickedObj ) {
		mLockPickedObject = true;
	} else {
		mLockPickedObject = false;
	}

}


void AGENT_GENERATOR_MANAGER::setLockPickedObject( bool flg )
{
	mLockPickedObject = false;
}

void AGENT_GENERATOR_MANAGER::mouseClick( const vector3 & p, float hitDistance )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_ADD_OBJ:
			addPoint_DefineObj( p );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			if ( mCurPickedObj ) {
			}
			pickObjForEditing( p, hitDistance );
			if ( mCurPickedObj ) {
				mLockPickedObject = true;
				mFlgNewlyPickedObj = true;
			}
			break;
	}
}

void AGENT_GENERATOR_MANAGER::passiveMouseFunc( const vector3 & p, float hitDistance )
{
	
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_ADD_OBJ:
			setTemporaryPointForNewObj( p );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			pickObj( p, hitDistance, false ); 
			break;
	}
}

void AGENT_GENERATOR_MANAGER::unpickSelectedObj( )
{
	if ( mCurPickedObj ) {
		mCurPickedObj->setObjState( CS_OBJECT_STATE_NONE );
	}
	mCurPickedObj = 0;
}

void AGENT_GENERATOR_MANAGER::pickObjForEditing( const vector3 &p, float hitDistance )
{
	unpickSelectedObj( );
	pickObj( p, hitDistance, true );
	if ( mCurPickedObj ) mCurPickedObj->setObjState( CS_OBJECT_STATE_EDIT );
}



CS_AGENT_GENERATOR *AGENT_GENERATOR_MANAGER::pickObj( const vector3 &p, float hitDistance, bool flgPickObj )
{
	float shortest_distance2 = -1.0;
	if ( flgPickObj ) mCurPickedObj = 0;
	m_bo_SelectedPoint = p;
	mCurHitObj = 0;
	mAgentGenProvider.resetIterator( );
	CS_AGENT_GENERATOR *obj;
	while ( obj = mAgentGenProvider.getNext( ) ) {
		bool flgPicked = obj->canBePicked( p, hitDistance, shortest_distance2 );
		if ( flgPicked ) {
			if ( flgPickObj ) {
				mCurPickedObj = obj;
			} else {
				mCurHitObj = obj;
			}
		}
	}
	
	//cout << "mCurPickedObj:" << mCurPickedObj << endl;
	return mCurPickedObj;
}


void AGENT_GENERATOR_MANAGER::movePickedObj( const vector3 &p )
{
	if ( mCurPickedObj == 0 ) return;
	mCurPickedObj->movePickedItem_XZ( p );
}

void AGENT_GENERATOR_MANAGER::setTemporaryPointForNewObj( const vector3 &p )
{
	mTmpObject.setTemporaryPoint( p );
}

void AGENT_GENERATOR_MANAGER::start_Action( )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			break;
	}
}

void AGENT_GENERATOR_MANAGER::cancel_DefineObj( )
{
	mTmpObject.clear( );
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	
}


void AGENT_GENERATOR_MANAGER::begin_DefineObj( int objType )
{
	mTmpObject.clear( );
	mTmpObject.setObjType( objType );
	mTmpObject.setObjState( CS_OBJECT_STATE_NEW );
}

void AGENT_GENERATOR_MANAGER::addPoint_DefineObj( const vector3 &p )
{
	cout << "AGENT_GENERATOR_MANAGER::addPoint_DefineObj" << endl;

	bt_enforce_printf_vec3( "addPoint_DefineObj:", p);
	mTmpObject.addPoint( p );
	mTmpObject.setTemporaryPoint( p );
	addAgentGenerator( &mTmpObject );
	//
	mTmpObject.clear( );
	mTmpObject.addPoint( p );
}

void AGENT_GENERATOR_MANAGER::end_Action( bool flgContinue )
{

}


CS_AGENT_GENERATOR *AGENT_GENERATOR_MANAGER::getCurPickedObj( bool &flgNew ) const
{
	flgNew = mFlgNewlyPickedObj;
	mFlgNewlyPickedObj = false;
	return mCurPickedObj;
}

bool AGENT_GENERATOR_MANAGER::isHitObj( ) const
{
	return mCurPickedObj != 0;
}

void AGENT_GENERATOR_MANAGER::deletePickedObj( )
{
	if ( mCurPickedObj == 0 ) return;


	mCurPickedObj->setUsedFlag( false );
	mCurPickedObj = 0;
	mFlgNewlyPickedObj = false;

	updateProvider( );

}
void AGENT_GENERATOR_MANAGER::updateProvider( )
{
	mAgentGenProvider.autoUpdate( );
}
