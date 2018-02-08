//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/12/10
//
#include <iostream>
#include "crowdSimulation.h"
#include "../../primitives/basic_tools.h"
#include "../../msc_utility_tools/timer/timer.h"
#include "../trainer.h"
#include "../timer_index.h"
#include "../cs_parameters.h"

using namespace std;

void CROWD_SIMULATION::setAgentsDestination( const vector3 &dp, float r )
{

	mAgentProvider.resetIterator( );
	CS_AGENT *obj;
	while ( obj = mAgentProvider.getNext( ) ) {
		obj->setDestinationPosition( dp, r );
	}
/*
	int *UIA = mAgentProvider.getUsedEleIndexArr( mCurNumOfAgents );
	for ( int i = 0; i < mCurNumOfAgents; ++i ) {
		mAgentArr[UIA[i]].setDestinationPosition( dp, r );
	}
*/
}

void CROWD_SIMULATION::pre_update( )
{
	mAgentProvider.resetIterator( );
	CS_AGENT *obj;
	while ( obj = mAgentProvider.getNext( ) ) {
		obj->pre_update( );

	}
}


void CROWD_SIMULATION::computeAgentMotion( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_COMPUTE_AGENT_MOTION);
		mMainTimer->BeginRecord(cTM_IDX_COMPUTE_AGENT_MOTION);
	}
	//-------------------------------------------------------------

	mAgentProvider.resetIterator( );
	CS_AGENT *obj;
	while ( obj = mAgentProvider.getNext( ) ) {
		obj->computeDesireVelocity( mTimeStep );

		
		if (obj->sv.x != obj->sv.x ) {
			cout << "computeAgentMotion NAN" << endl;
			system("pause");
		}
	}	
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_COMPUTE_AGENT_MOTION );
		mMainTimer->EndRecord( cTM_IDX_COMPUTE_AGENT_MOTION );
	}
}



void CROWD_SIMULATION::resolveAgentCollision_AllPairs( )
{
	int numAgents;
	int *UIA = mAgentProvider.getUsedEleIndexArr( numAgents );
	CS_AGENT *agentArr = mAgentProvider.getObjArr( );
	for ( int i = 0; i < numAgents; ++i ) {
		CS_AGENT *agi = &agentArr[UIA[i]];
		for ( int j = i+1; j < numAgents; ++j ) {
			CS_AGENT *agj = &agentArr[UIA[j]];
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

void CROWD_SIMULATION::resolveAgentCollision_Lattice( )
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

void CROWD_SIMULATION::modifyAgentVelocity_DueToAgents( bool flg_timer )
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
			bool flgCollide = adjustAgentVelocity_CollisionAvoidance( agi, agj );
		}
	}
}


void CROWD_SIMULATION::modifyAgentVelocity_DueToEnvironment( bool flg_timer)
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
		vector3 n = CS_ENVIRONMENT::instance( )->computeNormalDirection_Circle(q0, q1, r*1.06, flg_hit, preferred_dir );

		if (n.x != n.x ) {
			cout << "modifyAgentVelocity_DueToEnvironment NAN" << endl;
			cout << "n nan" << endl;
			system("pause");
		}

		if ( flg_hit ) {
			agent->preferred_dir = preferred_dir;
			agent->hitNormal = n;
			agent->flgHit = flg_hit;
			sv = agent->preferred_dir*agent->desired_v*mTimeStep;
			sv = bt_eliminateNegativeVectorComponent( sv, n );
			agent->p += 0.005*n;
		} else {
			agent->preferred_dir.zero( );
			agent->hitNormal.zero( );
			agent->flgHit = false;
		}
		agent->sv = sv;

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

void CROWD_SIMULATION::modifyAgentVelocity( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_MODIFY_AGENT_MOTION );
		mMainTimer->BeginRecord(cTM_IDX_MODIFY_AGENT_MOTION	);
	}
	//-------------------------------------------------------------
	modifyAgentVelocity_DueToEnvironment( );
	modifyAgentVelocity_DueToAgents( );
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_MODIFY_AGENT_MOTION	);
		mMainTimer->EndRecord( cTM_IDX_MODIFY_AGENT_MOTION );
	}
}

