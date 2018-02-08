//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/12/10
//
#include <iostream>
#include "crowdSimulation_Basic.h"
#include "../../primitives/basic_tools.h"
#include "../../msc_utility_tools/timer/timer.h"
#include "../trainer.h"
#include "../timer_index.h"
#include "../cs_parameters.h"

using namespace std;

void CROWD_SIMULATION_BASIC::resolveAgentCollision_Lattice_PositionBased( )
{
	mAgentProvider.resetIterator( );
	CS_AGENT *agi;
	while ( agi = mAgentProvider.getNext( ) ) {
		int ix, iy, iz;
		mLatticeManager->computeLatticeCoordinates(agi, ix, iy, iz);

		int idx, idy, idz;
		idx = 1;
		idy = 1;
		idz = 1;

		mLatticeManager->startIterator(ix, iy, iz, idx, idy, idz);
		CS_AGENT *agj;
		while ( agj = mLatticeManager->getNext( ) ) {
			if (agi == agj) continue;
			bool flgCollide = isAgentsColliding( agi, agj );
			if ( flgCollide ) {
				float sep_distance = moveAgentsApart(  agi, agj );
				mCrowdSimRecord.addAgentSeparationDistance( sep_distance );
				float ax = agi->p.x + agj->p.x;
				float az = agi->p.z + agj->p.z;
				ax = ax*0.5;
				az = az*0.5;
				mAgentSeparationDistance_Map->addAtPosition_Averaging( ax, az, sep_distance);
			}
		}
	}
}

void CROWD_SIMULATION_BASIC::resolveAgentCollision_PositionBased( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_RESOLVE_AGENT_COLLISION);
		mMainTimer->BeginRecord(cTM_IDX_RESOLVE_AGENT_COLLISION);
	}
	//-------------------------------------------------------------
	//resolveAgentCollision_AllPairs( );
	resolveAgentCollision_Lattice_PositionBased( );
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_RESOLVE_AGENT_COLLISION );
		mMainTimer->EndRecord( cTM_IDX_RESOLVE_AGENT_COLLISION );
	}
}

void CROWD_SIMULATION_BASIC::applyHardConstraints_PositionBased( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_APPY_HARD_CONSTRAINTS );
		mMainTimer->BeginRecord(cTM_IDX_APPY_HARD_CONSTRAINTS	);
	}
	//-------------------------------------------------------------

	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {

		float r = agent->r;
		vector3 q0, q1;
		q0 = agent->prev_p;
		q1 = agent->p;
		bool flg_hit;
		vector3 feasible_direction;
		vector3 s = CS_ENVIRONMENT::instance( )->computeSeparationVector_Circle(q0, q1, r*1.05, flg_hit, feasible_direction );
		agent->mFeasibleDirection = feasible_direction;
		s = 1.01*s;
		vector3 new_q1;
		new_q1 = q1 + s;
		if ( flg_hit ) {
			//new_q1 += agent->sv;
			float Ls = s.Length( );
			if ( Ls > 1.0 || Ls != Ls ) {
				//cout << "Ls:" << Ls << endl;
				//system("pause");
			}
			agent->flgHit = true;
			agent->p = q1 + s;
			//
			CROWD_MONITOR_MANAGER::instance( )->addHistoryRecord( agent->p, 1.0);
			//
		} else {
			agent->flgHit = false;
		}
	}
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_APPY_HARD_CONSTRAINTS );
		mMainTimer->EndRecord( cTM_IDX_APPY_HARD_CONSTRAINTS );
	}
}
//////////////////////////////////////////////////////////////////
void CROWD_SIMULATION_BASIC::modifyAgentVelocity_DueToEnvironment( bool flg_timer)
{

	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {

		float r = agent->r;
		vector3 q0, q1;
		q0 = agent->prev_p;
		vector3 sv = agent->sv;
		q1 = q0 + sv;
		bool flg_hit;
		vector3 preferred_dir;
		flg_hit = agent->flgHit;
		vector3 feasible_direction = agent->mFeasibleDirection;
		preferred_dir = sv;
		float Lp = preferred_dir.Normalize( );
		if ( Lp == 0.0 ) {
			preferred_dir.zero( );
		}
		//vector3 n = CS_ENVIRONMENT::instance( )->computeNormalDirection_Circle(q0, q1, r*1.06, flg_hit, preferred_dir );

		/*
		if (n.x != n.x ) {
		cout << "modifyAgentVelocity_DueToEnvironment NAN" << endl;
		cout << "n nan" << endl;
		system("pause");
		}
		*/

		if ( flg_hit ) {
			//
			agent->preferred_dir = preferred_dir;
			agent->hitNormal = feasible_direction;
			agent->flgHit = flg_hit;
			sv = agent->preferred_dir*agent->desired_v*mTimeStep;
			sv = bt_eliminateNegativeVectorComponent( sv, feasible_direction );
			float Ls = sv.Length( );
			if ( agent->desired_speed*mTimeStep*0.75 > Ls ) {
				//vector3 pd = feasible_direction.cross( vector3( 0.0, -1.0, 0.0) );
				//pd.y = 0.0;
				//sv = pd*agent->desired_v*mTimeStep*0.75;
				vector3 pd = sv;
				float d = pd.Normalize( );
				if ( d > 0.0 ) {
					sv = pd*agent->desired_speed*mTimeStep*0.75;
				}
			}
			agent->p += 0.005*feasible_direction;
			//
			CROWD_MONITOR_MANAGER::instance( )->addHistoryRecord( agent->p, 1.0);
			//
		} else {
			agent->preferred_dir.zero( );
			agent->hitNormal.zero( );
			agent->flgHit = false;
		}
		agent->sv = sv;
		agent->desired_v = sv;

		if (agent->sv.x != agent->sv.x ) {
			cout << "modifyAgentVelocity_DueToEnvironment NAN" << endl;
			cout << "sv nan" << endl;
			system("pause");
		}
		if (agent->p.x != agent->p.x ) {
			cout << "modifyAgentVelocity_DueToEnvironment NAN" << endl;
			cout << "p nan" << endl;

			system("pause");
		}
	}
}


