//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "crowdSimulationApp.h"
#include "../msc_utility_tools/io/file_mng.h"
#include "../textureManager/textureManager.h"
#include "timer_index.h"

using namespace std;

CROWD_SIMULATION_APP::CROWD_SIMULATION_APP( )
{
	mCrowdSimulation = new MY_CROWD_SIMULATOR;

	mFlgSimulationEnabled = true;
	mClickedPickedObj = 0;
	mPassivePickedObj = 0;

	mFlgShow_SeparationDistanceMap = true;
	mFlgShow_AgentMovementMap = false;

	mFlgShow_LatticePartition = false;

	mFlgShow_Agents = true;
	mFlgShow_Grid = true;
	mFlgShow_Paths = true;
	mFlgShow_AgentGenerators = true;
	mFlgShow_Environment = true;
	mFlgShow_Map = true;
	
	mFlgShow_CelluarAutomata = true;
	mFlgShow_Lattice = false;
	mFlgShow_ColorTable = false;
	mFlgShow_dFF_ColorTable = false;
	mFlgShow_node_intersectObstacle = false;
	mFlgShow_CellExit_Segment = false;
	mFlgShow_Group_Member = false;
	mFlgShow_Specific_Group_Member = false;
	mFlgShow_individual_agent_info = true;
	mFlgShow_Group_Circle = false;
	mFlgShow_Guider = false;
	mFlgShow_Agent_Path = false;
	mFlgShow_Escape_Direction = false;
	mFlgShow_Stress_Area = false;
	mFlgShow_Compressive_Leader = false;
	mFlgShow_Strength_Agent = true;
	mFlgShow_Selfish_Agent = false;
	mFlgShow_Neutral_Agent = false;
	mFlgShow_Brave_Agent = false;
	mFlgShow_Occupied_Cell = false;
	mFlgShow_Volunteer_ColorTable = false;
	mFlgShow_Agent_Anxiety = true;
	mFlgShow_Agent_Willness = false;
	mFlgShow_Obstalce_Density = false;
	mFlgShow_ColorTable_WithoutExit = false;
	mFlgShow_Special_ColorTable = false;
	mFlgShow_CurrentExitBlockAgent = false;
	mFlgShow_AFF = false;
	mFlgShow_TFF = false;
	mFlgShow_EvacuationTime = false;
	mFlgShow_AgentEscapeExit = false;
	mFlgShow_AgentFaceDirection = false;
	mFlg_Enabled_Draw = 1;

	mFlgShow_ClickedAgentInformation = true;
	readConfigExperiment();
}

void CROWD_SIMULATION_APP::initApp( )
{
	vector3 p = vector3( 100.0, 0.0, 100.0 );
	vector3 dp = vector3( 500.0, 0.0, 100.0 );
	setGridCellDimension( 20.0, 20.0 );
}

void CROWD_SIMULATION_APP::readConfigExperiment()
{
	ifstream file;
	file.open("crowd_cellularAutomata/cellular_environment.txt");
	while (true)
	{
		string s;
		file >> s;
		if (s == "EXPERIMENT")
		{
			int type;
			file >> type;
			if (type == 1)
			{
				mFlg_Enabled_Draw = 0;
			}
			else
			{
				mFlg_Enabled_Draw = 1;
			}
		}
		if (file.eof())
			break;
	}
}

