//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CS_PATH_MANAGER_H_
#define __CS_PATH_MANAGER_H_
#include "../headers.h"
#include <string>
#include "../imageReader/imageReader.h"
#include "../openGLbasic/opengl_stuff.h"
#include "../textureManager/textureManager.h"
#include "../painter/painter.h"
#include "../textureManager/texture.h"
#include "../basic_app.h"
#include "agent.h"
#include "cs_basic_structs.h"
//
#include "../primitives/provider.h"
#include "environment.h"

class CS_PATH_MANAGER : public BASE_OBJ
{
	static CS_PATH_MANAGER *_instance; // singleton pattern
protected:
	//
	int mMaxNumOfPaths;
	PROVIDER<CS_PATH> mPathObjProvider;

	CS_PATH mTmpObject;
	CS_PATH *mCurPickedObj;
	CS_PATH *mCurHitObj;
	bool mLockPickedObject;
	mutable bool mFlgNewlyPickedObj;
	//
	void draw_PickedObj( ) const;
	//
	void save_Paths( ofstream *fp ) const;
	//
	void pickObjForEditing( const vector3 &p, float hitDistance );

	//
public:
	CS_PATH_MANAGER( );
	bool isHitObj( ) const;
	void addPath( const CS_PATH &path );
	void addPath( int pathID, const CS_PATH &path );
	void addPath( int pathID, const vector3 *ptArr, int numPoints, float radius );
	const CS_PATH *getPathBasedOnPathIndex( int objIndex ) const;
	const CS_PATH *getPathBasedOnPathID( int objID ) const;

	vector3 getFirstPathPoint( int pathIndex ) const;
	const CS_PATH *getPathArr( int &num ) const;
	void read( const char *fileName );
	void setPathPoint( int pathID, int pointIndex, const vector3 &p );
	//
	float *getPathPoint_XPtr( int pathID, int pointIndex ) const;
	float *getPathPoint_ZPtr( int pathID, int pointIndex ) const;

	BASE_OBJ *pickPathPoint_XZPlane( float x, float z ) const;
	// editing tools
	void setObjAction( int action );
	void mouseClick( const vector3 & p, float hitDistance );
	void passiveMouseFunc( const vector3 & p, float hitDistance );
	CS_PATH *pickObj( const vector3 &p, float hitDistance, bool flgPickObj = true );
	void unpickSelectedObj( );
	void cancel_DefineObj( );
	void movePickedObj( const vector3 &p );
	void setTemporaryPointForNewObj( const vector3 &p );
	void start_Action( );
	void addObjNow( );
	void addObject_Circle( const vector3 &p0, const vector3 &p1 );
	void begin_DefineObj( int objType );
	void addPoint_DefineObj( const vector3 &p );
	void end_Action( );
	//
	int findClosestPathIndex( const vector3 &p ) const;
	int findClosestPathIndex_WholePath( const vector3 &p, int &node_index, bool flgContainedEndPoints ) const;
	int findOneRandomPathIndex( ) const;
	//
	void deletePickedObj( );
	void updateProvider( );
	CS_PATH *getCurPickedObj( bool &flgNew ) const;
	//
	PROVIDER<CS_PATH> *getProvider( );
	//
	void printf_PathData( ) const;
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	//
	void draw( ) const;
	//
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	//
	void save( const string &fileName ) const;
	//
	void resetPathLoad( );
	//
	static CS_PATH_MANAGER *instance( ) {
		if ( _instance == 0) {
			_instance = new CS_PATH_MANAGER( );
		}
		return _instance;
	}
};

#endif