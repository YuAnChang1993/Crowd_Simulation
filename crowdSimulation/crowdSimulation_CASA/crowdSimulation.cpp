//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "crowdSimulation.h"
#include "../../primitives/basic_tools.h"
#include "../../msc_utility_tools/timer/timer.h"
#include "../trainer.h"
#include "../timer_index.h"
#include "../../msc_utility_tools/io/file_mng.h"
#include "../../textureManager/textureManager.h"
#include "../agent_generator_manager.h"
#include "../cs_parameters.h"
//
using namespace std;
using namespace ns_opengl;

CROWD_SIMULATION::CROWD_SIMULATION( )
{
	initInternalVariables( );
}

void CROWD_SIMULATION::initInternalVariables( )
{
	//
	mCollisionDetectionScheme = 0;
	//
	mMaxNumIterationsPerStep = 100;
	mNumIterationsPerStep = 30;
	//mNumIterationsPerStep = 1;
	mPickedObj = 0;
	mFlgTrainedPOS = false;
	mActionType = CROWD_SIMULATION_ACTION_SIMULATION;

	mTotalNumOfSimulatedAgents = 0;
	mTotalNumOfFinishedAgents = 0;
	mMap2D_Grid_DX = 5.0;
	mMap2D_Grid_DZ = 5.0;

	mDefaultNumOfAgents = 0;
	mFlgSetupSceneEnvironment = false;
	mSimulationTime = 0.0;
	//mAgentUIA = 0;
	mFrame = 0;
	mTimeStep = 0.03;
	mMaxNumOfAgents = csp_maxNum_Agents;
	//mCurNumOfAgents = 0;
	mAgentProvider.init( mMaxNumOfAgents );
	//
	/*
	mCurNumOfPaths = 0;
	mMaxNumOfPaths = 16;
	mPathArr = new CS_PATH[mMaxNumOfPaths];
	*/
	//
	mAgentGeneratorManager = 0;
	//
	mCrowdSimRecord.reset( );
	//
	mMapMinX = -50.0;
	mMapMaxX =  250.0;
	mMapMinZ = -50.0;
	mMapMaxZ =  250.0;

	mCelluarAutomata = new CS_CELLULAR_AUTOMATA(); // 2017/04/06
	//
	mLatticeManager = new LATTICE_MANAGER<CS_AGENT>;
	/*
	mLatticeManager->init(
		150, 2, 150, 
		mMapMinX, mMapMaxX,		
		-10.0,		// miny
		10.0,		// maxy
		mMapMinZ, mMapMaxZ
		);
		*/
	mLatticeManager->init(
		200, 2, 200, 
		mMapMinX, mMapMaxX,		
		-10.0,		// miny
		10.0,		// maxy
		mMapMinZ, mMapMaxZ
		);

	mAgentSeparationDistance_Map = new T_GRID_2D<float>;

	int numOfSeparationDistanceMapLayers = 3;
	mAgentSeparationDistance_Map->setGridDimension
		( 150, 150, 
		mMapMinX, mMapMaxX,
		mMapMinZ, mMapMaxZ, 
		numOfSeparationDistanceMapLayers
		);
	
	mAgentSeparationDistance_Map->initializeGridValue( 0.0 );
	//
	mTraining_POS_NumFrames = 13000;
	//
	mAgentMovementPath_Map = new T_GRID_2D<float>;
	mAgentMovementPath_Map->setGridDimension
		( 300, 300, 
		mMapMinX, mMapMaxX,
		mMapMinZ, mMapMaxZ, 
		0
		);
	
	mFlgEnableRecordTime = false;
	mMainTimer = new cTIMER;
	mMainTimer->Register( cTM_IDX_TOTAL_TIME,			cTM_NAME_TOTAL_TIME);
	mMainTimer->Register( cTM_IDX_COMPUTE_AGENT_MOTION, cTM_NAME_COMPUTE_AGENT_MOTION);
	mMainTimer->Register( cTM_IDX_SEPARATION_ADJUST,	cTM_NAME_SEPARATION_ADJUST);
	mMainTimer->Register( cTM_IDX_MODIFY_AGENT_MOTION,	cTM_NAME_MODIFY_AGENT_MOTION );
	mMainTimer->Register( cTM_IDX_RESOLVE_AGENT_COLLISION, cTM_NAME_RESOLVE_AGENT_COLLISION );
	mMainTimer->Register( cTM_IDX_UPDATE_AGENT_POSITION,	cTM_NAME_UPDATE_AGENT_POSITION );
	mMainTimer->Register( cTM_IDX_APPY_HARD_CONSTRAINTS,	cTM_NAME_APPY_HARD_CONSTRAINTS );
	mMainTimer->Register( cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP, cTM_NAME_COMPUTE_AGENT_MOVEMENT_MAP );
}