void CROWD_SIMULATION_BASIC::modifyAgentVelocity_DueToAgents_VelocityBased( bool flg_timer )
{
	mAgentProvider.resetIterator( );
	CS_AGENT *agi;
	while ( agi = mAgentProvider.getNext( ) ) {

		int ix, iy, iz;
		mLatticeManager->computeLatticeCoordinates(agi, ix, iy, iz);

		int idx, idy, idz;
		idx = 1;
		idy = 1;
		idz = 1;

		mLatticeManager->startIterator(ix, iy, iz, idx, idy, idz);
		CS_AGENT *agj;
		if ( csp_enableAgentPerceivedVelocity ) {
			while ( agj = mLatticeManager->getNext( ) ) {
				if (agi == agj) continue;
				bool flgCollide;
				flgCollide = adjustAgentVelocity_CA_VelocityBased_Perceived( agi, agj );
			}
		} else {
			while ( agj = mLatticeManager->getNext( ) ) {
				if (agi == agj) continue;
				bool flgCollide;
				flgCollide = adjustAgentVelocity_CollisionAvoidance_VelocityBased( agi, agj );
			}
		}
	}
}

void CROWD_SIMULATION_BASIC::modifyAgentVelocity( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_MODIFY_AGENT_MOTION );
		mMainTimer->BeginRecord(cTM_IDX_MODIFY_AGENT_MOTION	);
	}
	//-------------------------------------------------------------
	modifyAgentVelocity_DueToEnvironment( );
	//cout << "csp_numOfStepsForAgentVelocityAdjustmentDueToAgents:" << csp_numOfStepsForAgentVelocityAdjustmentDueToAgents << endl;
	for ( int i = 0; i < csp_numOfStepsForAgentVelocityAdjustmentDueToAgents; ++i ) {
		modifyAgentVelocity_DueToAgents_VelocityBased( flg_timer );
	}


	//modifyAgentVelocity_DueToAgents( );
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_MODIFY_AGENT_MOTION	);
		mMainTimer->EndRecord( cTM_IDX_MODIFY_AGENT_MOTION );
	}
}


void CROWD_SIMULATION_BASIC::update_crowd_PositionBased( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_TOTAL_TIME);
		mMainTimer->BeginRecord(cTM_IDX_TOTAL_TIME);
	}

	//cTIMER::instance( )->Register( 0, "timer:" );
	//cTIMER::instance( )->Start( 0 );
	CS_ENVIRONMENT::instance( )->update( mTimeStep );
	//cTIMER::instance( )->ShowDuration( "CS_ENVIRONMENT::instance( )->update", 0 );
	//
	generateAgents( );
	//
	mAgentSeparationDistance_Map->update( );

	pre_update( );
	//
	computeAgentMotion( flg_timer );
	adjustAgentMotionBasedOnSeparationDistanceMap( flg_timer );
	//
	modifyAgentVelocity( flg_timer );
	//
	updateAgentPosition( flg_timer );
	//---------------------------------------------
	resolveAgentCollision_PositionBased( flg_timer );
	//
	applyHardConstraints_PositionBased( flg_timer );
	//---------------------------------------------
	//
	computeAgentMovementMap( flg_timer );
	//
	post_update( );
	//
	handleAgentsReachedDestination( );
	//
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_TOTAL_TIME );
		mMainTimer->EndRecord( cTM_IDX_TOTAL_TIME );
	}
	//
	//This last is put at the end.
	//mAgentUIA = mAgentProvider.getUsedEleIndexArr( mCurNumOfAgents );
	//
	mSimulationTime += mTimeStep;
	//



	++mFrame;
	//
}

