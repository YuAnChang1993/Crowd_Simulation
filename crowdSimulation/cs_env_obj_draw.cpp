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
#include "cs_env_obj.h"

using namespace std;

void CS_ENV_OBJECT::draw( ) const
{
	switch( m_bo_ObjType ) {
		case CS_ENV_OBJECT_TYPE_RECTANGLE:
			draw_Rect( );
			break;
		case CS_ENV_OBJECT_TYPE_POLYGON:
			draw_Polygon( );
			break;
	}
}

void CS_ENV_OBJECT::draw_Polygon( ) const
{
	int numPoints = ptArr.size( );
	if ( numPoints == 0 ) return;
	glDisable(GL_CULL_FACE);
	glLineWidth(6);
	
	vector3 c0 = vector3( 0.9, 0.9, 0.9 );
	vector3 c1 = vector3( 0.4, 0.4, 0.1 );

	if ( mFlgEnabledStatusAsAnObstacle == false ) 
	{
		c0 = vector3( 0.0, 1.0, 0.0 );
		c1 = vector3( 1.0, 1.0, 0.0 );
	}
	vector3 color_pickedObj_c0 = vector3( 0.9, 0.9, 0.9 );
	vector3 color_pickedObj_c1 = vector3( 0.9, 0.2, 0.2 );

	vector3 color_pickedVertex_c0 = vector3( 0.9, 0.9, 0.9 );
	vector3 color_pickedVertex_c1 = vector3( 0.9, 0.2, 0.2 );

	vector3 color_hit_Vertex_c0 = vector3( 0.5, 0.5, 0.5 );
	vector3 color_hit_Vertex_c1 = vector3( 0.0, 0.2, 0.5 );

	switch( m_bo_ObjState ) {
		case CS_OBJECT_STATE_NEW:
			break;
		case CS_OBJECT_STATE_EDIT:
			if ( m_bo_CurPickedEdgeIndex >= 0 ) {
				c0 = color_pickedObj_c0;
				c1 = color_pickedObj_c1;
			} else {
			}
			break;
		case CS_OBJECT_STATE_HIT:
			c0 = color_hit_Vertex_c0;
			c1 = color_hit_Vertex_c1;
			break;
	}

	float zLayer = 1.0;
	
	glBegin( GL_LINES );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p0 = ptArr[i];
		vector3 p1 = ptArr[(i+1)%numPoints];
		glColor3f( c0.x, c0.y, c0.z );
		glVertex3f( p0.x, zLayer, p0.z );
		glColor3f( c1.x, c1.y, c1.z );
		glVertex3f( p1.x, zLayer, p1.z );
	}
	glEnd( );

	
	//c0 = color_pickedVertex_c1;
	//glColor3f( c0.x, c0.y, c0.z );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p = ptArr[i];
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, mCornerThickness_Visual, 12);
	}

	if ( m_bo_CurPickedVertexIndex >= 0 ) {
		c0 = color_pickedObj_c1;
		glColor3f( c0.x, c0.y, c0.z );
		vector3 p = ptArr[m_bo_CurPickedVertexIndex];
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, mCornerThickness_Visual, 12);

	}
	

	if ( m_bo_ObjState == CS_OBJECT_STATE_NEW ) {
		glColor3f( 0.8, 0.8, 0.8 );
		vector3 p0 = ptArr[0] ;
		vector3 p1 = ptArr[numPoints-1] ;
		glBegin( GL_LINES );
		pl_glVertex3f( mTmpPointForDefiningNewObj );
		pl_glVertex3f( p0 );
		pl_glVertex3f( mTmpPointForDefiningNewObj );
		pl_glVertex3f( p1 );
		glEnd( );
	}

	if ( m_bo_ObjState == CS_OBJECT_STATE_EDIT ) {
		glLineWidth(3);
		glColor3f( 0.8, 0.8, 0.0 );
		ogl_drawUnFilledRectXZ( mAABBmin.x, mAABBmax.x, mAABBmin.z, mAABBmax.z, zLayer );
	}
}