void CROWD_SIMULATION_APP::setFlgShowLatticePartition( bool flg )
{
	mFlgShow_LatticePartition = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgents( bool flg )
{
	mFlgShow_Agents = flg;
}

void CROWD_SIMULATION_APP::setFlgShowMap( bool flg )
{
	mFlgShow_Map = flg;
}

void CROWD_SIMULATION_APP::setFlgShowPaths( bool flg )
{
	mFlgShow_Paths = flg;
}

void CROWD_SIMULATION_APP::setFlgShowEnvObjs( bool flg )
{
	mFlgShow_Environment = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentGenerators( bool flg )
{
	mFlgShow_AgentGenerators = flg;
}

void CROWD_SIMULATION_APP::setFlgShowGrid( bool flg )
{
	mFlgShow_Grid = flg;
}

void CROWD_SIMULATION_APP::setFlgShowSeparationMap( bool flg )
{
	mFlgShow_SeparationDistanceMap = flg;
}

void CROWD_SIMULATION_APP::setFlgShowCelluarAutomata(bool flg) //2017/04/13
{
	mFlgShow_CelluarAutomata = flg;
}

void CROWD_SIMULATION_APP::setFlgShowLattice(bool flg) //2017/04/13
{
	mFlgShow_Lattice = flg;
}

void CROWD_SIMULATION_APP::setFlgShowColorTable(bool flg) //2017/04/14
{
	mFlgShow_ColorTable = flg;
}

void CROWD_SIMULATION_APP::setFlgShowDFFColorTable(bool flg) //2017/07/21
{
	mFlgShow_dFF_ColorTable = flg;
}

void CROWD_SIMULATION_APP::setFlgShowNodeIntersectObstacle(bool flg) //2017/07/06
{
	mFlgShow_node_intersectObstacle = flg;
}

void CROWD_SIMULATION_APP::setFlgShowCellExitSegment(bool flg) //2017/07/07
{
	mFlgShow_CellExit_Segment = flg;
}

void CROWD_SIMULATION_APP::setFlgShowGroupMember(bool flg) //2017/07/24
{
	mFlgShow_Group_Member = flg;
}

void CROWD_SIMULATION_APP::setFlgShowSpecificGroupMember(bool flg) //2017/07/24
{
	mFlgShow_Specific_Group_Member = flg;
}

void CROWD_SIMULATION_APP::setFlgShowGroupCircle(bool flg) //2017/08/11
{
	mFlgShow_Group_Circle = flg;
}

void CROWD_SIMULATION_APP::setFlgShowGuider(bool flg) //2017/08/17
{
	mFlgShow_Guider = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentPath(bool flg) //2017/08/18
{
	mFlgShow_Agent_Path = flg;
}

void CROWD_SIMULATION_APP::setFlgShowEscapeDirection(bool flg) //2017/08/21
{
	mFlgShow_Escape_Direction = flg;
}

void CROWD_SIMULATION_APP::setFlgShowStressArea(bool flg) //2017/09/04
{
	mFlgShow_Stress_Area = flg;
}

void CROWD_SIMULATION_APP::setFlgShowCompressiveLeader(bool flg) //2017/09/16
{
	mFlgShow_Compressive_Leader = flg;
}

void CROWD_SIMULATION_APP::setFlgShowStrengthAgent(bool flg) //2017/09/18
{
	mFlgShow_Strength_Agent = flg;
}

void CROWD_SIMULATION_APP::setFlgShowSelfishAgent(bool flg) //2017/09/18
{
	mFlgShow_Selfish_Agent = flg;
}

void CROWD_SIMULATION_APP::setFlgShowNeutralAgent(bool flg) //2017/09/18
{
	mFlgShow_Neutral_Agent = flg;
}

void CROWD_SIMULATION_APP::setFlgShowBraveAgent(bool flg) //2017/09/18
{
	mFlgShow_Brave_Agent = flg;
}

void CROWD_SIMULATION_APP::setFlgShowOccupiedCell(bool flg) //2017/09/28
{
	mFlgShow_Occupied_Cell = flg;
}

void CROWD_SIMULATION_APP::setFlgShowVolunteerColorTable(bool flg) //2017/09/29
{
	mFlgShow_Volunteer_ColorTable = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentAnxiety(bool flg) //2017/10/18
{
	mFlgShow_Agent_Anxiety = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentWillness(bool flg) //2017/10/19
{
	mFlgShow_Agent_Willness = flg;
}

void CROWD_SIMULATION_APP::setFlgShowObstacleDensity(bool flg) //2017/10/25
{
	mFlgShow_Obstalce_Density = flg;
}


void CROWD_SIMULATION_APP::setFlgShowColorTableWithoutExit(bool flg) //2017/11/13
{
	mFlgShow_ColorTable_WithoutExit = flg;
}

void CROWD_SIMULATION_APP::setFlgShowSpecialColorTable(bool flg)
{
	mFlgShow_Special_ColorTable = flg;
}

void CROWD_SIMULATION_APP::setFlgShowCurrentExitBlockAgent(bool flg)
{
	mFlgShow_CurrentExitBlockAgent = flg;
	sendFlgCurrentExitBlockAgent();
}

void CROWD_SIMULATION_APP::setFlgShowAFF(bool flg)
{
	mFlgShow_AFF = flg;
}

void CROWD_SIMULATION_APP::setFlgShowTFF(bool flg)
{
	mFlgShow_TFF = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentEvacuationTime(bool flg)
{
	mFlgShow_EvacuationTime = flg;
}

void CROWD_SIMULATION_APP::setFlgShowAgentEscapeExit(bool flg)
{
	mFlgShow_AgentEscapeExit = flg;
	sendFlgCurrentExitBlockAgent();
}

void CROWD_SIMULATION_APP::setFlgShowAgentFaceDirection(bool flg)
{
	mFlgShow_AgentFaceDirection = flg;
}

void CROWD_SIMULATION_APP::sendFlgCurrentExitBlockAgent()
{
	mCrowdSimulation->sendFlgExitColor(mFlgShow_CurrentExitBlockAgent || mFlgShow_AgentEscapeExit);
}

void CROWD_SIMULATION_APP::resetCelluarAutomata() //2017/04/13
{
	mCrowdSimulation->reset_CellularAutomata();
}

void CROWD_SIMULATION_APP::startCellularAutomata() //2017/08/06
{
	mCrowdSimulation->start_CellularAutomata();
}

void CROWD_SIMULATION_APP::stopCellularAutomata() //2017/08/06
{
	mCrowdSimulation->stop_CellularAutomata();
}

void CROWD_SIMULATION_APP::addNewGroup() //2017/08/07
{
	mCrowdSimulation->add_new_group();
}

void CROWD_SIMULATION_APP::setGroupLeader() //2017/08/07
{
	mCrowdSimulation->set_group_leader();
}

void CROWD_SIMULATION_APP::showCellExitSegment(int x, int z) //2017/07/07
{
	mCrowdSimulation->draw_cell_exit_segments(x, z);
}

void CROWD_SIMULATION_APP::showSpecificGroupMember(int ID) //2017/07/24
{
	mCrowdSimulation->set_specific_group(ID);
}

void CROWD_SIMULATION_APP::setNumOfGroupSize(int size2_num, int size3_num) //2017/07/26
{
	mCrowdSimulation->set_numOfgroup_size(size2_num, size3_num);
}

void CROWD_SIMULATION_APP::setGroupRadius(int group_radius) //2017/08/14
{
	mCrowdSimulation->set_group_radius(group_radius);
}

void CROWD_SIMULATION_APP::setGuider(int guider_height) //2017/08/17
{
	mCrowdSimulation->set_guider(guider_height);
}

void CROWD_SIMULATION_APP::edit_obstacle()
{
	mCrowdSimulation->edit_obstacle();
}

void CROWD_SIMULATION_APP::enabled_edit_obstacle()
{
	mCrowdSimulation->enabled_edit_obstacle();
}

void CROWD_SIMULATION_APP::enabled_edit_exit()
{
	mCrowdSimulation->enabled_edit_exit();
}

void CROWD_SIMULATION_APP::update_scene()
{
	mCrowdSimulation->update_scene();
}

void CROWD_SIMULATION_APP::load_simulation()
{
	mCrowdSimulation->load_simulation();
}

void CROWD_SIMULATION_APP::setColorTableID(int ID)
{
	mCrowdSimulation->set_ColorTable_ID(ID);
}

void CROWD_SIMULATION_APP::setColorTableWithoutExitID(int ID)
{
	mCrowdSimulation->set_ColorTable_WithoutExit_ID(ID);
}

void CROWD_SIMULATION_APP::setWillingThreshold(float willing)
{
	mCrowdSimulation->set_WillingThreshold(willing);
}

void CROWD_SIMULATION_APP::setSpecialID(int ID)
{
	mCrowdSimulation->set_SpecialID(ID);
}

void CROWD_SIMULATION_APP::setColorTableNormalizeValue(float normalizeValue)
{
	mCrowdSimulation->set_colorTableNormalizeValue(normalizeValue);
}

void CROWD_SIMULATION_APP::update( )
{
	if ( mEnabledApp == false ) return;
	if ( mFlgSimulationEnabled == false ) return;

	mCrowdSimulation->update( );
}


