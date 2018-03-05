#include "crowdSimulationApp.h"
#include "cs_parameters.h"

#define RESET_CELLUAR_AUTOMATA 900
#define SHOW_CELL_EXIT_SEGMENT 901
#define SHOW_GROUP_MEMBER 902
#define SET_GROUP_SIZE 903
#define START_CELLULAR_AUTOMATA 904
#define STOP_CELLULAR_AUTOMATA 905
#define ADD_GROUP 906
#define SET_LEADER 907
#define SET_GROUP_RADIUS 908
#define SET_GUIDER 909
#define EDIT_OBSTACLE 910
#define SET_TABLE_ID 911
#define SET_TABLE_WITHOUT_EXIT_ID 912
#define SET_WILLING_THRESHOLD 913
#define SET_SPECIAL_TABLE_ID 914
#define SET_COLORTABLE_NORMALIZEVALUE 915
#define EDIT_EXIT 916
#define EDIT_OBSTACLES 917
#define EDIT_AGENT 918
#define UPDATE_SCENE 919
#define LOAD_SIMULATION 920

GLUI *glui_main;

namespace 
{

	CROWD_SIMULATION_APP *__crowdSimulationApp = 0;

	int flgShowCelluarAutomata = true; //2017/04/13
	int flgShowLattice = false; //2017/04/13
	int flgShowColorTable = false; //2017/04/14
	int flgShowDFFColorTable = false; //2017/07/21
	int flgShowNodeIntersectObstacle = false; //2017/07/06
	int flgShowCellExitSegment = false; //2017/07/06
	int flgShowGroupMember = false; //2017/07/24
	int flgShowSpecificGroupMember = false; //2017/07/24
	int flgShowGroupCircle = false; //2017/08/11
	int flgShowGuider = false; //2017/08/17
	int flgShowAgentPath = false; //2017/08/18
	int flgShowEscapeDirection = false; //2017/08/21
	int flgShowStressArea = false; //2017/09/04
	int flgShowCompressiveLeader = false; //2017/09/16
	int flgShowStrengthAgent = true;//2017/09/18
	int flgShowSelfishAgent = false;//2017/09/18
	int flgShowNeutralAgent = false;//2017/09/18
	int flgShowBraveAgent = false;//2017/09/18
	int flgShowOccupiedCell = false;//2017/09/28
	int flgShowVolunteerColorTable = false;//2017/09/29
	int flgShowAgentAnxiety = true;//2017/10/18
	int flgShowAgentWillness = false;//2017/10/19
	int flgShowObstacleDensity = false;//2017/10/25
	int flgShowColorTableWithoutExit = false;//2017/11/13
	int flgShowSpecialColorTable = false;
	int flgShowCurrentExitBlockAgent = false;
	int flgShowAFF = false;
	int flgShowTFF = false;
	int flgShowEvacuationTime = false;
	int flgShowEscapeExit = false;
	int flgShowAgentFaceDirection = false;

