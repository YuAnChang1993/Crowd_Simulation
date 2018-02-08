#include "crowdSimulation_Basic.h"
#include "../agent_generator_manager.h"
#include "../cs_parameters.h"
//
// inside the plane, neg y-axis
//
CROWD_SIMULATION_BASIC::CROWD_SIMULATION_BASIC( )
{
//	mCrowdMonitorManager = new CROWD_MONITOR_MANAGER( csp_maxNum_CrowdMonitorManagers );
	
	
	csp_init( );
	setCrowdSimulationParameters( );
	//

	//CROWD_MONITOR crowd_monitor;
	//crowd_monitor.set( vector3( 0.0, 0.0, 0.0), 10.0 ) ;
	CROWD_MONITOR_MANAGER::instance( csp_maxNum_CrowdMonitorManagers);
}

void CROWD_SIMULATION_BASIC::addEnvObj( )
{
	CS_ENVIRONMENT::instance( )->end_Action( true );
}

void CROWD_SIMULATION_BASIC::setActionType_Simulation( )
{
	mActionType = CROWD_SIMULATION_ACTION_SIMULATION;
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_NONE );
	//
	vector3 aabbMin0, aabbMax0;
	CS_ENVIRONMENT::instance( )-> computeAABB( aabbMin0, aabbMax0 );
	CS_PATH_MANAGER::instance( )-> computeAABB( aabbMin0, aabbMax0 );
}

void CROWD_SIMULATION_BASIC::setActionType_EditEnvObj( )
{
	mActionType = CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ;
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_EDIT_OBJ );
}

void CROWD_SIMULATION_BASIC::setActionType_EditAgentGenerator( )
{
	mActionType = CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR;
	AGENT_GENERATOR_MANAGER::instance( )->setObjAction( CS_EDIT_ACTION_EDIT_OBJ );
}

void CROWD_SIMULATION_BASIC::setActionType_EditPathObj( )
{
	mActionType = CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ;
	CS_PATH_MANAGER::instance( )->setObjAction( CS_EDIT_ACTION_EDIT_OBJ );
}


void CROWD_SIMULATION_BASIC::setActionType_AddEnvObj( )
{
	mActionType = CROWD_SIMULATION_ACTION_ADD_ENV_OBJ;
	CS_ENVIRONMENT::instance( )->setObjAction( CS_EDIT_ACTION_ADD_OBJ );
	CS_ENVIRONMENT::instance( )->start_Action( );
}

void CROWD_SIMULATION_BASIC::setActionType_AddAgentGenerator( )
{
	cout << "CROWD_SIMULATION_BASIC::setActionType_AddAgentGenerator( )" << endl;
	mActionType = CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR;
	AGENT_GENERATOR_MANAGER::instance( )->setObjAction( CS_EDIT_ACTION_ADD_OBJ );
	AGENT_GENERATOR_MANAGER::instance( )->start_Action( );
}

void CROWD_SIMULATION_BASIC::setActionType_AddPathObj( )
{
	cout << "CROWD_SIMULATION_BASIC::setActionType_AddPathObj( )" << endl;
	mActionType = CROWD_SIMULATION_ACTION_ADD_PATH_OBJ;
	
	CS_PATH_MANAGER::instance( )->setObjAction( CS_EDIT_ACTION_ADD_OBJ );
	CS_PATH_MANAGER::instance( )->start_Action( );
	
}

void CROWD_SIMULATION_BASIC::setActionType_AddPathObjNow( )
{
	CS_PATH_MANAGER::instance( )->addObjNow( );
	CS_PATH_MANAGER::instance( )->setObjAction( CS_EDIT_ACTION_ADD_OBJ );
}

void CROWD_SIMULATION_BASIC::unpickAllObjs( )
{
	CS_PATH_MANAGER::instance( )->unpickSelectedObj( );
	AGENT_GENERATOR_MANAGER::instance( )->unpickSelectedObj( );
	CS_ENVIRONMENT::instance( )->unpickSelectedObj( );

}

void CROWD_SIMULATION_BASIC::computeInformation( const CS_AGENT *agi, CS_AGENT_INFORMATION &g_info ) const
{
	if ( agi == 0 ) return;
	g_info.clear( );

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
			g_info.neighbor.push_back( agj );
		}
}