/////////////////////////////////////////////////////////////////////////////
void CROWD_SIMULATION_BASIC::resolveAgentCollision_Lattice_VelocityBased( )
{
	mAgentProvider.resetIterator( );
	CS_AGENT *agi;
	while ( agi = mAgentProvider.getNext( ) ) {
		int ix, iy, iz;
		mLatticeManager->computeLatticeCoordinates(agi, ix, iy, iz);

		int idx, idy, idz;
		idx = 1;
		idy = 1;
		idz = 1;

		mLatticeManager->startIterator(ix, iy, iz, idx, idy, idz);
		CS_AGENT *agj;
		while ( agj = mLatticeManager->getNext( ) ) {
			if (agi == agj) continue;
			bool flgCollide = isAgentsColliding( agi, agj );
			if ( flgCollide ) {
				//cout << "overlapping" << endl;
				float sep_distance = moveAgentsApart_VelocityBased(  agi, agj );
				mCrowdSimRecord.addAgentSeparationDistance( sep_distance );
				float ax = agi->p.x + agj->p.x;
				float az = agi->p.z + agj->p.z;
				ax = ax*0.5;
				az = az*0.5;
				mAgentSeparationDistance_Map->addAtPosition_Averaging( ax, az, sep_distance);
			}
		}
	}
}

void CROWD_SIMULATION_BASIC::resolveAgentCollision_VelocityBased( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_RESOLVE_AGENT_COLLISION);
		mMainTimer->BeginRecord(cTM_IDX_RESOLVE_AGENT_COLLISION);
	}
	//-------------------------------------------------------------
	//resolveAgentCollision_AllPairs( );
	resolveAgentCollision_Lattice_VelocityBased( );
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_RESOLVE_AGENT_COLLISION );
		mMainTimer->EndRecord( cTM_IDX_RESOLVE_AGENT_COLLISION );
	}
}

void CROWD_SIMULATION_BASIC::applyHardConstraints_VelocityBased( bool flg_timer )
{
	//flg_timer = false;
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_APPY_HARD_CONSTRAINTS );
		mMainTimer->BeginRecord(cTM_IDX_APPY_HARD_CONSTRAINTS	);
	}
	//-------------------------------------------------------------

	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {

		float r = agent->r;
		vector3 q0, q1;
		q0 = agent->p;
		q1 = agent->p + agent->sv;
		bool flg_hit;
		flg_hit = false;
		vector3 feasible_direction;
		vector3 s = CS_ENVIRONMENT::instance( )->computeSeparationVector_Circle(q0, q1, r*1.05, flg_hit, feasible_direction );
		agent->mFeasibleDirection = feasible_direction;
		s = 1.01*s;
		vector3 new_q1;
		new_q1 = q1 + s;
		if ( flg_hit ) {
			float Ls = s.Length( );
			if ( Ls > 1.0 || Ls != Ls ) {
				//cout << "Ls:" << Ls << endl;
				//system("pause");
			}
			agent->flgHit = true;
			agent->sv = (q1 + s) - agent->p;
		} else {
			agent->flgHit = false;
		}
	}
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_APPY_HARD_CONSTRAINTS );
		mMainTimer->EndRecord( cTM_IDX_APPY_HARD_CONSTRAINTS );
	}
}

void CROWD_SIMULATION_BASIC::adjustAgentMotionBasedOnSeparationDistanceMap( bool flg_timer )
{
	//cout << "BGN CROWD_SIMULATION_BASIC::adjustAgentMotionBasedOnSeparationDistanceMap" << endl;
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_SEPARATION_ADJUST);
		mMainTimer->BeginRecord(cTM_IDX_SEPARATION_ADJUST);
	}
	//-------------------------------------------------------------

	//return;
	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {

		int counter = mAgentSeparationDistance_Map->getGridCounterValue_AtCoordinates( agent->p.x, agent->p.z );

		if ( counter == 0 ) continue;
		//system("pause");
		float sd = mAgentSeparationDistance_Map->getValue_AtCoordinates( agent->p.x, agent->p.z );

		float L = agent->sv.Length( );
		//
		//if ( i == 0 ) {
		//cout << "Agent:" << i << "\tL:" << "L:" << L << "\tsd:" << sd << endl;
		//}
		//
		float newL = L - sd;
		if (newL < 0.0) newL = 0.0;
		if (L > 0.0) {
			if ( newL == 0.0 ) {
				if ( csp_enableAgentLowestSpeed ) {
					newL = L*csp_lowestAgentSpeedRatio;
				}
			}
		}
		//DEBUG
		//if ( newL  <= 0.1 ) {
		//	cout << "newL:" << newL << endl;
		//}
		//
		if ( L!= 0.0 ) {
			agent->sv = (newL/L)*agent->sv;
		}

		if (agent->sv.x != agent->sv.x ) {
			cout << "adjustAgentMotionBasedOnSeparationDistanceMap NAN" << endl;
			system("pause");
		}
	}
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_SEPARATION_ADJUST );
		mMainTimer->EndRecord( cTM_IDX_SEPARATION_ADJUST );
	}
}



