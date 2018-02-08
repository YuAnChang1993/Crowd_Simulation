//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/27
//
#include <fstream>
#include <iostream>
#include <string>
#include "agent_generator_manager.h"
#include "agent_generator_data_format.h"

using namespace std;



namespace {
	bool _flg_valid = false;
	bool _flg_begin = false;
	bool _flg_end = false;
	int _type = 0; // by default
	int _objID = -1;
	vector3 _position;	// position
	float _radius;	// radius
	float _frequency = 1.0;
	float _agent_speed_min = 1.0;

	float _agent_speed_max = 1.5;
	int _maxAgentNumber = -1;
	float _maxGenerationTime = -1.0;
	vector3 _destP; // destination point
	int _path_index;
	bool _flg_use_destination_point = false;
	bool _flg_use_path = false;

	CS_AGENT_GENERATOR _dummy_gen;
	//int _maxNumOfGenerator = 16;
	//int _curNumOfGenerator = 0;
	//CS_AGENT_GENERATOR **_generatorArr = new CS_AGENT_GENERATOR*[_maxNumOfGenerator];

	void reset( )
	{
		_flg_valid = 0;
		_maxGenerationTime = 0.0;
		_maxAgentNumber = 1.0;
		_objID = -1;
		_flg_begin = false;
		_flg_end = false;
		_flg_use_destination_point = false;
		_flg_use_path = false;
		_frequency = 1.0;
		_agent_speed_min = 1.0;
		_agent_speed_max = 1.5;
		_dummy_gen.resetInternalState( );
	}
	void begin_generator( ) {
		_dummy_gen.resetInternalState( );
	}
	
	void setDummyGenerator( )
	{
		_dummy_gen.setObjID( _objID );
		_dummy_gen.setGenerationFrequency( _frequency );
		_dummy_gen.setGenerationRegion( _position, _radius );
		_dummy_gen.setMaxGenerationTime( _maxGenerationTime );
		_dummy_gen.setMaxAgentNumber( _maxAgentNumber );

		_dummy_gen.setPathIndex( _path_index );
		_dummy_gen.setAgentSpeedRange( _agent_speed_min, _agent_speed_max );
	}
/*
	void addAgentGenerator( const CS_AGENT_GENERATOR &g )
	{
		if ( _curNumOfGenerator >= _maxNumOfGenerator ) {
			cout << "read addGenerator. Cannot add more agent generator:" << _curNumOfGenerator << endl;
			return;
		}
		CS_AGENT_GENERATOR *new_g = new CS_AGENT_GENERATOR;
		new_g->set( g );
		_generatorArr[ _curNumOfGenerator ] = new_g;
		++_curNumOfGenerator;
	}
*/
};
/*
void printf_GeneratorData( )
{
	for ( int i = 0; i < _curNumOfGenerator; ++i ) {
		_generatorArr[i]->printf_Data( );
	}
}
*/

/*
#define AGENT_GEN_DF_TAG_MAIN					"AGENT_GENERATOR"
#define AGENT_GEN_DF_TAG_OBJ_ID					"AGENT_GEN_ID"
#define AGENT_GEN_DF_TAG_BEGIN					"BEGIN"
#define AGENT_GEN_DF_TAG_AGENT_PATH_INDEX		"AGENT_PATH_INDEX"
#define AGENT_GEN_DF_TAG_GEOMETRY				"GEOMETRY"
#define AGENT_GEN_DF_TAG_GEOMETRY_CIRCLE		"CIRCLE"
#define AGENT_GEN_DF_TAG_POSITION				"POSITION"
#define AGENT_GEN_DF_TAG_RADIUS 				"RADIUS"
#define AGENT_GEN_DF_TAG_PLANE_XZ				"PLANE_XZ"
#define AGENT_GEN_DF_TAG_MAX_GENERATION_TIME	"MAX_GENERATION_TIME"
#define AGENT_GEN_DF_TAG_AGENT_SPEED			"AGENT_SPEED"
#define AGENT_GEN_DF_TAG_FREQUENCY				"FREQUENCY"
#define AGENT_GEN_DF_TAG_METHOD					"METHOD"
#define AGENT_GEN_DF_TAG_METHOD_RANDOM			"RANDOM"
*/
void AGENT_GENERATOR_MANAGER::read( const char *fileName )
{
	reset( );

	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "AGENT_GENERATOR_MANAGER::read. Cannot open data file:" << fileName << endl;
		delete fp;
		return;
	}

	std::string key;
	while (!fp->eof()) {
		*fp >> key;
		
		if (key.compare(AGENT_GEN_DF_TAG_MAIN) == 0) {
			_flg_valid = true;
		}
		if ( _flg_valid == false ) break;

		

		if ( key.compare(AGENT_GEN_DF_TAG_MAX_AGENT_NUMBER) == 0 ) {
			*fp >> _maxAgentNumber;
		}

		if ( key.compare(AGENT_GEN_DF_TAG_OBJ_ID) == 0 ) {
			*fp >> _objID;
		}

		if (key.compare(AGENT_GEN_DF_TAG_BEGIN) == 0) {
			_flg_begin = true;
			begin_generator( );
		}

			
		if ( key.compare(AGENT_GEN_DF_TAG_MAX_GENERATION_TIME) == 0 ) {
			*fp >> _maxGenerationTime;
		}

		if ( key.compare(AGENT_GEN_DF_TAG_AGENT_SPEED) == 0 ) {
			*fp >> _agent_speed_min >> _agent_speed_max;
		}

		

		if ( key.compare(AGENT_GEN_DF_TAG_POSITION) == 0 ) {
			*fp >> _position.x >> _position.y >> _position.z;
		}

		if ( key.compare(AGENT_GEN_DF_TAG_RADIUS) == 0 ) {
			*fp >> _radius;
		}

		if ( key.compare(AGENT_GEN_DF_TAG_FREQUENCY) == 0 ) {
			*fp >> _frequency;
		}

		if ( key.compare(AGENT_GEN_DF_TAG_AGENT_PATH_INDEX) == 0 ) {
			*fp >> _path_index;
		}
		
		if (key.compare(AGENT_GEN_DF_TAG_END) == 0) {
			setDummyGenerator( );
			addAgentGenerator( &_dummy_gen );
			_flg_begin = true;
		}

		key.clear( );
	}

	fp->close( );
	//printf_GeneratorData( );
	delete fp;
}