	GLUI_Checkbox *checkbox_showItem_celluarAutomata;
	GLUI_Checkbox *checkbox_showItem_lattice;
	GLUI_Checkbox *checkbox_showItem_colorTable;
	GLUI_Checkbox *checkbox_showItem_dFF_colorTable;
	GLUI_Checkbox *checkbox_showItem_nodeIntersectObstacle;
	GLUI_Checkbox *checkbox_showItem_cellExitSegment;
	GLUI_Checkbox *checkbox_showItem_groupMember;
	GLUI_Checkbox *checkbox_showItem_specificGroupMember;
	GLUI_Checkbox *checkbox_showItem_groupCircle;
	GLUI_Checkbox *checkbox_showItem_guider;
	GLUI_Checkbox *checkbox_showItem_agent_path;
	GLUI_Checkbox *checkbox_showItem_escape_direction;
	GLUI_Checkbox *checkbox_showItem_stress_area;
	GLUI_Checkbox *checkbox_showItem_compress_leader;
	GLUI_Checkbox *checkbox_showItem_strength_agent;
	GLUI_Checkbox *checkbox_showItem_selfish_agent;
	GLUI_Checkbox *checkbox_showItem_neutral_agent;
	GLUI_Checkbox *checkbox_showItem_brave_agent;
	GLUI_Checkbox *checkbox_showItem_occupied_cell;
	GLUI_Checkbox *checkbox_showItem_volunteer_colorTable;
	GLUI_Checkbox *checkbox_showItem_anxiety;
	GLUI_Checkbox *checkbox_showItem_willness;
	GLUI_Checkbox *checkbox_showItem_density;
	GLUI_Checkbox *checkbox_showItem_colorTableWithoutExit;
	GLUI_Checkbox *checkbox_showItem_specialColorTable;
	GLUI_Checkbox *checkbox_showItem_currentExitBlockAgent;
	GLUI_Checkbox *checkbox_showItem_AFF;
	GLUI_Checkbox *checkbox_showItem_TFF;
	GLUI_Checkbox *checkbox_showItem_evacuationTIme;
	GLUI_Checkbox *checkbox_showItem_escapeExit;
	GLUI_Checkbox *checkbox_showItem_agentFaceDirection;
	GLUI_Spinner *spinner_cell_exits_segment_x;
	GLUI_Spinner *spinner_cell_exits_segment_z;
	GLUI_Spinner *spinner_group_num;
	GLUI_Spinner *spinner_numOfgroup_size2;
	GLUI_Spinner *spinner_numOfgroup_size3;
	GLUI_Spinner *spinner_group_radius;
	GLUI_Spinner *spinner_guider_height;
	GLUI_Spinner *spinner_volunteer_colorTable;
	GLUI_Spinner *spinner_colorTable_withoutExit;
	GLUI_Spinner *spinner_willing_threshold;
	GLUI_Spinner *spinner_special_id;
	GLUI_Spinner *spinner_specialColorTableNormalizeValue;
	//GLUI_Panel *panel_agent_info;
	int cell_exits_segment_x = 0;
	int cell_exits_segment_z = 0;
	int group_num = 1;
	int numOfgroup_size2 = 0;
	int numOfgroup_size3 = 0;
	int group_radius = 5;
	int guider_height = 160;
	int colorTable_id = 0;
	int colorTable_withoutExit_id = 0;
	float willing_threshold = 0.3f;
	int specialTableID = 0;
	float specialColorTableNormalizeValue = 50.0f;

