//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/20
//
#ifndef __AGENT_GENRATOR_MANAGER_H_
#define __AGENT_GENRATOR_MANAGER_H_

#include "../headers.h"
#include <iostream>
#include <vector>
#include "agent_generator.h"
#include "../primitives/provider.h"

using namespace std;

class AGENT_GENERATOR_MANAGER : public BASE_OBJ {
private:
	static AGENT_GENERATOR_MANAGER *__instance;
protected:
	int mMaxNumOfAgentGenerators;
	PROVIDER<CS_AGENT_GENERATOR> mAgentGenProvider;
	//
	CS_AGENT_GENERATOR mTmpObject; // for adding a new object
	CS_AGENT_GENERATOR *mCurPickedObj;
	CS_AGENT_GENERATOR *mCurHitObj; // mouse on the object
	mutable bool mFlgNewlyPickedObj;
	//
	bool mLockPickedObject;
	//
	void updateProvider( );
	//
public:
	AGENT_GENERATOR_MANAGER( );
	void toggleLockPickedObject( );
	void setLockPickedObject( bool flg );
	void set( const AGENT_GENERATOR_MANAGER *m );
	void addAgentGenerator( const CS_AGENT_GENERATOR *g );
	//
	void setAgentGeneratorTemplate( const CS_AGENT_GENERATOR &g );
	//
	void setGenerators_Default( );
	void setObjAction( int action );
	//
	/////////////////////////////////////////////////////////
	// editing tools
	void start_Action( );
	void end_Action( bool flgContinue = true );
	//
	void passiveMouseFunc( const vector3 & p, float hitDistance );
	void mouseClick( const vector3 & p, float hitDistance );
	//
	void pickObjForEditing( const vector3 &p, float hitDistance );
	CS_AGENT_GENERATOR *pickObj( const vector3 &p, float hitDistance, bool flgPickObj = true );
	void unpickSelectedObj( );
	void cancel_DefineObj( );
	void movePickedObj( const vector3 &p );
	void setTemporaryPointForNewObj( const vector3 &p );
	void addObject_Circle( const vector3 &p0, const vector3 &p1 );
	void begin_DefineObj( int objType );
	void addPoint_DefineObj( const vector3 &p );
	CS_AGENT_GENERATOR *getCurPickedObj( bool &flgNew ) const;
	void deletePickedObj( );
	/////////////////////////////////////////////////////////
	bool isHitObj( ) const;
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	//
	void drawAgentGenerators( ) const;
	void draw( ) const;
	//
	void reset( );
	void read( const char *fileName );
	void save( const string &fileName ) const;
	//
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	//CS_AGENT_GENERATOR *getGenerators( int &num, int **UIObjIndexArrPtr ) const;
	//
	void resetIterator( );
	CS_AGENT_GENERATOR *getNext( );
	//
	static AGENT_GENERATOR_MANAGER *instance( )
	{
		if ( __instance == 0 ) {
			__instance = new AGENT_GENERATOR_MANAGER;
		}
		return __instance;
	}
};


#endif