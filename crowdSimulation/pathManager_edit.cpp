#include <iostream>
#include "pathManager.h"

using namespace std;

void CS_PATH_MANAGER::setObjAction( int action )
{
	m_bo_CurAction = action;

	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			begin_DefineObj( 0 );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			break;
	}

}

/*
#define CS_OBJECT_STATE_NONE	0	// 
#define CS_OBJECT_STATE_NEW		1	// newly created object
#define CS_OBJECT_STATE_EDIT	2	// being edited
*/
/*
#define CS_EDIT_ACTION_NONE		0
#define CS_EDIT_ACTION_ADD_OBJ	1	// adding a new object
#define CS_EDIT_ACTION_EDIT_OBJ	2
*/
void CS_PATH_MANAGER::passiveMouseFunc( const vector3 & p, float hitDistance )
{
	cout << "CS_PATH_MANAGER::passiveMouseFunc:" << p.x << "\t" << p.y << "\t" << p.z << endl;
	
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_ADD_OBJ:
			setTemporaryPointForNewObj( p );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			pickObj( p, hitDistance, false );
			break;
	}
}

void CS_PATH_MANAGER::mouseClick( const vector3 & p, float hitDistance )
{
	cout << "CS_PATH_MANAGER::mouseClick:" << p.x << "\t" << p.y << "\t" << p.z << endl;

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
				mCurPickedObj->computeAABB( );
			}
			break;
	}
}

bool CS_PATH_MANAGER::isHitObj( ) const
{
	return mCurHitObj != 0;
}


void CS_PATH_MANAGER::unpickSelectedObj( )
{
	if ( mCurPickedObj ) {
		mCurPickedObj->setObjState( CS_OBJECT_STATE_NONE );
	}
	mCurPickedObj = 0;
	mCurHitObj = 0;
}

void CS_PATH_MANAGER::pickObjForEditing( const vector3 &p, float hitDistance )
{
	unpickSelectedObj( );
	pickObj( p, hitDistance, true );
	if ( mCurPickedObj ) mCurPickedObj->setObjState( CS_OBJECT_STATE_EDIT );
	if ( mCurPickedObj ) {
		mLockPickedObject = true;
		mFlgNewlyPickedObj = true;
	}
}

CS_PATH *CS_PATH_MANAGER::pickObj( const vector3 &p, float hitDistance, bool flgPickObj )
{
	
	float shortest_distance2 = -1.0;
	
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

	cout << "CS_PATH_MANAGER::pickObj hitDistance:" << hitDistance << endl;
	//
	mPathObjProvider.resetIterator( );
	CS_PATH *obj;
	while ( obj = mPathObjProvider.getNext( ) ) {
		if ( obj->mFlgUsed == false ) continue;
		bool flgPicked = obj->canBePicked( p, hitDistance, shortest_distance2 );
		if ( flgPicked ) {
			if ( flgPickObj ) {
				mCurPickedObj = obj;
			} else {
				mCurHitObj = obj;
			}
		}
	}

	if ( mCurHitObj ) {
		if ( mCurHitObj == mCurPickedObj ) {
		} else mCurHitObj->setObjState( CS_OBJECT_STATE_HIT );
	}
	return mCurPickedObj;
}


void CS_PATH_MANAGER::movePickedObj( const vector3 &p )
{
	if ( mCurPickedObj == 0 ) return;
	mCurPickedObj->movePickedItem_XZ( p );
}

void CS_PATH_MANAGER::setTemporaryPointForNewObj( const vector3 &p )
{
	cout << "CS_PATH_MANAGER::setTemporaryPointForNewObj" << endl;
	mTmpObject.setTemporaryPoint( p );
}

void CS_PATH_MANAGER::addObjNow( )
{
	cout << "CS_PATH_MANAGER::addObjNow( )" << endl;
	addPath( mTmpObject );
	mTmpObject.clear( );
	mTmpObject.setObjState( CS_OBJECT_STATE_NONE );
}

void CS_PATH_MANAGER::start_Action( )
{
	//cout << "CS_PATH_MANAGER::defineObj( )" << endl;
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			mTmpObject.clear( );
			//mTmpObject.setObjState( CS_OBJECT_STATE_NEW  );
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			//end_DefineObj( );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			mTmpObject.clear( );
			break;
	}
}

void CS_PATH_MANAGER::cancel_DefineObj( )
{
	mTmpObject.clear( );
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	
}


void CS_PATH_MANAGER::begin_DefineObj( int objType )
{
	mTmpObject.clear( );
	mTmpObject.setObjType( objType );
	mTmpObject.setObjState( CS_OBJECT_STATE_NEW );
}

void CS_PATH_MANAGER::addPoint_DefineObj( const vector3 &p )
{
	cout << "CS_PATH_MANAGER::addPoint_DefineObj" << endl;

	bt_enforce_printf_vec3( "addPoint_DefineObj:", p);

	float radius = 5.0;

	mTmpObject.addPoint( p, radius );
	mTmpObject.setTemporaryPoint( p );
}

void CS_PATH_MANAGER::end_Action( )
{
	cout << "CS_PATH_MANAGER::end_DefineObj( )" << endl;
	mTmpObject.setObjState( CS_OBJECT_STATE_NONE );
	addPath( mTmpObject );
	mTmpObject.clear( );
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	
}
///////////////////////////////////////////
//Provider

CS_PATH *CS_PATH_MANAGER::getCurPickedObj( bool &flgNew ) const
{
	flgNew = mFlgNewlyPickedObj;
	mFlgNewlyPickedObj = false;
	return mCurPickedObj;
}

void CS_PATH_MANAGER::deletePickedObj( )
{
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
void CS_PATH_MANAGER::updateProvider( )
{
	mPathObjProvider.autoUpdate( );
}