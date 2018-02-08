//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/13
//
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "environment.h"
#include "environment_data_format.h"

using namespace std;

extern string getEnvironmentObjTypeName( int type );

namespace {
	

	void save_EnvironmentObject( ofstream *fp, const CS_ENV_OBJECT &envObj )
	{
		
		if ( fp == 0 ) return;
		int objID = envObj.getObjID( );

		int type = envObj.getObjType( );
		string envObjTypeName = ::getEnvironmentObjTypeName( type );
		
		*fp << ENV_DF_TAG_BEGIN << " \t" << objID << endl;
		*fp << envObjTypeName << endl;
		for ( int i = 0; i < envObj.ptArr.size( ); ++i ) {
			vector3 p = envObj.ptArr[i];
			*fp << ENV_DF_TAG_POSITION << " \t" << p.x << " \t" << p.y << " \t" << p.z << endl;
		}
		*fp << ENV_DF_TAG_END << endl;
		*fp << endl;
		
	}

};

void CS_ENVIRONMENT::save_EnvironmentObjects( ofstream *fp ) const
{
	mEnvObjProvider.resetIterator( );

	CS_ENV_OBJECT *envObj;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		save_EnvironmentObject( fp, *envObj );
	}
}

void CS_ENVIRONMENT::save( const string &fileName ) const
{
	cout << "BGN CS_ENVIRONMENT::save:" << fileName << endl;
	///////////////////////////////////////////////
	
	ofstream *fp;
	fp = new std::ofstream(fileName.data(), ios::out | ios::binary);

	if ( fp == 0 ) {
		cout << "Cannot create file:" << fileName << endl;
		return;
	}
	*fp << ENV_DF_TAG_MAIN << endl;
	save_EnvironmentObjects( fp );

	if ( mMapObj ) {
		int mapIndex = 0;
		*fp << ENV_DF_TAG_MAP << "\t" << mapIndex << endl;
		*fp << ::getEnvironmentObjTypeName( CS_ENV_OBJECT_TYPE_RECTANGLE ) << endl;
		vector3 p, q;
		p = mMapObj->ptArr[0]; // lower left
		q = mMapObj->ptArr[1]; // upper right

		*fp << ENV_DF_TAG_POSITION << "\t" << p.x << "\t" << p.y << "\t" << p.z << endl;
		*fp << ENV_DF_TAG_POSITION << "\t" << q.x << "\t" << q.y << "\t" << q.z << endl;
		*fp << ENV_DF_TAG_TEXTURE << "\t" << mMapObj->textureID << endl;
		*fp << ENV_DF_TAG_END << endl;
		*fp << endl;
	}

	fp->close( );
/*
	ENVIRONMENT


		BEGIN 0
RECTANGLE
POSITION -40.0 0.0 10.0
POSITION 40.0 0.0 40.0
TEXTURE 0
END
*/
	///////////////////////////////////////////////
	cout << "END CS_ENVIRONMENT::save:" << fileName << endl;

}