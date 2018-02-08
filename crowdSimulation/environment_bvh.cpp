//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include "environment.h"
#include "./cs_parameters.h"

using namespace std;

void computeAABB(
	vector3 &AABBmin, 
	vector3 &AABBmax, 
	const vector<CS_ENV_OBJECT*> &objArr)
{
	if ( objArr.size( ) == 0 ) return;
	const CS_ENV_OBJECT *obj = objArr[0];
	
	obj->getAABB( AABBmin, AABBmax );
	for ( int i = 1; i < objArr.size( ); ++i ) {
		vector3 aabbMin, aabbMax;
		const CS_ENV_OBJECT *obj = objArr[i];
		obj->getAABB( aabbMin, aabbMax );
		mergeAABB( AABBmin, AABBmax, aabbMin, aabbMax );
	}
}

BVH_NODE::BVH_NODE( )
{
	obj = 0;
	left = right = 0;
}

BVH_NODE *CS_ENVIRONMENT::buildBVHOfObjects( const vector<CS_ENV_OBJECT*> &objArr )
{
	BVH_NODE *node = 0;
	int numObjs = objArr.size( );
	if ( numObjs == 0 ) return node;
	vector3 AABBmin, AABBmax;
	::computeAABB(AABBmin, AABBmax, objArr);
	//
	node = new BVH_NODE;
	node->AABBmin = AABBmin;
	node->AABBmax = AABBmax;
	
	if ( numObjs == 1 ) {
		node->obj = objArr[ 0 ];
		return node;
	}

	BVH_NODE *left_node, *right_node;
	vector<CS_ENV_OBJECT*> L;
	vector<CS_ENV_OBJECT*> R;
	if ( numObjs == 2 ) {
		L.push_back(objArr[ 0 ]);
		left_node = buildBVHOfObjects( L );
		R.push_back(objArr[ 1 ]);
		right_node = buildBVHOfObjects( R );
	} else {
		float x_length = AABBmax[0] - AABBmin[0];
		float z_length = AABBmax[2] - AABBmin[2];
		int index = 0;
		if ( z_length > x_length ) { index = 2; }
		vector3 center = (AABBmax + AABBmin)*0.5;
		for ( int i = 0; i < objArr.size( ); ++i ) {
			vector3 aabbMin, aabbMax;
			CS_ENV_OBJECT *obj = objArr[i];
			obj->getAABB( aabbMin, aabbMax );
			vector3 obj_center = (aabbMax + aabbMin)*0.5;
			if ( obj_center[index] > center[index] ) {
				R.push_back( obj );
			} else {
				L.push_back( obj );
			}
		}
		if ( R.size( ) == 0 ) {
			R.push_back( L.back( ) );
			L.resize( L.size() - 1 );
		} else if ( L.size( ) == 0 ) {
				L.push_back( R.back( ) );
			R.resize( R.size() - 1 );
		}
		left_node = buildBVHOfObjects(L);
		right_node = buildBVHOfObjects(R);

	}

	node->left = left_node;
	node->right = right_node;
	return node;
}

void CS_ENVIRONMENT::buildBVHOfObjects( )
{
	mBVHRoot = 0;
	CS_ENV_OBJECT *objArr = mEnvObjProvider.getObjArr();
	int numObj;
	int *usedIndexArr = mEnvObjProvider.getUsedEleIndexArr( numObj );
	cout << "numObj:" << numObj << endl;

	vector<CS_ENV_OBJECT *> v_objArr;
	for ( int i = 0; i < numObj; ++i ) {
		v_objArr.push_back( &objArr[usedIndexArr[i]] );
	}

	mBVHRoot = buildBVHOfObjects( v_objArr );
}

void traverseBVH( const BVH_NODE *node, vector<CS_ENV_OBJECT *> &objArr, const vector3 & q0, const vector3 & q1, float r )
{
	if ( !insideAABB_lineSegment_XZ( node->AABBmin, node->AABBmax, q0, q1 ) ) return;

	if ( node->obj ) {
		objArr.push_back( node->obj);
		return;
	}
	traverseBVH(node->left, objArr, q0, q1, r );
	traverseBVH(node->right, objArr, q0, q1, r );


}

void CS_ENVIRONMENT::traverseBVH( vector<CS_ENV_OBJECT *> &objArr, const vector3 & q0, const vector3 & q1, float r ) const
{
	if ( mBVHRoot == 0 ) return;
	::traverseBVH( mBVHRoot, objArr, q0, q1, r);
}

//bool flg = envObj->isContained( q0, q1, r );
		//if ( flg == false ) continue;