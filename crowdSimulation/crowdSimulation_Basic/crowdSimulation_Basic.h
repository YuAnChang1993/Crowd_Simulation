//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CROWD_SIMULATION_BASIC_H_
#define __CROWD_SIMULATION_BASIC_H_
#include "../crowdSimulation_CASA/crowdSimulation.h"
#include "../crowdMonitor/crowd_monitor_manager.h"
class CROWD_SIMULATION_BASIC : public CROWD_SIMULATION {
protected:
	//
	vector3 mDebug_Path_Q0;
	vector3 mDebug_Path_Q1;

	//
	//////////////////////////////////////////////
	//CROWD_MONITOR_MANAGER *mCrowdMonitorManager;
	//////////////////////////////////////////////
	virtual void modifyAgentVelocity( bool flg_timer );
	
	//
	virtual void save_appConfig( const string &fileName ) const;
	//
	virtual void save_environment( ) const;
	virtual void save_generators( ) const;
	virtual void save_paths( ) const;
	virtual void save_appConfig( ) const;
	//
	virtual void update_glui_property_environment( );
	virtual void update_glui_property_agent_generator( );
	virtual void update_glui( );
	//
	virtual void update_glui_property_path_obj( );
	virtual void createGLUI_EnvironmentObject( int mainWinID, void (global_idle_func()), void *glui_ptr );
	virtual void createGLUI_PathObject( int mainWinID, void (global_idle_func()), void *glui_ptr );
	//
	virtual void resolveAgentCollision_Lattice_PositionBased( );
	virtual void resolveAgentCollision_PositionBased( bool flg_timer = true );
	virtual void applyHardConstraints_PositionBased( bool flg_timer );
	virtual void update_crowd_PositionBased( bool flg_timer );
	//
	

	virtual void adjustAgentMotionBasedOnSeparationDistanceMap( bool flg_timer );
	virtual void adjustPathsBasedOnPathAffectingPoints( CS_PATH *path, float time_step );
	virtual void adjustPathsBasedOnPathAffectingPoints_01( CS_PATH *path, float time_step );
	//
	virtual void resolveAgentCollision_Lattice_VelocityBased( );
	virtual void resolveAgentCollision_VelocityBased( bool flg_timer = true );
	virtual void applyHardConstraints_VelocityBased( bool flg_timer );
	virtual void modifyAgentVelocity_DueToAgents_VelocityBased( bool flg_timer );
	virtual void update_crowd_VelocityBased( bool flg_timer );
	//
	void adjustPathsBasedOnCrowdMonitorManager_01( float time_step );
	void adjustPathsBasedOnCrowdMonitorManager_02( float time_step );
	void adjustPathsBasedOnCrowdMonitorManager_03( float time_step );

	void adjustPathsBasedOnPathAffectingPoints( float time_step );

	void computeClosestPointsOfEnvObjsToPaths( );

	void adjustPathsBasedOnCrowdMonitorManager( float time_step );
	//
	void updateLoadOfAgentsOfPaths( float time_step );
	//
	void draw_debug( ) const;

public:
	CROWD_SIMULATION_BASIC( );
	virtual void createGLUI( int mainWinID, void (global_idle_func()), void *gui );
	//
	virtual void setActionType_AddAgentGenerator( );
	virtual void setActionType_Simulation( );
	virtual void setActionType_AddEnvObj( );
	virtual void setActionType_EditEnvObj( );
	virtual void setActionType_AddPathObj( );
	virtual void setActionType_AddPathObjNow( );
	virtual void setActionType_EditPathObj( );
	virtual void setActionType_EditAgentGenerator( );

	//
	virtual void update_crowd( bool flg_timer );
	//
	virtual void unpickAllObjs( );
	//
	virtual void modifyAgentVelocity_DueToEnvironment( bool flg_timer = true);
	
	//
	virtual void addEnvObj( );
	//

	virtual void draw( ) const;
	//
	virtual void save( );
	//
	virtual void setCrowdSimulationParameters( );
	//
	virtual void computeInformation( const CS_AGENT *g, CS_AGENT_INFORMATION &g_info ) const;

};

#endif