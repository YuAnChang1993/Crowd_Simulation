//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include "cs_record.h"

using namespace std;

CS_SIMULATION_RECORD::CS_SIMULATION_RECORD( )
{
	reset( );
}

void CS_SIMULATION_RECORD::reset( )
{
	mNumAgentsReachDestination = 0;
	mTotalTravelTime = 0.0;
	mTotalTravelLength = 0.0;
	mTotalSeparationDistance = 0.0;
	mNumRecordsOfTravelTime = 0;
	mNumRecordsOfTravelLength = 0;
	mNumRecordsOfSeparationDistance = 0;
}

void CS_SIMULATION_RECORD::addAgentSeparationDistance( double sep_d )
{
	mTotalSeparationDistance += sep_d;
	++mNumRecordsOfSeparationDistance;
}

void CS_SIMULATION_RECORD::addTravelTime( double t )
{
	mTotalTravelTime += t;
	++mNumRecordsOfTravelTime;
}

void CS_SIMULATION_RECORD::addTravelLength( double L )
	{
		mTotalTravelLength += L;
		++mNumRecordsOfTravelLength;
	}
double CS_SIMULATION_RECORD::getAverageTravelTime( ) const
{
	if ( mNumRecordsOfTravelTime == 0 ) return 0.0;
	double avgTime = mTotalTravelTime / mNumRecordsOfTravelTime;
	return avgTime;
}

double CS_SIMULATION_RECORD::getAverageTravelLength( ) const
{
	if ( mNumRecordsOfTravelLength == 0 ) return 0.0;
	double avgLength = mTotalTravelLength / mNumRecordsOfTravelLength;
	return avgLength;
}

double CS_SIMULATION_RECORD::getAverageSeparationDistance( ) const
{
	if ( mNumRecordsOfSeparationDistance == 0.0) return 0.0;
	return mTotalSeparationDistance/mNumRecordsOfSeparationDistance;
}

void CS_SIMULATION_RECORD::showStatistics( ) const
{
	cout << "BGN CS_SIMULATION_RECORD::showStatistics( )" << endl;
	double avgTravelTime = getAverageTravelTime( );
	double avgTravelLength = getAverageTravelLength( );
	double avgSeparationDistance = getAverageSeparationDistance( );

	cout << "Avg travel time:\t" << avgTravelTime << endl;
	cout << "Avg travel length:\t" << avgTravelLength << endl;
	cout << "avgSeparationDistance:\t" << avgSeparationDistance << endl;
	cout << "mNumRecordsOfTravelTime:\t" << mNumRecordsOfTravelTime << endl;
	cout << "mNumRecordsOfTravelLength:\t" << mNumRecordsOfTravelLength << endl;
	cout << "mNumRecordsOfSeparationDistance:\t" << mNumRecordsOfSeparationDistance << endl;


	cout << "END CS_SIMULATION_RECORD::showStatistics( )" << endl;


}