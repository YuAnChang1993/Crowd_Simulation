//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __CROWD_SIMULATION_APP_H_
#define __CROWD_SIMULATION_APP_H_

#include "../headers.h"
#include <string>
#include "../imageReader/imageReader.h"
#include "../openGLbasic/opengl_stuff.h"
#include "../textureManager/textureManager.h"
#include "../painter/painter.h"
#include "../textureManager/texture.h"
#include "./crowdSimulation_CASA/crowdSimulation.h"
#include "./crowdSimulation_Basic/crowdSimulation_Basic.h"
#include "./myCrowdSimulator/myCrowdSimulation.h"

#include "../freeCanvas2D/freeCanvas2D.h"

using namespace ns_opengl;
class CROWD_SIMULATION_APP : public FREE_CANVAS_2D
{
	
protected:

	//CROWD_SIMULATION *mCrowdSimulation;

	bool mFlgSimulationEnabled;

	int mFlgShow_SeparationDistanceMap;
	bool mFlgShow_AgentMovementMap;
	bool mFlgShow_Agents;
	bool mFlgShow_Grid;
	bool mFlgShow_Paths;
	bool mFlgShow_AgentGenerators;
	bool mFlgShow_Environment;
	bool mFlgShow_LatticePartition;
	bool mFlgShow_Map;
	bool mFlgShow_CelluarAutomata; //2017/04/07
	bool mFlgShow_Lattice; //2017/04/10
	bool mFlgShow_ColorTable; //2017/04/14
	bool mFlgShow_dFF_ColorTable; //2017/07/21
	bool mFlgShow_node_intersectObstacle; //2017/07/06
	bool mFlgShow_CellExit_Segment; //2017/07/07
	bool mFlgShow_Group_Member; //2017/07/24
	bool mFlgShow_Specific_Group_Member; //2017/07/24
	bool mFlgShow_individual_agent_info; //2017/08/03
	bool mFlgShow_Group_Circle; //2017/08/11
	bool mFlgShow_Guider; //2017/08/17
	bool mFlgShow_Agent_Path; //2017/08/18
	bool mFlgShow_Escape_Direction; //2017/08/21
	bool mFlgShow_Stress_Area; //2017/09/04
	bool mFlgShow_Compressive_Leader; //2017/09/16
	bool mFlgShow_Strength_Agent; //2017/09/18
	bool mFlgShow_Selfish_Agent; //2017/09/18
	bool mFlgShow_Neutral_Agent; //2017/09/18
	bool mFlgShow_Brave_Agent; //2017/09/18
	bool mFlgShow_Occupied_Cell; //2017/09/28
	bool mFlgShow_Volunteer_ColorTable; //2017/09/29
	bool mFlgShow_Agent_Anxiety; //2017/10/18
	bool mFlgShow_Agent_Willness; //2017/10/18
	bool mFlgShow_Obstalce_Density; //2017/10/25
	bool mFlgShow_ColorTable_WithoutExit; //2017/11/13
	bool mFlgShow_Special_ColorTable;
	bool mFlgShow_CurrentExitBlockAgent;
	bool mFlgShow_AFF;
	bool mFlgShow_TFF;
	bool mFlgShow_EvacuationTime;
	bool mFlgShow_AgentEscapeExit;
	bool mFlgShow_AgentFaceDirection;
	int mFlg_Enabled_Draw;
	int mFlgShow_ClickedAgentInformation;

	int mMousePressedButton;

	BASE_OBJ *mPassivePickedObj;
	BASE_OBJ *mClickedPickedObj;
	vector<BASE_OBJ *> mSelectedObjArr;
	vector3 mSelectedPoint;

	void drawOrigin( ) const;

	void test_draw( ) const;

	void clearPickedObj( );

	bool mouseMotionFunc_State_AddPathObj( int mx, int my );
	bool mouseMotionFunc_State_EditPathObj( int mx, int my );
	bool mouseMotionFunc_State_EditAgentGenerator( int mx, int my );
	bool mouseMotionFunc_State_AddAgentGenerator( int mx, int my );
	bool mouseMotionFunc_State_AddEnvObj( int mx, int my );
	bool mouseMotionFunc_State_EditEnvObj( int mx, int my );
	bool mouseMotionFunc_State_Simulation( int mx, int my );

	bool mouseFunc_State_AddPathObj( int button, int state, int x, int y, float hitDistance );
	bool mouseFunc_State_EditPathObj( int button, int state, int x, int y, float hitDistance );
	bool mouseFunc_State_AddAgentGenerator( int button, int state, int mx, int my, float hitDistance );
	bool mouseFunc_State_EditAgentGenerator( int button, int state, int mx, int my, float hitDistance );
	bool mouseFunc_State_AddEnvObj( int button, int state, int x, int y, float hitDistance );
	bool mouseFunc_State_EditEnvObj( int button, int state, int x, int y, float hitDistance );
	bool mouseFunc_State_Simulation( int button, int state, int x, int y );