void CROWD_SIMULATION::setNumOfIterationsPerStep( int num )
{
	mNumIterationsPerStep = num;
	if ( mNumIterationsPerStep == 0 ) mNumIterationsPerStep = 1;
	if ( mNumIterationsPerStep > mMaxNumIterationsPerStep ) mNumIterationsPerStep = mMaxNumIterationsPerStep;
}

void CROWD_SIMULATION::initializeBasedOnConfig( )
{
	if ( mConfigIniFileName.data( ) != 0 ) {
		pushFilePath( );
		changeFilePath( mConfigDirectoryName );
		readAppConfigFile( mConfigIniFileName.data( ) );
		setup_scene_Environment_BasedOnDataFile( );
		//
		CS_ENVIRONMENT::instance( )->setCollisionDetectionScheme( mCollisionDetectionScheme );
		//
		TEXTURE_MANAGER::instance( )->readConfig( mDataFile_Textures.data( ) );
		popFilePath( );
		//
		pushFilePath( );
		changeFilePath( mTextureDirectoryName );
		TEXTURE_MANAGER::instance( )->loadAllTexturesFromFiles( );
		popFilePath( );


		//
	} else {
		setup_scene_Environment( );
	}
	//setup_scene_SimTypePath( 20 );

	//setGridCellDimension( mMap2D_Grid_DX, mMap2D_Grid_DZ );

	prepareAgentsBasedonSimulationType( );
}

void CROWD_SIMULATION::setActionType_Simulation( )
{
	mActionType = CROWD_SIMULATION_ACTION_SIMULATION;
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_NONE );
}

void CROWD_SIMULATION::setActionType_EditEnvObj( )
{
	mActionType = CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ;
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_EDIT_OBJ );
}

void CROWD_SIMULATION::setActionType_AddEnvObj( )
{
	mActionType = CROWD_SIMULATION_ACTION_ADD_ENV_OBJ;
	
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_ADD_OBJ );
	CS_ENVIRONMENT::instance( )->start_Action( );
}

void CROWD_SIMULATION::setActionType_EditAgentGenerator( )
{
}

void CROWD_SIMULATION::setActionType_AddAgentGenerator( )
{
}

void CROWD_SIMULATION::setActionType_AddPathObj( )
{
}

void CROWD_SIMULATION::setActionType_AddPathObjNow( )
{
}

void CROWD_SIMULATION::refreshSimulation( )
{
	cout << "CROWD_SIMULATION::refreshSimulation( )" << endl;
	//system("pause");
	mAgentGeneratorManager->set( AGENT_GENERATOR_MANAGER::instance( ) );
	//mAgentGeneratorArr = mAgentGeneratorManager->getGenerators( mCurNumOfAgentGenerators, &mUIAgentGenArr);
	//
	vector3 aabbMin0, aabbMax0;
	vector3 aabbMin, aabbMax;
	mAgentGeneratorManager->computeAABB( aabbMin, aabbMax );
	CS_ENVIRONMENT::instance( )-> computeAABB( aabbMin0, aabbMax0 );
	mergeAABB( aabbMin, aabbMax, aabbMin0, aabbMax0 );
	CS_PATH_MANAGER::instance( )-> computeAABB( aabbMin0, aabbMax0 );
	mergeAABB( aabbMin, aabbMax, aabbMin0, aabbMax0 );
	//
	CS_ENVIRONMENT::instance( )->buildBVHOfObjects( );
	//
	mLatticeManager->setVolume( 
		aabbMin.x, 
		aabbMax.x,
		aabbMin.y, 
		aabbMax.y,
		aabbMin.z, 
		aabbMax.z,
		(csp_agent_radius+csp_max_agent_speed*0.03)*2.0
		);

	mAgentSeparationDistance_Map->setGridSpace( aabbMin.x, aabbMax.x, aabbMin.z, aabbMax.z );

	reset( );


}

void CROWD_SIMULATION::reset( )
{
	mTotalNumOfFinishedAgents = 0;
	mTotalNumOfSimulatedAgents = 0;
	mFrame = 0;
	mSimulationTime = 0.0;

	if ( mLatticeManager ) {
		mLatticeManager->reset( );
	}

	mAgentProvider.reset( );
	setup_scene_SimTypePath( mDefaultNumOfAgents );

	mAgentGeneratorManager->reset( );
	
	mAgentSeparationDistance_Map->initializeGridValue( 0.0 );
	mAgentMovementPath_Map->initializeGridValue( 0.0 );

	mCrowdSimRecord.reset( );
}

void CROWD_SIMULATION::prepareAgentsBasedonSimulationType( )
{
		mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		agent->prepareBasedonSimulationType( );
	}
}

