#ifndef __MANAGER_TEMPLATE__
#define __MANAGER_TEMPLATE__

//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
#include "../../headers.h"
#include <string>
#include "../../imageReader/imageReader.h"
#include "../../openGLbasic/opengl_stuff.h"
#include "../../textureManager/textureManager.h"
#include "../../painter/painter.h"
#include "../../textureManager/texture.h"
#include "../../basic_app.h"
#include "../agent.h"
#include "../cs_basic_structs.h"
//
#include "../../primitives/provider.h"
#include "../base_obj.h"

template <class DT> class MANAGER_TEMPLATE : public BASE_OBJ
{
private:
	static MANAGER_TEMPLATE<DT> *_instance; // singleton pattern

protected:
	int mMaxNumOfObjs;
	PROVIDER<DT> mObjProvider;

	DT mTmpObject;
	DT *mCurPickedObj;
	DT *mCurHitObj;
	bool mLockPickedObject;
	mutable bool mFlgNewlyPickedObj;
	//
	void draw_PickedObj( ) const;
public:
	MANAGER_TEMPLATE( int maxNum );
	virtual void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	virtual void setObjAction( int action );
	virtual DT *addObject( const DT &obj ) = 0;
	//
	// editing tools
	virtual void mouseClick( const vector3 & p, float hitDistance );
	virtual void passiveMouseFunc( const vector3 & p, float hitDistance );

	virtual void cancel_DefineObj( );
	virtual void setTemporaryPointForNewObj( const vector3 &p );
	//virtual void addObject_Rect( const vector3 &p0, const vector3 &p1 );
	virtual void begin_DefineObj( int objType );
	virtual void addPoint_DefineObj( const vector3 &p );
	virtual void start_Action( );
	virtual void end_Action( bool flgContinue = true );
	//
	virtual bool isHitObj( ) const;
	virtual void deletePickedObj( );
	virtual void updateProvider( );
	//general methods
	virtual void draw( ) const = 0;
	virtual void update( float time_step ) = 0;
	virtual void read( const char *fileName ) = 0;
	virtual void save( const string &fileName ) const = 0;
	virtual void printf_Data( ) const = 0;
	
	//
	virtual const PROVIDER<DT> *getProvider( ) const;
	//
	
	virtual DT *getCurPickedObj( bool &flgNew ) const;
	virtual void movePickedObj( const vector3 &p );
	virtual void pickObjForEditing( const vector3 &p, float hitDistance );
	virtual DT *pickObj( const vector3 &p, float hitDistance, bool flgPickObj = true );
	virtual void unpickSelectedObj( );
	//
	virtual bool bo_isPicked_XZPlane( float x, float z );
	virtual void bo_setNewPosition_XZ( float x, float z );
	virtual void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	virtual void bo_drawWithHighLight( ) const;
	virtual void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	static MANAGER_TEMPLATE<DT> *instance( int maxNumObjs = 0 ) {
		if ( _instance == 0) {
			_instance = new MANAGER_TEMPLATE<DT>( maxNumObjs );
		}
		return _instance;
	}
};


template <class DT> const PROVIDER<DT> *MANAGER_TEMPLATE<DT>::getProvider( ) const
{
	return &mObjProvider;
}

template <class DT> void MANAGER_TEMPLATE<DT>::mouseClick( const vector3 & p, float hitDistance )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_ADD_OBJ:
			addPoint_DefineObj( p );
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			
			pickObjForEditing( p, hitDistance );
			if ( mCurPickedObj ) {
				mCurPickedObj->computeAABB( );
			}
			break;
	}
}

template <class DT> void MANAGER_TEMPLATE<DT>::passiveMouseFunc( const vector3 & p, float hitDistance )
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

template <class DT> void MANAGER_TEMPLATE<DT>::unpickSelectedObj( )
{
	if ( mCurPickedObj ) {
		mCurPickedObj->setObjState( CS_OBJECT_STATE_NONE );
	}
	mCurPickedObj = 0;
}

template <class DT> void MANAGER_TEMPLATE<DT>::pickObjForEditing( const vector3 &p, float hitDistance )
{
	unpickSelectedObj( );
	pickObj( p, hitDistance, true );
	if ( mCurPickedObj ) mCurPickedObj->setObjState( CS_OBJECT_STATE_EDIT );
	if ( mCurPickedObj ) {
		mLockPickedObject = true;
		mFlgNewlyPickedObj = true;
	}
}

