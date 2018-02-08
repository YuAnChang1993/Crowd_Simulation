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
#include "trainer.h"

using namespace std;


void TRAINER::read(const char *fileName)
{
	ifstream *fp;
	fp = new ifstream(fileName, ios::in | ios::binary);
	if (fp == 0 || fp->fail( ) ) {
		cout << "TRAINER::read. Cannot open data file:" << fileName << endl;
		return;
	}

	int pathPointIndex = -1;
	int pathIndex = -1;
	bool flg_valid = false;
	std::string key;
	while (!fp->eof()) {
		*fp >> key;

		if (key.compare("PSO") == 0) {
			flg_valid = true;
		}
		if ( flg_valid == false ) {
			cout << "The data file is not for defining environment." << endl;
			break;
		}

		if (key.compare("NUM_PARTICLES") == 0) {
			*fp >> mNumPSOParticles;
		}		

		if (key.compare("PSO_PARAM_W") == 0) {
			*fp >> mPOS_param_w;
		}

		if (key.compare("PSO_PARAM_CP") == 0) {
			*fp >> mPOS_param_cp;
		}

		if (key.compare("PSO_PARAM_CG") == 0) {
			*fp >> mPOS_param_cg;
		}

		if (key.compare("Training_POS_NumFrames") == 0) {
			*fp >> mTraining_POS_NumFrames;
		}

		if (key.compare("BEGIN") == 0) {
		}

		if (key.compare("PATH") == 0) {
			*fp >> pathIndex;
		}

		if (key.compare("POINTX") == 0) {
			float negRange, posRange;
			*fp >> pathPointIndex >> negRange >> posRange;
			if ( pathIndex >= 0 ) {
				addTrainingElement( TRAINER_DATA_TYPE_PATH_POINTX, pathIndex, pathPointIndex, negRange, posRange );
			}
		}

		if (key.compare("POINTZ") == 0) {
			float negRange, posRange;
			*fp >> pathPointIndex >> negRange >> posRange;
			if ( pathIndex >= 0 ) {
				addTrainingElement( TRAINER_DATA_TYPE_PATH_POINTZ, pathIndex, pathPointIndex, negRange, posRange );
			}
		}

		if (key.compare("END") == 0) {
			pathIndex = -1;
		}

		key.clear( );
	}

	fp->close( );
}

void TRAINER::printf( ) const
{
	cout << "BEGIN TRAINER::printf( ) --------------" << endl;
	cout << "Training_POS_NumFrames:\t" << mTraining_POS_NumFrames << endl;
	cout << "NUM_PARTICLES:\t" << mNumPSOParticles << endl;

	cout << "mPOS_param_w:\t" << mPOS_param_w << endl;
	cout << "mPOS_param_cp:\t" << mPOS_param_cp << endl;
	cout << "mPOS_param_cg:\t" << mPOS_param_cg << endl;
	cout << "END TRAINER::printf( ) --------------" << endl;

}