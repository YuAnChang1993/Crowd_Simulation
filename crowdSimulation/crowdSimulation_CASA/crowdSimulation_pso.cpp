//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "crowdSimulation.h"
#include "../../machine_learning/PSO/pso_basic.h"
#include "../mapper.h"
#include "../../primitives/basic_tools.h"
#include "../trainer.h"

using namespace std;


namespace {

	MAPPER_ARRAY_TO_SPARSE_DATA<PS_Float, float> _mapper;

	float computeFitness( float T, float L, float SeparationDistance )
	{
		float desired_v = 1.5;
		float f;
		//f = T;
		int nFrame = T/0.03;
		float wSD = 2.0;
		f = T;
		cout << "f:T:" << f << endl;
		f = T + wSD*(nFrame*SeparationDistance/desired_v);
		cout << "f:T+SD:" << f << endl;

		return f;
	}

	PS_Float *LBArr = new PS_Float[ 1024 ];
	PS_Float *UBArr = new PS_Float[ 1024 ];
};

void CROWD_SIMULATION::train_UsingPOS_OneGeneration( int numOfTimeSteps )
{

	for ( int i = 0; i < numOfTimeSteps; ++i ) {
		update_crowd( false );
	}

}

void CROWD_SIMULATION::train_UsingPOS_init( int pso_numParticles )
{
	int pso_numGenerations = 20;
	//
	int dim;
	int pathID = 1;
	const CS_PATH *path = CS_PATH_MANAGER::instance( )->getPathBasedOnPathID( pathID );
	int num_moving_points = path->numPoints - 2;
	dim = num_moving_points*2; // X, Z coordinates
	//
	cout << "pathID:" << pathID << endl;
	cout << "num_moving_points:" << num_moving_points << endl;
	cout << "dim:" << dim << endl;

	cout << "Step 1: PSO_BASIC::instance( )->initMemory( pso_numParticles, dim);" << endl;
	//PSO_BASIC::instance( )->initMemory( pso_numParticles, dim);

	cout << "Step 2: set up LBArr and UBArr " << endl;

	float min_x, max_x;
	float min_z, max_z;

	min_x = 60.0;
	max_x = 90.0;
	min_z = 0.0;
	max_z = 50.0;
	/*
	for ( int i = 0; i < num_moving_points; ++i ) {
	LBArr[i*2+0] = min_x;
	LBArr[i*2+1] = min_z;
	//
	UBArr[i*2+0] = max_x;
	UBArr[i*2+1] = max_z;
	}
	*/
	int k;
	k = 0;
	LBArr[k*2+0] = 60; // min_x
	LBArr[k*2+1] = 40; // min_z
	//
	UBArr[k*2+0] = 80; // max_x
	UBArr[k*2+1] = 60; // max_z

	k = 1;
	LBArr[k*2+0] = 55; // min_x
	LBArr[k*2+1] = 20; // min_z
	//
	UBArr[k*2+0] = 70; // max_x
	UBArr[k*2+1] = 30; // max_z
	k = 2;
	LBArr[k*2+0] = 60; // min_x
	LBArr[k*2+1] = -10; // min_z
	//
	UBArr[k*2+0] = 80; // max_x
	UBArr[k*2+30] = 5; // max_z


	PS_Float w, cp, cg;
	w = 0.5;
	cp = 1.5;
	cg = 1.5;

	cout << "PSO_BASIC::instance( )->setParameters" << endl;

	//PSO_BASIC::instance( )->setParameters(w, cp, cg, LBArr, UBArr);

	pso_numParticles = PSO_BASIC::instance( )->getNumberOfParticles( );

	cout << "PSO_BASIC::instance( )->pre_init( );" << endl;
	PSO_BASIC::instance( )->pre_init( );
	cout << "initialize all particles" << endl;
	
	for ( int i = 0; i < pso_numParticles; ++i ) {
		cout << "Particle: " << i << endl;

		reset( );
		int k;
		//
		/*
		const PS_Float *X = PSO_BASIC::instance( )->getParticleXArr( i, dim );
		_mapper.registerArr( X );
		_mapper.update( );
		bt_enforce_printf_Array<double>( "Particle Path", X, dim );
		*/
		TRAINER::instance( )->update( i );

/*
		k = 0;
		CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 1, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
		k = 1;
		CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 2, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
		k = 2;
		CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 3, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
*/
		//
		//system("pause");

		//
		cout << "\tbegin simulation" << endl;
		train_UsingPOS_OneGeneration( mTraining_POS_NumFrames );
		cout << "\tend simulation" << endl;

		int numRemainedAgents = CROWD_SIMULATION::recordDataForAllUnfinishedAgents_WithPenality( 2.0, 2.0 );
		float L = mCrowdSimRecord.getAverageTravelLength( );
		float T = mCrowdSimRecord.getAverageTravelTime( );
		float SD = mCrowdSimRecord.getAverageSeparationDistance( );

		float f = computeFitness( T, L, SD );
		//penalty for not reaching agents
			if ( numRemainedAgents > 0 ) {
				//f = f * numRemainedAgents*10;
			}
			if ( mTotalNumOfFinishedAgents > 0 ) {
				f = f/mTotalNumOfFinishedAgents;
			}
		cout << "mTotalNumOfFinishedAgents:" << mTotalNumOfFinishedAgents << endl;
		cout << "numRemainedAgents:" << numRemainedAgents << endl;


		PSO_BASIC::instance( )->setParticleFitness( i, f );
		cout << "Particle:" << i << "\t" << "fitness:" << f << endl;
	}

	cout << "PSO_BASIC::instance( )->post_init( );" << endl;
	PSO_BASIC::instance( )->post_init( );
	cout << "----- start to training-----" << endl;
}

