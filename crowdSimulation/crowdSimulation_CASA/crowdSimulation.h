//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CROWD_SIMULATION_H_
#define __CROWD_SIMULATION_H_
#include "../../headers.h"
#include <string>
#include "../../imageReader/imageReader.h"
#include "../../openGLbasic/opengl_stuff.h"
#include "../../textureManager/textureManager.h"
#include "../../painter/painter.h"
#include "../../textureManager/texture.h"
#include "../../basic_app.h"
#include "../agent.h"
#include "../cs_basic_structs.h"
#include "../agent_generator.h"
#include "../agent_generator_manager.h"
#include "../myCellularAutomata/cellular_automata.h"
#include "../myDijkstra/myDijkstra.h"

//
#include "../../primitives/provider.h"
#include "../environment.h"

#include "../cs_record.h"
#include "../../spatial_partition_structures/lattice_manager.h"

#include "../pathManager.h"
#include "../../primitives/grid2D.h"

#include "../../msc_utility_tools/timer/timer.h"

#define CROWD_SIMULATION_ACTION_NONE					0
#define CROWD_SIMULATION_ACTION_SIMULATION				1
#define CROWD_SIMULATION_ACTION_ADD_ENV_OBJ				2
#define CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ			3
#define CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR		4
#define CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR	5
#define CROWD_SIMULATION_ACTION_ADD_PATH_OBJ			6
#define CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ			7

class CROWD_SIMULATION
{
protected:
	std::string mConfigExportDirectoryName;
	std::string mConfigDirectoryName;
	std::string mConfigIniFileName;
	std::string mTextureDirectoryName;
	float mMap2D_Grid_DX;
	float mMap2D_Grid_DZ;

	float mMapMinX, mMapMaxX, mMapMinZ, mMapMaxZ;

	int mActionType;

	int mMaxNumIterationsPerStep;
	int mNumIterationsPerStep;

	vector3 mAgentColor;
	vector3 mHighLightedAgentColor;

	mutable BASE_OBJ *mPickedObj;

	std::string mDataFile_Environment;
	std::string mDataFile_AgentPath;
	std::string mDataFile_AgentGenerator;
	std::string mDataFile_Textures;
	std::string mDataFile_PSOTraining;
	int mCollisionDetectionScheme;

	int mTraining_POS_NumFrames;

	int mDefaultNumOfAgents;
	bool mFlgSetupSceneEnvironment;

	int mFrame;
	float mTimeStep;
	double mSimulationTime;

	PROVIDER<CS_AGENT> mAgentProvider;
	AGENT_GENERATOR_MANAGER *mAgentGeneratorManager;
	LATTICE_MANAGER<CS_AGENT> *mLatticeManager;
	CS_CELLULAR_AUTOMATA *mCelluarAutomata; //2017/04/06
	//CS_DIJKSTRA *mDijkstra; //2017/04/23

	T_GRID_2D<float> *mAgentSeparationDistance_Map;
	T_GRID_2D<float> *mAgentMovementPath_Map;
	//
	int	mTotalNumOfFinishedAgents;
	int mTotalNumOfSimulatedAgents;
	//
	int mMaxNumOfAgents;
	//int mCurNumOfAgents;
	//CS_AGENT *mAgentArr;
	//int *mAgentUIA; // used index array for agents
	//
	///////////////////////////////////////////
	//
	
	CS_SIMULATION_RECORD mCrowdSimRecord;
	//
	
	//
	bool mFlgEnableRecordTime;
	cTIMER *mMainTimer;
	//
	bool mFlgTrainedPOS;
	//

	virtual void initInternalVariables( );


