
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/26
//
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "pathManager.h"
#include "pathManager_data_format.h"

using namespace std;

namespace {
	

	void save_Path( ofstream *fp, int pathID, const CS_PATH &obj )
	{
		
		if ( fp == 0 ) return;
		if ( obj.mFlgUsed == false ) return;

		*fp << PATH_MANAGER_DF_TAG_BEGIN << " \t" << pathID << endl;
		for ( int i = 0; i < obj.numPoints; ++i ) {
			vector3 p = obj.ptArr[i].p;
			float r = obj.ptArr[i].r;
			*fp << PATH_MANAGER_DF_TAG_POSITION << " \t" << p.x << " \t" << p.y << " \t" << p.z << endl;
			*fp << PATH_MANAGER_DF_TAG_RADIUS << " \t" << r << endl;
		}
		*fp << PATH_MANAGER_DF_TAG_END << endl;
		*fp << endl;
	}

};

void CS_PATH_MANAGER::save_Paths( ofstream *fp ) const
{

	mPathObjProvider.resetIterator( );
	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		save_Path( fp, path->getObjID( ), *path );
	}
}

void CS_PATH_MANAGER::save( const string &fileName ) const
{
	cout << "BGN CS_PATH_MANAGER::save:" << fileName << endl;
	///////////////////////////////////////////////
	
	ofstream *fp;
	fp = new std::ofstream(fileName.data(), ios::out | ios::binary);

	if ( fp == 0 ) {
		cout << "Cannot create file:" << fileName << endl;
		return;
	}
	*fp << PATH_MANAGER_DF_TAG_MAIN << endl;

	save_Paths( fp );

	fp->close( );
	cout << "END CS_PATH_MANAGER::save:" << fileName << endl;

}