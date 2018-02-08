//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "../headers.h"
#include "cs_basic_structs.h"
#include "../openGLbasic/opengl_stuff.h"
#include "environment.h"
#include "../textureManager/textureManager.h"
using namespace std;



void CS_ENVIRONMENT::draw_PickedObj( ) const
{
	if ( mCurPickedObj == 0 ) return;
	if ( mCurPickedObj 
		&& mCurPickedObj->getUsedFlg( ) 
		&& mCurPickedObj->getActivatedFlg( )
		) {
		mCurPickedObj->draw( );
	}
}

void CS_ENVIRONMENT::drawMap( ) const
{
	if ( mMapObj ) {
		//cout << "mMapObj->draw" << endl;
		mMapObj->draw( 5.0 );
	}
}

void drawBVH_AABB( const BVH_NODE *node ) 
{
	vector3 p = node->AABBmax;
	vector3 q = node->AABBmin;
	glBegin(GL_LINE_LOOP);
	pl_glVertex3f( p );
	glVertex3f( q.x, p.y, p.z );
	pl_glVertex3f( q );
	glVertex3f( p.x, p.y, q.z );
	glEnd( );
}

void CS_ENVIRONMENT::drawBVH( const BVH_NODE *node, int cur_depth, int max_depth ) const
{
	if ( node == 0 ) return;
	if ( cur_depth > max_depth ) return;
	drawBVH_AABB( node );
	drawBVH( node->left, cur_depth+1, max_depth );
	drawBVH( node->right, cur_depth+1, max_depth );

}

void CS_ENVIRONMENT::drawBVH( ) const
{
	if ( mBVHRoot == 0 ) return;
	int depth = 8;
	glColor3f( 0.5, 0.5, 0.5);
	glLineWidth(2.0);
	drawBVH( mBVHRoot, 0, depth );
	glLineWidth(1.0);

}

void CS_ENVIRONMENT::draw( ) const
{
	mEnvObjProvider.resetIterator( );

	CS_ENV_OBJECT *envObj;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		envObj->draw( );
		envObj->draw_for_cd( 1.0, 2.0 );
	}

	draw_PickedObj( );

	if ( mCurHitObj && (mCurHitObj != mCurPickedObj ) 
		&& mCurHitObj->getUsedFlg( ) && mCurHitObj->getActivatedFlg( )
		) {
		glLineWidth(3.0);
		glColor3f(0.2, 0.2, 0.2);
		mCurHitObj->draw( );
	}

	if ( mTmpEnvObject.ptArr.size( ) != 0 ) {
		mTmpEnvObject.draw( );
	}

	drawBVH();
}