void CROWD_SIMULATION_BASIC::update_crowd_VelocityBased( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_TOTAL_TIME);
		mMainTimer->BeginRecord(cTM_IDX_TOTAL_TIME);
	}

	//cTIMER::instance( )->Register( 0, "timer:" );
	//cTIMER::instance( )->Start( 0 );
	CROWD_MONITOR_MANAGER::instance( )->resetHistory( );
	//
	CS_ENVIRONMENT::instance( )->update( mTimeStep );
	//cTIMER::instance( )->ShowDuration( "CS_ENVIRONMENT::instance( )->update", 0 );
	//
	generateAgents( );
	//
	mAgentSeparationDistance_Map->update( );

	pre_update( );
	//
	computeAgentMotion( flg_timer );
	//
	if ( csp_enableAgentSeparationMap ) {
		adjustAgentMotionBasedOnSeparationDistanceMap( flg_timer );
	}
	//
	modifyAgentVelocity( flg_timer );
	//---------------------------------------------
	resolveAgentCollision_VelocityBased( flg_timer );
	//
	applyHardConstraints_VelocityBased( flg_timer );
	//
	updateAgentPosition( flg_timer );
	//---------------------------------------------
	//
	computeAgentMovementMap( flg_timer );
	//
	post_update( );
	//
	handleAgentsReachedDestination( );
	//
	CS_ENVIRONMENT::instance( )->update( mTimeStep );
	//
	//CROWD_MONITOR_MANAGER::instance( )->update( mTimeStep );
	//
	updateLoadOfAgentsOfPaths( mTimeStep );
	//adjustPathsBasedOnCrowdMonitorManager( mTimeStep ); // old

	//adjustPathsBasedOnPathAffectingPoints( mTimeStep ); // new
	//

	//


	//
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_TOTAL_TIME );
		mMainTimer->EndRecord( cTM_IDX_TOTAL_TIME );
	}
	//
	//This last is put at the end.
	//mAgentUIA = mAgentProvider.getUsedEleIndexArr( mCurNumOfAgents );
	//
	mSimulationTime += mTimeStep;
	//



	++mFrame;
	//
}