//return -1: invalid
void CROWD_SIMULATION::addPath( int pathIndex, const vector3 *p, int numPoints, float radius )
{
	/*
	if ( mCurNumOfPaths >= mMaxNumOfPaths) return -1;

	for (int i = 0; i < numPoints; ++i ) {
		mPathArr[mCurNumOfPaths].addPoint( p[i], radius );
	}

	return mCurNumOfPaths++;
	*/
	CS_PATH_MANAGER::instance( )->addPath( pathIndex, p, numPoints, radius );
}

//return -1: invalid
CS_AGENT *CROWD_SIMULATION::addAgent( const vector3 &p )
{
	return addAgent( p.x, p.y, p.z, 0.0, 0.0, 0.0 );
}

//return -1: invalid
CS_AGENT *CROWD_SIMULATION::addAgent( const vector3 &p, const vector3 &destPosition )
{
	return addAgent( p.x, p.y, p.z, destPosition.x, destPosition.y, destPosition.z );
}

CS_AGENT *CROWD_SIMULATION::addAgent( float x, float y, float z)
{
	return addAgent( x, y, z, 0.0, 0.0, 0.0 );
}

// (x, y, z): current position
// (dx, dy, dz): destinatin position
//return -1: invalid
CS_AGENT *CROWD_SIMULATION::addAgent( float x, float y, float z, float dx, float dy, float dz)
{
	

	int index = mAgentProvider.getFreeElement( );
	if ( index < 0 ) return 0;
	CS_AGENT *agent = mAgentProvider.getEle( index );
	agent->reset( );
	agent->setPosition( x, y, z );
	agent->setDestinationPosition( dx, dy, dz );
	//
	if ( mLatticeManager ) {
		mLatticeManager->addNewElement( agent );
	}
	//
	return agent;
}

void CROWD_SIMULATION::addAgentsInsideCircle( int num, const vector3 &p, float r )
{
	for ( int i = 0; i < num; ++i ) {
		float x, z;
		float fx, fz;
		fx = bt_random_value_interval(-1.0, 1.0);
		fz = bt_random_value_interval(-1.0, 1.0);
		x = fx*r;
		z = fz*r;
		addAgent( p.x+x, 0.0, p.z+z );
	}
}

void CROWD_SIMULATION::addAgentGenerator( CS_AGENT_GENERATOR *g )
{
	/*
	if ( mCurNumOfAgentGenerators >= mMaxNumOfAgentGenerators ) return;
	mAgentGeneratorArr[mCurNumOfAgentGenerators].set( *g );
	++mCurNumOfAgentGenerators;
	*/
}

void CROWD_SIMULATION::addAgentGenerator( int pathIndex, float radius, int numPerSecond, double maxGenerationTime )
{
	/*
	if ( mCurNumOfAgentGenerators >= mMaxNumOfAgentGenerators ) return;
	vector3 p = CS_PATH_MANAGER::instance( )->getFirstPathPoint( pathIndex );
	mAgentGeneratorArr[mCurNumOfAgentGenerators].setPathIndex( pathIndex );
	mAgentGeneratorArr[mCurNumOfAgentGenerators].setGenerationRegion( p, radius );
	mAgentGeneratorArr[mCurNumOfAgentGenerators].setGenerationFrequency( numPerSecond );
	mAgentGeneratorArr[mCurNumOfAgentGenerators].setMaxGenerationTime( maxGenerationTime );
	++mCurNumOfAgentGenerators;
	*/
}

void CROWD_SIMULATION::setup_scene_Environment_BasedOnDataFile( )
{
	cout << "BEGIN CROWD_SIMULATION::setup_scene_Environment_BasedOnDataFile( )" << endl;
	mFlgSetupSceneEnvironment = true;

	CS_PATH_MANAGER::instance( )->read( mDataFile_AgentPath.data( ) );

	AGENT_GENERATOR_MANAGER::instance( )->read( mDataFile_AgentGenerator.data( ) );

	if ( mAgentGeneratorManager == 0 ) {
		mAgentGeneratorManager = new AGENT_GENERATOR_MANAGER;
	}

	mAgentGeneratorManager->set( AGENT_GENERATOR_MANAGER::instance( ) );

	//mAgentGeneratorArr = mAgentGeneratorManager->getGenerators( mCurNumOfAgentGenerators, &mUIAgentGenArr );

	CS_ENVIRONMENT::instance( )->read( mDataFile_Environment.data( ) );

	TRAINER::instance( )->read( mDataFile_PSOTraining.data( ) );
	//

	//PS_Float w, cp, cg;
	//w = 0.5;
	//cp = 1.5;
	//cg = 1.5;
	//TRAINER::instance( )->installPSOSystem( PSO_BASIC::instance( ), w, cp, cg );
	TRAINER::instance( )->installPSOSystem( PSO_BASIC::instance( ) );
	TRAINER::instance( )->buildMapper( );
	//
	cout << "END CROWD_SIMULATION::setup_scene_Environment_BasedOnDataFile( )" << endl;
}

