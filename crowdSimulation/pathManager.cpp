#include "pathManager.h"
#include "./cs_parameters.h"

using namespace std;

CS_PATH_MANAGER *CS_PATH_MANAGER::_instance = 0;

CS_PATH_MANAGER::CS_PATH_MANAGER( )
{
	mFlgNewlyPickedObj = false;
	mCurHitObj = 0;
	mCurPickedObj = 0;

	mMaxNumOfPaths = csp_maxNum_Paths;
	mPathObjProvider.init( mMaxNumOfPaths );
}

PROVIDER<CS_PATH> *CS_PATH_MANAGER::getProvider( )
{
	return &mPathObjProvider;
}


void CS_PATH_MANAGER::resetPathLoad( )
{
	mPathObjProvider.resetIterator( );

	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		if ( path->mFlgUsed == false ) continue;
		path->resetLoad( );
	}
}

void CS_PATH_MANAGER::addPath( const CS_PATH &path )
{
	if ( path.numPoints <= 0 ) return;

	int objIndex = mPathObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_PATH *pathObj = mPathObjProvider.getEle(objIndex );
	pathObj->set( path );
}

void CS_PATH_MANAGER::addPath( int pathID, const CS_PATH &path )
{
	if ( path.numPoints <= 0 ) return;

	int objIndex = mPathObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_PATH *pathObj = mPathObjProvider.getEle(objIndex );
	pathObj->set( path );
	pathObj->setObjID( pathID );
	pathObj->computeAABB( );
	//
	mPathObjProvider.autoSetUniqueObjID( pathObj );
}

void CS_PATH_MANAGER::addPath( int pathID, const vector3 *ptArr, int numPoints, float radius )
{
	int objIndex = mPathObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_PATH *pathObj = mPathObjProvider.getEle( objIndex );
	
	pathObj->mFlgUsed = true;
	pathObj->setActivatedFlag ( true );
	pathObj->setObjID( pathID );
	pathObj->numPoints = 0;
	for (int i = 0; i < numPoints; ++i ) {
		pathObj->addPoint( ptArr[i], radius );
	}
}

float *CS_PATH_MANAGER::getPathPoint_XPtr( int pathID, int pointIndex ) const
{
	CS_PATH *pathObj = mPathObjProvider.getEleBasedOnObjID( pathID );
	if ( pathObj == 0 ) return 0;
	return &(pathObj->ptArr[pointIndex].p.x);
}

float *CS_PATH_MANAGER::getPathPoint_ZPtr( int pathID, int pointIndex ) const
{
	CS_PATH *pathObj = mPathObjProvider.getEleBasedOnObjID( pathID );
	if ( pathObj == 0 ) return 0;
	return &(pathObj->ptArr[pointIndex].p.z);
}

void CS_PATH_MANAGER::setPathPoint( int pathID, int pointIndex, const vector3 &p )
{
	CS_PATH *pathObj = mPathObjProvider.getEleBasedOnObjID( pathID );
	if ( pathObj == 0 ) return;
	pathObj->ptArr[pointIndex].p = p;
}


int CS_PATH_MANAGER::findClosestPathIndex_WholePath( const vector3 &p, int &node_index, bool flgContainedEndPoints ) const
{
	int objIndex = 0;

	float distance2 = -1.0;

	mPathObjProvider.resetIterator( );

	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		if ( path->mFlgUsed == false ) continue;
		vector3 p_path = path->getFirstPointPosition( );
		int numPonts = path->getNumPoints( );
		CS_PATH_POINT *vertArr = path->getVertArr( );
		int startIndex, endIndex;
		startIndex = 0;
		endIndex = numPonts;
		if ( flgContainedEndPoints == false ) {
			startIndex++;
			endIndex--;
		}
		for ( int i = startIndex; i < endIndex; ++i ) {
			vector3 p_path = vertArr[ i ].p;
			float d2 = p_path.distance2( p );
			if ( distance2 < 0.0 || distance2 > d2 ) {
				objIndex = path->getImplicitIndex( );
				distance2 = d2;
				node_index = i;
			}
		}
	}

	return objIndex;
}

int CS_PATH_MANAGER::findClosestPathIndex( const vector3 &p ) const
{
	int objIndex = 0;

	float distance2 = -1.0;

	mPathObjProvider.resetIterator( );

	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		if ( path->mFlgUsed == false ) continue;
		vector3 p_path = path->getFirstPointPosition( );
		float d2 = p_path.distance2( p );
		if ( distance2 < 0.0 || distance2 > d2 ) {
			objIndex = path->getImplicitIndex( );
			distance2 = d2;
		}
	}

	return objIndex;
}

int CS_PATH_MANAGER::findOneRandomPathIndex( ) const
{
	int objIndex = -1;
	int num;
	int *UIA = mPathObjProvider.getUsedEleIndexArr( num );
	if ( num == 0 ) return objIndex;
	int index = (rand()%num);
	objIndex = UIA[ index ];

	return objIndex;
}

const CS_PATH *CS_PATH_MANAGER::getPathBasedOnPathID( int objID ) const
{
	return mPathObjProvider.getEleBasedOnObjID( objID );

}

const CS_PATH *CS_PATH_MANAGER::getPathBasedOnPathIndex( int pathIndex ) const
{
	return mPathObjProvider.getEle( pathIndex );
}

vector3 CS_PATH_MANAGER::getFirstPathPoint( int pathIndex ) const
{
	CS_PATH *path = mPathObjProvider.getEle( pathIndex );
	if ( path ) {
		return path->getFirstPointPosition( );
	}
	return vector3(0, 0, 0);
}

/*
const CS_PATH *CS_PATH_MANAGER::getPathArr( int &num ) const
{
	num = mMaxNumOfPaths;
	return mPathArr;
}
*/

BASE_OBJ *CS_PATH_MANAGER::pickPathPoint_XZPlane( float x, float z ) const
{
	BASE_OBJ *obj = 0;

	mPathObjProvider.resetIterator( );
	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		obj = path->pickPathPoint_XZPlane( x, z );
		if ( obj ) break;
	}
	return obj;
}

void CS_PATH_MANAGER::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	mPathObjProvider.resetIterator( );
	CS_PATH *path;
	
	vector3 aabbMin, aabbMax;
	bool flgFirst = true;
	while ( path = mPathObjProvider.getNext( ) ) {
		path->computeAABB( aabbMin, aabbMax );
		if ( flgFirst ) { 
			AABBmin = aabbMin;
			AABBmax = aabbMax;
			flgFirst = false;
		} else {
			mergeAABB( AABBmin, AABBmax, aabbMin, aabbMax );
		}
	}
}

//////////////////////////////////////
bool CS_PATH_MANAGER::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void CS_PATH_MANAGER::bo_setNewPosition_XZ( float x, float z )
{
}

void CS_PATH_MANAGER::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}