void CROWD_SIMULATION_BASIC::update_crowd( bool flg_timer )
{
	//update_crowd_PositionBased( flg_timer );
	update_crowd_VelocityBased( flg_timer );

}
//---------------------
void CROWD_SIMULATION_BASIC::adjustPathsBasedOnPathAffectingPoints( CS_PATH *path, float time_step )
{
	int printfInfo_DeltaFrames = 15;
	if ( mFrame % printfInfo_DeltaFrames == 0 ) {
		//path->showInfo( );
	}
	int numPoints = path->getNumPoints( );
	CS_PATH_POINT *pArr = path->getVertArr( );
	CS_PATH_AFFECT_DATA g;
	float *pw = path->getExpectedPathWidth( );
	path->getAffectingInfo( g );

	vector3 f[16];
	vector3 f0[16];
	vector3 f1[16];
	float DL0[16];
	float DL1[16];
	float r[16];
	float k_factor = 0.35;
	float k_factor_r = 0.05;


	float agent_radius = 1.0;
	// the first and last points are fixed.
	// Case : the point is affected by the previous path.
	// Consider the second affecting point.
	//
	f0[ 0 ] = f1[ 0 ] = vector3( 0.0, 0.0, 0.0);
	

	for ( int i = 1; i < numPoints - 1; ++i ) {
		//
		int kc = i;
		int kp = i - 1; // previous path
		for ( int j = 1; j < 2; ++j ) {
			
			if ( g.affectingPointD2[ j ][ kp ] < 0.0 ) continue; // not exist
			float d = sqrt( g.affectingPointD2[ j ][ kp ] );
			vector3 p, q;
			p = g.affectingSelfPointArr[ j ][ kp ];
			q = g.affectingPointArr[ j ][kp ];
			vector3 pq = p - q;
			pq.Normalize( );
			float W = pw[ kp ]; // path expected width
			float W1 = pw[ kc ];
			if ( g.maxProjectedDistance[ kp ] > W ) {
				W = g.maxProjectedDistance[ kp ];
				//cout << "." << endl;
			}
			
			if ( g.maxProjectedDistance[ kc ] > W ) {
				W = g.maxProjectedDistance[ kc ];
				//cout << "." << endl;
			}
			
			if ( W1 > W ) W = W1;

			r[ i ] = W;

			float minPathWidth = pArr[i].r - agent_radius;
			if ( minPathWidth > W ) W = minPathWidth;

			W = W + agent_radius;

			float DL = W - d;
			DL0[ i ] = DL;

			//if ( pw[ k ] <= 0.0 ) cout << "0 pw:" << pw[ k ] << endl;
			if ( i == 0 ) {
				//cout << "i:" << i << "\tj:" << j << endl;
				//cout << "pw:" << pw[i] << "\td:" << d << "\tDL:" << DL << endl;
			}

			
			if ( mFrame % printfInfo_DeltaFrames == 0 ) {
				//cout << "i(0):" << i << "\t" << DL << "\tminPathWidth:" << minPathWidth << endl;
				
			}

			f0[ i ] = k_factor*pq*( DL );

		}
	}
	//

	for ( int i = 1; i < numPoints - 1; ++i ) {
		int k = i;
		for ( int j = 0; j < 1; ++j ) {
			//if ( i >= 2 ) cout << "i:" << i << "\t" << "g.affectingPointD2[ j ][k ]:" << g.affectingPointD2[ j ][k ] << endl;
			if ( g.affectingPointD2[ j ][k ] < 0.0 ) continue;
			float d = sqrt( g.affectingPointD2[ j ][k ] );
			vector3 p, q;
			p = g.affectingSelfPointArr[ j ][ k ];
			q = g.affectingPointArr[ j ][ k ];
			vector3 pq = p - q;
			pq.Normalize( );

			float W = pw[ k ];
			float W_0 = pw[ k - 1 ];

			if ( g.maxProjectedDistance[ k - 1 ] > W ) {
				W = g.maxProjectedDistance[ k - 1];
			}

			if ( g.maxProjectedDistance[ k ] > W ) {
				W = g.maxProjectedDistance[ k ];
			}

			//if ( i >= 2 ) cout << "W:" << W << "\tW_0:" << W_0 << endl;

			if ( W_0 > W ) W = W_0;

			if ( W > r[ i ] ) r[ i ] = W;

			float minPathWidth = pArr[i].r - agent_radius;

			if ( W < minPathWidth ) W = minPathWidth;

			W = W + agent_radius;

			float DL = W - d;
			DL1[ i ] = DL;
			//if ( pw[ k ] <= 0.0 ) cout << "1 pw:" << pw[k] << endl;
			
			

			if ( mFrame % printfInfo_DeltaFrames == 0 ) {
				//cout << "i(1):" << i << "\t" << DL << "\tminPathWidth:" << minPathWidth << endl;
			}

			f1[ i ] = k_factor*pq*( DL );

		}
	}

	for ( int i = 1; i < numPoints - 1; ++i ) {
		if ( DL0[ i ] <= 0.0 && DL1[ i ] >= 0.0 ) {
			// DL1 > 0, need to handle first
			f[ i ] = f1[ i ];
			vector3 nf = f1[ i ].Normalized( );
			if ( nf.x != nf.x ) continue;
			float d = f0[ i ].dot( nf );
			if ( d < 0.0 ) {
				f[ i ] += f0[ i ] - d*f0[ i ];
			} else {
				f[ i ] += f0[ i ];
			}
		}
		else
		if ( DL0[ i ] >= 0.0 && DL1[ i ] <= 0.0 ) {
			f[ i ] = f0[ i ];
			vector3 nf = f0[ i ].Normalized( );
			if ( nf.x != nf.x ) continue;
			float d = f1[ i ].dot( nf );
			if ( d < 0.0 ) {
				f[ i ] += f1[ i ] - d*f1[ i ];
			} else {
				f[ i ] += f1[ i ];
			}
		} else if ( DL0[ i ] <= 0.0 && DL1[ i ] <= 0.0 ) {
			f[ i ] = f0[ i ] + f1[ i ];
		} else {
			//both are positive
			//cout << ".....Both are positive" << endl;
			f[ i ] = f0[ i ] + f1[ i ];
		}
	}

	for ( int i = 1; i < numPoints - 1; ++i ) {
		pArr[ i ].p = pArr[ i ].p + f[ i ]*time_step*csp_max_agent_speed;
		float dr = r[ i ] + agent_radius - pArr[ i ].r;
		pArr[ i ].r = pArr[ i ].r + dr*time_step*k_factor_r;
	}
}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnPathAffectingPoints_01( CS_PATH *path, float time_step )
{
	int printfInfo_DeltaFrames = 15;
	if ( mFrame % printfInfo_DeltaFrames == 0 ) {
		path->showInfo( );
	}
	int numPoints = path->getNumPoints( );
	CS_PATH_POINT *pArr = path->getVertArr( );
	CS_PATH_AFFECT_DATA g;
	float *pw = path->getExpectedPathWidth( );
	path->getAffectingInfo( g );

	vector3 f[16];

	float k_factor = 0.05;

	for ( int i = 0; i < numPoints - 1; ++i ) {
		f[ i ].zero( );
	}
	// the first and last points are fixed.
	// Case : the point is affected by the previous path.
	// Consider the second affecting point.
	//
	for ( int i = 1; i < numPoints - 1; ++i ) {
		//
		int kc = i;
		int kp = i - 1; // previous path
		for ( int j = 1; j < 2; ++j ) {
			
			if ( g.affectingPointD2[ j ][ kp ] < 0.0 ) continue; // not exist
			float d = sqrt( g.affectingPointD2[ j ][ kp ] );
			vector3 p, q;
			p = g.affectingSelfPointArr[ j ][ kp ];
			q = g.affectingPointArr[ j ][kp ];
			vector3 pq = p - q;
			pq.Normalize( );
			float W = pw[ kp ]; // path expected width
			float W1 = pw[ kc ];
			if ( g.maxProjectedDistance[ kp ] > W ) {
				W = g.maxProjectedDistance[ kp ];
				//cout << "." << endl;
			}
			
			if ( g.maxProjectedDistance[ kc ] > W ) {
				W = g.maxProjectedDistance[ kc ];
				//cout << "." << endl;
			}
			
			if ( W1 > W ) W = W1;

			float minPathWidth = pArr[i].r;
			if ( minPathWidth > W ) W = minPathWidth;

			float DL = W - d;

			//if ( pw[ k ] <= 0.0 ) cout << "0 pw:" << pw[ k ] << endl;
			if ( i == 0 ) {
				//cout << "i:" << i << "\tj:" << j << endl;
				//cout << "pw:" << pw[i] << "\td:" << d << "\tDL:" << DL << endl;
			}

			if ( mFrame % printfInfo_DeltaFrames == 0 ) {
				cout << "i(0):" << i << "\t" << DL << "\tminPathWidth:" << minPathWidth << endl;
				
			}

			f[ i ] += k_factor*pq*( DL );

		}
	}
	//

	for ( int i = 1; i < numPoints - 1; ++i ) {
		int k = i;
		for ( int j = 0; j < 1; ++j ) {
			//if ( i >= 2 ) cout << "i:" << i << "\t" << "g.affectingPointD2[ j ][k ]:" << g.affectingPointD2[ j ][k ] << endl;
			if ( g.affectingPointD2[ j ][k ] < 0.0 ) continue;
			float d = sqrt( g.affectingPointD2[ j ][k ] );
			vector3 p, q;
			p = g.affectingSelfPointArr[ j ][ k ];
			q = g.affectingPointArr[ j ][ k ];
			vector3 pq = p - q;
			pq.Normalize( );

			float W = pw[ k ];
			float W_0 = pw[ k - 1 ];

			if ( g.maxProjectedDistance[ k - 1 ] > W ) {
				W = g.maxProjectedDistance[ k - 1];
			}

			if ( g.maxProjectedDistance[ k ] > W ) {
				W = g.maxProjectedDistance[ k ];
			}

			//if ( i >= 2 ) cout << "W:" << W << "\tW_0:" << W_0 << endl;

			if ( W_0 > W ) W = W_0;

			float minPathWidth = pArr[i].r;

			if ( W < minPathWidth ) W = minPathWidth;

			float DL = W - d;

			//if ( pw[ k ] <= 0.0 ) cout << "1 pw:" << pw[k] << endl;


			if ( mFrame % printfInfo_DeltaFrames == 0 ) {
				cout << "i(1):" << i << "\t" << DL << "\tminPathWidth:" << minPathWidth << endl;
			}

			f[ i ] += k_factor*pq*( DL );

		}
	}


	for ( int i = 0; i < numPoints - 1; ++i ) {
		pArr[ i ].p = pArr[ i ].p + f[ i ]*time_step*csp_max_agent_speed;
	}
}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnPathAffectingPoints( float time_step )
{
	computeClosestPointsOfEnvObjsToPaths( );

	PROVIDER<CS_PATH> *pathProvider = CS_PATH_MANAGER::instance( )->getProvider( );


	pathProvider->resetIterator( );
	CS_PATH *path;
	while ( path = pathProvider->getNext( ) ) {
		path->computeExpectedPathWidth( );
		adjustPathsBasedOnPathAffectingPoints( path, time_step );
	}
}


