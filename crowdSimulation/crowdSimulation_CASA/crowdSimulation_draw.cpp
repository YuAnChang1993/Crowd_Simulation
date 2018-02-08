//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "../../headers.h"
#include <string>
#include "crowdSimulation.h"
#include "../../imageReader/imageReader.h"
#include "../../openGLbasic/opengl_stuff.h"
#include "../../textureManager/textureManager.h"
#include "../../textureManager/texture.h"

void CROWD_SIMULATION::drawAgent( const CS_AGENT &ag ) const
{
	vector3 p = ag.p;
	//printf_vector3("agent position:", p );
	float r = ag.r;
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix( );
	glTranslatef( p.x, p.y, p.z );
	glBegin(GL_POINTS);
	//glVertex3f(0.0, 0.0, 0.0);
	//glutSolidSphere( r*10, 5, 5 );
	glEnd( );
	glPopMatrix( );
}

void CROWD_SIMULATION::drawPath( const CS_PATH &path )  const
{
	//path.draw( );
}

void CROWD_SIMULATION::drawPaths( )  const
{
	/*
	glLineWidth( 5.0 );
	glColor3f( 0.5, 0.5, 0.0 );
	int numPaths;
	const CS_PATH *pathArr = CS_PATH_MANAGER::instance()->getPathArr( numPaths );
	for ( int i = 0; i < numPaths; ++i ) {
		drawPath( pathArr[i] );
	}
	*/
}

void CROWD_SIMULATION::drawAgents( )  const
{
	

	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		vector3 color = mAgentColor;
		int group_id = agent->group_id;
		group_id = group_id%4;
		switch ( group_id ) {
			case 0:
				color = mAgentColor;
			break;
			case 1:
				color = vector3( 1.0, 0.0, 0.0 );
				break;
			case 2:
				color = vector3( 0.0, 1.0, 0.0 );
				break;
			case 3:
				color = vector3( 0.5, 0.0, 0.5 );
				break;
		}
		agent->draw( color );
	}

	
	if ( mPickedObj ) {
		mPickedObj->bo_drawWithHighLight( mHighLightedAgentColor.r, mHighLightedAgentColor.g, mHighLightedAgentColor.b );
	}
	
}

void CROWD_SIMULATION::drawAgentGenerators( ) const
{
	glLineWidth(3.0);
	glColor3f(1.0, 1.0, 1.0);

	mAgentGeneratorManager->resetIterator( );
	CS_AGENT_GENERATOR *obj;
	while ( obj = mAgentGeneratorManager->getNext( ) ) {
		vector3 p;
		float r;
		obj->getGenerationPosition( p, r );
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
	}
}

void CROWD_SIMULATION::drawMap( ) const
{
	 CS_ENVIRONMENT::instance( )->drawMap( );

}

void CROWD_SIMULATION::drawEnvironment( ) const
{
	 CS_ENVIRONMENT::instance( )->draw( );
}

void mapSeparationDistanceToColor( float sd, vector3 &c )
{
	float d = 0.09;
	vector3 c0, c1;
	c0 = vector3( 1.0, 1.0, 1.0 );
	c1 = vector3( 1.0, 0.0, 0.0 );
	float f = sd/d;
	if ( f > 1.0) f = 1.0;
	c = (1-f)*c0 + f*c1;
	
	//if ( sd > 0.0) cout << sd << endl;
}

void CROWD_SIMULATION::drawSeparationDistanceMap( ) const
{
	int nx, nz;
	float *grid = mAgentSeparationDistance_Map->getGrid( nx, nz );
	int *counter = mAgentSeparationDistance_Map->getGridCounter( );
	float minx, maxx, minz, maxz;
	mAgentSeparationDistance_Map->getDimension( minx, maxx, minz, maxz );

	float x, y, z;
	y = 2.0;
	float x0, x1;
	float z0, z1;
	float dx = (maxx - minx)/(nx-1);

	float dz = (maxz - minz)/(nz-1);
	//
	//cout << "minx:" << minx << endl;
	//cout << "maxx:" << maxx << endl;

	//
	vector3 c00, c01, c10, c11;
	glBegin( GL_TRIANGLES );
	for ( int i = 0; i < nx-1; ++i ) {
		x0 = minx + i*dx;
		x1 = x0 + dx;
		for ( int j = 0; j < nz-1; ++j ) {
			z0 = minz + j*dz;
			z1 = z0 + dz;
			int i00 = i*nz + j;
			int i01 = i00 + 1;
			int i10 = i00 + nz;
			int i11 = i10 + 1;
			if ( counter[i00] == 0) { 
				mapSeparationDistanceToColor( 0.0, c00 );
			} else mapSeparationDistanceToColor( grid[i00], c00 );

			if ( counter[i01] == 0) { 
				mapSeparationDistanceToColor( 0.0, c01 );
			} else mapSeparationDistanceToColor( grid[i01], c01 );

			if ( counter[i10] == 0) { 
				mapSeparationDistanceToColor( 0.0, c10 );
			} else mapSeparationDistanceToColor( grid[i10], c10 );

			if ( counter[i11] == 0) { 
				mapSeparationDistanceToColor( 0.0, c11 );
			} else mapSeparationDistanceToColor( grid[i11], c11 );

			/*
			mapSeparationDistanceToColor( grid[i00], c00 );
			mapSeparationDistanceToColor( grid[i01], c01 );
			mapSeparationDistanceToColor( grid[i10], c10 );
			mapSeparationDistanceToColor( grid[i11], c11 );
			*/
			

			pl_glColor3f( c00 );
			glVertex3f( x0, y, z0 );
			pl_glColor3f( c10 );
			glVertex3f( x1, y, z0 );
			pl_glColor3f( c11 );
			glVertex3f( x1, y, z1 );

			pl_glColor3f( c00 );
			glVertex3f( x0, y, z0 );
			pl_glColor3f( c11 );
			glVertex3f( x1, y, z1 );
			pl_glColor3f( c01 );
			glVertex3f( x0, y, z1 );

		}
	}
	glEnd( );
}