	bool passiveMouseFunc_State_AddPathObj( int mx, int my );
	bool passiveMouseFunc_State_EditPathObj( int mx, int my, float hitDistance );
	bool passiveMouseFunc_State_AddAgentGenerator( int mx, int my );
	bool passiveMouseFunc_State_EditAgentGenerator( int mx, int my, float hitDistance );
	bool passiveMouseFunc_State_AddEnvObj( int mx, int my );
	bool passiveMouseFunc_State_EditEnvObj( int mx, int my, float hitDistance );
	bool passiveMouseFunc_State_Simulation( int mx, int my );

	void save( ) const;
	
	void handleSpaceBarEvent( );
public:
	CROWD_SIMULATION_APP( );
	virtual void update( );
	virtual void draw( ) const;
	virtual void initApp();
	void readConfigExperiment();
	
	void resetCelluarAutomata(); // 2017/04/13
	void startCellularAutomata(); //2017/08/06
	void stopCellularAutomata(); //2017/08/06
	void addNewGroup(); //2017/08/07
	void setGroupLeader(); //2017/08/07
	void showCellExitSegment(int, int); // 2017/07/07
	void showSpecificGroupMember(int); // 2017/07/24 
	void setNumOfGroupSize(int, int); //2017/07/26
	void setGroupRadius(int); //2017/08/14
	void setGuider(int); //2017/08/17
	void edit_obstacle(); //2017/09/11
	void enabled_edit_obstacle(); //2017/09/11
	void enabled_edit_exit();
	void update_scene();
	void load_simulation();
	void setColorTableID(int); //2017/09/29
	void setColorTableWithoutExitID(int); //2017/11/13
	void setWillingThreshold(float); //2017/11/30
	void setSpecialID(int);
	void setColorTableNormalizeValue(float);

	void setFlgShowLatticePartition( bool flg );
	void setFlgShowAgents( bool flg );
	void setFlgShowMap( bool flg );
	void setFlgShowPaths( bool flg );
	void setFlgShowEnvObjs( bool flg );
	void setFlgShowAgentGenerators( bool flg );
	void setFlgShowGrid( bool flg );
	void setFlgShowSeparationMap( bool flg );
	void setFlgShowCelluarAutomata(bool flg); // 2017/04/13
	void setFlgShowLattice(bool flg); //2017/04/13
	void setFlgShowColorTable(bool flg); //2017/04/14
	void setFlgShowDFFColorTable(bool flg); //2017/07/21
	void setFlgShowGroupMember(bool flg); //2017/07/24
	void setFlgShowSpecificGroupMember(bool flg); //2017/07/24
	void setFlgShowNodeIntersectObstacle(bool flg); //2017/07/06
	void setFlgShowCellExitSegment(bool flg); //2017/07/07
	void setFlgShowGroupCircle(bool flg); //2017/08/11
	void setFlgShowGuider(bool flg); //2017/08/17
	void setFlgShowAgentPath(bool flg); //2017/08/18
	void setFlgShowEscapeDirection(bool flg); //2017/08/21
	void setFlgShowStressArea(bool flg); //2017/09/04
	void setFlgShowCompressiveLeader(bool flg); //2017/09/16
	void setFlgShowStrengthAgent(bool flg); //2017/09/18
	void setFlgShowSelfishAgent(bool flg); //2017/09/18 
	void setFlgShowNeutralAgent(bool flg); //2017/09/18
	void setFlgShowBraveAgent(bool flg); //2017/09/18
	void setFlgShowOccupiedCell(bool flg); //2017/09/28
	void setFlgShowVolunteerColorTable(bool flg); //2017/09/29
	void setFlgShowAgentAnxiety(bool flg); //2017/10/18
	void setFlgShowAgentWillness(bool flg); //2017/10/19
	void setFlgShowObstacleDensity(bool flg); //2017/10/25
	void setFlgShowColorTableWithoutExit(bool flg); //2017/11/13
	void setFlgShowSpecialColorTable(bool flg);
	void setFlgShowCurrentExitBlockAgent(bool flg);
	void setFlgShowAFF(bool flg);
	void setFlgShowTFF(bool flg);
	void setFlgShowAgentEvacuationTime(bool flg);
	void setFlgShowAgentEscapeExit(bool flg);
	void setFlgShowAgentFaceDirection(bool flg);
	void sendFlgCurrentExitBlockAgent();

	bool handleKeyEvent( unsigned char key );
	bool passiveMouseFunc( int mx, int my );
	bool mouseMotionFunc( int x, int y );
	bool mouseFunc( int button, int state, int x, int y );
	bool specialFunc(int key, int x, int y);

	virtual void createGLUI( int mainWinID, void (global_idle_func()) );

};

#endif