void CS_ENV_OBJECT::draw_Rect( ) const
{

	vector3 p0, p1;
	p0 = ptArr[0];
	p1 = ptArr[1];

	glDisable(GL_CULL_FACE);
	glLineWidth(6);
	glColor3f( 0.6, 0.2, 0.1 );

	if ( textureID >= 0 ) {
		glColor3f( 1.0, 1.0, 1.0 );
		ns_opengl::TEXTURE_MANAGER::instance( )->bgn_use( textureID );
		ogl_drawFilledRectXZ_withTexture( p0.x, p1.x, p0.z, p1.z, 1.0 );
		ns_opengl::TEXTURE_MANAGER::instance( )->end_use( textureID );

	} else {
		ogl_drawFilledRectXZ( p0.x, p1.x, p0.z, p1.z, 1.0 );
	}
	
}

void CS_ENV_OBJECT::draw( float zLayer ) const
{

	vector3 p0, p1;
	p0 = ptArr[0];
	p1 = ptArr[1];

	glDisable(GL_CULL_FACE);
	glLineWidth(6);
	//glColor3f( 0.6, 0.2, 0.1 );
	glColor3f( color.r, color.g, color.b );

	if ( textureID >= 0 ) {
		glColor3f( 1.0, 1.0, 1.0 );
		ns_opengl::TEXTURE_MANAGER::instance( )->bgn_use( textureID );
		ogl_drawFilledRectXZ_withTexture( p0.x, p1.x, p0.z, p1.z, zLayer );
		ns_opengl::TEXTURE_MANAGER::instance( )->end_use( textureID );

	} else {
		ogl_drawFilledRectXZ( p0.x, p1.x, p0.z, p1.z, zLayer );
	}
}

void CS_ENV_OBJECT::draw_for_cd( float zLayer, float thickness ) const
{
	//
	thickness = mCornerThickness_CD;
	//
	glLineWidth( 2.0 );
	glColor3f( 0.0, 0.0, 0.0 );
	int numPoints = ptArr.size( );
	glBegin( GL_LINES );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p0 = ptArr[i];
		vector3 p1 = ptArr[(i+1)%numPoints];
		glVertex3f( p0.x, zLayer, p0.z );
		glVertex3f( p1.x, zLayer, p1.z );
	}
	glEnd( );

	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p = ptArr[i];
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, thickness, 12);
	}

	glColor3f( 0.5, 0.5, 0.5 );
	glBegin( GL_LINES );
	/*
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p0 = ptArr[i];
		vector3 p1 = ptArr[(i+1)%numPoints];
		vector3 q0 = vector3( p0.x, zLayer, p0.z );
		vector3 q1 = vector3( p1.x, zLayer, p1.z );
		//
		vector3 n = ( p1 - p0 ).cross( vector3(0.0, -1.0, 0.0) );
		n.Normalize( );
		q0 = q0 + thickness*n;
		q1 = q1 + thickness*n;
		//
		pl_glVertex3f( p0 );
		pl_glVertex3f( (p0 + n) );
		pl_glVertex3f( q0 );
		pl_glVertex3f( q1 );
	}
	*/
	//draw the boundary
	glColor3f( 0.0, 0.5, 0.0 );
	glBegin( GL_LINES );
	for ( int i = 0; i < ptArr_extendedObj.size( ); ++i ) {
		vector3 p0 = ptArr_extendedObj[i].p0;
		vector3 p1 = ptArr_extendedObj[i].p1;
		pl_glVertex3f( p0 );
		pl_glVertex3f( p1 );
	}

	glEnd( );

	//draw the normal direction
	glColor3f( 0.5, 0.5, 0.5 );
	glBegin( GL_LINES );
	for ( int i = 0; i < ptArr_extendedObj.size( ); ++i ) {
		vector3 p0 = ptArr_extendedObj[i].p0;

		vector3 p1 = ptArr_extendedObj[i].p1;
		vector3 n = ptArr_extendedObj[i].n*5.0;
		vector3 p = (p0 + p1)*0.5;
		pl_glVertex3f( p );
		pl_glVertex3f( ( p + n ) );
	}

	glEnd( );
}