void mapAgentMovementToColor( float sd, vector3 &c )
{
	float d = 1.0;
	vector3 c0, c1;
	c0 = vector3( 1.0, 1.0, 1.0 );
	c1 = vector3( 0.0, 0.0, 0.0 );
	float f = sd/d;
	if ( f > 1.0) f = 1.0;
	c = (1-f)*c0 + f*c1;
	
	//if ( sd > 0.0) cout << sd << endl;
}

void CROWD_SIMULATION::drawAgentMovementMap( ) const
{
	int nx, nz;
	float *grid = mAgentMovementPath_Map->getGrid( nx, nz );
	int *counter = mAgentMovementPath_Map->getGridCounter( );
	float minx, maxx, minz, maxz;
	mAgentMovementPath_Map->getDimension( minx, maxx, minz, maxz );

	float x, y, z;
	y = 2.0;
	float x0, x1;
	float z0, z1;
	float dx = (maxx - minx)/(nx-1);

	float dz = (maxz - minz)/(nz-1);
	//
	//cout << "minx:" << minx << endl;
	//cout << "maxx:" << maxx << endl;

	//
	vector3 c00, c01, c10, c11;
	glBegin( GL_TRIANGLES );
	for ( int i = 0; i < nx-1; ++i ) {
		x0 = minx + i*dx;
		x1 = x0 + dx;
		for ( int j = 0; j < nz-1; ++j ) {
			z0 = minz + j*dz;
			z1 = z0 + dz;
			int i00 = i*nz + j;
			int i01 = i00 + 1;
			int i10 = i00 + nz;
			int i11 = i10 + 1;
			if ( counter[i00] == 0 
				&&
				counter[i01] == 0 
				&&
				counter[i10] == 0 
				&&
				counter[i11] == 0 
				) continue;
			if ( counter[i00] == 0) { 
				mapAgentMovementToColor( 0.0, c00 );
			} else mapAgentMovementToColor( grid[i00], c00 );

			if ( counter[i01] == 0) { 
				mapAgentMovementToColor( 0.0, c01 );
			} else mapAgentMovementToColor( grid[i01], c01 );

			if ( counter[i10] == 0) { 
				mapAgentMovementToColor( 0.0, c10 );
			} else mapAgentMovementToColor( grid[i10], c10 );

			if ( counter[i11] == 0) { 
				mapAgentMovementToColor( 0.0, c11 );
			} else mapAgentMovementToColor( grid[i11], c11 );

			/*
			mapSeparationDistanceToColor( grid[i00], c00 );
			mapSeparationDistanceToColor( grid[i01], c01 );
			mapSeparationDistanceToColor( grid[i10], c10 );
			mapSeparationDistanceToColor( grid[i11], c11 );
			*/
			

			pl_glColor3f( c00 );
			glVertex3f( x0, y, z0 );
			pl_glColor3f( c10 );
			glVertex3f( x1, y, z0 );
			pl_glColor3f( c11 );
			glVertex3f( x1, y, z1 );

			pl_glColor3f( c00 );
			glVertex3f( x0, y, z0 );
			pl_glColor3f( c11 );
			glVertex3f( x1, y, z1 );
			pl_glColor3f( c01 );
			glVertex3f( x0, y, z1 );
		}
	}
	glEnd( );
}

void CROWD_SIMULATION::drawLatticeSpace( ) const
{
	float zLayer = 2.0;
	vector3 minExtent, maxExtent;
	int nx, ny, nz;
	float minx, maxx, miny, maxy, minz, maxz;
	mLatticeManager->getInfo(nx, ny, nz, minx, maxx, miny, maxy, minz, maxz );

	minExtent = vector3( minx, miny, minz );
	maxExtent = vector3( maxx, maxy, maxz );

	vector3 q0, q1, q2, q3;
	q0 = minExtent;
	q1 = q0;
	q2 = maxExtent;
	q3 = q2;
	q1.x = q2.x;
	q3.x = q0.x;

	q0.y = q1.y = q2.y = q3.y = zLayer;

	glLineWidth( 3.0 );
	vector3 color_boundary = vector3( 0.0, 0.0, 1.0 );
	vector3 color_interior = vector3( 0.6, 0.6, 1.0 );


	pl_glColor3f( color_boundary );
	glBegin( GL_LINE_LOOP );
	pl_glVertex3f( q0 );
	pl_glVertex3f( q1 );
	pl_glVertex3f( q2 );
	pl_glVertex3f( q3 );
	glEnd( );

	pl_glColor3f( color_interior );
	q0.z = minz;
	q1.z = maxz;
	float dx = (maxx - minx)/nx;
	glBegin( GL_LINES );
	for ( int i = 0; i < nx; ++i ) {
		q0.x = minx + i*dx;
		q1.x = minx + i*dx;
		pl_glVertex3f( q0 );
		pl_glVertex3f( q1 );
	}
	//
	float dz = (maxz - minz)/nz;
	q0.x = minx;
	q1.x = maxx;
	for ( int i = 0; i < nz; ++i ) {
		q0.z = minz + i*dz;
		q1.z = minz + i*dz;
		pl_glVertex3f( q0 );
		pl_glVertex3f( q1 );
	}
	glEnd( );
}

void CROWD_SIMULATION::draw( ) const
{

}
