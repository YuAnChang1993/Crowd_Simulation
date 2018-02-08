//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CS_ENVIRONMENT_H_
#define __CS_ENVIRONMENT_H_

#include "../headers.h"
#include <string>
#include <vector>
#include "../openGLbasic/opengl_stuff.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"
#include "cs_env_obj.h"
#include "../primitives/provider.h"
#include "myDijkstra/myDijkstra.h"

class CS_CELLULAR_AUTOMATA;
class CS_DIJKSTRA;

class BVH_NODE {
public:
	BVH_NODE( );
	vector3 AABBmin, AABBmax;
	CS_ENV_OBJECT *obj;
	BVH_NODE *left, *right;
};

class CS_ENVIRONMENT : public BASE_OBJ
{
private:
	static CS_ENVIRONMENT *_instance; // singleton
protected:
	//int mCurNumEnvObjects;
	//int mMaxNumEnvObjects;
	//CS_ENV_OBJECT *mEnvObjectArr;
	CS_ENV_OBJECT *mMapObj;
	CS_ENV_OBJECT mTmpEnvObject; // for adding a new environment object
	CS_ENV_OBJECT *mCurPickedObj;
	CS_ENV_OBJECT *mCurHitObj;
	mutable bool mFlgNewlyPickedObj;
	bool mLockPickedObject;
	//
	int mMaxNumEnvObjects;
	PROVIDER<CS_ENV_OBJECT> mEnvObjProvider;
	//
	BVH_NODE *mBVHRoot;
	//
	vector3 mMinAABB_CD, mMaxAABB_CD;
	float mCDGrid_DX, mCDGrid_DZ;
	float mCDGrid_WX, mCDGrid_WZ; // range

	int mEncodedCD_GridNX, mEncodedCD_GridNZ;
	unsigned int *mEncodedCDData;
	int mCDEncodingSchemeType;
	//
	void draw_PickedObj( ) const;
	//
	void save_EnvironmentObjects( ofstream *fp ) const;
	//
	vector3 computeSeparationVector_Circle_BVHScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle_NormalScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle_EncodedScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;

	unsigned int computeCDCode( const vector3 &p, float r );
	int computeGridIndex_EncodedScheme( const vector3 & q0, const vector3 & q1 ) const;
	vector3 computeSeparationVector_Circle_EncodedScheme_Type_01( unsigned int cdCode, const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;
	vector3 computeSeparationVector_Circle_EncodedScheme_Type_02( unsigned int cdCode, const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;
	//
	BVH_NODE *buildBVHOfObjects( const vector<CS_ENV_OBJECT*> &objArr );
	//
	//void traverseBVH( const BVH_NODE *node, vector<CS_ENV_OBJECT *> &objArr, const vector3 & q0, const vector3 & q1, float r ) const;

	void traverseBVH( vector<CS_ENV_OBJECT *> &objArr, const vector3 & q0, const vector3 & q1, float r ) const;
	void drawBVH( const BVH_NODE *node, int cur_depth, int max_depth ) const;
public:
	CS_ENVIRONMENT( );
	void addEnvObject( const CS_ENV_OBJECT &obj );
	//
	void precomputeCollisionDetectionAcceleratedStructure_EncodingScheme( );
	void setCollisionDetectionScheme( int schemeType );
	void setCollisionDetectionGridDimension( int nx, int nz );
	void buildBVHOfObjects( );
	//
	// editing tools
	void mouseClick( const vector3 & p, float hitDistance );
	void passiveMouseFunc( const vector3 & p, float hitDistance );
	void cancel_DefineEnvironmentObj( );
	void setTemporaryPointForNewObj( const vector3 &p );
	void addEnvironmentObject_Rect( const vector3 &p0, const vector3 &p1 );
	void begin_DefineEnvironmentObj( int objType );
	void addPoint_DefineEnvironmentObj( const vector3 &p );
	void start_Action( );
	void end_Action( bool flgContinue = true );
	//
	bool isHitObj( ) const;
	void deletePickedObj( );
	void updateProvider( );
	//general methods
	void drawBVH( ) const;
	void draw( ) const;
	void update( float time_step );
	void read( const char *fileName );
	void save( const string &fileName ) const;
	void printf_Data( ) const;
	void setObjAction( int action );
	//
	void computeAABB( vector3 &AABBmin, vector3 &AABBmax );
	//
	CS_ENV_OBJECT *getCurPickedObj( bool &flgNew ) const;
	void movePickedObj( const vector3 &p );
	void pickObjForEditing( const vector3 &p, float hitDistance );
	CS_ENV_OBJECT *pickObj( const vector3 &p, float hitDistance, bool flgPickObj = true );
	void unpickSelectedObj( );
	//
	//specific methods
	void setMapObj( const CS_ENV_OBJECT &obj );
	void inverseVertexOrderForSelectedObj( );
	vector3 computeSeparationVector_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const;
	vector3 computeNormalDirection_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &prefer_vec ) const;
	//
	void drawMap( ) const;
	
	//
	bool bo_isPicked_XZPlane( float x, float z );
	void bo_setNewPosition_XZ( float x, float z );
	void bo_translatePosition_XZ( float x, float z, float global_posx = 0.0, float global_posz = 0.0 );
	void bo_drawWithHighLight( ) const;
	void bo_drawWithHighLight( float r, float g, float b ) const;
	
	//
	PROVIDER<CS_ENV_OBJECT> *getProvider( );
	
	//
	static CS_ENVIRONMENT *instance( ) {
		if ( _instance == 0) {
			_instance = new CS_ENVIRONMENT( );
		}
		return _instance;
	}

	//2017/04/06
	void drawCelluarPedestrian(CS_CELLULAR_AUTOMATA &mCelluarAutomata);

	//2017/04/23
	void setDijkstraPoints(CS_DIJKSTRA *mDijkstra, float mMapMinX, float mMapMaxX, float mMapMinZ, float mMapMaxZ);
};
#endif