void CROWD_SIMULATION::train_UsingPOS( )
{
	//if ( mFlgTrainedPOS ) return;
	mTraining_POS_NumFrames = TRAINER::instance( )->getNumberOfTrainingFrames( );
	cout << "BEGIN CROWD_SIMULATION::train_UsingPOS( )" << endl;
	cout << "mTraining_POS_NumFrames:" << mTraining_POS_NumFrames << endl;
	int pso_numParticles = 10;


	int pathID = 1;

	

	if ( mFlgTrainedPOS == false ) {
		_mapper.setNumOfElements( 6 );
		_mapper.registerSparseData( 0, CS_PATH_MANAGER::instance( )->getPathPoint_XPtr( pathID, 1 ) );
		_mapper.registerSparseData( 1, CS_PATH_MANAGER::instance( )->getPathPoint_ZPtr( pathID, 1 ) );
		_mapper.registerSparseData( 2, CS_PATH_MANAGER::instance( )->getPathPoint_XPtr( pathID, 2 ) );
		_mapper.registerSparseData( 3, CS_PATH_MANAGER::instance( )->getPathPoint_ZPtr( pathID, 2 ) );
		_mapper.registerSparseData( 4, CS_PATH_MANAGER::instance( )->getPathPoint_XPtr( pathID, 3 ) );
		_mapper.registerSparseData( 5, CS_PATH_MANAGER::instance( )->getPathPoint_ZPtr( pathID, 3 ) );
		train_UsingPOS_init( pso_numParticles );
	}
	mFlgTrainedPOS = true;
	//

	int pso_numGenerations = 1;
	//
	int dim;
	
	const CS_PATH *path = CS_PATH_MANAGER::instance( )->getPathBasedOnPathID( pathID );
	int num_moving_points = path->numPoints - 2;
	dim = num_moving_points*2; // X, Z coordinates
	//

	//
	//
	pso_numParticles = PSO_BASIC::instance( )->getNumberOfParticles( );
	for ( int g = 0; g < pso_numGenerations; ++g ) {
		PSO_BASIC::instance( )->pre_update( );
		for ( int i = 0; i < pso_numParticles; ++i ) {
			reset( );
			TRAINER::instance( )->update( i );
			/*
			int dim;
			const PS_Float *X = PSO_BASIC::instance( )->getParticleXArr( i, dim );
			_mapper.registerArr( X );
			_mapper.update( );
			bt_enforce_printf_Array<double>( "Particle Path", X, dim );
			*/
			/*
			int k;
			k = 0;
			CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 1, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
			k = 1;
			CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 2, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
			k = 2;
			CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 3, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
			*/
			//
			train_UsingPOS_OneGeneration( mTraining_POS_NumFrames );
			int numRemainedAgents = CROWD_SIMULATION::recordDataForAllUnfinishedAgents_WithPenality( 2.0, 2.0 );
			float L = mCrowdSimRecord.getAverageTravelLength( );
			float T = mCrowdSimRecord.getAverageTravelTime( );
			float SD = mCrowdSimRecord.getAverageSeparationDistance( );

			float f = computeFitness( T, L, SD );
		cout << "mTotalNumOfFinishedAgents:" << mTotalNumOfFinishedAgents << endl;

			cout << "numRemainedAgents:" << numRemainedAgents << endl;

			//-------------------------------------
			//penalty for not reaching agents
			if ( numRemainedAgents > 0 ) {
				//f = f * numRemainedAgents*10;
			}
			if ( mTotalNumOfFinishedAgents > 0 ) {
				f = f/mTotalNumOfFinishedAgents;
			}
			//-------------------------------------
			PSO_BASIC::instance( )->setParticleFitness( i, f );
			cout << "Particle:" << i << "\t" << "fitness:" << f << endl;
		}
		PSO_BASIC::instance( )->post_update( );
	}

	const PS_Float *X = PSO_BASIC::instance( )->getGlobalBestXArr( dim );
	PS_Float best_f = PSO_BASIC::instance( )->getGlobalBestFitnessValue( );
	cout << "global best_f:" << best_f << endl;
	
	bt_enforce_printf_Array<double>( "Particle Path", X, dim );
	TRAINER::instance( )->updateBest( );
	//_mapper.registerArr( X );
	//_mapper.update( );

	/*
	int k;
	k = 0;
	CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 1, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
	k = 1;
	CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 2, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
	k = 2;
	CS_PATH_MANAGER::instance( )->setPathPoint(pathIndex, 3, vector3( X[k*2+0], 0.0, X[k*2+1] ) );
	*/


	reset( );
	
	cout << "END CROWD_SIMULATION::train_UsingPOS( )" << endl;

}