void CROWD_SIMULATION::resolveAgentCollision( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_RESOLVE_AGENT_COLLISION);
		mMainTimer->BeginRecord(cTM_IDX_RESOLVE_AGENT_COLLISION);
	}
	//-------------------------------------------------------------
	//resolveAgentCollision_AllPairs( );
	resolveAgentCollision_Lattice( );
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_RESOLVE_AGENT_COLLISION );
		mMainTimer->EndRecord( cTM_IDX_RESOLVE_AGENT_COLLISION );
	}
}

void CROWD_SIMULATION::updateAgentPosition( bool flg_timer )
{
	//cout << "mCurNumOfAgents:" << mCurNumOfAgents << endl;
	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		agent->updatePosition( mTimeStep );
	}
}

void CROWD_SIMULATION::generateAgents( bool flg_timer )
{

	mAgentGeneratorManager->resetIterator( );
	CS_AGENT_GENERATOR *agentGen;

	while ( agentGen = mAgentGeneratorManager->getNext( ) ) {
		agentGen->update( mTimeStep );
		vector3 p;
		bool flgOK = true;
		int agentGenPathID = agentGen->getAssignedAgentPathID( );
		vector3 p_agent_gen = agentGen->getPosition( );
		while ( flgOK ) {
			flgOK = agentGen->generatePosition( p );
			if ( flgOK == false ) break;
			++mTotalNumOfSimulatedAgents;
			CS_AGENT *agent = addAgent( p );
			float speed = agentGen->getAgentSpeed( );
			agent->group_id = agentGen->getObjID( );
			agent->desired_speed = speed;
			agent->cur_desired_speed = speed;
			agent->setRadius( agentGen->getAgentRadius( ) );
			agent->setSimulationType( CS_AGENT_SIM_TYPE_ALONG_PATH );
			
			int agentGenPathIndex;
			if ( agentGenPathID < 0 ) {
				//cout << "pathIndex:" << pathID << endl;
			switch( agentGenPathID ) {
				case CS_AGENT_GEN_PATH_TYPE_CLOSEST:
					agentGenPathIndex = CS_PATH_MANAGER::instance()->findClosestPathIndex( p_agent_gen );
					break;
				case CS_AGENT_GEN_PATH_TYPE_RANDOM:
					agentGenPathIndex = CS_PATH_MANAGER::instance()->findOneRandomPathIndex( );

					break;
			}
				
				
				agent->setExternalPath( CS_PATH_MANAGER::instance()->getPathBasedOnPathIndex( agentGenPathIndex ) );
			} else {
				agent->setExternalPath( CS_PATH_MANAGER::instance()->getPathBasedOnPathID( agentGenPathID ) );
			}
			agent->reset( );
			agent->prepareBasedonSimulationType( );
		}
	}
	/*
	for ( int i = 0; i < mCurNumOfAgentGenerators; ++i ) 
	{
		int objIndex = mUIAgentGenArr[ i ];
		mAgentGeneratorArr[ objIndex ].update( mTimeStep );
		vector3 p;
		bool flgOK = true;
		int pathIndex = mAgentGeneratorArr[ objIndex ].getPathIndex( );
		vector3 p_agent_gen = mAgentGeneratorArr[ objIndex ].getPosition( );
		while ( flgOK ) {
			flgOK = mAgentGeneratorArr[ objIndex ].generatePosition( p );
			if ( flgOK == false ) break;
			++mTotalNumOfSimulatedAgents;
			int agentIndex = addAgent( p );
			CS_AGENT &agent = mAgentArr[agentIndex];
			float speed = mAgentGeneratorArr[ objIndex ].getSpeed( );
			agent.group_id = i;
			agent.desired_v = speed;
			agent.cur_desired_v = speed;
			agent.setSimulationType( CS_AGENT_SIM_TYPE_ALONG_PATH );
			
			if ( pathIndex < 0 ) {
				cout << "pathIndex:" << pathIndex << endl;
			switch( pathIndex ) {
				case CS_AGENT_GEN_PATH_TYPE_CLOSEST:
					pathIndex = CS_PATH_MANAGER::instance()->findClosestPathIndex( p_agent_gen );
					break;
				case CS_AGENT_GEN_PATH_TYPE_RANDOM:
					pathIndex = CS_PATH_MANAGER::instance()->findOneRandomPathIndex( );

					break;
			}
				
				
				agent.setExternalPath( CS_PATH_MANAGER::instance()->getPath( pathIndex ) );
			} else {
				agent.setExternalPath( CS_PATH_MANAGER::instance()->getPath( pathIndex ) );
			}
			agent.reset( );
			agent.prepareBasedonSimulationType( );
		}
	}
	*/
}

