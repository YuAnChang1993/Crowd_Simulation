
#include <iostream>
#include "../headers.h"
#include "pathManager.h"
#include "../openGLbasic/opengl_stuff.h"
using namespace std;

void CS_PATH_MANAGER::bo_drawWithHighLight( ) const 
{
}

void CS_PATH_MANAGER::bo_drawWithHighLight( float r, float g, float b ) const
{
}


void CS_PATH_MANAGER::draw_PickedObj( ) const
{
	if ( mCurPickedObj == 0 ) return;
	mCurPickedObj->draw( );
}

void CS_PATH_MANAGER::draw( ) const
{
	glLineWidth( 5.0 );
	

	glLineWidth( 5.0 );
	glColor3f( 0.5, 0.5, 0.0 );

	mPathObjProvider.resetIterator( );
	CS_PATH *path;

	while ( path = mPathObjProvider.getNext( ) ) {
		path->draw( );
	}

	mTmpObject.draw( );

	draw_PickedObj( );
}


