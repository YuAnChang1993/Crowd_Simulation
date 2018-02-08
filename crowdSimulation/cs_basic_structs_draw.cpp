//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include "../headers.h"
#include "cs_basic_structs.h"
#include "../openGLbasic/opengl_stuff.h"
#include "cs_parameters.h"

bool insideAABB( const vector3 &mAABBmin, const vector3 &mAABBmax, const vector3 &q )
{
	return ( q.x >= mAABBmin.x && mAABBmax.x >= q.x 
		&&
		q.z >= mAABBmin.z && mAABBmax.z >= q.z 
		);
}

bool insideAABB_lineSegment_XZ( const vector3 &mAABBmin, const vector3 &mAABBmax, const vector3 &q0, const vector3 &q1 )
{
	bool flg = false;
	if ( ( q0.x < mAABBmin.x && q1.x < mAABBmin.x )
		||
		( q0.z < mAABBmin.z && q1.z < mAABBmin.z )
		||
		( q0.x > mAABBmax.x && q1.x > mAABBmax.x )
		||
		( q0.z > mAABBmax.z && q1.z > mAABBmax.z )
		) {
		return false;
	}

	if ( insideAABB( mAABBmin, mAABBmax, q0 ) 
		&&
		insideAABB( mAABBmin, mAABBmax, q1 ) 	
		) 
	{
		return true;
	}
	float t;
	float dd;
	dd = q1.z - q0.z;
	if ( dd != 0.0 ) {
		float dz = mAABBmin.z - q0.z;
		t = dz/dd;
		if ( t >= 0.0 && 1.0 >= t ) {
			float x = q0.x + t*( q1.x - q0.x );
			if ( x >= mAABBmin.x && mAABBmax.x >= x ) return true;
		}
	}
	
	if ( q1.z - q0.z != 0.0 ) {
		float dz = mAABBmax.z - q0.z;
		t = dz/dd;
		if ( t >= 0.0 && 1.0 >= t ) {
			float x = q0.x + t*( q1.x - q0.x );
			if ( x >= mAABBmin.x && mAABBmax.x >= x ) return true;
		}
	}

	dd = q1.x - q0.x;
	if ( dd != 0.0 ) {
		float dx = mAABBmin.x - q0.x;
		t = dx/dd;
		if ( t >= 0.0 && 1.0 >= t ) {
			float z = q0.z + t*( q1.z - q0.z );
			if ( z >= mAABBmin.z && mAABBmax.z >= z ) return true;
		}
	}

	if ( dd != 0.0 ) {
		float dx = mAABBmax.x - q0.x;
		t = dx/dd;
		if ( t >= 0.0 && 1.0 >= t ) {
			float z = q0.z + t*( q1.z - q0.z );
			if ( z >= mAABBmin.z && mAABBmax.z >= z ) return true;
		}
	}

	return flg;
}

void mergeAABB( vector3 &AABBmin, vector3 &AABBmax, const vector3 &aabbMin, const vector3 &aabbMax )
{
	if ( AABBmin.x > aabbMin.x ) {
		AABBmin.x = aabbMin.x;
	}
	if ( AABBmin.y > aabbMin.y ) {
		AABBmin.y = aabbMin.y;
	}
	if ( AABBmin.z > aabbMin.z ) {
		AABBmin.z = aabbMin.z;
	}
	//
	if ( AABBmax.x < aabbMax.x ) {
		AABBmax.x = aabbMax.x;
	}
	if ( AABBmax.y < aabbMax.y ) {
		AABBmax.y = aabbMax.y;
	}
	if ( AABBmax.z < aabbMax.z ) {
		AABBmax.z = aabbMax.z;
	}
}

void CS_PATH_POINT::bo_drawWithHighLight( ) const
{
	glColor3f(1.0, 0.5, 0.0);
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 6 );
}

void CS_PATH_POINT::bo_drawWithHighLight( float r, float g, float b ) const
{
		glColor3f( r, g, b );
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 6 );
}

void CS_PATH::bo_drawWithHighLight( ) const 
{
	glColor3f(1.0, 0.5, 0.0);
}

void CS_PATH::bo_drawWithHighLight( float r, float g, float b ) const 
{
	glColor3f( r, g, b );
}

void CS_PATH::drawAffectintPoints( ) const
{
	vector3 color_pos_near_p0( 0.0, 1.0, 0.0 );
	vector3 color_pos_near_p1( 0.0, 0.5, 0.0 );
	vector3 color_neg_near_p0( 1.0, 0.0, 0.0 );
	vector3 color_neg_near_p1( 0.5, 0.0, 0.0 );

	vector3 color_p[4];
	color_p[0] = color_pos_near_p0;
	color_p[1] = color_pos_near_p1;
	color_p[2] = color_neg_near_p0;
	color_p[3] = color_neg_near_p1;

	glPointSize( 5.0 );
	glBegin( GL_POINTS );
	for ( int i = 0; i < numPoints - 1; ++i ) {
		for ( int j = 0; j < 4; ++j ) {
			float d2 = affectingPointD2[ j ][i];
			if ( d2 < 0.0 ) continue;
			pl_glColor3f( color_p[j]  );
			pl_glVertex3f( affectingSelfPointArr[j][i] );
			pl_glVertex3f( affectingPointArr[j][i] );
		}
	}
	glEnd( );
	//
		glLineWidth( 1.0 );
	glBegin( GL_LINES );
	for ( int i = 0; i < numPoints - 1; ++i ) {
		for ( int j = 0; j < 4; ++j ) {
			float d2 = affectingPointD2[ j ][i];
			if ( d2 < 0.0 ) continue;
			pl_glColor3f( color_p[j]  );
			pl_glVertex3f( affectingSelfPointArr[j][i] );
			pl_glVertex3f( affectingPointArr[j][i] );
		}
	}
	glEnd( );
}


