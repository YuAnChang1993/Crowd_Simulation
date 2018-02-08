//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CS_TRAINER_H_
#define __CS_TRAINER_H_

#include "mapper.h"
#include "../machine_learning/PSO/pso_basic.h"

#define TRAINER_DATA_TYPE_NONE	0
#define TRAINER_DATA_TYPE_PATH_POINT	1
#define TRAINER_DATA_TYPE_PATH_POINTX	2
#define TRAINER_DATA_TYPE_PATH_POINTZ	3

class TRAINER_ELEMENT
{
public:
	TRAINER_ELEMENT( );
	int type;
	int pathID;
	int pathPointIndex;
	float positionNegRange;
	float positionPosRange;

};

class TRAINER {
private:
	static TRAINER *_instance;
protected:
	int mTraining_POS_NumFrames;
	int mNumPSOParticles;
	int mCurNumOfElements;
	int mMaxNumOfElements;
	TRAINER_ELEMENT *mElementArr;
	//
	PSO_BASIC *mPSOSystem;
	MAPPER_ARRAY_TO_SPARSE_DATA<PS_Float, float> mMapper;
	//
	PS_Float *mLBArr;
	PS_Float *mUBArr;
	//
	PS_Float mPOS_param_w;
	PS_Float mPOS_param_cp;
	PS_Float mPOS_param_cg;

	//
public:
	TRAINER( );
	void read(const char *fileName);
	void installPSOSystem( PSO_BASIC *pso_system, float w, float cp, float cg );
	void installPSOSystem( PSO_BASIC *pso_system );

	void addTrainingElement( int type, int pathID, int pathPointIndex, float negRange, float posRange );
	void buildMapper( );
	void update( int psoParticleIndex );
	void updateBest( );
	int getNumberOfTrainingFrames( ) const { return mTraining_POS_NumFrames;}
	void printf( ) const;
	static TRAINER *instance( ) 
	{
		if ( _instance == 0) {
			_instance = new TRAINER;
		} 
		return _instance;
	}
};

#endif