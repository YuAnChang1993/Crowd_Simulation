//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/10/12
//
#include <fstream>
#include <iostream>
#include "crowdSimulation.h"
#include <string>
#include "../agent_generator.h"

using namespace std;

void CROWD_SIMULATION::readConfigIniFile( const char *fileName )
{
	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "CROWD_SIMULATION::readConfigIniFile. Cannot open data file:" << fileName << endl;
		return;
	}

	std::string key;
	while (!fp->eof()) {
		*fp >> key;

		

		if (key.compare("EXPORT_CROWD_DIR") == 0) {
			*fp >> mConfigExportDirectoryName;
		}

		if (key.compare("CONFIG_DIR") == 0) {
			*fp >> mConfigDirectoryName;
		}
		
		if (key.compare("CROWD_APP_CONFIG") == 0) {
			*fp >> mConfigIniFileName;
		}

		if (key.compare("TEXTURE_DIR") == 0) {
			*fp >> mTextureDirectoryName;
		}

		
		//break;
	}
	printf_ConfigIniData( );
	delete fp;
}

void CROWD_SIMULATION::printf_ConfigIniData( ) const
{
	cout << "mConfigDirectoryName:\t" << mConfigDirectoryName << endl;
	cout << "CROWD_APP_CONFIG FILE NAME:\t" << mConfigIniFileName << endl;
	cout << "mTextureDirectoryName:\t" << mTextureDirectoryName << endl;
}

void CROWD_SIMULATION::readAppConfigFile( const char *fileName )
{
	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "CROWD_SIMULATION::readAppConfigFile. Cannot open data file:" << fileName << endl;
		delete fp;
		return;
	}
	/*
	MAXIMUM_AGENT	100000
ENVIRONMENT	../config_env/env.txt
PATH_DATA	../config_path/path.txt
AGENT_GENERATOR	../config_agent_gen/gen_circle.txt

	*/
int collision_detection_grid_nx = 0;
int collision_detection_grid_nz = 0;

	std::string key;
	while (!fp->eof()) {
		*fp >> key;
		if (key.compare("MAXIMUM_AGENT") == 0) {
			*fp >> mMaxNumOfAgents;
		}

		if (key.compare("MAP2D_GRID_DX") == 0) {
			*fp >> mMap2D_Grid_DX;
		}

		if (key.compare("MAP2D_GRID_DZ") == 0) {
			*fp >> mMap2D_Grid_DZ;
		}


		if (key.compare("AGENT_COLOR") == 0) {
			float r, g, b;
			*fp >> r >> g >> b;
			mAgentColor = vector3(r, g, b);
		}

		if (key.compare("AGENT_HIGHLIGHT_COLOR") == 0) {
			float r, g, b;
			*fp >> r >> g >> b;
			mHighLightedAgentColor = vector3(r, g, b);
		}
		

		if (key.compare("PATH_DATA") == 0) {
			*fp >> mDataFile_AgentPath;
		}
		if (key.compare("AGENT_GENERATOR") == 0) {
			*fp >> mDataFile_AgentGenerator;
		}


		if (key.compare("ENVIRONMENT") == 0) {
			*fp >> mDataFile_Environment;
		}

		if (key.compare("TEXTURE_DATA") == 0) {
			*fp >> mDataFile_Textures;
		}

		if (key.compare("PSO_TRAINING") == 0) {
			*fp >> mDataFile_PSOTraining;
		}

		if (key.compare("COLLISION_DETECTION_SCHEME") == 0) {
			*fp >> mCollisionDetectionScheme;
		}
		
		if (key.compare("COLLISION_DETECTION_GRID_NX") == 0) {
			*fp >> collision_detection_grid_nx;
		}

		if (key.compare("COLLISION_DETECTION_GRID_NZ") == 0) {
			*fp >> collision_detection_grid_nz;
		}

		//cout << "key:" << key << endl;
		key.clear( );
	
	}
	printf_AppConfigData( );
	delete fp;
	//
	//
	CS_ENVIRONMENT::instance( )->setCollisionDetectionGridDimension( 
		collision_detection_grid_nx,
		collision_detection_grid_nz );
}

void CROWD_SIMULATION::readConfig(){

	ifstream file;
	file.open("");
	if (file.fill())
	{
		cout << "CRWPD_SIMULATION::cellular_environment. Cannot open data file." << endl;
		while (true)
		{
			string dataType;
			file >> dataType;
			if (dataType == "EXPERIMENT")
			{
				int type;
				file >> type;
			}
		}
	}
}

void CROWD_SIMULATION::printf_AppConfigData( ) const
{
	cout << "mCollisionDetectionScheme:" << mCollisionDetectionScheme << endl;
	cout << "mMaxNumOfAgents:\t" << mMaxNumOfAgents << endl;
	cout << "mDataFile_Environment:\t" << mDataFile_Environment << endl;
	cout << "mDataFile_AgentPath:\t" << mDataFile_AgentPath << endl;
	cout << "mDataFile_AgentGenerator:\t" << mDataFile_AgentGenerator << endl;
	cout << "mDataFile_Textures:\t" << mDataFile_Textures << endl;
	cout << "mDataFile_PSOTraining:\t" << mDataFile_PSOTraining << endl;
	cout << "mMap2D_Grid_DX:\t" << mMap2D_Grid_DX << endl;
	cout << "mMap2D_Grid_DZ:\t" << mMap2D_Grid_DZ << endl;


}
