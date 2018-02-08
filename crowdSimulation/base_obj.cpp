#include <iostream>
#include "base_obj.h"

using namespace std;

BASE_OBJ::BASE_OBJ( )
{
	m_bo_ObjType = 0;
	m_bo_ObjState = CS_OBJECT_STATE_NONE;
	m_bo_CurAction = CS_EDIT_ACTION_NONE;

	//
	m_bo_CurPickedVertexIndex = -1;
	m_bo_CurPickedEdgeIndex = -1;
	m_bo_FlgPicked = false;
	//
	mFlgUsed = false;
	mFlgActivated = false;
	//
	mObjID = -1; // ID is not assigned yet
}

void BASE_OBJ::setObjID( int objID )
{
	mObjID = objID;
}

bool BASE_OBJ::getUsedFlg( ) const
{
	return mFlgUsed;
}

bool BASE_OBJ::getActivatedFlg( ) const
{
	return mFlgActivated;
}

void BASE_OBJ::setUsedFlag( bool flg )
{
	mFlgUsed = flg;		
}

void BASE_OBJ::setActivatedFlag( bool flg )
{
	mFlgActivated = flg;
}

int BASE_OBJ::getObjType( ) const
{
	return m_bo_ObjType;
}

void BASE_OBJ::setObjType( int type )
{
	m_bo_ObjType = type;
}

void BASE_OBJ::setObjState( int state )
{
	m_bo_ObjState = state;
}

void BASE_OBJ::setObjAction( int action )
{
	m_bo_CurAction = action;
}

void BASE_OBJ::unPicked( )
{
	m_bo_CurPickedVertexIndex = -1;
	m_bo_CurPickedEdgeIndex = -1;
}

void BASE_OBJ::moveObjPosition_XZ( const vector3 &p )
{
	cout << "BASE_OBJ::setObjPosition_XZ undefined" << endl;
	/*
	vector3 translated_vector = p - mSelectedPoint;
	for ( int i = 0; i < ptArr.size( ); ++i ) {
		ptArr[i] += translated_vector;

	}
	mSelectedPoint = p;
	*/
}

void BASE_OBJ::moveSelectedVertexPosition_XZ( const vector3 &p )
{
	cout << "BASE_OBJ::setSelectedVertexPosition_XZ undefined" << endl;

	/*
	//cout << "mCurPickedVertexIndex:" << mCurPickedVertexIndex << endl;
	if ( m_bo_CurPickedVertexIndex < 0 ) return;
	if ( m_bo_CurPickedVertexIndex >= ptArr.size( ) ) return;
	vector3 translated_vector = p - m_bo_SelectedPoint;

	//cout << "setSelectedVertexPosition_XZ, mCurPickedVertexIndex:" << mCurPickedVertexIndex << endl;
	ptArr[ mCurPickedVertexIndex ] += translated_vector;
	mSelectedPoint = p;
	*/

}

void BASE_OBJ::translateAABB( const vector3 &tx )
{
	mAABBmin += tx;
	mAABBmax += tx;

}

void BASE_OBJ::movePickedItem_XZ( const vector3 &p )
{
	if ( m_bo_CurPickedEdgeIndex >= 0 ) {
		moveObjPosition_XZ( p );
	} else {
		moveSelectedVertexPosition_XZ( p );
	}
}

void BASE_OBJ::setTemporaryPoint( const vector3 & p )
{
	m_bo_TmpPointForDefiningNewObj = p;
}

//square
float BASE_OBJ::computeDistance2_XZ( const vector3 & p ) const
{
	return 0.0;
}