	void control_show_items( int control )
	{
		/********************************************************************
		Here we'll print the user id of the control that generated the
		callback, and we'll also explicitly get the values of each control.
		Note that we really didn't have to explicitly get the values, since
		they are already all contained within the live variables:
		'wireframe',  'segments',  'obj',  and 'text'  
		********************************************************************/
		flgShowCelluarAutomata = checkbox_showItem_celluarAutomata->get_int_val();
		flgShowLattice = checkbox_showItem_lattice->get_int_val();
		flgShowColorTable = checkbox_showItem_colorTable->get_int_val();
		flgShowDFFColorTable = checkbox_showItem_dFF_colorTable->get_int_val();
		flgShowNodeIntersectObstacle = checkbox_showItem_nodeIntersectObstacle->get_int_val();
		/*flgShowGroupMember = checkbox_showItem_groupMember->get_int_val();
		flgShowSpecificGroupMember = checkbox_showItem_specificGroupMember->get_int_val();
		flgShowGroupCircle = checkbox_showItem_groupCircle->get_int_val();
		flgShowGuider = checkbox_showItem_guider->get_int_val();
		flgShowAgentPath = checkbox_showItem_agent_path->get_int_val();
		flgShowEscapeDirection = checkbox_showItem_escape_direction->get_int_val();*/
		flgShowStressArea = checkbox_showItem_stress_area->get_int_val();
		flgShowStrengthAgent = checkbox_showItem_strength_agent->get_int_val();
		/*flgShowSelfishAgent = checkbox_showItem_selfish_agent->get_int_val();
		flgShowNeutralAgent = checkbox_showItem_neutral_agent->get_int_val();
		flgShowBraveAgent = checkbox_showItem_brave_agent->get_int_val();*/
		flgShowOccupiedCell = checkbox_showItem_occupied_cell->get_int_val();
		//flgShowVolunteerColorTable = checkbox_showItem_volunteer_colorTable->get_int_val();
		flgShowAgentAnxiety = checkbox_showItem_anxiety->get_int_val();
		flgShowAgentWillness = checkbox_showItem_willness->get_int_val();
		flgShowObstacleDensity = checkbox_showItem_density->get_int_val();
		flgShowColorTableWithoutExit = checkbox_showItem_colorTableWithoutExit->get_int_val();
		flgShowSpecialColorTable = checkbox_showItem_specialColorTable->get_int_val();
		flgShowCurrentExitBlockAgent = checkbox_showItem_currentExitBlockAgent->get_int_val();
		flgShowAFF = checkbox_showItem_AFF->get_int_val();
		flgShowTFF = checkbox_showItem_TFF->get_int_val();
		flgShowEvacuationTime = checkbox_showItem_evacuationTIme->get_int_val();
		flgShowEscapeExit = checkbox_showItem_escapeExit->get_int_val();
		flgShowAgentFaceDirection = checkbox_showItem_agentFaceDirection->get_int_val();
		__crowdSimulationApp->setFlgShowCelluarAutomata((flgShowCelluarAutomata == 0) ? false : true);
		__crowdSimulationApp->setFlgShowLattice((flgShowLattice == 0) ? false : true);
		__crowdSimulationApp->setFlgShowColorTable((flgShowColorTable == 0) ? false : true);
		__crowdSimulationApp->setFlgShowDFFColorTable((flgShowDFFColorTable == 0) ? false : true);
		__crowdSimulationApp->setFlgShowNodeIntersectObstacle((flgShowNodeIntersectObstacle == 0) ? false : true);
		__crowdSimulationApp->setFlgShowCellExitSegment((flgShowCellExitSegment == 0) ? false : true);
		__crowdSimulationApp->setFlgShowGroupMember((flgShowGroupMember == 0) ? false : true);
		__crowdSimulationApp->setFlgShowSpecificGroupMember((flgShowSpecificGroupMember == 0) ? false : true);
		__crowdSimulationApp->setFlgShowGroupCircle((flgShowGroupCircle == 0) ? false : true);
		__crowdSimulationApp->setFlgShowGuider((flgShowGuider == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentPath((flgShowAgentPath == 0) ? false : true);
		__crowdSimulationApp->setFlgShowEscapeDirection((flgShowEscapeDirection == 0) ? false : true);
		__crowdSimulationApp->setFlgShowStressArea((flgShowStressArea == 0) ? false : true);
		__crowdSimulationApp->setFlgShowCompressiveLeader((flgShowCompressiveLeader == 0) ? false : true);
		__crowdSimulationApp->setFlgShowStrengthAgent((flgShowStrengthAgent == 0) ? false : true);
		__crowdSimulationApp->setFlgShowSelfishAgent((flgShowSelfishAgent == 0) ? false : true);
		__crowdSimulationApp->setFlgShowNeutralAgent((flgShowNeutralAgent == 0) ? false : true);
		__crowdSimulationApp->setFlgShowBraveAgent((flgShowBraveAgent == 0) ? false : true);
		__crowdSimulationApp->setFlgShowOccupiedCell((flgShowOccupiedCell == 0) ? false : true);
		__crowdSimulationApp->setFlgShowVolunteerColorTable((flgShowVolunteerColorTable == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentAnxiety((flgShowAgentAnxiety == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentWillness((flgShowAgentWillness == 0) ? false : true);
		__crowdSimulationApp->setFlgShowObstacleDensity((flgShowObstacleDensity == 0) ? false : true);
		__crowdSimulationApp->setFlgShowColorTableWithoutExit((flgShowColorTableWithoutExit == 0) ? false : true);
		__crowdSimulationApp->setFlgShowSpecialColorTable((flgShowSpecialColorTable == 0) ? false : true);
		__crowdSimulationApp->setFlgShowCurrentExitBlockAgent((flgShowCurrentExitBlockAgent == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAFF((flgShowAFF == 0) ? false : true);
		__crowdSimulationApp->setFlgShowTFF((flgShowTFF == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentEvacuationTime((flgShowEvacuationTime == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentEscapeExit((flgShowEscapeExit == 0) ? false : true);
		__crowdSimulationApp->setFlgShowAgentFaceDirection((flgShowAgentFaceDirection == 0) ? false : true);
		glutPostRedisplay( );
	}

	void control_cb( int control ) 
	{
	
	}

	void control_sp( int control ) 
	{
	}

	void control_reset_celluar(int control) //2017/04/13
	{
		__crowdSimulationApp->resetCelluarAutomata();
		glutPostRedisplay();
	}

	void control_start_cellular(int control) //2017/08/06
	{
		__crowdSimulationApp->startCellularAutomata();
		glutPostRedisplay();
	}

	void control_stop_cellular(int control) //2017/08/06
	{
		__crowdSimulationApp->stopCellularAutomata();
		glutPostRedisplay();
	}

	void control_add_group(int control) //2017/08/07
	{
		__crowdSimulationApp->addNewGroup();
		glutPostRedisplay();
	}

	void control_set_group_leader(int control) //2017/08/07
	{
		__crowdSimulationApp->setGroupLeader();
		glutPostRedisplay();
	}

	void control_sleep_time(int control) //2017/04/13
	{
		
	}

	void control_cell_exits_segment_x(int control) //2017/07/07
	{
		cell_exits_segment_x = spinner_cell_exits_segment_x->get_int_val();
	}

	void control_cell_exits_segment_z(int control) //2017/07/07
	{
		cell_exits_segment_z = spinner_cell_exits_segment_z->get_int_val();
	}

	void control_group_num(int num) //2017/07/24
	{
		group_num = spinner_group_num->get_int_val();
	}

	void control_numOfgroup_size2(int num) //2017/07/26
	{
		numOfgroup_size2 = spinner_numOfgroup_size2->get_int_val();
	}

	void control_numOfgroup_size3(int num) //2017/07/26
	{
		numOfgroup_size3 = spinner_numOfgroup_size3->get_int_val();
	}

	void control_group_radius(int num) //2017/08/14
	{
		group_radius = spinner_group_radius->get_int_val();
	}

	void control_guider_height(int num) //2017/08/18
	{
		guider_height = spinner_guider_height->get_int_val();
	}

	void control_colorTable_ID(int num) //2017/09/29
	{
		colorTable_id = spinner_volunteer_colorTable->get_int_val();
	}

	void control_colorTable_withoutExit_ID(int num)
	{
		colorTable_withoutExit_id = spinner_colorTable_withoutExit->get_int_val();
	}

	void control_willing_threshold(int control) //2017/11/30
	{
		willing_threshold = spinner_willing_threshold->get_float_val();
	}

	void control_special_id(int control)
	{
		specialTableID = spinner_special_id->get_int_val();
	}

	void control_colorTableNormalizeValue(int control)
	{
		specialColorTableNormalizeValue = spinner_specialColorTableNormalizeValue->get_float_val();
	}

	void control_show_segment(int control) //2017/07/07
	{
		__crowdSimulationApp->showCellExitSegment(cell_exits_segment_x, cell_exits_segment_z);
		glutPostRedisplay();
	}

	void control_show_specific_group(int control) //2017/07/24
	{
		__crowdSimulationApp->showSpecificGroupMember(group_num);
		glutPostRedisplay();
	}
	// setting
	void control_set_numOfgroup_size(int control) //2017/07/26
	{
		__crowdSimulationApp->setNumOfGroupSize(numOfgroup_size2, numOfgroup_size3);
	}

	void control_set_group_radius(int control) //2017/08/14
	{
		__crowdSimulationApp->setGroupRadius(group_radius);
	}

	void control_set_guider(int control) //2017/08/17
	{
		__crowdSimulationApp->setGuider(guider_height);
	}

	void control_set_colorTable_ID(int control) //2017/09/29
	{
		__crowdSimulationApp->setColorTableID(colorTable_id);
	}

	void control_set_colorTable_withoutExit_ID(int control) //2017/11/13
	{
		__crowdSimulationApp->setColorTableWithoutExitID(colorTable_withoutExit_id);
	}

	void control_set_willing_threshold(int control) //2017/11/30
	{
		__crowdSimulationApp->setWillingThreshold(willing_threshold);
	}

	void control_set_special_id(int control)
	{
		__crowdSimulationApp->setSpecialID(specialTableID);
	}

	void control_set_sepcialColorTableNormalizeValie(int control)
	{
		__crowdSimulationApp->setColorTableNormalizeValue(specialColorTableNormalizeValue);
	}
	// editing
	void control_edit_obstacle(int control) //2017/09/11
	{
		__crowdSimulationApp->enabled_edit_obstacle();
	}

	void control_edit_exit(int control)
	{
		__crowdSimulationApp->enabled_edit_exit();
	}

	void control_edit_obstacles(int control)
	{
		__crowdSimulationApp->enabled_edit_obstacle();
	}

	void control_edit_agent(int control)
	{

	}

	void control_update_scene(int control)
	{
		__crowdSimulationApp->update_scene();
	}

	void control_load_simulation(int control)
	{
		__crowdSimulationApp->load_simulation();
	}
};

	
void CROWD_SIMULATION_APP::createGLUI( int mainWinID, void (global_idle_func()) )
{
	__crowdSimulationApp = this;

	GLUI *glui = glui_main = GLUI_Master.create_glui("Tools");
	//GLUI *glui = glui_main = GLUI_Master.create_glui_subwindow(mainWinID, GLUI_SUBWINDOW_RIGHT);
	GLUI *glui_threshold = GLUI_Master.create_glui("Threshold Experiment");
	//GLUI *glui_edit = GLUI_Master.create_glui("Editing Tools");

	checkbox_showItem_celluarAutomata = new GLUI_Checkbox(glui, "Pedestrian", &flgShowCelluarAutomata, 0, control_show_items); //2017/04/13
	checkbox_showItem_lattice = new GLUI_Checkbox(glui, "Lattice", &flgShowLattice, 1, control_show_items); //2017/04/13
	checkbox_showItem_colorTable = new GLUI_Checkbox(glui, "ColorTable", &flgShowColorTable, 2, control_show_items); //2017/04/14
	checkbox_showItem_dFF_colorTable = new GLUI_Checkbox(glui, "DFF_ColorTable", &flgShowDFFColorTable, 3, control_show_items); //2017/07/21
	checkbox_showItem_nodeIntersectObstacle = new GLUI_Checkbox(glui, "Node IntersectObstacle", &flgShowNodeIntersectObstacle, 4, control_show_items); //2017/07/06
	checkbox_showItem_occupied_cell = new GLUI_Checkbox(glui, "Occupied Cell", &flgShowOccupiedCell, 19, control_show_items); //2017/09/28
	checkbox_showItem_anxiety = new GLUI_Checkbox(glui, "Agent anxiey", &flgShowAgentAnxiety, 21, control_show_items); //2017/10/18
	checkbox_showItem_willness = new GLUI_Checkbox(glui, "Agent willness", &flgShowAgentWillness, 22, control_show_items); //2017/10/19
	checkbox_showItem_density = new GLUI_Checkbox(glui, "Density around obstacle", &flgShowObstacleDensity, 23, control_show_items); //2017/10/25
	checkbox_showItem_evacuationTIme = new GLUI_Checkbox(glui, "Agent Evacuation Time", &flgShowEvacuationTime, 26, control_show_items);
	checkbox_showItem_escapeExit = new GLUI_Checkbox(glui, "Agent Escape Exit", &flgShowEscapeExit, 27, control_show_items);
	checkbox_showItem_agentFaceDirection = new GLUI_Checkbox(glui, "Agent Face Direction", &flgShowAgentFaceDirection, 28, control_show_items);
	new GLUI_Spinner(glui, "slow", GLUI_SPINNER_INT, &milliseconds, 5, control_sleep_time);

	new GLUI_Separator(glui);
	new GLUI_Button(glui, "start", START_CELLULAR_AUTOMATA, control_start_cellular); //2017/08/06
	new GLUI_Button(glui, "pause", STOP_CELLULAR_AUTOMATA, control_stop_cellular); //2017/08/06
	new GLUI_Button(glui, "reset", RESET_CELLUAR_AUTOMATA, control_reset_celluar); //2017/04/13
	/*
	new GLUI_Separator(glui);
	checkbox_showItem_cellExitSegment = new GLUI_Checkbox(glui, "Segment between cell and exits", &flgShowCellExitSegment, 6, control_show_items); //2017/07/07
	spinner_cell_exits_segment_x = new GLUI_Spinner(glui, "cell_x", GLUI_SPINNER_INT, &cell_exits_segment_x, 0, control_cell_exits_segment_x);
	spinner_cell_exits_segment_z = new GLUI_Spinner(glui, "cell_z", GLUI_SPINNER_INT, &cell_exits_segment_z, 0, control_cell_exits_segment_z);
	new GLUI_Button(glui, "set cell position", SHOW_CELL_EXIT_SEGMENT, control_show_segment); //2017/07/07
	*/
	/*
	new GLUI_Separator(glui);
	checkbox_showItem_groupMember = new GLUI_Checkbox(glui, "Group member", &flgShowGroupMember, 7, control_show_items); //2017/07/24
	checkbox_showItem_specificGroupMember = new GLUI_Checkbox(glui, "Specific Group", &flgShowSpecificGroupMember, 8, control_show_items); //2017/07/24
	checkbox_showItem_groupCircle = new GLUI_Checkbox(glui, "Group area", &flgShowGroupCircle, 9, control_show_items); //2017/08/11
	checkbox_showItem_agent_path = new GLUI_Checkbox(glui, "Agent path", &flgShowAgentPath, 10, control_show_items); //2017/08/18
	spinner_group_num = new GLUI_Spinner(glui, "group ID", GLUI_SPINNER_INT, &group_num, 0, control_group_num);
	new GLUI_Button(glui, "set group ID", SHOW_GROUP_MEMBER, control_show_specific_group); //2017/07/24
	spinner_numOfgroup_size2 = new GLUI_Spinner(glui, "group size2 number", GLUI_SPINNER_INT, &numOfgroup_size2, 0, control_numOfgroup_size2); //2017/07/26
	spinner_numOfgroup_size3 = new GLUI_Spinner(glui, "group size3 number", GLUI_SPINNER_INT, &numOfgroup_size3, 0, control_numOfgroup_size3); //2017/07/26
	new GLUI_Button(glui, "set group size", SET_GROUP_SIZE, control_set_numOfgroup_size); //2017/07/26
	spinner_group_radius = new GLUI_Spinner(glui, "group radius", GLUI_SPINNER_INT, &group_radius, 0, control_group_radius); //2017/08/14
	new GLUI_Button(glui, "set group radius", SET_GROUP_RADIUS, control_set_group_radius); //2017/08/14
	*/
	/*
	new GLUI_Separator(glui);
	new GLUI_Button(glui, "set leader", SET_LEADER, control_set_group_leader); //2017/08/07
	new GLUI_Button(glui, "add group", ADD_GROUP, control_add_group); //2017/08/07
	*/
	/*
	new GLUI_Separator(glui);
	checkbox_showItem_escape_direction = new GLUI_Checkbox(glui, "Escape Direction", &flgShowEscapeDirection, 11, control_show_items); //2017/08/21
	checkbox_showItem_guider = new GLUI_Checkbox(glui, "Guider", &flgShowGuider, 12, control_show_items); //2017/08/17
	spinner_guider_height = new GLUI_Spinner(glui, "guider height", GLUI_SPINNER_INT, &guider_height, 0, control_guider_height); //2017/08/18
	new GLUI_Button(glui, "set guider", SET_GUIDER, control_set_guider); //2017/08/17
	*/
	
	new GLUI_Separator(glui);
	checkbox_showItem_stress_area = new GLUI_Checkbox(glui, "Stress Area", &flgShowStressArea, 13, control_show_items); //2017/09/04
	checkbox_showItem_compress_leader = new GLUI_Checkbox(glui, "Compressive Leader", &flgShowCompressiveLeader, 14, control_show_items); //2017/09/16
	checkbox_showItem_strength_agent = new GLUI_Checkbox(glui, "Strength Agent", &flgShowStrengthAgent, 15, control_show_items); //2017/09/18
	/*
	checkbox_showItem_selfish_agent = new GLUI_Checkbox(glui, "Selfish Agent", &flgShowSelfishAgent, 16, control_show_items); //2017/09/18
	checkbox_showItem_neutral_agent = new GLUI_Checkbox(glui, "Neutral Agent", &flgShowNeutralAgent, 17, control_show_items); //2017/09/18
	checkbox_showItem_brave_agent = new GLUI_Checkbox(glui, "Brave Agent", &flgShowBraveAgent, 18, control_show_items); //2017/09/18
	new GLUI_Button(glui, "edit obstacle", EDIT_OBSTACLE, control_edit_obstacle); //2017/09/11
	checkbox_showItem_volunteer_colorTable = new GLUI_Checkbox(glui, "volunteer colorTable", &flgShowVolunteerColorTable, 20, control_show_items); //2017/09/29
	spinner_volunteer_colorTable = new GLUI_Spinner(glui, "colorTable ID", GLUI_SPINNER_INT, &colorTable_id, 0, control_colorTable_ID); //2017/09/29
	new GLUI_Button(glui, "set table ID", SET_TABLE_ID, control_set_colorTable_ID); //2017/09/29
	*/
	new GLUI_Separator(glui);
	checkbox_showItem_colorTableWithoutExit = new GLUI_Checkbox(glui, "colorTable without exit", &flgShowColorTableWithoutExit, 21, control_show_items); //2017/11/13
	spinner_colorTable_withoutExit = new GLUI_Spinner(glui, "colorTable without exit ID", GLUI_SPINNER_INT, &colorTable_withoutExit_id, 0, control_colorTable_withoutExit_ID); //2017/11/13
	new GLUI_Button(glui, "set color table without exit ID", SET_TABLE_WITHOUT_EXIT_ID, control_set_colorTable_withoutExit_ID); //2017/11/13
	new GLUI_Button(glui, "Edit Exit", EDIT_EXIT, control_edit_exit);
	new GLUI_Button(glui, "Edit Obstacle", EDIT_OBSTACLES, control_edit_obstacles);
	new GLUI_Button(glui, "Edit Agent", EDIT_AGENT, control_edit_agent);
	new GLUI_Button(glui, "Update Scene", UPDATE_SCENE, control_update_scene);
	new GLUI_Button(glui, "Load Simulation", LOAD_SIMULATION, control_load_simulation);

	//---------------------------------------------------------------------------
	spinner_willing_threshold = new GLUI_Spinner(glui_threshold, "willing threshold", GLUI_SPINNER_FLOAT, &willing_threshold, 0.3f, control_willing_threshold);
	new GLUI_Button(glui_threshold, "Set willing", SET_WILLING_THRESHOLD, control_set_willing_threshold);
	checkbox_showItem_specialColorTable = new GLUI_Checkbox(glui_threshold, "Special Table", &flgShowSpecialColorTable, 22, control_show_items);
	spinner_special_id = new GLUI_Spinner(glui_threshold, "Special Table ID", &specialTableID, 0, control_special_id);
	new GLUI_Button(glui_threshold, "set special table ID", SET_SPECIAL_TABLE_ID, control_set_special_id);
	spinner_specialColorTableNormalizeValue = new GLUI_Spinner(glui_threshold, "Normalize value of color table", &specialColorTableNormalizeValue, 50, control_colorTableNormalizeValue);
	new GLUI_Button(glui_threshold, "set normalize value", SET_COLORTABLE_NORMALIZEVALUE, control_set_sepcialColorTableNormalizeValie);
	checkbox_showItem_currentExitBlockAgent = new GLUI_Checkbox(glui_threshold, "Exit block the agent", &flgShowCurrentExitBlockAgent, 23, control_show_items);
	checkbox_showItem_AFF = new GLUI_Checkbox(glui_threshold, "AFF", &flgShowAFF, 24, control_show_items);
	checkbox_showItem_TFF = new GLUI_Checkbox(glui_threshold, "TFF", &flgShowTFF, 25, control_show_items);

	//---------------------------------------------------------------------------
	/*new GLUI_Button(glui_edit, "Edit Exit", EDIT_EXIT, control_edit_exit);
	new GLUI_Button(glui_edit, "Edit Obstacle", EDIT_OBSTACLES, control_edit_obstacles);
	new GLUI_Button(glui_edit, "Edit Agent", EDIT_AGENT, control_edit_agent);
	new GLUI_Button(glui_edit, "Update Scene", UPDATE_SCENE, control_update_scene);*/
}