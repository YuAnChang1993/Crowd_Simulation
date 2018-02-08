//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CS_RECORD_H_
#define __CS_RECORD_H_

#include <iostream>

class CS_SIMULATION_RECORD
{
protected:
	int mNumAgentsReachDestination;
	double mTotalTravelTime;
	double mTotalTravelLength;
	double mTotalSeparationDistance;
	int mNumRecordsOfTravelTime;
	int mNumRecordsOfTravelLength;
	int mNumRecordsOfSeparationDistance;

public:
	CS_SIMULATION_RECORD( );
	void reset( );
	void addTravelTime( double t );
	void addTravelLength( double L );
	void addAgentSeparationDistance( double sep_d );
	double getAverageTravelTime( ) const;
	double getAverageTravelLength( ) const;
	double getAverageSeparationDistance( ) const;
	void showStatistics( ) const;
};

#endif