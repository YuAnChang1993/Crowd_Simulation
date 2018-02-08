//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "crowd_monitor_manager.h"
#include "../openGLbasic/opengl_stuff.h"

using namespace std;

void CROWD_MONITOR_MANAGER::draw( ) const
{
	//cout << "CROWD_MONITOR_MANAGER::draw( )" << endl;
	glLineWidth(3.0);
	glColor3f(1.0, 0.7, 0.0);
	
	int count = 0;
	mObjProvider.resetIterator( );
	CROWD_MONITOR *g;
	while ( g = mObjProvider.getNext( ) ) {
		vector3 p;
		float r;
		g->getInfo( p, r );
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
		++count;
	}
	//cout << "crowd monitor number:" <<count << endl;
}
