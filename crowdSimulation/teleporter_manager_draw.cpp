//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/10/12
//
#include <iostream>
#include "../headers.h"
#include "cs_basic_structs.h"
#include "../openGLbasic/opengl_stuff.h"
#include "teleporter_manager.h"
#include "../textureManager/textureManager.h"
using namespace std;

void TELEPORTER_MANAGER::draw( ) const
{
		mObjProvider.resetIterator( );

	TELEPORTER *obj;
	while ( obj = mObjProvider.getNext( ) ) {
		obj->draw( );
	}
}