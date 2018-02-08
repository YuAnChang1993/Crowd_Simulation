//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/29
//
#include <iostream>
#include <fstream>
#include <string>
#include "crowdSimulation_Basic.h"
#include "../../msc_utility_tools/io/file_mng.h"
#include "../crowdSimulation_constants.h"
#include "../agent_generator_manager.h"
#include "../textureManager/textureManager.h"

using namespace std;

void CROWD_SIMULATION_BASIC::save_environment( ) const
{
	int flg;
	pushFilePath();
	flg = changeFilePath( CS_FOLDERNAME_CONFIG_ENV );
	cout << "flg:" << flg << endl;
	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( CS_FOLDERNAME_CONFIG_ENV );
		flg = changeFilePath( CS_FOLDERNAME_CONFIG_ENV );
	}

	showCurFilePath();
	if ( flg == 0 ) {
		CS_ENVIRONMENT::instance( )->save( CS_FILENAME_CONFIG_ENV );
	} else {
		//not success
		cout << "error CROWD_SIMULATION_BASIC::save_environment( )" << endl;

	}
	popFilePath();
}

void CROWD_SIMULATION_BASIC::save_generators( ) const
{
	int flg;
	pushFilePath();
	flg = changeFilePath( CS_FOLDERNAME_CONFIG_AGENT_GEN );
	cout << "flg:" << flg << endl;
	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( CS_FOLDERNAME_CONFIG_AGENT_GEN );
		flg = changeFilePath( CS_FOLDERNAME_CONFIG_AGENT_GEN );
	}

	showCurFilePath();
	if ( flg == 0 ) {
		AGENT_GENERATOR_MANAGER::instance( )->save( CS_FILENAME_CONFIG_AGENT_GEN );
	} else {
		cout << "error CROWD_SIMULATION_BASIC::save_generators( )" << endl;
	}
	popFilePath();
	//
}

void CROWD_SIMULATION_BASIC::save_paths( ) const
{
	int flg;
	pushFilePath();
	flg = changeFilePath( CS_FOLDERNAME_CONFIG_PATH );
	cout << "flg:" << flg << endl;
	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( CS_FOLDERNAME_CONFIG_PATH );
		flg = changeFilePath( CS_FOLDERNAME_CONFIG_PATH );
	}

	showCurFilePath();
	if ( flg == 0 ) {
		string fileName = CS_FILENAME_CONFIG_PATH;
		CS_PATH_MANAGER::instance( )->save( fileName );
		
	} else {
		cout << "error CROWD_SIMULATION_BASIC::save_paths( )" << endl;
	}
	popFilePath();
}

string combineStringsToFormFileName( const string &folderName, const string &fileName )
{
	string s;
	s = "./";
	s += folderName;
	s += "/";
	s += fileName;
	return s;
}

void oFP( ofstream *fp, const string &tag, float v )
{
	*fp << tag << " \t" << v << endl;
}

void oFP( ofstream *fp,  const string &tag,  const string &v )
{
	*fp << tag << " \t" << v << endl;
}

void oFP( ofstream *fp,  const string &tag,  const vector3 &v )
{
	*fp << tag << " \t" << v.x << " \t" << v.y << " \t" << v.z << endl;
}