//---------------------
void CROWD_SIMULATION_BASIC::updateLoadOfAgentsOfPaths( float time_step )
{
	CS_PATH_MANAGER::instance( )->resetPathLoad( );
	//
	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		agent->addPathLoad( );
		agent->addProjectedDistanceToPath( );
	}
}

void computeClosestPointsforPath( CS_PATH *path, CS_ENV_OBJECT *envObj )
{
	int numPoints = path->getNumPoints( );
	CS_PATH_POINT *pointArr = path->getVertArr( );
	for ( int i = 0; i < numPoints - 1; ++i ) {
		vector3 p0 = pointArr[ i ].p;
		vector3 p1 = pointArr[ i + 1 ].p;

		vector3 near_p[4];
		vector3 near_q[4];
		float d2[4];
		envObj->computeClosestPointForLineSegment( 
			p0, p1, 
			near_p,
			near_q,
			d2
			);
		path->setNearestAffectingPointsForEdge( i, near_p, near_q, d2 );
	}
}

void CROWD_SIMULATION_BASIC::computeClosestPointsOfEnvObjsToPaths( )
{
	PROVIDER<CS_PATH> *pathProvider = CS_PATH_MANAGER::instance( )->getProvider( );
	PROVIDER<CS_ENV_OBJECT> *envProvider = CS_ENVIRONMENT::instance( )->getProvider( );

	pathProvider->resetIterator( );
	CS_PATH *path;
	while ( path = pathProvider->getNext( ) ) {
		path->resetNearestAffectingPointsForEdge( );
		envProvider->resetIterator( );
		CS_ENV_OBJECT *envObj;
		while ( envObj = envProvider->getNext( ) ) {
			computeClosestPointsforPath( path, envObj );
		}
	}
}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnCrowdMonitorManager( float time_step )
{
	//adjustPathsBasedOnCrowdMonitorManager_01( time_step );
	adjustPathsBasedOnCrowdMonitorManager_02( time_step );
	//adjustPathsBasedOnCrowdMonitorManager_03( time_step );

}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnCrowdMonitorManager_03( float time_step )
{

}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnCrowdMonitorManager_02( float time_step )
{

	if ( csp_enableAdaptivePaths == false ) return;
	const PROVIDER<CROWD_MONITOR> *mObjProvider = CROWD_MONITOR_MANAGER::instance( )->getProvider( );

	vector3 g_p;
	CROWD_MONITOR *g;
	int numMonitors = 0;
	mObjProvider->resetIterator( );
	while ( g = mObjProvider->getNext( ) ) {
		++numMonitors;
		int flgSig = g->isProblemSignificant( );
		//cout << "Monitor:" << numMonitors << "\tflgSig:" << flgSig << endl;
		if ( g->isProblemSignificant( ) == false ) {
			//continue;
		}
		//if ( g->hasNoRecord( ) ) continue;

		g_p = g->getPosition( );
		int node_index;
		int agentGenPathIndex = CS_PATH_MANAGER::instance()->findClosestPathIndex_WholePath( g_p, node_index, false );
		CS_PATH *path = ( CS_PATH *) CS_PATH_MANAGER::instance()->getPathBasedOnPathIndex( agentGenPathIndex );
		vector3 path_p;
		vector3 dir_edge;
		path_p = path->getPathNodePosition( node_index );
		dir_edge = path->getEdgeDirectionBasedOnPathNode( node_index );
		dir_edge.Normalize( );
		vector3 q0, q1;
		path->computeClosestPointsOnTwoEdges( g_p, node_index, q0, q1 );
		//
		vector3 dir_q0 = q0 - g_p;
		vector3 dir_q1 = q1 - g_p;
		float Lq0 = dir_q0.Normalize( );
		float Lq1 = dir_q1.Normalize( );

		//
		mDebug_Path_Q0 = q0;
		mDebug_Path_Q1 = q1;

		//
		float k_q0, k_q1;
		float rest_Lq0 = 8.0;
		float rest_Lq1 = 8.0;

		//cout << "Lq0:" << Lq0 << "\tLq1:" << Lq1 << endl;
		float LLq;
		float a = rest_Lq0 - Lq0;
		float b = rest_Lq1 - Lq1;
		b = b*1.2;
		LLq = sqrt((a*a + b*b+0.001));
		k_q0 = a/LLq;
		k_q1 = b/LLq;
		k_q0 = k_q0*time_step;
		k_q1 = k_q1*time_step;
		//

		//
		float k_g = 0.3*time_step;
		float k_ortho = 0.7*time_step;
		vector3 g_dp = (g_p - path_p);
		g_dp.Normalize( );
		//
		vector3 t_dp;
		t_dp = bt_rotateVectorAboutAxis( dir_edge, 3.14159/2.0, vector3(0.0, 1.0, 0.0) );
		t_dp.zero( );
		//
		vector3 new_p;
		if ( flgSig == 1 ) {
			cout << "+";
			float k_dir = time_step*0.2;
			new_p = path_p;
			new_p += k_q0*dir_q0;
			new_p += k_q1*dir_q1;
			//
			float worseValue = g->getReportValue( );
			bool flgWorse = path->setWorseValue( node_index, worseValue, path_p );
			///if ( flgWorse == false ) {
			vector3 worsePosition;
			worsePosition = path->getWorsePosition( node_index, worseValue);
			float r = 1.0;
			if ( r > worsePosition.distance( new_p )  && worseValue > 0.0  ) {
				new_p -= k_q0*dir_q0;
				new_p -= k_q1*dir_q1;
				k_q0 = k_q0*( (rand( )%10000)/9999.0 * 0.5 );
				k_q1 = k_q1*( (rand( )%10000)/9999.0 * 0.5 );
				new_p += k_q0*dir_q0;
				new_p += k_q1*dir_q1;


			}
			//}
			path->setPathNodePosition( new_p, node_index );
		} else if ( flgSig == -1 ) {
			cout << "-";

			if ( k_q0 > 0.0 ) k_q0 = -k_q0;
			if ( k_q1 > 0.0 ) k_q1 = -k_q1;
			new_p = path_p;
			new_p += k_q1*dir_q0;
			new_p += k_q0*dir_q1;
			//

			float worseValue = g->getReportValue( );
			vector3 worsePosition;
			worsePosition = path->getWorsePosition( node_index, worseValue);
			float r = 1.0;
			if ( r > worsePosition.distance( new_p ) && worseValue > 0.0 ) {
				new_p -= k_q1*dir_q0;
				new_p -= k_q0*dir_q1;
				k_q0 = 0.25*k_q0*( (rand( )%10000)/9999.0 - 0.5 );
				k_q1 = 0.25*k_q1*( (rand( )%10000)/9999.0 - 0.5 );
				//new_p += k_q0*dir_q0;
				//new_p += k_q1*dir_q1;
			} else {

			}
			path->resetWorsePosition( node_index, time_step );

			//computeExpectedPathWidth
			//
			//path->computeClosestPointsOnTwoEdges( g_p, node_index, q0, q1 );

			path->setPathNodePosition( new_p, node_index );
			//cout << ".";
			//float k_dir = time_step*0.02;
			//new_p = path_p + dir_edge*k_dir;
			//path->setPathNodePosition( new_p, node_index );
		}
	}


}