void CS_PATH::draw( ) const
{
	if ( numPoints == 0 ) return;
	//

	glLineWidth(3);

	vector3 c0 = vector3( 0.9, 0.9, 0.9 );
	vector3 c1 = vector3( 0.4, 0.2, 0.1 );

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


	float zLayer = 0.0;

	glBegin( GL_LINES );
	if ( csp_flgShowPathLoad ) {
		
		for ( int i = 0; i < numPoints-1; ++i ) {
			vector3 q0 = ptArr[i].p;
			vector3 q1 = ptArr[i+1].p;
//
			float L = q0.distance( q1 );
			float w = mEdgeLoadArr[mEdgeLoadIndex][ i ]/L;
//
			csp_ColorTable_PathLoad.updateMaxValue( w );
			c0 = csp_ColorTable_PathLoad.getColor_Linear( w );
		pl_glColor3f( c0 );
		pl_glVertex3f( q0 );
		c1 = c0*0.75;
		pl_glColor3f( c1 );
		pl_glVertex3f( q1 );
	}
	} else {
		for ( int i = 0; i < numPoints-1; ++i ) {
		pl_glColor3f( c0 );
		pl_glVertex3f( ptArr[i].p );
		pl_glColor3f( c1 );
		pl_glVertex3f( ptArr[i+1].p );
	}
	}
	glEnd( );
	
	
	//float *rArr;


	pl_glColor3f( c1 );
	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p;
		p = ptArr[i].p;
		float r = ptArr[i].r;
		ogl_drawSimpleCircleXZ( p.x, zLayer, p.z, r, 12 );
	}

	if ( m_bo_ObjState == CS_OBJECT_STATE_NEW ) {
		glColor3f( 0.8, 0.8, 0.8 );
		vector3 p1 = ptArr[numPoints-1].p;
		float r = ptArr[numPoints-1].r;
		glBegin( GL_LINES );
		pl_glVertex3f( m_bo_TmpPointForDefiningNewObj );
		pl_glVertex3f( p1 );
		glEnd( );
		ogl_drawSimpleCircleXZ( p1.x, zLayer, p1.z, r, 12 );

	}

	if ( m_bo_CurPickedVertexIndex >= 0 ) {
		c0 = color_pickedObj_c0;
		c1 = color_pickedObj_c1;
		glLineWidth(3);
		pl_glColor3f( c1 );
		vector3 p = ptArr[m_bo_CurPickedVertexIndex].p;
		float r = ptArr[m_bo_CurPickedVertexIndex].r;
		ogl_drawSimpleCircleXZ( p.x, zLayer, p.z, r, 12);
	}

	if ( m_bo_ObjState == CS_OBJECT_STATE_EDIT ) {
		glLineWidth(3);
		glColor3f( 0.8, 0.8, 0.0 );
		ogl_drawUnFilledRectXZ( mAABBmin.x, mAABBmax.x, mAABBmin.z, mAABBmax.z, zLayer );
	}

	//
	if ( csp_flgShowPathLoad ) {
		
glBegin( GL_LINES );
		for ( int i = 0; i < numPoints-1; ++i ) {
			vector3 p0 = ptArr[i].p;
			vector3 p1 = ptArr[i+1].p;
			vector3 n = (p0 - p1).cross( vector3( 0.0, -1.0, 0.0 ) );
			n.Normalize( );
//
			//float L = p0.distance( p1 );
			float w = mExpectedPathWidth[ i ];
//
			csp_ColorTable_PathLoad.updateMaxValue( w );
			c0 = csp_ColorTable_PathLoad.getColor_Linear( w );
			float thickness = w;

			vector3 q0, q1;
			q0 = p0 + n*thickness;
			q1 = p1 + n*thickness;

		pl_glColor3f( c0 );
		pl_glVertex3f( q0 );
		c1 = c0*0.75;
		pl_glColor3f( c1 );
		pl_glVertex3f( q1 );

		q0 = p0 - n*thickness;
		q1 = p1 - n*thickness;
		pl_glColor3f( c0 );
		pl_glVertex3f( q0 );
		c1 = c0*0.75;
		pl_glColor3f( c1 );
		pl_glVertex3f( q1 );

	}
		glEnd( );
	}

	//
	drawAffectintPoints( );
}