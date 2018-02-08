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
#include "teleporter.h"
#include "../textureManager/textureManager.h"
using namespace std;

	void TELEPORTER::bo_drawWithHighLight( ) const
	{
	}

	void TELEPORTER::bo_drawWithHighLight( float r, float g, float b ) const
	{

	}

void TELEPORTER::draw( ) const
{
	glLineWidth(3.0);
	vector3 color_src( 0.0, 0.5, 0.0 );
	pl_glColor3f( color_src );
	vector3 p = mSourcePoint;
	float r = mSourceRadius;
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );

	vector3 color_dst( 0.2, 0.0, 0.5 );
	p = mTargetPoint;
	r = mTargetRadius;
	pl_glColor3f( color_dst );

	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
}