void CROWD_SIMULATION_BASIC::adjustPathsBasedOnCrowdMonitorManager_01( float time_step )
{

	if ( csp_enableAdaptivePaths == false ) return;
	const PROVIDER<CROWD_MONITOR> *mObjProvider = CROWD_MONITOR_MANAGER::instance( )->getProvider( );

	vector3 g_p;
	CROWD_MONITOR *g;
	int numMonitors = 0;
	mObjProvider->resetIterator( );
	while ( g = mObjProvider->getNext( ) ) {
		++numMonitors;
		int flgSig = g->isProblemSignificant( );
		//cout << "Monitor:" << numMonitors << "\tflgSig:" << flgSig << endl;
		if ( g->isProblemSignificant( ) == false ) {
			//continue;
		}

		g_p = g->getPosition( );
		int node_index;
		int agentGenPathIndex = CS_PATH_MANAGER::instance()->findClosestPathIndex_WholePath( g_p, node_index, false );
		CS_PATH *path = ( CS_PATH *) CS_PATH_MANAGER::instance()->getPathBasedOnPathIndex( agentGenPathIndex );
		vector3 path_p;
		vector3 dir_edge;
		path_p = path->getPathNodePosition( node_index );
		dir_edge = path->getEdgeDirectionBasedOnPathNode( node_index );
		dir_edge.Normalize( );
		vector3 q0, q1;
		path->computeClosestPointsOnTwoEdges( g_p, node_index, q0, q1 );
		//
		float k_g = 0.3*time_step;
		float k_ortho = 0.7*time_step;
		vector3 g_dp = (g_p - path_p);
		g_dp.Normalize( );
		//
		vector3 t_dp;
		t_dp = bt_rotateVectorAboutAxis( dir_edge, 3.14159/2.0, vector3(0.0, 1.0, 0.0) );
		t_dp.zero( );
		//
		vector3 new_p;
		if ( flgSig == 1 ) {
			float k_dir = time_step*0.2;
			new_p = path_p;
			new_p += -k_g*g_dp;
			new_p += k_ortho*t_dp;
			new_p -= dir_edge*k_dir;	
			path->setPathNodePosition( new_p, node_index );
		} else if ( flgSig == -1 ) {
			float k_dir = time_step*0.02;
			new_p = path_p + dir_edge*k_dir;
			path->setPathNodePosition( new_p, node_index );
		}
	}


}