void CROWD_SIMULATION::applyHardConstraints( bool flg_timer )
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
		s = 1.01*s;
		vector3 new_q1;
		new_q1 = q1 + s;
		if ( agent->flgHit ) {
			new_q1 += agent->sv;
		}

		agent->p = new_q1;
		if ( agent->p.x != agent->p.x ) {
			cout << "applyHardConstraints" << endl;
			cout << "NAN" << endl;
			printf_vector3("agent.p:", agent->p );
			printf_vector3("agent.sv:", agent->sv );
			printf_vector3("s:", s );
			printf_vector3("new_q1", new_q1 );
		}
		//mAgentArr[index].flgHit = flg_hit;
	}
	//-------------------------------------------------------------
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_APPY_HARD_CONSTRAINTS );
		mMainTimer->EndRecord( cTM_IDX_APPY_HARD_CONSTRAINTS );
	}
}

void CROWD_SIMULATION::handleAgentsReachedDestination( )
{
	mAgentProvider.resetIterator( );
	mAgentProvider.begin_update( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		bool flg = agent->isAtDestination( );
		if ( flg == true ) {
			++mTotalNumOfFinishedAgents;
			agent->mFlgUsed = false;
			mCrowdSimRecord.addTravelLength( agent->travelLength );
			mCrowdSimRecord.addTravelTime( agent->travelTime );

			if ( mLatticeManager ) {
				mLatticeManager->deleteElement( agent );
			}
		}
		mAgentProvider.updateUsedElement( agent );
	}
	mAgentProvider.end_update( );
}

int CROWD_SIMULATION::recordDataForAllUnfinishedAgents_WithPenality( float penLength, float penTime )
{

	mAgentProvider.begin_update( );
	mAgentProvider.begin_update( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		bool flg = agent->isAtDestination( );
		if ( flg == false ) {
			agent->mFlgUsed = false;
			mCrowdSimRecord.addTravelLength( penLength*agent->travelLength );
			mCrowdSimRecord.addTravelTime( penTime*agent->travelTime );

			if ( mLatticeManager ) {
				mLatticeManager->deleteElement( agent );
			}
		}
		mAgentProvider.updateUsedElement( agent );
	}
	mAgentProvider.end_update( );
	return mAgentProvider.getNumEle( );
}


void CROWD_SIMULATION::post_update( ) 
{
	mAgentProvider.resetIterator( );
	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		agent->post_update( mTimeStep );
		//mAgentArr[UIA[i]].printf( );

		if ( mLatticeManager ) {
			mLatticeManager->updateLatticeOfElement( agent );
		}
	}
	//mAgentProvider.end_update( );
}

