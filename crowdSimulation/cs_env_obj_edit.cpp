//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/11
//
#include <iostream>
#include <cmath>
#include "cs_env_obj.h" // environment object

void CS_ENV_OBJECT::moveObjPosition_XZ( const vector3 &p )
{
	vector3 translated_vector = p - m_bo_SelectedPoint;
	for ( int i = 0; i < ptArr.size( ); ++i ) {
		ptArr[i] += translated_vector;

	}
	m_bo_SelectedPoint = p;
	translateAABB( translated_vector );
	computeExtendedObj( mCornerThickness_CD );
}

void CS_ENV_OBJECT::moveSelectedVertexPosition_XZ( const vector3 &p )
{
	//cout << "mCurPickedVertexIndex:" << mCurPickedVertexIndex << endl;
	if ( m_bo_CurPickedVertexIndex < 0 ) return;
	if ( m_bo_CurPickedVertexIndex >= ptArr.size( ) ) return;
	vector3 translated_vector = p - m_bo_SelectedPoint;

	//cout << "setSelectedVertexPosition_XZ, mCurPickedVertexIndex:" << mCurPickedVertexIndex << endl;
	ptArr[ m_bo_CurPickedVertexIndex ] += translated_vector;
	m_bo_SelectedPoint = p;

	computeAABB( );
}

void CS_ENV_OBJECT::movePickedItem_XZ( const vector3 &p )
{
	//cout << "CS_ENV_OBJECT::movePickedItem_XZ" << endl;
	//cout << "m_bo_CurPickedVertexIndex:" << m_bo_CurPickedVertexIndex << endl;
	//cout << "m_bo_CurPickedEdgeIndex:" << m_bo_CurPickedEdgeIndex << endl;

	if ( m_bo_CurPickedEdgeIndex >= 0 ) {
		moveObjPosition_XZ( p );
	} else {
		moveSelectedVertexPosition_XZ( p );
	}
	
}

void CS_ENV_OBJECT::setTemporaryPoint( const vector3 & p )
{
	mTmpPointForDefiningNewObj = p;
}
