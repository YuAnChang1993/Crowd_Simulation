//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/01/12
//
#ifndef __BASE_OBJ_H_
#define __BASE_OBJ_H_
#include "../primitives/vector3d.h"

#define CS_OBJECT_STATE_NONE	0	// 
#define CS_OBJECT_STATE_NEW		1	// newly created object
#define CS_OBJECT_STATE_EDIT	2	// being edited
#define CS_OBJECT_STATE_HIT		3	// mouse hit the object

#define CS_EDIT_ACTION_NONE		0
#define CS_EDIT_ACTION_ADD_OBJ	1	// adding a new object
#define CS_EDIT_ACTION_EDIT_OBJ	2	// editing an object

#define CS_OBJ_TYPE_NONE		0
#define CS_OBJ_TYPE_CIRCLE		1
#define CS_OBJ_TYPE_POLYGON		2

class BASE_OBJ {
protected:
	int m_bo_ObjType;
	int m_bo_ObjState;
	int m_bo_CurAction;
	int m_bo_CurPickedVertexIndex;
	int m_bo_CurPickedEdgeIndex;
	bool m_bo_FlgPicked;
	vector3 m_bo_SelectedPoint;
	vector3 m_bo_TmpPointForDefiningNewObj;
	bool mFlgActivated;	// is activated?
	
	//
	vector3 mAABBmin, mAABBmax;
	int mObjID; // explicit ID, PROVIDER can help to set a unique ID
public:
	BASE_OBJ( );
	//
	bool mFlgUsed;		// is being used?
	int _index; // implicit ID. do not change this value, used in PROVIDER
	
	///////////////////////////////////////////////////////
	int getImplicitIndex( ) const { return _index; }
	int getObjID( ) const { return mObjID; }
	bool getActivatedFlg( ) const;
	bool getUsedFlg( ) const;
	void setObjID( int objID );
	virtual void setUsedFlag( bool flg );
	virtual void setActivatedFlag( bool flg );
	virtual void setObjType( int type );
	virtual int getObjType( ) const;
	virtual void setObjState( int state );
	virtual void setObjAction( int action );
	virtual void unPicked( );
	virtual void moveObjPosition_XZ( const vector3 &p );
	virtual void moveSelectedVertexPosition_XZ( const vector3 &p );
	virtual void movePickedItem_XZ( const vector3 &p );
	virtual void setTemporaryPoint( const vector3 & p );
	virtual float computeDistance2_XZ( const vector3 & p ) const;
	virtual float getRadius2( ) const { return -1.0; } // radius*radius
	///////////////////////////////////////////////////////
	virtual void printf_Data( ) const { }
	///////////////////////////////////////////////////////
	void translateAABB( const vector3 &p );
	//Abstract methods
	virtual bool bo_isPicked_XZPlane( float x, float z ) = 0;
	virtual void bo_setNewPosition_XZ( float x, float z ) = 0;
	virtual void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 ) = 0;
	virtual void bo_drawWithHighLight( ) const = 0;
	virtual void bo_drawWithHighLight( float r, float g, float b ) const = 0;
	///////////////////////////////////////////////////////
	virtual void draw_debug( const vector3 agent_color,  bool used_own_color ) const { }

};

#endif