	virtual CS_AGENT * addAgent( const vector3 &p );
	virtual CS_AGENT * addAgent( const vector3 &p, const vector3 &destPosition );
	virtual CS_AGENT * addAgent( float x, float y, float z );
	virtual CS_AGENT * addAgent( float x, float y, float z, float dx, float dy, float dz );
	virtual void setAgentsDestination( const vector3 &dp, float r );
	//
	virtual void addPath( int pathIndex, const vector3 *p, int numPoints, float radius);
	//
	virtual void resolveAgentCollision_AllPairs( );
	virtual void resolveAgentCollision_Lattice( );
	//
	virtual void addAgentGenerator( CS_AGENT_GENERATOR *g );
	virtual void addAgentGenerator( int pathIndex, float radius, int numPerSecond, double maxGenerationTime = -1.0 );
	//
	virtual void pre_update( );
	virtual void generateAgents( bool flg_timer = false );
	virtual void computeAgentMotion(  bool flg_timer = false );
	virtual void adjustAgentMotionBasedOnSeparationDistanceMap(  bool flg_timer = false );
	virtual void modifyAgentVelocity_DueToEnvironment(  bool flg_timer = false );
	virtual void modifyAgentVelocity_DueToAgents( bool flg_timer = false );
	virtual void modifyAgentVelocity(  bool flg_timer = false );
	virtual void resolveAgentCollision(  bool flg_timer = false );
	virtual void updateAgentPosition(  bool flg_timer = false );
	virtual void applyHardConstraints(  bool flg_timer = false );
	virtual void computeAgentMovementMap(  bool flg_timer = false );
	virtual void post_update( );
	virtual void handleAgentsReachedDestination( );
	//
	virtual int recordDataForAllUnfinishedAgents_WithPenality( float penLength, float penTime );
	//
	virtual void prepareAgentsBasedonSimulationType( );
	//
	virtual void addAgentsInsideCircle( int num, const vector3 &p, float r );
	virtual void setup_scene_Environment( );
	virtual void setup_scene_SimTypePath( int num );
	
	//
	virtual void setup_scene_Environment_BasedOnDataFile( );
	//

