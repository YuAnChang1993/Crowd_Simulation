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
#include "crowdSimulation_Basic.h"
#include "../../imageReader/imageReader.h"
#include "../../openGLbasic/opengl_stuff.h"
#include "../../textureManager/textureManager.h"
#include "../../textureManager/texture.h"

void CROWD_SIMULATION_BASIC::draw_debug( ) const
{
	
	glPointSize( 5.0);
	glBegin( GL_POINTS );
	glColor3f( 1.0, 0.0, 0.0 );
	pl_glVertex3f( mDebug_Path_Q0 );
	glColor3f( 0.0, 0.7, 0.0 );
	pl_glVertex3f( mDebug_Path_Q1 );

	glEnd( );
	glPointSize( 1.0 );

}

void CROWD_SIMULATION_BASIC::draw( ) const
{
	/*
	if ( mCrowdMonitorManager ) {
		mCrowdMonitorManager->draw( );
	}
	*/
	//CROWD_MONITOR_MANAGER::instance( )->draw( );

	draw_debug( );
}