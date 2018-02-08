//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <fstream>
#include <iostream>
#include <string>
#include "environment.h"
#include "environment_data_format.h"

using namespace std;

namespace {
	CS_ENV_OBJECT dummy_obj;
};

void CS_ENVIRONMENT::read( const char *fileName )
{
	
	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "CS_ENVIRONMENT::read. Cannot open data file:" << fileName << endl;
		delete fp;
		return;
	}

	dummy_obj.clear( );
	bool flg_valid = false;
	bool flg_map = false;
	std::string key;
	while (!fp->eof()) {
		*fp >> key;
	
		if (key.compare(ENV_DF_TAG_MAIN) == 0) {
			flg_valid = true;
		}
		if ( flg_valid == false ) {
			cout << "The data file is not for defining environment." << endl;
			break;
		}

		if (key.compare(ENV_DF_TAG_BEGIN) == 0) {
			int id;
			*fp >> id;
			dummy_obj.setObjID( id );
		}

		if (key.compare(ENV_DF_TAG_MAP) == 0) {
			int id;
			*fp >> id;
			dummy_obj.setObjID( id );
			flg_map = true;
		}

		if (key.compare("RECTANGLE") == 0) {
			dummy_obj.setObjType( CS_ENV_OBJECT_TYPE_RECTANGLE );
		}

		if (key.compare("POLYGON") == 0) {
			dummy_obj.setObjType( CS_ENV_OBJECT_TYPE_POLYGON );
		}

		if (key.compare(ENV_DF_TAG_POSITION) == 0) {
			float x, y, z;
			*fp >> x >> y >> z;
			dummy_obj.addPoint( x, y, z );
		}

		if (key.compare(ENV_DF_TAG_TEXTURE) == 0) {
			*fp >> dummy_obj.textureID;
		}

		if (key.compare(ENV_DF_TAG_END) == 0) {
			if ( flg_map ) {
				setMapObj( dummy_obj );
			} else {
				addEnvObject( dummy_obj );
			}
			dummy_obj.clear( );
			flg_map = false;
		}
		key.clear( );
	}

	fp->close( );
}



void CS_ENVIRONMENT::printf_Data( ) const
{
}