BASE_OBJ *CROWD_SIMULATION::pickObj_XZ_BasedOnLattice( float x, float z ) const
{
	BASE_OBJ *obj = 0;
	int ix, iy, iz;
	vector3 p = vector3( x, 0.0, z );
	mLatticeManager->computeLatticeCoordinates( x, 0.0, z, ix, iy, iz );

		int idx, idy, idz;
		idx = 1;
		idy = 1;
		idz = 1;

		mLatticeManager->startIterator(ix, iy, iz, idx, idy, idz);
		CS_AGENT *agj;
		float distance2 = -1.0;
		while ( agj = mLatticeManager->getNext( ) ) {
			float d2 = agj->computeDistance2_XZ( p );
			if ( distance2 < 0.0 || distance2 > d2 ) {
				obj = agj;
				distance2 = d2;
			}
		}
/*
		if ( obj ) {
		cout << "obj:" << obj << endl;
		cout << "obj->getRadius2( ):" << obj->getRadius2( ) << endl;
		cout << "distance2:" << distance2 << endl;
		}
*/
		if ( distance2 >= 0.0 && obj->getRadius2( ) >= distance2 ) {
			mPickedObj = obj;
		} else {
			obj = 0;
		}
	
	return obj;
}

BASE_OBJ *CROWD_SIMULATION::pickObj_XZ( float x, float z ) const
{
	BASE_OBJ *obj = 0;

	mAgentProvider.resetIterator( );

	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		bool flg = agent->bo_isPicked_XZPlane( x, z );
		if ( flg ) {
			obj = agent;
			break;
		}
	}

	if ( obj == 0 ) {
		obj = CS_PATH_MANAGER::instance( )->pickPathPoint_XZPlane( x, z );
	}

	mPickedObj = obj;
	return obj;
}

void CROWD_SIMULATION::adjustAgentMotionBasedOnSeparationDistanceMap( bool flg_timer )
{
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

void CROWD_SIMULATION::computeAgentMovementMap( bool flg_timer )
{
	if ( flg_timer ) {
		mMainTimer->Start(cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP);
		mMainTimer->BeginRecord(cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP);
	}
	//-------------------------------------------------------------

	mAgentProvider.resetIterator( );

	CS_AGENT *agent;
	while ( agent = mAgentProvider.getNext( ) ) {
		float x, z;
		x = agent->p.x;
		z = agent->p.z;
		float v = 0.0002;
		mAgentMovementPath_Map->addAtPosition( x, z, v );
	}
	if ( flg_timer ) {
		mMainTimer->Record( cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP );
		mMainTimer->EndRecord( cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP );
	}
}


void CROWD_SIMULATION::update_crowd( bool flg_timer )
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
	resolveAgentCollision( flg_timer );
	//
	applyHardConstraints( flg_timer );
	//---------------------------------------------
	//resolveAgentCollision( flg_timer );
	//
	//applyHardConstraints( flg_timer );
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
	//
	mSimulationTime += mTimeStep;
	//
	


	++mFrame;
	//
}

void CROWD_SIMULATION::update_State_Simulation( )
{
	if (mFlgEnableRecordTime) {
		mMainTimer->OpenFile(TIMER_LOG_FILE_NAME);
	}

	mMainTimer->Reset();

	for ( int i = 0; i < mNumIterationsPerStep; ++i ) {
		update_crowd( true );
	}

	mMainTimer->ShowStats( );
	
	mCrowdSimRecord.showStatistics( );

	cout << "mFrame:\t" << mFrame << endl;
	cout << "mTimeStep:\t" << mTimeStep << endl;
	cout << "Simulation Time:\t" << mSimulationTime << endl;
	cout << "mTotalNumOfFinishedAgents:\t" << mTotalNumOfFinishedAgents << endl;
	int curNumOfAgents = mAgentProvider.getNumEle( );
	cout << "mCurNumOfAgents:\t" << curNumOfAgents << endl;
	cout << "mTotalNumOfSimulatedAgents:" << mTotalNumOfSimulatedAgents << endl;
}

void CROWD_SIMULATION::update_State_EditEnvObj( )
{

}

void CROWD_SIMULATION::update_State_AddEnvObj( )
{

}

void CROWD_SIMULATION::update( bool flg_timer ) 
{
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_SIMULATION:
			update_State_Simulation( );
			break;
		case CROWD_SIMULATION_ACTION_ADD_ENV_OBJ:
			update_State_AddEnvObj( );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			update_State_EditEnvObj( );
			break;
	}
}