//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/12/10
//
#include <iostream>
#include "trainer.h"
#include "pathManager.h"

using namespace std;

TRAINER *TRAINER::_instance = 0;

TRAINER_ELEMENT::TRAINER_ELEMENT( )
{

	type = TRAINER_DATA_TYPE_NONE;
	pathID = -1;
	pathPointIndex= - 1;
}

TRAINER::TRAINER( )
{
	mTraining_POS_NumFrames = 10000;
	mPSOSystem = 0;
	mCurNumOfElements = 0;
	mMaxNumOfElements = 36;
	mElementArr = new TRAINER_ELEMENT[mMaxNumOfElements];
	mLBArr = new PS_Float[ 1024 ];
	mUBArr = new PS_Float[ 1024 ];
	mNumPSOParticles = 10;
	//
	mPOS_param_w = 0.5; // inertia for velocity
	mPOS_param_cp = 1.5; // local parameter
	mPOS_param_cg = 1.5; // global parameter

}

void TRAINER::installPSOSystem( PSO_BASIC *pso_system )
{
	installPSOSystem( pso_system, mPOS_param_w, mPOS_param_cp, mPOS_param_cg );
}

void TRAINER::installPSOSystem( PSO_BASIC *pso_system, float w, float cp, float cg )
{
	mPSOSystem = pso_system;
	mPOS_param_w = w;
	mPOS_param_cp = cp;
	mPOS_param_cg = cg;

	//
	cout << "mNumPSOParticles:" << mNumPSOParticles << endl;
	pso_system->initMemory( mNumPSOParticles, mCurNumOfElements );

	int i = 0;
	while ( i < mCurNumOfElements ) {

		int pathID = mElementArr[i].pathID;
		int pathPointIndex = mElementArr[i].pathPointIndex;

		const CS_PATH *path = CS_PATH_MANAGER::instance( )->getPathBasedOnPathID(pathID);
		
		float v;
		switch( mElementArr[i].type ) {
			case TRAINER_DATA_TYPE_PATH_POINTX:
				v = path->ptArr[ pathPointIndex ].p.x;
				break;
			case TRAINER_DATA_TYPE_PATH_POINTZ:
				v = path->ptArr[ pathPointIndex ].p.z;

				break;
		}
		

		mLBArr[i] = v  - mElementArr[i].positionNegRange;
		mUBArr[i] = v  + mElementArr[i].positionPosRange;
		++i;
	}

	//
	mPSOSystem->setParameters(w, cp, cg, mLBArr, mUBArr);
}


void TRAINER::addTrainingElement( int type, int pathID, int pathPointIndex, float negRange, float posRange )
{
	if (  mCurNumOfElements >= mMaxNumOfElements ) {
		cout << "TRAINER::addTrainingElement. Error." << endl;
		cout << "mMaxNumOfElements" << mMaxNumOfElements << endl;
		cout << "mCurNumOfElements" << mCurNumOfElements << endl;
		return;
	}
	TRAINER_ELEMENT &e = mElementArr[ mCurNumOfElements ];
	e.type = type;
	e.pathID = pathID;
	e.pathPointIndex = pathPointIndex;
	e.positionNegRange = negRange;
	e.positionPosRange = posRange;

	//
	cout << "addTrainingElement:" << endl;
	cout << "type:" << type << endl;
	cout << "pathID:" << pathID << endl;
	cout << "pathPointIndex:" << pathPointIndex << endl;
	cout << "positionNegRange:" << negRange << endl;
	cout << "positionPosRange:" << posRange << endl;


	//
	++mCurNumOfElements;
}	

void TRAINER::buildMapper( )
{
	mMapper.setNumOfElements( mCurNumOfElements );
	int i = 0;
	while ( i < mCurNumOfElements ) {
		int pathID = mElementArr[i].pathID;
		int pathPointIndex = mElementArr[i].pathPointIndex;

		switch ( mElementArr[i].type ) {
			case TRAINER_DATA_TYPE_PATH_POINTX:
				mMapper.registerSparseData( 
				i, 
				CS_PATH_MANAGER::instance( )->getPathPoint_XPtr( pathID, pathPointIndex ) );
				break;
			case TRAINER_DATA_TYPE_PATH_POINTZ:
				mMapper.registerSparseData( 
				i, 
				CS_PATH_MANAGER::instance( )->getPathPoint_ZPtr( pathID, pathPointIndex ) );
				break;
		}
		
		++i;
	} // while

	//

}

void TRAINER::updateBest( )
{
	int dim;
	const PS_Float *X = PSO_BASIC::instance( )->getGlobalBestXArr( dim );
	mMapper.registerArr( X );
	mMapper.update( );
}

void TRAINER::update( int psoParticleIndex )
{
	int dim;
	const PS_Float *X = PSO_BASIC::instance( )->getParticleXArr( psoParticleIndex, dim );
	mMapper.registerArr( X );
	mMapper.update( );
}




