#include <iostream>
#include "environment.h"
#include "cs_env_obj.h"

using namespace std;

void CS_ENVIRONMENT::mouseClick( const vector3 & p, float hitDistance )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_ADD_OBJ:
			addPoint_DefineEnvironmentObj( p );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			
			pickObjForEditing( p, hitDistance );
			if ( mCurPickedObj ) {
				mCurPickedObj->computeAABB( );
			}
			break;
	}
}

void CS_ENVIRONMENT::passiveMouseFunc( const vector3 & p, float hitDistance )
{
	switch( m_bo_CurAction ) {
		case CS_OBJECT_STATE_NEW:
			setTemporaryPointForNewObj( p );
			break;
		case CS_OBJECT_STATE_EDIT:
			pickObj( p, hitDistance, false );
			
			break;
	}
	
}

void CS_ENVIRONMENT::unpickSelectedObj( )
{
	if ( mCurPickedObj ) {
		mCurPickedObj->setObjState( CS_OBJECT_STATE_NONE );
	}
	mCurPickedObj = 0;
}

void CS_ENVIRONMENT::pickObjForEditing( const vector3 &p, float hitDistance )
{
	unpickSelectedObj( );
	pickObj( p, hitDistance, true );
	if ( mCurPickedObj ) mCurPickedObj->setObjState( CS_OBJECT_STATE_EDIT );
	if ( mCurPickedObj ) {
		mLockPickedObject = true;
		mFlgNewlyPickedObj = true;
	}
}

CS_ENV_OBJECT *CS_ENVIRONMENT::pickObj( const vector3 &p, float hitDistance, bool flgPickObj  )
{

	
	if ( flgPickObj ) {
		if ( mCurPickedObj ) mCurPickedObj->setObjState( CS_OBJECT_STATE_NONE );
		mCurPickedObj = 0;
	}
	if ( mCurHitObj ) {
		if ( mCurHitObj != mCurPickedObj ) {
			mCurHitObj->setObjState( CS_OBJECT_STATE_NONE );
		}
		mCurHitObj = 0;
	}

	float shortest_distance2 = -1.0;
	//
	mEnvObjProvider.resetIterator( );
	CS_ENV_OBJECT *envObj;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		bool flgPicked = envObj->canBePicked( p, hitDistance, shortest_distance2 );
		if ( flgPicked ) {
			if ( flgPickObj ) {
				mCurPickedObj = envObj;
			} else {
				mCurHitObj = envObj;
			}
		}
	}
	if ( mCurHitObj ) {
		if ( mCurHitObj != mCurPickedObj ) mCurHitObj->setObjState( CS_OBJECT_STATE_HIT );
	}
	
	//cout << "mCurPickedObj:" << mCurPickedObj << endl;
	return mCurPickedObj;
}


void CS_ENVIRONMENT::movePickedObj( const vector3 &p )
{
	//cout << "mCurPickedObj:" << mCurPickedObj << endl;
	
	if ( mCurPickedObj == 0 ) return;
	mCurPickedObj->movePickedItem_XZ( p );
}

void CS_ENVIRONMENT::setTemporaryPointForNewObj( const vector3 &p )
{
	mTmpEnvObject.setTemporaryPoint( p );
}

void CS_ENVIRONMENT::start_Action( )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			begin_DefineEnvironmentObj(CS_ENV_OBJECT_TYPE_POLYGON);
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			break;
	}
}

void CS_ENVIRONMENT::cancel_DefineEnvironmentObj( )
{
	mTmpEnvObject.clear( );
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	
}

void CS_ENVIRONMENT::inverseVertexOrderForSelectedObj( )
{
	if ( mCurPickedObj && (mCurHitObj == mCurPickedObj) ) {
		mCurPickedObj->inverseVertexOrder( );
	}
}


void CS_ENVIRONMENT::begin_DefineEnvironmentObj( int objType )
{
	mTmpEnvObject.clear( );
	mTmpEnvObject.setObjType( objType );
	mTmpEnvObject.setObjState( CS_OBJECT_STATE_NEW );
}

void CS_ENVIRONMENT::addPoint_DefineEnvironmentObj( const vector3 &p )
{
	cout << "CS_ENVIRONMENT::addPoint_DefineEnvironmentObj" << endl;

	bt_enforce_printf_vec3( "addPoint_DefineEnvironmentObj:", p);
	mTmpEnvObject.addPoint( p );
	mTmpEnvObject.setTemporaryPoint( p );
}

void CS_ENVIRONMENT::end_Action( bool flgContinue )
{
	mTmpEnvObject.setObjState( CS_OBJECT_STATE_NONE );
	addEnvObject( mTmpEnvObject );
	mTmpEnvObject.clear( );
	if ( flgContinue ) {
		start_Action( );
	} else {
		m_bo_CurAction = CS_EDIT_ACTION_NONE;
	}
}

bool CS_ENVIRONMENT::isHitObj( ) const
{
	return mCurHitObj != 0;
}

CS_ENV_OBJECT *CS_ENVIRONMENT::getCurPickedObj( bool &flgNew ) const
{
	flgNew = mFlgNewlyPickedObj;
	mFlgNewlyPickedObj = false;
	return mCurPickedObj;
}

void CS_ENVIRONMENT::deletePickedObj( )
{
	/*
	if ( mCurPickedObj == 0 ) return;

	mCurPickedObj->setUsedFlag( false );
	mCurPickedObj = 0;
	mFlgNewlyPickedObj = false;

	updateProvider( );
	*/
		if ( mCurPickedObj == 0 ) return;

	if ( mCurPickedObj->getNumPoints( ) <= 2
		||
		mCurPickedObj->isPickedObj( ) ) {
		mCurPickedObj->setUsedFlag( false );
		mCurPickedObj = 0;
		mFlgNewlyPickedObj = false;

		updateProvider( );
	} else if ( mCurPickedObj->isPickedVertex( ) ) {
		mCurPickedObj->deletePickedVertex( );
		mCurPickedObj->computeAABB( );
	}

}
void CS_ENVIRONMENT::updateProvider( )
{
	mEnvObjProvider.autoUpdate( );
}