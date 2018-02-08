#include "../myCellularAutomata/cellular_automata.h"

void CELL::reset(){

	probability = 0;
	dFF = 0;
	temp_dFF = 0;
	occupied = 0;
	inobstacle = false;
	//cell_type = 0;
	intersect_obstacle = false;
	obstacle = false;
	normalize_dFF = 0;
	_sFF = 0;
	obstacle_ = 1;
	click = false;
	obstacle_id = -1;
	occupant_id = -1;
	mark = false;
	mark_ = false;
	check = false;
	max_sFF = 0.0f;
}

void FFMODEL::reset(){

	start = false;
	group_count = 0;
	pause_start_time = clock();
	pause_end_time = clock();
	total_pause_through_time = 0;
	pause_through_time = 0;
	select_agent_ID = 0;
	pre_select_agent_ID = 0;
	guider_ID.clear();
	out = false;
	out_anxiety = false;
	remain_agent = agent_number;
	//size2_num = 0;
	//size3_num = 0;
	
}