	//
	virtual void reset( );
	//
	virtual void printf_ConfigIniData( ) const;
	virtual void printf_AppConfigData( ) const;
	//
	virtual void update_State_Simulation( );
	virtual void update_State_AddEnvObj( );
	virtual void update_State_EditEnvObj( );

public:
	CROWD_SIMULATION( );
	//
	virtual void deleteObj( );
	//
	virtual void update_crowd(  bool flg_timer = false );
	virtual void update(  bool flg_timer = false );
	virtual void readConfigIniFile( const char *fileName );
	virtual void readAppConfigFile( const char *fileName );
	virtual void readConfig();
	virtual void initializeBasedOnConfig( );
	virtual BASE_OBJ *pickObj_XZ( float x, float z ) const;
	virtual BASE_OBJ *pickObj_XZ_BasedOnLattice( float x, float z ) const;
	//
	virtual int getAction( ) const { return mActionType; }
	virtual bool handleKeyEvent( unsigned char key );
	//
	virtual void train_UsingPOS_OneGeneration( int numOfTimeSteps );
	virtual void train_UsingPOS_init( int pso_numParticles );
	virtual void train_UsingPOS( );
	// Drawing methods
	virtual void draw( ) const;
	virtual void drawAgent( const CS_AGENT &ag ) const;
	virtual void drawAgents( ) const;
	virtual void drawPath( const CS_PATH &path ) const;
	virtual void drawPaths( ) const;
	virtual void drawAgentGenerators( ) const;
	virtual void drawMap( ) const;
	virtual void drawEnvironment( ) const;
	virtual void drawSeparationDistanceMap( ) const;
	virtual void drawAgentMovementMap( ) const;
	virtual void drawLatticeSpace( ) const;
	//////////////////////////////////////////////
	virtual void createGLUI( int mainWinID, void (global_idle_func()), void *gui ) { }
	virtual void save( ) { }
	//
	virtual void setNumOfIterationsPerStep( int num );
	virtual void update_glui( ) { }
	virtual void refreshSimulation( );
	//
	virtual void setActionType_Simulation( );
	virtual void setActionType_AddEnvObj( );
	virtual void setActionType_EditEnvObj( );
	virtual void setActionType_AddAgentGenerator( );
	virtual void setActionType_AddPathObj( );
	virtual void setActionType_AddPathObjNow( );
	virtual void setActionType_EditAgentGenerator( );
	virtual void addEnvObj( ) { }
	//
	virtual void computeInformation( const CS_AGENT *g, CS_AGENT_INFORMATION &g_info ) const { }
	//
	virtual void draw_CelluarAutomata() const = 0; //2017/04/08
	virtual void draw_Lattice() const = 0; //2017/04/10
	virtual void draw_Border() const = 0; //2017/04/13
	virtual void draw_ColorTable() const = 0; //2017/04/14
	virtual void draw_dFF_ColorTable() const = 0; //2017/07/21
	virtual void draw_cell_exit_segments(int, int) const = 0; //2017/07/06
	virtual void draw_group() const = 0; //2017/07/24
	virtual void draw_specific_group() const = 0; //2017/07/24
	virtual void draw_group_circle() const = 0; //2017/08/11
	virtual void draw_guider() const = 0; //2017/08/17
	virtual void draw_agent_path() const = 0; //2017/08/18 
	virtual void draw_escape_direction() const = 0; //2017/08/21
	virtual void draw_stress_area() const = 0; //2017/09/04
	virtual void draw_compressive_leader() const = 0; //2017/09/16
	virtual void draw_strength_agent() const = 0; //2017/09/18
	virtual void draw_selfish_agent() const = 0; //2017/09/18
	virtual void draw_neutral_agent() const = 0; //2017/09/18
	virtual void draw_brave_agent() const = 0; //2017/09/18
	virtual void draw_occupied_cell() const = 0; //2017/09/28
	virtual void draw_volunteer_colorTable() const = 0; //2017/09/29
	virtual void draw_agent_anxiety() const = 0; //2017/10/18
	virtual void draw_agent_willness() const = 0; //2017/10/19
	virtual void draw_obstalce_density() const = 0; //2017/10/25
	virtual void draw_colorTable_withoutExit() const = 0; //2017/11/13
	virtual void draw_special_table() const = 0;
	virtual void draw_CurrentExitBlockAgent() const = 0;
	virtual void draw_AFF() const = 0;
	virtual void draw_TFF() const = 0;
	virtual void draw_AgentEvacuationTime() const = 0;
	virtual void draw_AgentEscapeExit() const = 0;
	virtual void sendFlgExitColor(bool) const = 0;
	virtual void reset_CellularAutomata() = 0; //2017/04/13
	virtual void start_CellularAutomata() = 0; //2017/08/06
	virtual void stop_CellularAutomata() = 0; //2017/08/06
	virtual void set_specific_group(int) const = 0; //2017/07/24
	virtual void set_numOfgroup_size(int, int) const = 0; //2017/07/26
	virtual void set_group_radius(int) const = 0; //2017/08/14
	virtual void show_node_intersectObstacle() const = 0; //2017/07/06
	virtual void show_cell_exit_segments() const = 0; //2017/07/07
	virtual void show_individual_agent_info() const = 0; //2017/08/03
	virtual void set_corner_coordinate(int, int) const = 0; //2017/08/03
	virtual void get_agent_info(int, int) const = 0; //2017/08/02
	virtual void set_cur_mouse_position(int, int) const = 0; //2017/09/04
	virtual void add_new_group() = 0; //2017/08/07
	virtual void set_group_leader() = 0; //2017/08/07
	virtual void set_guider(int) const = 0; //2017/08/17
	virtual void editScene() const = 0;
	virtual void edit_obstacle() = 0; //2017/09/11
	virtual void enabled_edit_obstacle() = 0; //2017/09/11 
	virtual void enabled_edit_exit() = 0;
	virtual void update_scene() = 0;
	virtual void load_simulation() = 0;
	virtual void set_ColorTable_ID(int) = 0; //2017/09/29
	virtual void set_ColorTable_WithoutExit_ID(int) = 0; //2017/11/13
	virtual void set_WillingThreshold(float) = 0;
	virtual void set_SpecialID(int) = 0;
	virtual void set_colorTableNormalizeValue(float) = 0;
	virtual int getMillisecond() const = 0; //2017/04/13
};
#endif