void CROWD_SIMULATION_BASIC::save_appConfig( const string &fileName ) const
{
	ofstream *fp;
	fp = new std::ofstream(fileName.data(), ios::out | ios::binary);

	if ( fp == 0 ) {
		cout << "Cannot create file:" << fileName << endl;
		return;
	}

	

	string env_fileName = combineStringsToFormFileName(CS_FOLDERNAME_CONFIG_ENV, CS_FILENAME_CONFIG_ENV);
	string path_fileName = combineStringsToFormFileName(CS_FOLDERNAME_CONFIG_PATH, CS_FILENAME_CONFIG_PATH);
	string agent_gen_fileName = combineStringsToFormFileName(CS_FOLDERNAME_CONFIG_AGENT_GEN, CS_FILENAME_CONFIG_AGENT_GEN);
	string texture_fileName = combineStringsToFormFileName(CS_FOLDERNAME_CONFIG_TEXTURE, CS_FILENAME_CONFIG_TEXTURE);
	string pso_fileName = combineStringsToFormFileName(CS_FOLDERNAME_CONFIG_PSO_TRAINING, CS_FILENAME_CONFIG_PSO_TRAINING);

	int maxAgents = 100000;
	oFP( fp, CS_APP_CONFIG_TAG_MAXIMUM_AGENT, maxAgents );
	oFP( fp, CS_APP_CONFIG_TAG_MAP2D_GRID_DX, mMap2D_Grid_DX );
	oFP( fp, CS_APP_CONFIG_TAG_MAP2D_GRID_DZ, mMap2D_Grid_DZ );
	oFP( fp, CS_APP_CONFIG_TAG_AGENT_COLOR, mAgentColor );
	oFP( fp, CS_APP_CONFIG_TAG_AGENT_HIGHLIGHT_COLOR, mHighLightedAgentColor );
	oFP( fp, CS_APP_CONFIG_TAG_ENVIRONMENT, env_fileName );
	oFP( fp, CS_APP_CONFIG_TAG_PATH_DATA, path_fileName );
	oFP( fp, CS_APP_CONFIG_TAG_AGENT_GENERATOR, agent_gen_fileName );
	oFP( fp, CS_APP_CONFIG_TAG_TEXTURE_DATA, texture_fileName );
	oFP( fp, CS_APP_CONFIG_TAG_PSO_TRAINING, pso_fileName );
	
	fp->close( );
}

void CROWD_SIMULATION_BASIC::save_appConfig( ) const
{
	cout << "BGN CROWD_SIMULATION::save_appConfig( )" << endl;

	int flg;
	pushFilePath();
	flg = changeFilePath( CS_FOLDERNAME_CONFIG_APP );
	cout << "flg:" << flg << endl;
	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( CS_FOLDERNAME_CONFIG_APP );
		flg = changeFilePath( CS_FOLDERNAME_CONFIG_APP );
	}

	showCurFilePath();

	if ( flg == 0 ) {
		save_appConfig( CS_FILENAME_CONFIG_APP );
		//
		
	} else {
		cout << "error CROWD_SIMULATION::save_appConfig( )" << endl;
	}
	popFilePath();

	cout << "END CROWD_SIMULATION::save_appConfig( )" << endl;
}

void save_textures( )
{
	int flg;
	pushFilePath();
	flg = changeFilePath( CS_FOLDERNAME_CONFIG_TEXTURE );
	cout << "flg:" << flg << endl;
	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( CS_FOLDERNAME_CONFIG_TEXTURE );
		flg = changeFilePath( CS_FOLDERNAME_CONFIG_TEXTURE );
	}

	showCurFilePath();
	if ( flg == 0 ) {     
		string fileName = CS_FILENAME_CONFIG_TEXTURE;
		ns_opengl::TEXTURE_MANAGER::instance( )->save( fileName );
		
	} else {
		cout << "error CROWD_SIMULATION_BASIC::save_paths( )" << endl;
	}
	popFilePath();
}

void CROWD_SIMULATION_BASIC::save( )
{
	cout << "BGN CROWD_SIMULATION_BASIC::save( )" << endl;

	showCurFilePath();
	pushFilePath();
	////////////////////////////////////////////////////////
	int flg = changeFilePath(mConfigExportDirectoryName);

	if ( flg == 0 ) {
		//ok
	} else {
		//not success
		//create the folder
		flg = _mkdir ( mConfigExportDirectoryName.data( ) );
		flg = changeFilePath( mConfigExportDirectoryName );
	}
	if ( flg != 0 ) {
		cout << "Error cannot save:" << mConfigExportDirectoryName << endl;
		popFilePath();
		return;
	}
	showCurFilePath();
	////////////////////////////////////////////////////////
	cout << "Start to save..." << endl;
	save_environment( );
	save_generators( );
	save_paths( );
	save_appConfig( );
	save_textures( );
	////////////////////////////////////////////////////////
	cout << "End save..." << endl;
	popFilePath();
	showCurFilePath();
	cout << "END CROWD_SIMULATION_BASIC::save( )" << endl;

}