template <class DT> DT* MANAGER_TEMPLATE<DT>::pickObj( const vector3 &p, float hitDistance, bool flgPickObj  )
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
	mObjProvider.resetIterator( );
	DT *obj;
	while ( obj = mObjProvider.getNext( ) ) {
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
		if ( mCurHitObj != mCurPickedObj ) mCurHitObj->setObjState( CS_OBJECT_STATE_HIT );
	}
	
	//cout << "mCurPickedObj:" << mCurPickedObj << endl;
	return mCurPickedObj;
}


template <class DT> void MANAGER_TEMPLATE<DT>::movePickedObj( const vector3 &p )
{
	//cout << "mCurPickedObj:" << mCurPickedObj << endl;
	
	if ( mCurPickedObj == 0 ) return;
	mCurPickedObj->movePickedItem_XZ( p );
}

template <class DT> void MANAGER_TEMPLATE<DT>::setTemporaryPointForNewObj( const vector3 &p )
{
	mTmpObject.setTemporaryPoint( p );
}

template <class DT> void MANAGER_TEMPLATE<DT>::start_Action( )
{
	switch( m_bo_CurAction ) {
		case CS_EDIT_ACTION_NONE:
			break;
		case CS_EDIT_ACTION_ADD_OBJ:
			begin_DefineObj(CS_OBJ_TYPE_POLYGON);
			break;
		case CS_EDIT_ACTION_EDIT_OBJ:
			break;
	}
}

template <class DT> void MANAGER_TEMPLATE<DT>::cancel_DefineObj( )
{
	mTmpObject.clear( );
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	
}

template <class DT> void MANAGER_TEMPLATE<DT>::begin_DefineObj( int objType )
{
	mTmpObject.clear( );
	mTmpObject.setObjType( objType );
	mTmpObject.setObjState( CS_OBJECT_STATE_NEW );
}

template <class DT> void MANAGER_TEMPLATE<DT>::addPoint_DefineObj( const vector3 &p )
{
	cout << "template <class DT> void MANAGER_TEMPLATE<DT>::addPoint_DefineObj" << endl;

	mTmpObject.addPoint( p );
	mTmpObject.setTemporaryPoint( p );
}

template <class DT> void MANAGER_TEMPLATE<DT>::end_Action( bool flgContinue )
{
	mTmpObject.setObjState( CS_OBJECT_STATE_NONE );
	addObject( mTmpObject );
	mTmpObject.clear( );
	if ( flgContinue ) {
		start_Action( );
	} else {
		m_bo_CurAction = CS_EDIT_ACTION_NONE;
	}
}

template <class DT> bool MANAGER_TEMPLATE<DT>::isHitObj( ) const
{
	return mCurHitObj != 0;
}

template <class DT> DT *MANAGER_TEMPLATE<DT>::getCurPickedObj( bool &flgNew ) const
{
	flgNew = mFlgNewlyPickedObj;
	mFlgNewlyPickedObj = false;
	return mCurPickedObj;
}

template <class DT> void MANAGER_TEMPLATE<DT>::deletePickedObj( )
{
	/*
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
	*/

}
template <class DT> void MANAGER_TEMPLATE<DT>::updateProvider( )
{
	mObjProvider.autoUpdate( );
}

//-------------------------------------------------
template <class DT> MANAGER_TEMPLATE<DT>::MANAGER_TEMPLATE( int maxNum )
{
	mMaxNumOfObjs = maxNum;
	mObjProvider.init( mMaxNumOfObjs );

}

template <class DT> void MANAGER_TEMPLATE<DT>::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	mObjProvider.resetIterator( );

	DT *obj;

	bool flgFirst = true;
	vector3 aabbMin, aabbMax;
	while ( obj = mObjProvider.getNext( ) ) {
		obj->computeAABB( aabbMin, aabbMax );
		if ( flgFirst ) { 
			AABBmin = aabbMin;
			AABBmax = aabbMax;
			flgFirst = false;
		} else {
			mergeAABB( AABBmin, AABBmax, aabbMin, aabbMax );
		}
	}

}

template <class DT> void MANAGER_TEMPLATE<DT>::setObjAction( int action )
{
	m_bo_CurAction = action;
}

template <class DT> void MANAGER_TEMPLATE<DT>::update( float time_step )
{

}

//////////////////////////////////////////////////
template <class DT> bool MANAGER_TEMPLATE<DT>::bo_isPicked_XZPlane( float x, float z )
{
	return false;
}

template <class DT> void  MANAGER_TEMPLATE<DT>::bo_setNewPosition_XZ( float x, float z )
{
}

template <class DT> void MANAGER_TEMPLATE<DT>::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
	 {
	 }

template <class DT> void MANAGER_TEMPLATE<DT>::bo_drawWithHighLight( ) const
	{
	}

template <class DT> void MANAGER_TEMPLATE<DT>::bo_drawWithHighLight( float r, float g, float b ) const
	{
			}


#endif