void CROWD_SIMULATION::setup_scene_Environment( )
{
	mFlgSetupSceneEnvironment = true;
	vector3 p[4];
	
	p[0] = vector3( 0.0, 0.0, 0.0 );
	p[1] = vector3( 50.0, 0.0, 0.0 );
	p[2] = vector3( 50.0, 0.0, 50.0 );
	p[3] = vector3( 0.0, 0.0, 50.0 );
	/*
	p[0] = vector3( 0.0, 0.0, 0.0 );
	p[1] = vector3( 10.0, 0.0, 0.0 );
	p[2] = vector3( 10.0, 0.0, 10.0 );
	p[3] = vector3( 0.0, 0.0, 10.0 );
*/
	int pathIndex = 0;
	int numPathPoints = 4;
	int pathPointRadius = 9.0;
	cout << "addPath..." << endl;

	addPath( pathIndex, p, numPathPoints, pathPointRadius );
	//
	//
	float generationRegionRadius = 8;
	float numAgentsPerSecond = 10.5;
	//
	float maxGenerationTime = 100.0; // second
	addAgentGenerator(
		pathIndex,
		generationRegionRadius,
		numAgentsPerSecond,
		maxGenerationTime );
	//
	//mEnvironment->a
	CS_ENVIRONMENT::instance( )->addEnvironmentObject_Rect( 
		vector3(0.0, 0.0, 10.0),
		vector3(40.0, 0.0, 40.0));
	//
}

void CROWD_SIMULATION::setup_scene_SimTypePath( int numAgents )
{
	/*
	cout << "BGN CROWD_SIMULATION::setup_scene_SimTypePath:" << numAgents << endl;

	if ( mFlgSetupSceneEnvironment == false ) {
		cout << "mFlgSetupSceneEnvironment:" << mFlgSetupSceneEnvironment << endl;
		cout << "Please call CROWD_SIMULATION::setup_scene_Environment( ) first" << endl;
		system("pause");
		return;
	}
	int pathIndex = 0;
	cout << "create agents..." << endl;
	mDefaultNumOfAgents = numAgents;
	for ( int i = 0; i < mDefaultNumOfAgents; ++i ) {
		vector3 q;
		q = bt_random_sample_circle_XZPlane( mPathArr[pathIndex].getFirstPointPosition( ), 5.0 );
		int agentIndex = addAgent( q );
		mAgentArr[agentIndex].setPath( mPathArr[pathIndex] );
		mAgentArr[agentIndex].setSimulationType( CS_AGENT_SIM_TYPE_ALONG_PATH );
		mAgentArr[agentIndex].prepareBasedonSimulationType( );
	}
	cout << "END CROWD_SIMULATION::setup_scene_SimTypePath" << endl;
	*/
}

void CROWD_SIMULATION::deleteObj( )
{
	/*
	#define CROWD_SIMULATION_ACTION_SIMULATION				1
#define CROWD_SIMULATION_ACTION_ADD_ENV_OBJ				2
#define CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ			3
#define CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR		4
#define CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR	5
#define CROWD_SIMULATION_ACTION_ADD_PATH_OBJ			6
#define CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ			7
	*/
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			CS_ENVIRONMENT::instance( )->deletePickedObj( );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR:
			AGENT_GENERATOR_MANAGER::instance( )->deletePickedObj( );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ:
			CS_PATH_MANAGER::instance( )->deletePickedObj( );
			

			break;
	}
}

bool CROWD_SIMULATION::handleKeyEvent( unsigned char key )
{
	bool flgHandled = false;
	switch( key ) {
		//case 't':
			//update_crowd( true );

			//flgHandled = true;
		//	break;
		case 127: // delete
			deleteObj( );
			flgHandled = true;
			cout << "deleteObj" << endl;
			break;
		case 'k':
			flgHandled = true;
			setActionType_EditEnvObj( );
			cout << "setActionType_EditEnvObj" << endl;
			break;
		case 's':
			flgHandled = true;
			setActionType_Simulation( );
			cout << "setActionType_Simulation" << endl;
			break;
		case 'e':
			flgHandled = true;
			setActionType_AddEnvObj( );
			cout << "setActionType_EditEnvObj" << endl;
			break;
		case 'a':
			cout << "Reset crowd simulation" << endl;
			reset( );
			flgHandled = true;

			break;
		case 'p':
			train_UsingPOS( );
			system("pause");
			flgHandled = true;

			break;
		case 'o':
			cout << "train_UsingPOS: 20 times" << endl;
			for ( int i = 0; i < 20; ++i ) {
				cout << "i:" << i << endl;
				train_UsingPOS( );
			}
			system("pause");
			flgHandled = true;

			break;
	}
	return flgHandled;
}
