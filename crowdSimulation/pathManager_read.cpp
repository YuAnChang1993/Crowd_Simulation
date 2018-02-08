#include <fstream>
#include <iostream>
#include <string>
#include "pathManager.h"

using namespace std;


void CS_PATH_MANAGER::read( const char *fileName )
{
	CS_PATH dummy_path;
	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "read_Generator_DataFile. Cannot open data file:" << fileName << endl;
		return;
	}

	bool flg_valid = false;
	std::string key;
	while (!fp->eof()) {
		*fp >> key;
		
		if (key.compare("AGENT_PATH") == 0) {
			flg_valid = true;
		}
		if ( flg_valid == false ) {
			cout << "The data file is not for defining paths." << endl;
			break;
		}

		if (key.compare("BEGIN") == 0) {
			int id;
			*fp >> id;
			dummy_path.setObjID( id );
		}

		if (key.compare("RADIUS") == 0) {
			float r;
			*fp >> r;
			dummy_path.setRadius( r );
		}

		if (key.compare("POSITION") == 0) {
			float x, y, z;
			*fp >> x >> y >> z;
			dummy_path.addPoint( x, y, z );
		}

		if (key.compare("END") == 0) {
			addPath(dummy_path.getObjID( ), dummy_path);
			dummy_path.clear( );
		}

		key.clear( );
	}

	fp->close( );

	printf_PathData( );
}

void CS_PATH_MANAGER::printf_PathData( ) const
{
	cout << "BEGIN CS_PATH_MANAGER::printf_PathData( ) const" << endl;

	mPathObjProvider.resetIterator( );
	CS_PATH *path;
	while ( path = mPathObjProvider.getNext( ) ) {
		if ( path->numPoints == 0 ) continue;
		cout << "Path:" << path->getObjID( ) << endl;
		for ( int j = 0; j < path->numPoints; ++j ) {
			vector3 &p = path->ptArr[j].p;
			float &r = path->ptArr[j].r;
			cout << p.x << "\t" << p.y <<"\t" << p.z << endl;
			cout << "radius:" << r << endl;
		}
	}
	cout << "END CS_PATH_MANAGER::printf_PathData( ) const" << endl;

}