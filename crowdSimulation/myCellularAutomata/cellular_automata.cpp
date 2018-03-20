#include "cellular_automata.h"
#include <random>
#include <queue>

#define EXP_SCALER 25.0f
#define DIAGONAL_VALUE 1.5f
#define SCALER 10.0f
#define OFFSET 5.0f
#define WAITING_VALUE 3.0f
#define MAX_DFF 1.4f

void CS_CELLULAR_AUTOMATA::crowd_simulation_actionType(){

	agent_psychology.actionType = CROWD_SIMULATION_WITHOUT_AGENT_PERSONALITY;
	//agent_psychology.actionType = CROWD_SIMULATION_WITH_AGENT_PERSONALITY;
	//agent_psychology.obstacle_moveType = CS_OBSTACLE_AUTOMATIC_MOVE;
	//agent_psychology.obstacle_moveType = CS_OBSTACLE_MANUAL_MOVE;
}

CS_CELLULAR_AUTOMATA::CS_CELLULAR_AUTOMATA(){

	read_data();
	initialize_variables();
}

void CELL::create_space(int size){

	specific_sFF.clear();
	specific_normalize_sFF.clear();
	specific_color_sFF.clear();
	specific_max_sFF.clear();
	specific_sFF.resize(size);
	specific_normalize_sFF.resize(size);
	specific_color_sFF.resize(size);
	specific_max_sFF.resize(size);
	for (int i = 0; i < size; i++)
	{
		specific_sFF[i] = 0.0f;
		specific_normalize_sFF[i] = 0.0f;
		specific_color_sFF[i] = 0.0f;
		specific_max_sFF[i] = 0.0f;
	}
}

void CELL::create_spaceForSpecialMap(int size){

	special_sFF.resize(size);
	special_normalize_sFF.resize(size);
	special_color_sFF.resize(size);
	special_max_sFF.resize(size);
	for (int i = 0; i < size; i++)
	{
		special_sFF[i] = 0.0f;
		special_normalize_sFF[i] = 0.0f;
		special_color_sFF[i] = 0.0f;
		special_max_sFF[i] = 0.0f;
	}
}

void CS_CELLULAR_AUTOMATA::initialize_variables(){
	
	if (model->mNewSeed == 1)
		model->seed = (unsigned)time(NULL);
	cout << "seed: " << model->seed << endl;
	srand(model->seed);
	outputSeed();
	mTimeStep = 0.0f;
	mTimeCounter = 0;
	// decide crowd simulation type
	crowd_simulation_actionType();
	editing_mode = false;
	change = false;
	cell = new CELL*[model->size];
	//agent = new AGENT[model->agent_number];
	model->remain_agent = model->agent_number;
	gaussian_generate_personality();
	computeAgentLeadership();
	computeAgentPanic();
	cell_distance_.resize(model->size);
	for (int j = 0; j < model->size; j++)
	{
		cell[j] = new CELL[model->size];
		cell_distance_[j].resize(model->size);
	}

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].x = i;
			cell[i][j].y = j;
		}
	}
	mExperimentData.mAverageEvacuationTime.resize(mExit.size());
	mExperimentData.mAverageLastAgentEvacuationTime.resize(mExit.size());
	mExperimentData.mEscapeAgentNumber.resize(mExit.size());
	mExperimentData.mTotalAverageEvaciationTime = new float[100];
	mExperimentData.mTotalAverageLastAgentEvacuationTime = new float[100];
	mRandomAgentOrder.resize(model->agent_number);
	// collect exit cell
	/*for (int i = 0; i < exits.size(); i++)
	{
		cell[exits[i].first][exits[i].second].cell_type = 1;
	}*/
	int e_count = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		if (!mExit[i].active)
			continue;
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			e_count++;
			cell[x + dx*j][z + dz*j].cell_type = 1;
			cell[x + dx*j][z + dz*j].exit_id = i;
			cell[x + dx*j][z + dz*j].exit = true;
		}
	}
	distance_map_.resize(e_count);
	clean_distance_map.resize(e_count);
	special_distance_map.resize(e_count);
	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].blockByExit = new int[mExit.size()];
		for (int j = 0; j < (int)mExit.size(); j++)
		{
			agent[i].blockByExit[j] = 0;
		}
	}
	randomGenerateExitColor();
	set_obstacle();
	set_wall();
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].create_space(obstacles.size());
			cell[i][j].create_spaceForSpecialMap((int)pow(2, mExit.size()));
		}
	}
	update_obstalce_type();
	cell_manager.specific_max_sFF.resize(obstacles.size());
	compute_staticFF();
	compute_clean_sFF();
	assignColorProportion();
	initialize_agent_anxiety();
	int count = 0;
	for (int j = 0; j < (int)obstacles.size(); j++)
	{
		count += obstacles[j].component.size();
		obstacles[j].moveDestination.resize(obstacles[j].volunteer_id.size());
	}
	volunteer_distance_map.resize(count);
	for (int j = 0; j < (int)obstacles.size(); j++)
	{
		for (int k = 0; k < (int)obstacles[j].component.size(); k++)
		{
			compute_volunteer_staticFF(j, k);
		}
		//computeNewMapWithoutExit(i);
	}
	// generate pow(2,exit.size()) floor field maps
	//special_distance_map.resize(pow(2, mExit.size()));
	distance_buffer.resize(model->size);
	for (int j = 0; j < model->size; j++)
		distance_buffer[j].resize(model->size);
	int *exit_block = new int[mExit.size()];
	eb = new int[mExit.size()];
	//cout << pow(2, mExit.size()) << endl;
	if (obstacles.size() > 0)
	{
		for (int j = 0; j < pow(2, mExit.size()); j++)
		{
			int value = j;
			for (int k = mExit.size() - 1; k >= 0; k--)
			{
				if (value >= pow(2, k))
				{
					value -= (int)pow(2, k);
					exit_block[k] = 1;
				}
				else
				{
					exit_block[k] = 0;
				}
			}
			computeNewMapWithoutExit(exit_block);
		}
	}
	//blocked_obstacles.reserve(obstacles.size() + 1);
	//normal_obstacle.reserve(obstacles.size() + 1);
	//cout << model->mMoveDistance << endl;
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_AUTOMATIC_MOVE)
		find_obstacleMustMove();
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_MANUAL_MOVE)
	{
		for (unsigned int i = 0; i < obstacles.size(); i++)
		{
			blocked_obstacles.push_back(i);
			obstacles[i].pool_index = (int)blocked_obstacles.size() - 1;
		}
		if (obstacles.size() == 1)
			obstacles[0].volunteer_destination = PAIR_INT(obstacles[0].component[0].first, obstacles[0].component[0].second + model->mDisToExit);
		//for (int j = 0; j < (int)obstacles.size(); j++)
		//{
		//	normal_obstacle.push_back(j);
		//	obstacles[j].pool_index = normal_obstacle.size() - 1;
		//}
	}
	//initialize agents' position and psychology parameter
	for (int j = 0; j < model->agent_number; j++)
	{
		//cout << agent[i].psychology.willness << endl;
	}
	weakness_agent.reserve(model->agent_number + 1);
	for (int j = 0; j < model->agent_number; j++)
	{
		if (agent[j].psychology.willness >= guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			agent[j].strength = true;
			agent[j].pool_index = strength_agent.size();
			strength_agent.push_back(j);
			continue;
		}
		if (agent[j].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			//cout << j << endl;
			agent[j].strength = false;
			agent[j].pool_index = weakness_agent.size();
			//cout << weakness_agent.size() << endl;
			weakness_agent.push_back(j);
		}
	}
	for (int i = 0; i < model->agent_number; i++)
	{
		//
		//cout << i << endl;
		agent[i].map_size = model->size;
		agent[i].agent_id = i;
		agent[i].psychology.willness = 0.0f;
		agent[i].psychology.resistance = (float)rand() / RAND_MAX;
		agent[i].strength = false;
		agent[i].remain_anxiety.resize(10);
		agent[i].mAvoidObstacle.resize(obstacles.size(), false);
		//weakness_agent.push_back(i);
	}
	// random generate leader
	/*for (int i = 0; i < model->agent_number * LEADER_PROPORTION; i++)
	{
		int rnd = rand() % model->agent_number;
		while (agent[rnd].compressive_leader)
		{
			rnd = rand() % model->agent_number;
		}
		agent[rnd].compressive_leader = true;
		agent[rnd].psychology.leadership = (double)rand() / RAND_MAX;
		leader_pool.push_back(rnd);
	}*/

	set_group();
	initialize_agent_position();
	findNeighboringAgent();
	/*
	collect exit cell
	*/
	for (int i = 0; i < (int)exits.size(); i++)
	{
		cell[exits[i].first][exits[i].second].cell_type = 1;
	}
	if (!!model->mAnxietyExperiment || !!model->mTimeEffectExperiment || !!model->mTendencyExperiment)
		model->mSameSeed = true;
	//for (int i = 0; i < wall.size(); i++)
	//{
	//	cout << wall[i].first << " " << wall[i].second << endl;
	//}
} 

void CS_CELLULAR_AUTOMATA::findNeighboringAgent(){

	for (int i = 0; i < model->agent_number; i++)
	{
		int px = agent[i].position.first;
		int pz = agent[i].position.second;
		for (int x = -5; x <= 5; x++)
		{
			for (int z = -5; z <= 5; z++)
			{
				if (!isValid(px + x, pz + z))
					continue;
				if (cell[px + x][pz + z].obstacle)
					continue;
				if (cell[px + x][pz + z].occupied == 1)
				{
					int id = cell[px + x][pz + z].occupant_id;
					agent[i].mNeighboringAgent.push_back(id);
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::initialize_agent_position(){

	float minx = 0, maxx = model->size, minz = 0, maxz = model->size;
	for (int i = 0; i < (int)agent_group.size(); i++)
	{
		switch (model->mLeaderDistrubution)
		{
		case 0:
			minz = (float)model->size * 0.66f;
			break;
		case 1:
			minz = (float)model->size * 0.33f;
			maxz = (float)model->size * 0.66f;
			break;
		case 2:
			maxz = (float)model->size * 0.33f;
			break;
		case 3:
			maxx = (float)model->size * 0.33f;
			break;
		case 4:
			minx = (float)model->size * 0.33f;
			maxx = (float)model->size * 0.66f;
			break;
		case 5:
			minx = (float)model->size * 0.66f;
			break;
		}
		int leader_ID = agent_group[i].member[0];
		if (!agent[leader_ID].leader)
		{
			continue;
		}
		agent[leader_ID].position.first = rand() % model->size;
		agent[leader_ID].position.second = rand() % model->size;
		while (cell[agent[leader_ID].position.first][agent[leader_ID].position.second].occupied == 1 
			|| cell[agent[leader_ID].position.first][agent[leader_ID].position.second].obstacle
			/*|| (agent[leader_ID].compressive_leader && agent[leader_ID].position.second < model->size / 2)*/
			|| (agent[leader_ID].compressive_leader && agent[leader_ID].position.second < minz)
			|| (agent[leader_ID].compressive_leader && agent[leader_ID].position.second >= maxz)
			|| (agent[leader_ID].compressive_leader && agent[leader_ID].position.first < minx)
			|| (agent[leader_ID].compressive_leader && agent[leader_ID].position.first >= maxz)
			|| cell[agent[leader_ID].position.first][agent[leader_ID].position.second].cell_type == 3)
		{ // cell_type value != 0 means it's obstacle or exit
			agent[leader_ID].position.first = rand() % model->size;
			agent[leader_ID].position.second = rand() % model->size;
		}
		cell[agent[leader_ID].position.first][agent[leader_ID].position.second].occupied = 1;
		cell[agent[leader_ID].position.first][agent[leader_ID].position.second].occupant_id = leader_ID;
		agent[leader_ID].escape_position = agent[leader_ID].position;
		//cout << agent[leader_ID].position.first << " " << agent[leader_ID].position.second << endl;
		initialize_group_member_position(i);
	}
	for (int i = 0; i < model->agent_number; i++)
	{
		//cout << "agent_" << i << " " << agent[i].position.first << " " << agent[i].position.second << endl;
	}
}

void CS_CELLULAR_AUTOMATA::initialize_group_member_position(int group_ID){
	// 0 is leader, already initialize the position
	for (int i = 1; i < (int)agent_group[group_ID].member.size(); i++)
	{
		int range = model->group_radius;
		int negative;
		int x = rand() % range;
		int y = rand() % range;
		negative = rand() % 2;
		if (negative == 0)
			x = -x;
		negative = rand() % 2;
		if (negative == 0)
			y = -y;

		int member_ID = agent_group[group_ID].member[i];
		agent[member_ID].position.first = agent[agent_group[group_ID].member[0]].position.first + x;
		agent[member_ID].position.second = agent[agent_group[group_ID].member[0]].position.second + y;
		while (!isValid(agent[member_ID].position.first, agent[member_ID].position.second) || cell[agent[member_ID].position.first][agent[member_ID].position.second].obstacle || cell[agent[member_ID].position.first][agent[member_ID].position.second].occupied == 1)
		{ //
			x = rand() % range;
			y = rand() % range;
			negative = rand() % 2;
			if (negative == 0)
				x = -x;
			negative = rand() % 2;
			if (negative == 0)
				y = -y;
			agent[member_ID].position.first = agent[agent_group[group_ID].member[0]].position.first + x;
			agent[member_ID].position.second = agent[agent_group[group_ID].member[0]].position.second + y;
		}
		cell[agent[member_ID].position.first][agent[member_ID].position.second].occupied = 1;
		cell[agent[member_ID].position.first][agent[member_ID].position.second].occupant_id = member_ID;
		agent[member_ID].escape_position = agent[member_ID].position;
		//cout << agent[member_ID].position.first << " " << agent[member_ID].position.second << endl;
	}
}

void CS_CELLULAR_AUTOMATA::initialize_agent_anxiety(){

	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].anxiety = agent[i].mPersonality.neuroticism * (1-cell[agent[i].position.first][agent[i].position.second]._sFF);
		//cout << cell[agent[i].position.first][agent[i].position.second]._sFF << endl;
		//agent[i].anxiety = 0.5f;
	}
}

void CS_CELLULAR_AUTOMATA::set_group(){

	/*agent_group.clear();

	vector<AGENT> agent_buffer;
	for (int i = 0; i < model->agent_number; i++){
		agent_buffer.push_back(agent[i]);
	}

	int max_group_size = 3;
	enum group_type{
		single, _double, triple
	};
	//plus one to fix the bug, but still don't know the reason!
	int size2_remain = model->size2_num;
	int size3_remain = model->size3_num;

	while (agent_buffer.size()){
		int agent_id = rand() % agent_buffer.size();
		AGENT_GROUP group;
		agent[agent_buffer[agent_id].agent_id].leader = true;
		agent[agent_buffer[agent_id].agent_id].assigned = true;
		agent[agent_buffer[agent_id].agent_id].leader_id = agent_buffer[agent_id].agent_id; //set lead id to its self
		agent[agent_buffer[agent_id].agent_id].group_size = 1;
		group.member.push_back(agent_buffer[agent_id].agent_id);
		agent_buffer.erase(agent_buffer.begin() + agent_id); //pop out the agent already be assign a group
		if (size2_remain){
			size2_remain--;
			if (agent_buffer.size()){
				int rand_num = rand() % agent_buffer.size();
				agent[agent_buffer[rand_num].agent_id].leader = false;
				agent[agent_buffer[rand_num].agent_id].assigned = true;
				agent[agent_buffer[rand_num].agent_id].leader_id = agent_buffer[agent_id].agent_id;
				agent[agent_buffer[rand_num].agent_id].group_size = 2;
				group.member.push_back(agent_buffer[rand_num].agent_id);
				agent_buffer.erase(agent_buffer.begin() + rand_num);
			}
		}
		if (size3_remain && !size2_remain){
			size3_remain--;
			for (int i = 0; i < 2; i++){
				if (agent_buffer.size()){
					int rand_num = rand() % agent_buffer.size();
					agent[agent_buffer[rand_num].agent_id].leader = false;
					agent[agent_buffer[rand_num].agent_id].assigned = true;
					agent[agent_buffer[rand_num].agent_id].leader_id = agent_buffer[agent_id].agent_id;
					agent[agent_buffer[rand_num].agent_id].group_size = 3;
					group.member.push_back(agent_buffer[rand_num].agent_id);
					agent_buffer.erase(agent_buffer.begin() + rand_num);
				}
			}
		}
		//cout << group.member.size() << endl;
		agent_group.push_back(group);

		if (!agent_buffer.size())
			break;
	}

	for (int i = 0; i < agent_group.size(); i++)
	{
		//cout << agent_group[i].member.size() << endl;
		for (int j = 0; j < agent_group[i].member.size(); j++)
		{
			int agent_id = agent_group[i].member[j];
			agent[agent_id].group_id = i;
			cout << agent_id << " ";
		}
		cout << endl;
	}*/
	agent_group.clear();
	vector<AGENT> agent_buffer(model->agent_number);
	for (int i = 0; i < model->agent_number; i++)
	{
		//cout << i << endl;
		agent_buffer[i]=agent[i];
	}
	int amount_TwoMemberGroup = model->size2_num;
	int amount_ThreeMemberGroup = model->size3_num;
	cout << amount_ThreeMemberGroup << " " << amount_TwoMemberGroup << endl;
	//seperate agent to the group
	for (int i = 0; i < amount_TwoMemberGroup; i++)
	{
		AGENT_GROUP group;
		int leader_id;
		for (int j = 0; j < 2; j++)
		{
			int buffer = rand() % agent_buffer.size();
			if (j == 0)
			{
				agent[agent_buffer[buffer].agent_id].leader = true;
				leader_id = agent_buffer[buffer].agent_id;
			}
			else
				agent[agent_buffer[buffer].agent_id].leader = false;
			agent[agent_buffer[buffer].agent_id].assigned = true;
			agent[agent_buffer[buffer].agent_id].leader_id = leader_id; //set lead id to its self
			agent[agent_buffer[buffer].agent_id].group_size = 2;
			group.member.push_back(agent_buffer[buffer].agent_id);
			agent_buffer.erase(agent_buffer.begin() + buffer);
		}
		agent_group.push_back(group);
	}
	for (int i = 0; i < amount_ThreeMemberGroup; i++)
	{
		AGENT_GROUP group;
		int leader_id;
		for (int j = 0; j < 3; j++)
		{
			int buffer = rand() % agent_buffer.size();
			if (j == 0)
			{
				agent[agent_buffer[buffer].agent_id].leader = true;
				leader_id = agent_buffer[buffer].agent_id;
			}
			else
				agent[agent_buffer[buffer].agent_id].leader = false;
			agent[agent_buffer[buffer].agent_id].assigned = true;
			agent[agent_buffer[buffer].agent_id].leader_id = leader_id; //set lead id to its self
			agent[agent_buffer[buffer].agent_id].group_size = 3;
			group.member.push_back(agent_buffer[buffer].agent_id);
			agent_buffer.erase(agent_buffer.begin() + buffer);
		}
		agent_group.push_back(group);
	}
	for (int i = 0; i < model->agent_number - amount_ThreeMemberGroup*3 - amount_TwoMemberGroup*2; i++)
	{
		AGENT_GROUP group;
		//cout << group.member.size() << endl;
		int buffer = rand() % agent_buffer.size();
		agent[agent_buffer[i].agent_id].assigned = true;
		agent[agent_buffer[buffer].agent_id].leader_id = agent_buffer[buffer].agent_id; //set lead id to its self
		agent[agent_buffer[buffer].agent_id].group_size = 1;
		//cout << "BU " << buffer << endl;
		//cout << "AI " << agent_buffer[buffer].agent_id << endl;
		//cout << "AB " << agent_buffer.size() << endl;
		//cout << "AG " << agent_group.size() << endl;
		group.member.push_back(agent_buffer[buffer].agent_id);
		agent_buffer.erase(agent_buffer.begin() + buffer);
		agent_group.push_back(group);
	}
	for (int i = 0; i < (int)agent_group.size(); i++)
	{
		//cout << agent_group[i].member.size() << endl;
		for (int j = 0; j < (int)agent_group[i].member.size(); j++)
		{
			int agent_id = agent_group[i].member[j];
			agent[agent_id].group_id = i;
			//cout << agent_id << " ";
		}
		//cout << endl;
	}
}

void CS_CELLULAR_AUTOMATA::compute_staticFF(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].pre_sFF = cell[i][j].sFF;
		}
	}
	int e_id = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		int xx = mExit[i].position.first;
		int zz = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{	
			for (int x = 0; x < model->size; x++)
			{
				for (int z = 0; z < model->size; z++)
				{
					cell_distance_[x][z].visisted = false;
					cell_distance_[x][z].distance = FLT_MAX;
					cell_distance_[x][z].x = x;
					cell_distance_[x][z].y = z;
				}
			}
			int e_x = xx + dx*j;
			int e_z = zz + dz*j;
			if (cell[e_x][e_z].obstacle && !obstacles[cell[e_x][e_z].obstacle_id].block_exit)
				continue;
			cell_distance_[e_x][e_z].distance = 0;
			distance_queue.push(cell_distance_[e_x][e_z]);
			float verticalValue;
			float diagonalValue;
			float total_width = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				total_width += mExit[k].width;
			}
			switch (model->mType)
			{
			case 0:
				verticalValue = exp(0.5f * (mExit[i].width / total_width));
				diagonalValue = exp(0.5f * (mExit[i].width / total_width));
				break;
			case 1:
				verticalValue = exp(0.5f * (mExit[i].width / total_width));
				diagonalValue = exp(0.5f * DIAGONAL_VALUE * (mExit[i].width / total_width));
				break;
			}
			while (!distance_queue.empty())
			{
				CELL_BFS temp = distance_queue.front();
				distance_queue.pop();
				int x = temp.x;
				int y = temp.y;
				cell_distance_[x][y].visisted = true;
				//Von Neumann
				if (isValid(x - 1, y) && !isBlock(x - 1, y) && cell_distance_[x - 1][y].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x - 1][y].distance)
						cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x - 1][y].visisted = true;
					distance_queue.push(cell_distance_[x - 1][y]);
				}

				if (isValid(x + 1, y) && !isBlock(x + 1, y) && cell_distance_[x + 1][y].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x + 1][y].distance)
						cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x + 1][y].visisted = true;
					distance_queue.push(cell_distance_[x + 1][y]);
				}

				if (isValid(x, y - 1) && !isBlock(x, y - 1) && cell_distance_[x][y - 1].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y - 1].distance)
						cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x][y - 1].visisted = true;
					distance_queue.push(cell_distance_[x][y - 1]);
				}

				if (isValid(x, y + 1) && !isBlock(x, y + 1) && cell_distance_[x][y + 1].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y + 1].distance)
						cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x][y + 1].visisted = true;
					distance_queue.push(cell_distance_[x][y + 1]);
				}
				//Moore
				if (model->mNeighborType != 0)
				{
					if (isValid(x + 1, y + 1) && !isBlock(x + 1, y + 1) && cell_distance_[x + 1][y + 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x + 1][y + 1].distance)
							cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x + 1][y + 1].visisted = true;
						distance_queue.push(cell_distance_[x + 1][y + 1]);
					}

					if (isValid(x + 1, y - 1) && !isBlock(x + 1, y - 1) && cell_distance_[x + 1][y - 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x + 1][y - 1].distance)
							cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x + 1][y - 1].visisted = true;
						distance_queue.push(cell_distance_[x + 1][y - 1]);
					}

					if (isValid(x - 1, y + 1) && !isBlock(x - 1, y + 1) && cell_distance_[x - 1][y + 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x - 1][y + 1].distance)
							cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x - 1][y + 1].visisted = true;
						distance_queue.push(cell_distance_[x - 1][y + 1]);
					}

					if (isValid(x - 1, y - 1) && !isBlock(x - 1, y - 1) && cell_distance_[x - 1][y - 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x - 1][y - 1].distance)
							cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x - 1][y - 1].visisted = true;
						distance_queue.push(cell_distance_[x - 1][y - 1]);
					}
				}
			}
			distance_map_[e_id] = cell_distance_;
			e_id++;
		}
	}

	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{

			if (distance_map_[0][i][j].distance == FLT_MAX)
				continue;

			float min_exit = FLT_MAX;
			int id = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				int x = mExit[k].position.first;
				int z = mExit[k].position.second;
				int dx = mExit[k].direction.first;
				int dz = mExit[k].direction.second; 
				for (int l = 0; l < mExit[k].width; l++)
				{
					int ex = x + dx*l;
					int ez = z + dz*l;
					if (cell[ex][ez].obstacle && !obstacles[cell[ex][ez].obstacle_id].block_exit)
						continue;
					float distance = abs(distance_map_[id][ex][ez].distance - distance_map_[id][i][j].distance);
					id++;
					if (distance < min_exit)
						min_exit = distance;
				}
			}

			cell[i][j].sFF = min_exit;

			if (cell[i][j].sFF > max)
				max = cell[i][j].sFF;
		}
	}
	// scale the value in 0~exp(13)
	cell_manager.max_sFF = max;
	for (int i = 0; i < model->size; i++){
		for (int j = 0; j < model->size; j++){
			if (distance_map_[0][i][j].distance == FLT_MAX)
				continue;
			cell[i][j].sFF_color = (max - cell[i][j].sFF) / max;
			cell[i][j]._sFF = (max - cell[i][j].sFF) / max;
			//cout << cell[i][j]._sFF << endl;
		}
	}
}

void CS_CELLULAR_AUTOMATA::compute_clean_sFF(){

	int e_id = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		for (int j = 0; j < (int)mExit[i].width; j++)
		{
			int x = mExit[i].position.first;
			int z = mExit[i].position.second;
			int dx = mExit[i].direction.first;
			int dz = mExit[i].direction.second;
			for (int x = 0; x < model->size; x++)
			{
				for (int z = 0; z < model->size; z++)
				{
					cell_distance_[x][z].visisted = false;
					cell_distance_[x][z].distance = FLT_MAX;
					cell_distance_[x][z].x = x;
					cell_distance_[x][z].y = z;
				}
			}
			int e_x = x + dx*j;
			int e_z = z + dz*j;
			cell_distance_[e_x][e_z].distance = 0;
			distance_queue.push(cell_distance_[e_x][e_z]);
			float verticalValue;
			float diagonalValue;
			float total_width = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				total_width += mExit[k].width;
			}
			switch (model->mType)
			{
			case 0:
				verticalValue = exp(0.5f * (mExit[i].width / total_width));
				diagonalValue = exp(0.5f * (mExit[i].width / total_width));
				break;
			case 1:
				verticalValue = exp(0.5f * (mExit[i].width / total_width));
				diagonalValue = exp(0.5f * DIAGONAL_VALUE * (mExit[i].width / total_width));
				break;
			}
			while (!distance_queue.empty())
			{
				CELL_BFS temp = distance_queue.front();
				distance_queue.pop();
				int x = temp.x;
				int y = temp.y;
				cell_distance_[x][y].visisted = true;
				if (isValid(x - 1, y) && cell_distance_[x - 1][y].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x - 1][y].distance)
						cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x - 1][y].visisted = true;
					distance_queue.push(cell_distance_[x - 1][y]);
				}

				if (isValid(x + 1, y) && cell_distance_[x + 1][y].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x + 1][y].distance)
						cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x + 1][y].visisted = true;
					distance_queue.push(cell_distance_[x + 1][y]);
				}

				if (isValid(x, y - 1) && cell_distance_[x][y - 1].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y - 1].distance)
						cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x][y - 1].visisted = true;
					distance_queue.push(cell_distance_[x][y - 1]);
				}

				if (isValid(x, y + 1) && cell_distance_[x][y + 1].visisted == false)
				{
					if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y + 1].distance)
						cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + verticalValue;
					cell_distance_[x][y + 1].visisted = true;
					distance_queue.push(cell_distance_[x][y + 1]);
				}
				// Moore
				if (model->mNeighborType != 0)
				{
					if (isValid(x + 1, y + 1) && cell_distance_[x + 1][y + 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x + 1][y + 1].distance)
							cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x + 1][y + 1].visisted = true;
						distance_queue.push(cell_distance_[x + 1][y + 1]);
					}

					if (isValid(x + 1, y - 1) && cell_distance_[x + 1][y - 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x + 1][y - 1].distance)
							cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x + 1][y - 1].visisted = true;
						distance_queue.push(cell_distance_[x + 1][y - 1]);
					}

					if (isValid(x - 1, y + 1) && cell_distance_[x - 1][y + 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x - 1][y + 1].distance)
							cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x - 1][y + 1].visisted = true;
						distance_queue.push(cell_distance_[x - 1][y + 1]);
					}

					if (isValid(x - 1, y - 1) && cell_distance_[x - 1][y - 1].visisted == false)
					{
						if (cell_distance_[x][y].distance + diagonalValue < cell_distance_[x - 1][y - 1].distance)
							cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + diagonalValue;
						cell_distance_[x - 1][y - 1].visisted = true;
						distance_queue.push(cell_distance_[x - 1][y - 1]);
					}
				}
			}
			clean_distance_map[e_id] = cell_distance_;
			e_id++;
		}
	}

	for (int i = 0; i < model->size; i++){
		for (int j = 0; j < model->size; j++){

			if (clean_distance_map[0][i][j].distance == FLT_MAX)
				continue;

			float min_exit = FLT_MAX;
			int id = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				int x = mExit[k].position.first;
				int z = mExit[k].position.second;
				int dx = mExit[k].direction.first;
				int dz = mExit[k].direction.second;
				for (int l = 0; l < mExit[k].width; l++)
				{
					int ex = x + dx*l;
					int ez = z + dz*l;
					float distance = abs(clean_distance_map[id][ex][ez].distance - clean_distance_map[id][i][j].distance);
					id++;
					if (distance < min_exit)
						min_exit = distance;
				}
			}
			cell[i][j].clean_sFF = min_exit;
		}
	}
}
// Volunteers take the blocked obstacle as the exit
void CS_CELLULAR_AUTOMATA::compute_volunteer_staticFF(int o_id, int com_id){

	int id = 0;
	for (int i = 0; i < o_id; i++)
	{
		id += obstacles[i].component.size();
	}
	id += com_id;
	for (int x = 0; x < model->size; x++)
	{
		for (int y = 0; y < model->size; y++)
		{
			cell_distance_[x][y].visisted = false;
			cell_distance_[x][y].distance = FLT_MAX;
			cell_distance_[x][y].x = x;
			cell_distance_[x][y].y = y;
		}
	}
	int exit_x = obstacles[o_id].component[com_id].first;
	int exit_y = obstacles[o_id].component[com_id].second;
	cell_distance_[exit_x][exit_y].distance = 0;
	distance_queue.push(cell_distance_[exit_x][exit_y]);
	float verticalValue = 1;
	while (!distance_queue.empty()){
		CELL_BFS temp = distance_queue.front();
		distance_queue.pop();
		int x = temp.x;
		int y = temp.y;
		cell_distance_[x][y].visisted = true;

		if (isValid(x - 1, y) && !cell[x - 1][y].obstacle && cell_distance_[x - 1][y].visisted == false){
			if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x - 1][y].distance)
				cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + verticalValue;
			cell_distance_[x - 1][y].visisted = true;
			distance_queue.push(cell_distance_[x - 1][y]);
		}

		if (isValid(x + 1, y) && !cell[x + 1][y].obstacle && cell_distance_[x + 1][y].visisted == false){
			if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x + 1][y].distance)
				cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + verticalValue;
			cell_distance_[x + 1][y].visisted = true;
			distance_queue.push(cell_distance_[x + 1][y]);
		}

		if (isValid(x, y - 1) && !cell[x][y - 1].obstacle&& cell_distance_[x][y - 1].visisted == false){
			if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y - 1].distance)
				cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + verticalValue;
			cell_distance_[x][y - 1].visisted = true;
			distance_queue.push(cell_distance_[x][y - 1]);
		}

		if (isValid(x, y + 1) && !cell[x][y + 1].obstacle && cell_distance_[x][y + 1].visisted == false){
			if (cell_distance_[x][y].distance + verticalValue < cell_distance_[x][y + 1].distance)
				cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + verticalValue;
			cell_distance_[x][y + 1].visisted = true;
			distance_queue.push(cell_distance_[x][y + 1]);
		}
		// moore
		if (model->mNeighborType != 0)
		{
			if (isValid(x + 1, y + 1) && !cell[x + 1][y + 1].obstacle && cell_distance_[x + 1][y + 1].visisted == false){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y + 1].distance)
					cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y + 1]);
			}

			if (isValid(x + 1, y - 1) && !cell[x + 1][y - 1].obstacle && cell_distance_[x + 1][y - 1].visisted == false){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y - 1].distance)
					cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y - 1]);
			}

			if (isValid(x - 1, y + 1) && !cell[x - 1][y + 1].obstacle && cell_distance_[x - 1][y + 1].visisted == false){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y + 1].distance)
					cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y + 1]);
			}

			if (isValid(x - 1, y - 1) && !cell[x - 1][y - 1].obstacle && cell_distance_[x - 1][y - 1].visisted == false){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y - 1].distance)
					cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y - 1]);
			}
		}
	}
	volunteer_distance_map[id] = cell_distance_;
	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (volunteer_distance_map[id][i][j].distance == FLT_MAX)
				continue;
			volunteer_distance_map[id][i][j].sFF = volunteer_distance_map[id][i][j].distance;
			if (volunteer_distance_map[id][i][j].distance > max)
				max = volunteer_distance_map[id][i][j].distance;
		}
	}
	// scale the value in 0~exp(13)
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (volunteer_distance_map[id][i][j].distance == FLT_MAX)
				continue;
			volunteer_distance_map[id][i][j].sFF_color = (max - volunteer_distance_map[id][i][j].distance) / max;
			volunteer_distance_map[id][i][j]._sFF = (max - volunteer_distance_map[id][i][j].distance) / max;
		}
	}
}

void CS_CELLULAR_AUTOMATA::computeVolunteerSFFNotConsiderBlockPosition(int o_id, int com_id){

	int mapID = getVolunteerMapID(o_id, com_id);
	for (int x = 0; x < model->size; x++)
	{
		for (int y = 0; y < model->size; y++)
		{
			cell_distance_[x][y].visisted = false;
			cell_distance_[x][y].distance = FLT_MAX;
			cell_distance_[x][y].x = x;
			cell_distance_[x][y].y = y;
		}
	}

	bool firstRound = true;
	int volunteerID = obstacles[o_id].volunteer_id[com_id];
	int exit_x = obstacles[o_id].component[com_id].first;
	int exit_y = obstacles[o_id].component[com_id].second;
	cell_distance_[exit_x][exit_y].distance = 0;
	distance_queue.push(cell_distance_[exit_x][exit_y]);

	while (!distance_queue.empty()){
		CELL_BFS temp = distance_queue.front();
		distance_queue.pop();
		int x = temp.x;
		int y = temp.y;
		cell_distance_[x][y].visisted = true;

		if (isValid(x - 1, y) && !cell[x - 1][y].obstacle && cell_distance_[x - 1][y].visisted == false
			&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x - 1, y)) ){
			if (cell_distance_[x][y].distance + 1 < cell_distance_[x - 1][y].distance)
				cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + 1;
			cell_distance_[x - 1][y].visisted = true;
			distance_queue.push(cell_distance_[x - 1][y]);
			firstRound = false;
		}

		if (isValid(x + 1, y) && !cell[x + 1][y].obstacle && cell_distance_[x + 1][y].visisted == false
			&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x + 1, y)) ){
			if (cell_distance_[x][y].distance + 1 < cell_distance_[x + 1][y].distance)
				cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + 1;
			cell_distance_[x + 1][y].visisted = true;
			distance_queue.push(cell_distance_[x + 1][y]);
			firstRound = false;
		}

		if (isValid(x, y - 1) && !cell[x][y - 1].obstacle&& cell_distance_[x][y - 1].visisted == false
			&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x, y - 1)) ){
			if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y - 1].distance)
				cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + 1;
			cell_distance_[x][y - 1].visisted = true;
			distance_queue.push(cell_distance_[x][y - 1]);
			firstRound = false;
		}

		if (isValid(x, y + 1) && !cell[x][y + 1].obstacle && cell_distance_[x][y + 1].visisted == false
			&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x, y + 1)) ){
			if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y + 1].distance)
				cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + 1;
			cell_distance_[x][y + 1].visisted = true;
			distance_queue.push(cell_distance_[x][y + 1]);
			firstRound = false;
		}
		// moore
		if (model->mNeighborType != 0)
		{
			if (isValid(x + 1, y + 1) && !cell[x + 1][y + 1].obstacle && cell_distance_[x + 1][y + 1].visisted == false
				&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x + 1, y + 1)) ){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y + 1].distance)
					cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y + 1]);
			}

			if (isValid(x + 1, y - 1) && !cell[x + 1][y - 1].obstacle && cell_distance_[x + 1][y - 1].visisted == false
				&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x + 1, y - 1)) ){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y - 1].distance)
					cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y - 1]);
			}

			if (isValid(x - 1, y + 1) && !cell[x - 1][y + 1].obstacle && cell_distance_[x - 1][y + 1].visisted == false
				&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x - 1, y + 1)) ){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y + 1].distance)
					cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y + 1]);
			}

			if (isValid(x - 1, y - 1) && !cell[x - 1][y - 1].obstacle && cell_distance_[x - 1][y - 1].visisted == false
				&& !(firstRound && agent[volunteerID].block_pos == PAIR_INT(x - 1, y - 1)) ){
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y - 1].distance)
					cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y - 1]);
			}
		}
	}
	volunteer_distance_map[mapID] = cell_distance_;
	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (volunteer_distance_map[mapID][i][j].distance == FLT_MAX)
				continue;
			volunteer_distance_map[mapID][i][j].sFF = volunteer_distance_map[mapID][i][j].distance;
			if (volunteer_distance_map[mapID][i][j].distance > max)
				max = volunteer_distance_map[mapID][i][j].distance;
		}
	}
	// scale the value in 0~exp(13)
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (volunteer_distance_map[mapID][i][j].distance == FLT_MAX)
				continue;
			volunteer_distance_map[mapID][i][j].sFF_color = (max - volunteer_distance_map[mapID][i][j].distance) / max;
			volunteer_distance_map[mapID][i][j]._sFF = (max - volunteer_distance_map[mapID][i][j].distance) / max;
		}
	}
}

void CS_CELLULAR_AUTOMATA::computeNewMapWithoutExit(int exit_block[]){

	int FloorFieldMapID = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		FloorFieldMapID += (int)(pow(2, i) * exit_block[i]);
	}
	if (FloorFieldMapID == pow(2, mExit.size()) - 1)
		return;
	int e_id = 0;
	int count = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		if (exit_block[i] == 1)
			continue;
		int ex = mExit[i].position.first;
		int ez = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			for (int x = 0; x < model->size; x++)
			{
				for (int z = 0; z < model->size; z++)
				{
					distance_buffer[x][z].visisted = false;
					distance_buffer[x][z].distance = FLT_MAX;
					distance_buffer[x][z].x = x;
					distance_buffer[x][z].y = z;
				}
			}
			int e_x = ex + dx*j;
			int e_z = ez + dz*j;
			distance_buffer[e_x][e_z].distance = 0;
			distance_queue.push(distance_buffer[e_x][e_z]);
			float verticalValue;
			float diagonalValue;
			float total_width = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				total_width += mExit[k].width;
			}
			switch (model->mType)
			{
			case 0:
				verticalValue = 1;
				diagonalValue = 1.5f;
				break;
			case 1:
				verticalValue = exp((mExit[i].width / total_width));
				diagonalValue = exp(DIAGONAL_VALUE * (mExit[i].width / total_width));
				break;
			}
			while (!distance_queue.empty())
			{
				CELL_BFS temp = distance_queue.front();
				distance_queue.pop();
				int x = temp.x;
				int y = temp.y;
				distance_buffer[x][y].visisted = true;
				if (isValid(x - 1, y) && distance_buffer[x - 1][y].visisted == false && !cell[x - 1][y].obstacle)
				{
					if (distance_buffer[x][y].distance + verticalValue < distance_buffer[x - 1][y].distance)
						distance_buffer[x - 1][y].distance = distance_buffer[x][y].distance + verticalValue;
					distance_buffer[x - 1][y].visisted = true;
					distance_queue.push(distance_buffer[x - 1][y]);
				}

				if (isValid(x + 1, y) && distance_buffer[x + 1][y].visisted == false && !cell[x + 1][y].obstacle)
				{
					if (distance_buffer[x][y].distance + verticalValue < distance_buffer[x + 1][y].distance)
						distance_buffer[x + 1][y].distance = distance_buffer[x][y].distance + verticalValue;
					distance_buffer[x + 1][y].visisted = true;
					distance_queue.push(distance_buffer[x + 1][y]);
				}

				if (isValid(x, y - 1) && distance_buffer[x][y - 1].visisted == false && !cell[x][y - 1].obstacle)
				{
					if (distance_buffer[x][y].distance + verticalValue < distance_buffer[x][y - 1].distance)
						distance_buffer[x][y - 1].distance = distance_buffer[x][y].distance + verticalValue;
					distance_buffer[x][y - 1].visisted = true;
					distance_queue.push(distance_buffer[x][y - 1]);
				}

				if (isValid(x, y + 1) && distance_buffer[x][y + 1].visisted == false && !cell[x][y + 1].obstacle)
				{
					if (distance_buffer[x][y].distance + verticalValue < distance_buffer[x][y + 1].distance)
						distance_buffer[x][y + 1].distance = distance_buffer[x][y].distance + verticalValue;
					distance_buffer[x][y + 1].visisted = true;
					distance_queue.push(distance_buffer[x][y + 1]);
				}
				// Moore
				if (isValid(x + 1, y + 1) && distance_buffer[x + 1][y + 1].visisted == false && !cell[x + 1][y + 1].obstacle)
				{
					if (distance_buffer[x][y].distance + diagonalValue < distance_buffer[x + 1][y + 1].distance)
						distance_buffer[x + 1][y + 1].distance = distance_buffer[x][y].distance + diagonalValue;
					distance_buffer[x + 1][y + 1].visisted = true;
					distance_queue.push(distance_buffer[x + 1][y + 1]);
				}

				if (isValid(x + 1, y - 1) && distance_buffer[x + 1][y - 1].visisted == false && !cell[x + 1][y - 1].obstacle)
				{
					if (distance_buffer[x][y].distance + diagonalValue < distance_buffer[x + 1][y - 1].distance)
						distance_buffer[x + 1][y - 1].distance = distance_buffer[x][y].distance + diagonalValue;
					distance_buffer[x + 1][y - 1].visisted = true;
					distance_queue.push(distance_buffer[x + 1][y - 1]);
				}

				if (isValid(x - 1, y + 1) && distance_buffer[x - 1][y + 1].visisted == false && !cell[x - 1][y + 1].obstacle)
				{
					if (distance_buffer[x][y].distance + diagonalValue < distance_buffer[x - 1][y + 1].distance)
						distance_buffer[x - 1][y + 1].distance = distance_buffer[x][y].distance + diagonalValue;
					distance_buffer[x - 1][y + 1].visisted = true;
					distance_queue.push(distance_buffer[x - 1][y + 1]);
				}

				if (isValid(x - 1, y - 1) && distance_buffer[x - 1][y - 1].visisted == false && !cell[x - 1][y - 1].obstacle)
				{
					if (distance_buffer[x][y].distance + diagonalValue < distance_buffer[x - 1][y - 1].distance)
						distance_buffer[x - 1][y - 1].distance = distance_buffer[x][y].distance + diagonalValue;
					distance_buffer[x - 1][y - 1].visisted = true;
					distance_queue.push(distance_buffer[x - 1][y - 1]);
				}
				
			}
			special_distance_map[e_id] = distance_buffer;
			e_id++;
		}
	}

	float max = -FLT_MAX;
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			// obstacle
			if (special_distance_map[0][i][j].distance == FLT_MAX)
				continue;

			float min_exit = FLT_MAX;
			int id = 0;
			for (int k = 0; k < (int)mExit.size(); k++)
			{
				if (exit_block[k] == 1)
					continue;
				int x = mExit[k].position.first;
				int z = mExit[k].position.second;
				int dx = mExit[k].direction.first;
				int dz = mExit[k].direction.second;
				for (int l = 0; l < mExit[k].width; l++)
				{
					int e_x = x + dx*l;
					int e_z = z + dz*l;
					float distance = abs(special_distance_map[id][e_x][e_z].distance - special_distance_map[id][i][j].distance);
					id++;
					if (distance < min_exit)
						min_exit = distance;
				}
			}
			cell[i][j].special_sFF[FloorFieldMapID] = min_exit;

			if (cell[i][j].special_sFF[FloorFieldMapID] > max)
				max = cell[i][j].special_sFF[FloorFieldMapID];
		}
	}

	cell_manager.specific_max_sFF[FloorFieldMapID] = max;
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (special_distance_map[0][i][j].distance == FLT_MAX)
				continue;
			cell[i][j].special_color_sFF[FloorFieldMapID] = (max - cell[i][j].special_sFF[FloorFieldMapID]) / max;
			cell[i][j].special_normalize_sFF[FloorFieldMapID] = (max - cell[i][j].special_sFF[FloorFieldMapID]) / max;
		}
	}
	if (FloorFieldMapID == 1)
	{
		//output_specialColorTable(exit_block);
	}
}
// compute sFF without exit_o_id
void CS_CELLULAR_AUTOMATA::computeNewMapWithoutExit(int o_id){

	vector<vector<vector<CELL_BFS>>> m;
	obstacles[o_id].distance_map.swap(m);
	obstacles[o_id].distance_map.clear();
	vector<PAIR_INT> n_exits;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < (int)mExit[i].width; j++)
		{
			int ex = x + dx*j;
			int ez = z + dz*j;
			n_exits.push_back(PAIR_INT(ex, ez));
		}
	}
	bool flg = true;
	while (flg)
	{
		flg = false;
		for (int i = 0; i < (int)n_exits.size(); i++)
		{
			int e_x = n_exits[i].first;
			int e_z = n_exits[i].second;
			for (int j = 0; j < (int)obstacles[o_id].component.size(); j++)
			{
				int o_x = obstacles[o_id].component[j].first;
				int o_z = obstacles[o_id].component[j].second;
				if (o_x == e_x && o_z == e_z)
				{
					//cout << o_x << " " << o_z << endl;
					n_exits.erase(n_exits.begin() + i);
				}
			}
		}
		for (int i = 0; i < (int)n_exits.size(); i++)
		{
			int e_x = n_exits[i].first;
			int e_z = n_exits[i].second;
			for (int j = 0; j < (int)obstacles[o_id].component.size(); j++)
			{
				int o_x = obstacles[o_id].component[j].first;
				int o_z = obstacles[o_id].component[j].second;
				if (o_x == e_x && o_z == e_z)
				{
					flg = true;
				}
			}
		}
	}

	for (int i = 0; i < (int)n_exits.size(); i++)
	{
		//vector<vector<CELL_BFS>> cell_distance;
		//cell_distance.resize(model->size);
		//for (int j = 0; j < model->size; j++)
		//{
		//	cell_distance[j].resize(model->size);
		//}
		for (int x = 0; x < model->size; x++)
		{
			for (int z = 0; z < model->size; z++)
			{
				cell_distance_[x][z].visisted = false;
				cell_distance_[x][z].distance = FLT_MAX;
				cell_distance_[x][z].x = x;
				cell_distance_[x][z].y = z;
			}
		}

		queue<CELL_BFS> distance_queue;
		int exit_x = n_exits[i].first;
		int exit_y = n_exits[i].second;
		cell_distance_[exit_x][exit_y].distance = 0;
		distance_queue.push(cell_distance_[exit_x][exit_y]);

		while (!distance_queue.empty())
		{
			CELL_BFS temp = distance_queue.front();
			distance_queue.pop();
			int x = temp.x;
			int y = temp.y;
			cell_distance_[x][y].visisted = true;

			if (isValid(x - 1, y) && !isBlock(x - 1, y, o_id) && cell_distance_[x - 1][y].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x - 1][y].distance)
					cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x - 1][y].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y]);
			}

			if (isValid(x + 1, y) && !isBlock(x + 1, y, o_id) && cell_distance_[x + 1][y].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x + 1][y].distance)
					cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x + 1][y].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y]);
			}

			if (isValid(x, y - 1) && !isBlock(x, y - 1, o_id) && cell_distance_[x][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y - 1].distance)
					cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x][y - 1]);
			}

			if (isValid(x, y + 1) && !isBlock(x, y + 1, o_id) && cell_distance_[x][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y + 1].distance)
					cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x][y + 1]);
			}

			if (isValid(x + 1, y + 1) && !isBlock(x + 1, y + 1, o_id) && cell_distance_[x + 1][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y + 1].distance)
					cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y + 1]);
			}

			if (isValid(x + 1, y - 1) && !isBlock(x + 1, y - 1, o_id) && cell_distance_[x + 1][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y - 1].distance)
					cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y - 1]);
			}

			if (isValid(x - 1, y + 1) && !isBlock(x - 1, y + 1, o_id) && cell_distance_[x - 1][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y + 1].distance)
					cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y + 1]);
			}

			if (isValid(x - 1, y - 1) && !isBlock(x - 1, y - 1, o_id) && cell_distance_[x - 1][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y - 1].distance)
					cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y - 1]);
			}
		}
		obstacles[o_id].distance_map.push_back(cell_distance_);
	}

	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			// obstacle
			if (obstacles[o_id].distance_map[0][i][j].distance == FLT_MAX)
				continue;

			float min_exit = FLT_MAX;

			for (int k = 0; k < (int)n_exits.size(); k++)
			{
				int x = n_exits[k].first;
				int y = n_exits[k].second;
				float distance = abs(obstacles[o_id].distance_map[k][x][y].distance - obstacles[o_id].distance_map[k][i][j].distance);
				if (distance < min_exit)
					min_exit = distance;
			}
			cell[i][j].specific_sFF[o_id] = min_exit;

			if (cell[i][j].specific_sFF[o_id] > max)
				max = cell[i][j].specific_sFF[o_id];
		}
	}
	
	cell_manager.specific_max_sFF[o_id] = max;
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (obstacles[o_id].distance_map[0][i][j].distance == FLT_MAX)
				continue;
			cell[i][j].special_color_sFF[o_id] = (max - cell[i][j].special_sFF[o_id]) / max;
			cell[i][j].special_normalize_sFF[o_id] = (max - cell[i][j].special_sFF[o_id]) / max;
		}
	}

	//output_colorTableWithoutExit(o_id);
	/*for (int i = 0; i < exits.size(); i++)
	{
		vector<vector<CELL_BFS>> cell_distance_;
		cell_distance_.resize(model->size);
		for (int j = 0; j < model->size; j++)
		{
			cell_distance_[j].resize(model->size);
		}

		for (int x = 0; x < model->size; x++)
		{
			for (int y = 0; y < model->size; y++)
			{
				cell_distance_[x][y].visisted = false;
				cell_distance_[x][y].distance = INFINITY;
				cell_distance_[x][y].x = x;
				cell_distance_[x][y].y = y;
			}
		}

		queue<CELL_BFS> distance_queue;
		int exit_x = exits[i].first;
		int exit_y = exits[i].second;
		bool block_exit = false;
		for (int j = 0; j < obstacles[o_id].component.size(); j++)
		{
			int o_x = obstacles[o_id].component[j].first;
			int o_z = obstacles[o_id].component[j].second;
			if (exit_x == o_x && exit_y == o_z)
			{
				block_exit = true;
				break;
			}
		}
		if (block_exit)
			continue;
		cell_distance_[exit_x][exit_y].distance = 0;
		distance_queue.push(cell_distance_[exit_x][exit_y]);

		while (!distance_queue.empty())
		{
			CELL_BFS temp = distance_queue.front();
			distance_queue.pop();
			int x = temp.x;
			int y = temp.y;
			cell_distance_[x][y].visisted = true;

			if (isValid(x - 1, y) && !isBlock(x - 1, y, o_id) && cell_distance_[x - 1][y].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x - 1][y].distance)
					cell_distance_[x - 1][y].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x - 1][y].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y]);
			}

			if (isValid(x + 1, y) && !isBlock(x + 1, y, o_id) && cell_distance_[x + 1][y].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x + 1][y].distance)
					cell_distance_[x + 1][y].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x + 1][y].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y]);
			}

			if (isValid(x, y - 1) && !isBlock(x, y - 1, o_id) && cell_distance_[x][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y - 1].distance)
					cell_distance_[x][y - 1].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x][y - 1]);
			}

			if (isValid(x, y + 1) && !isBlock(x, y + 1, o_id) && cell_distance_[x][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + 1 < cell_distance_[x][y + 1].distance)
					cell_distance_[x][y + 1].distance = cell_distance_[x][y].distance + 1;
				cell_distance_[x][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x][y + 1]);
			}

			if (isValid(x + 1, y + 1) && !isBlock(x + 1, y + 1, o_id) && cell_distance_[x + 1][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y + 1].distance)
					cell_distance_[x + 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y + 1]);
			}

			if (isValid(x + 1, y - 1) && !isBlock(x + 1, y - 1, o_id) && cell_distance_[x + 1][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x + 1][y - 1].distance)
					cell_distance_[x + 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x + 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x + 1][y - 1]);
			}

			if (isValid(x - 1, y + 1) && !isBlock(x - 1, y + 1, o_id) && cell_distance_[x - 1][y + 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y + 1].distance)
					cell_distance_[x - 1][y + 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y + 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y + 1]);
			}

			if (isValid(x - 1, y - 1) && !isBlock(x - 1, y - 1, o_id) && cell_distance_[x - 1][y - 1].visisted == false)
			{
				if (cell_distance_[x][y].distance + DIAGONAL_VALUE < cell_distance_[x - 1][y - 1].distance)
					cell_distance_[x - 1][y - 1].distance = cell_distance_[x][y].distance + DIAGONAL_VALUE;
				cell_distance_[x - 1][y - 1].visisted = true;
				distance_queue.push(cell_distance_[x - 1][y - 1]);
			}
		}
		obstacles[o_id].distance_map.push_back(cell_distance_);
	}

	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			// obstacle
			if (obstacles[o_id].distance_map[0][i][j].distance == INFINITY)
				continue;

			float min_exit = FLT_MAX;

			int index = 0;
			for (int k = 0; k < exits.size(); k++)
			{
				int x = exits[k].first;
				int y = exits[k].second;
				bool skip = false;
				for (int l = 0; l < obstacles[o_id].component.size(); l++)
				{
					int o_x = obstacles[o_id].component[l].first;
					int o_z = obstacles[o_id].component[l].second;
					if (x == o_x && y == o_z)
					{
						skip = true;
						break;
					}
				}
				if (skip)
					continue;
				float distance = abs(obstacles[o_id].distance_map[index][x][y].distance - obstacles[o_id].distance_map[index][i][j].distance);
				if (distance < min_exit)
					min_exit = distance;
				index++;
			}
			cell[i][j].specific_sFF[o_id] = min_exit;

			if (cell[i][j].specific_sFF[o_id] > max)
				max = cell[i][j].specific_sFF[o_id];
		}
	}

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (obstacles[o_id].distance_map[0][i][j].distance == INFINITY)
				continue;
			cell[i][j].specific_color_sFF[o_id] = (max - cell[i][j].specific_sFF[o_id]) / max;
			cell[i][j].specific_normalize_sFF[o_id] = (max - cell[i][j].specific_sFF[o_id]) / max * EXP_SCALER;
		}
	}*/
}

void CS_CELLULAR_AUTOMATA::computeAgentLeadership(){

	float extraversion;
	float neuroticism;
	float l_total = 0, m_total = 0;
	for (int i = 0; i < model->agent_number; i++)
	{
		extraversion = agent[i].mPersonality.extraversion;
		neuroticism = agent[i].mPersonality.neuroticism;
		agent[i].psychology.leadership = mParameter.W_E_L * extraversion + mParameter.W_N_L * (1 - neuroticism);
		if (agent[i].psychology.leadership > 1.0f)
			agent[i].psychology.leadership = 1.0f;
		if (agent[i].compressive_leader)
		{
			//cout << "neuroticism: " << neuroticism << endl;
			//cout << "extraversion: " << extraversion << endl;
			//cout << "leadership: " << agent[i].psychology.leadership << endl;
			l_total += agent[i].psychology.leadership;
			continue;
		}
		m_total += agent[i].psychology.leadership;
	}
	cout << "leader's leadership: " << l_total / (model->agent_number * guiParameter.leader_proportion + 0.0000001f) << endl;
	cout << "member's leadership: " << m_total / (model->agent_number * (1 - guiParameter.leader_proportion) + 0.0000001f) << endl;
}

void CS_CELLULAR_AUTOMATA::computeAgentPanic(){

	float neuroticism;
	float conscientiousness;
	for (int i = 0; i < model->agent_number; i++)
	{
		conscientiousness = agent[i].mPersonality.conscientiousness;
		neuroticism = agent[i].mPersonality.neuroticism;
		agent[i].psychology.panic = mParameter.W_N_P * neuroticism + mParameter.W_C_P * agent[i].computePanicConscientiousnessFunction();
		if (agent[i].psychology.panic > 1.0f)
			agent[i].psychology.panic = 1.0f;
	}
}

void CS_CELLULAR_AUTOMATA::copy_dynamicFF(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].temp_dFF = cell[i][j].dFF;
		}
	}
}

void CS_CELLULAR_AUTOMATA::copy_preUpdate_time(){

	model->total_pause_through_time += model->pause_through_time;
}

void CS_CELLULAR_AUTOMATA::resolve_conflicts(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
		}
	}
}

void CS_CELLULAR_AUTOMATA::scale_dFF(){

	float max = -FLT_MAX;

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (cell[i][j].dFF > max)
				max = cell[i][j].dFF;
		}
	}
	if (model->remain_agent != 0)
	{
		mDynamicFloorField.push_back(max);
	}
	//cout << "dynamic dFF: " << max << endl;
	// scale the dynamic floor field value to 0~13
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (cell[i][j].obstacle == true)
				continue;
			cell[i][j].dFF_color = cell[i][j].dFF / MAX_DFF;
			if (cell[i][j].dFF / MAX_DFF > 1)
				cell[i][j].dFF_color = 1.0f;
			cell[i][j].normalize_dFF = cell[i][j].dFF_color;
			//cell[i][j].dFF_color = cell[i][j].dFF / max;
			//cell[i][j].normalize_dFF = (cell[i][j].dFF / max);
			//cout << cell[i][j].normalize_dFF << " ";
		}
		//cout << endl;
	}
}

float CS_CELLULAR_AUTOMATA::distanceToLeader(int x, int z, int agent_num){

	float distance;
	float x_diff;
	float z_diff;
	if (!agent[agent[agent_num].leader_id].arrival)
	{
		x_diff = (float)abs(agent[agent[agent_num].leader_id].position.first - x);
		z_diff = (float)abs(agent[agent[agent_num].leader_id].position.second - z);
	}
	if (agent[agent[agent_num].leader_id].arrival)
	{
		x_diff = (float)abs(agent[agent[agent_num].leader_id].escape_position.first - x);
		z_diff = (float)abs(agent[agent[agent_num].leader_id].escape_position.second - z);
	}

	distance = (float)sqrt(pow(x_diff, 2) + pow(z_diff, 2));

	//return abs(cell[x][z].sFF - cell[agent[agent[agent_num].leader_id].position.first][agent[agent[agent_num].leader_id].position.second].sFF);

	return distance;
}

void CS_CELLULAR_AUTOMATA::switchVolunteer(int o_id, int com0, int com1){

	int temp = obstacles[o_id].volunteer_id[com0];
	agent[temp].obstacle_component_id = com1;
	obstacles[o_id].volunteer_id[com0] = obstacles[o_id].volunteer_id[com1];
	agent[obstacles[o_id].volunteer_id[com1]].obstacle_component_id = com0;
	obstacles[o_id].volunteer_id[com1] = temp;
}

void CS_CELLULAR_AUTOMATA::check_arrival(){

	for (int i = 0; i < model->agent_number; i++)
	{
		int x = agent[i].position.first;
		int y = agent[i].position.second;
		if (!isValid(x, y))
			continue;
		for (int j = 0; j < (int)mExit.size(); j++)
		{
			for (int k = 0; k < mExit[j].width; k++)
			{
				int ex = mExit[j].position.first + mExit[j].direction.first*k;
				int ez = mExit[j].position.second + mExit[j].direction.second*k;
				if (x == ex && y == ez)
				{
					mExit[j].mEscapeTime = mTimeCounter;
					cell[x][y].occupied = 0;
					agent[i].arrival = true;
					agent[i].escape_position = agent[i].position;
					agent[i].mEscapeExit = j;
					agent[i].escapeTime = (float)mTimeCounter;
					agent[i].position = PAIR_INT(-1, -1);
					agent[i].volunteer = false;
					if (agent[i].volunteer)
					{
						obstacles[agent[i].blocked_obstacle_id].volunteer_id[agent[i].obstacle_component_id] = -1;
						agent[i].blocked_obstacle_id = -1;
						agent[i].obstacle_component_id = -1;
					}
				}
			}
		}
		/*for (int j = 0; j < exits.size(); j++)
		{
			if (x == exits[j].first && y == exits[j].second)
			{
				cell[x][y].occupied = 0;
				agent[i].arrival = true;
				agent[i].escape_position = agent[i].position;
			}
		}*/
		/*if (cell[x][y].cell_type == 1)
		{
			cell[x][y].occupied = 0;
			agent[i].arrival = true;
			agent[i].escape_position = agent[i].position;
			cout << i << endl;
			//system("pause");
		}*/
	}
	int escape_agent = 0;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			escape_agent++;
		if (!agent[i].arrival)
			agent[i].escape_time = clock();
	}
	model->remain_agent = model->agent_number - escape_agent;
}

void CS_CELLULAR_AUTOMATA::check_task_finish(){

	for (int i = 0; i < (int)removed_obstacles.size(); i++)
	{
		int x = removed_obstacles[i].position.first;
		int z = removed_obstacles[i].position.second;
		if (!isValid(x, z))
			continue;
		if (removed_obstacles[i].volunteer_id == -1)
			continue;
		if (sqrt(pow(agent[removed_obstacles[i].volunteer_id].position.first - x, 2))
			+ pow(agent[removed_obstacles[i].volunteer_id].position.second - z, 2) == 1
			&& !agent[removed_obstacles[i].volunteer_id].beside_obstacle)
			agent[removed_obstacles[i].volunteer_id].beside_obstacle = true;
		if (removed_obstacles[i].position == removed_obstacles[i].destination)
		{
			removed_obstacles[i].arrival_destination = true;
			agent[removed_obstacles[i].volunteer_id].finish_task = true;
			cell[x][z].obstacle_ = 0;
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_volunteer_besideObstacle(){

	cout << "check voluteer beside obstacle." << endl;
	for (int i = 0; i < (int)strength_agent.size(); i++)
	{
		int id = strength_agent[i];
		if (!agent[id].volunteer)
			continue;
		int o_id = agent[id].push_obstacle_id;
		int com_id = agent[id].obstacle_component_id;
		int x = agent[id].position.first;
		int z = agent[id].position.second;
		int diff_x = x - obstacles[o_id].component[com_id].first;
		int diff_z = z - obstacles[o_id].component[com_id].second;
		if (sqrt(pow(diff_x, 2) + pow(diff_z, 2) == 1))
		{
			agent[id].beside_obstacle = true;
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_volunteer_beside_obstacle(){

	for (int i = 0; i < model->agent_number; i++)

	{
		if (agent[i].beside_obstacle)
		{
			//cout << agent[i].blocked_obstacle_id << endl;
			//cout << agent[i].obstacle_component_id << endl;
		}
	}
	// block obstacle size
	for (unsigned int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		// block component size
		for (unsigned int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			/*if (obstacles[o_id].volunteer_id[j] != -1)
				continue;
			// block component candidate volunteer size
			for (int k = 0; k < (int)obstacles[o_id].candidate_id[j].size(); k++)
			{
				int id = obstacles[o_id].candidate_id[j][k];
				if (pair_int_distance(agent[id].position, obstacles[o_id].component[j]) <= 1.0f)
				{
					obstacles[o_id].volunteer_id[j] = id;
					agent[id].beside_obstacle = true;
					clearCandidateVolunteer(o_id, j);
					break;
				}
			}*/
			if (obstacles[o_id].volunteer_id[j] == -1)
			{
				//cout << "got you bro! " << j << endl;
				continue;
			}
			int id = obstacles[o_id].volunteer_id[j];
			if (agent[id].arrival)
			{
				obstacles[o_id].volunteer_id[j] = -1;
				continue;
			}
			if (pair_int_distance(agent[id].position, obstacles[o_id].component[j]) <= 1)
			{
				agent[id].beside_obstacle = true;
				agent[id].mBlockWay = false;
			}
			else if (agent[id].mBlockWay && agent[id].position != agent[id].block_pos && pair_int_distance(agent[id].position, obstacles[o_id].component[j]) <= 1.0f)
			{
				agent[id].beside_obstacle = true;
				agent[id].mBlockWay = false;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_volunteer_position(){

	/*for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			int id = obstacles[o_id].volunteer_id[j];
			if (id == -1)
				continue;
			if (agent[id].arrival)
				continue;
			for (int k = 0; k < obstacles[o_id].volunteer_id.size(); k++)
			{
				int _id = obstacles[o_id].volunteer_id[k];
				if (pair_int_distance(agent[id].position, obstacles[o_id].component[k]) == 1 || _id == -1 && !agent[_id].beside_obstacle)
				{
					switchVolunteer(o_id, j, k);
					agent[id].beside_obstacle = true;
				}
			}
		}
	}*/
	/*for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			int id = obstacles[o_id].volunteer_id[j];
			if (id == -1)
				continue;
			if (agent[id].arrival)
				continue;
			switch (obstacles[o_id].type)
			{
				//vertical
			case 0:
				if (agent[id].position.first == obstacles[o_id].moveDestination[0].first)
				{
					delete_volunteer(o_id, j, id);
				}
				break;
				//horizontal
			case 1:
				if (agent[id].position.second == obstacles[o_id].moveDestination[0].second)
				{
					delete_volunteer(o_id, j, id);
				}
				break;
			}
		}
	}*/
	for (unsigned int i = 0; i < blocked_obstacles.size(); i++)
	{
		int id = blocked_obstacles[i];
		for (unsigned int j = 0; j < obstacles[id].volunteer_id.size(); j++)
		{
			int a_id = obstacles[id].volunteer_id[j];
			if (a_id == -1)
				continue;
			for (unsigned int k = 0; k < obstacles[id].moveDestination.size(); k++)
			{
				if (agent[a_id].position == obstacles[id].moveDestination[k])
				{
					//cout << agent[a_id].position.first << " " << agent[a_id].position.second << endl;
					//system("pause");
					delete_volunteer(id, j, a_id);
					cout << "delete " << j << endl;
					//agent[a_id].block_pos = agent[id].position;
					//agent[a_id].mBlockWay = true;
					//agent[a_id].beside_obstacle = false;
					//int mapID = getVolunteerMapID(id, j);
					//computeVolunteerSFFNotConsiderBlockPosition(id, j);
				}
			}
		}
	}
	for (int i = 0; i < model->agent_number; i++)
	{
		for (int j = 0; j < model->agent_number; j++)
		{
			if (i == j)
				continue;
			if (agent[i].volunteer && agent[j].volunteer &&(agent[i].position == agent[j].position))
			{
				//delete_volunteer()
				//add_normal_obstacle(agent[i].blocked_obstacle_id);
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_guider_position(){

	for (unsigned int i = 0; i < mGuiderID.size(); i++)
	{
		int id = mGuiderID[i];
		//if ()
	}
}

void CS_CELLULAR_AUTOMATA::check_obstacle_arrive(int o_id){

	if (obstacles[o_id].component[0].first == obstacles[o_id].moveDestination[0].first && obstacles[o_id].component[0].second == obstacles[o_id].moveDestination[0].second)
		obstacles[o_id].arrive_destination = true;
}

void CS_CELLULAR_AUTOMATA::check_obstacleMovement_blocked(int o_id, Direction& d){

	switch (agent_psychology.obstacle_moveType)
	{
	case CS_OBSTACLE_MANUAL_MOVE:
		check_obstacleMovement_blocked_manual(o_id);
		break;
	case CS_OBSTACLE_AUTOMATIC_MOVE:
		check_obstacleMovement_blocked_auto(o_id, d);
		break;
	}
}

void CS_CELLULAR_AUTOMATA::check_obstacleMovement_blocked_auto(int o_id, Direction& d){

	/*PAIR_INT direction = obstacles[o_id].direction;
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
		//reset_advanced_sFF(a_id);
		agent[a_id].direction = d;
		//addsFF_resolveCounterFlow(a_id);
		//if (!agent[a_id].add_sFF)
		//{
		//	agent[a_id].add_sFF = true;
		//	addsFF_resolveCounterFlow(a_id);
		//}
		int x = agent[a_id].position.first + direction.first;
		int z = agent[a_id].position.second + direction.second;
		// check agent blocked or not
		// if cell occupied by agent 
		if (cell[x][z].occupied == 1)
		{
			if (!check_occupiedAgent_sameTask(x, z, o_id, a_id))
			{
				cout << "agent" << a_id << " blocked by agent" << cell[x][z].occupant_id << endl << endl;
				cout << "agent" << a_id << " push obstacle id: " << o_id << endl;
				cout << "agent" << cell[x][z].occupant_id << " push obstacle id: " << agent[cell[x][z].occupant_id].blocked_obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		// if cell occupied by obstacle
		if (cell[x][z].obstacle)
		{
			if (cell[x][z].obstacle_id != o_id)
			{
				cout << "agent" << a_id << " blocked by obstacle" << cell[x][z].obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		// check obstacle blocked or not
		// if cell occupied by agent
		if (cell[o_x][o_z].occupied == 1)
		{
			if (!check_occupiedAgent_sameTask(o_x, o_z, o_id, a_id))
			{
				cout << "obstacle" << o_id << " blocked by agent" << cell[o_x][o_z].occupant_id << endl << endl;
				cout << "agent" << a_id << " push obstacle id: " << o_id << endl;
				cout << "agent" << cell[x][z].occupant_id << " push obstacle id: " << agent[cell[x][z].occupant_id].blocked_obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		// if cell occupied by obstacle
		if (cell[o_x][o_z].obstacle)
		{
			if (cell[o_x][o_z].obstacle_id != o_id)
			{
				cout << "obstacle" << o_id << " blocked by obstacle" << cell[o_x][o_z].obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
	}
	obstacles[o_id].block = false;*/
	bool movable = true;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
		bool stock_ob = false;
		for (unsigned int j = 0; j < obstacles[o_id].moveDestination.size(); j++)
		{
			if (agent[a_id].position == obstacles[o_id].moveDestination[j])
			{
				//delete_volunteer(o_id, i, a_id);
				stock_ob = true;
				break;
			}
		}
		if (stock_ob)
			continue;
		if (check_obstacle_movement_block(o_id) || check_volunteer_movement_block(a_id) || !check_moveToLowSFF(o_id))
		{
			//obstacles[o_id].movable = true;
			//obstacles[o_id].movable = false;
			//cout << "obstacle " << o_id << " not movable" << endl;
			//system("pause");
			movable = false;
			break;
		}
	}
	//if (movable)
	//	cout << "obstacle " << o_id << " movable!" << endl;
	obstacles[o_id].movable = movable;
	if (obstacles[o_id].movable)
		return;
	float top = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	int px0 = obstacles[o_id].component[0].first;
	int pz0 = obstacles[o_id].component[0].second;
	int desx0 = obstacles[o_id].moveDestination[0].first;
	int desz0 = obstacles[o_id].moveDestination[0].second;
	if (isValid(px0, pz0 + 1))
		top = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0, pz0 + 1));
	if (isValid(px0, pz0 - 1))
		down = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0, pz0 - 1));
	if (isValid(px0 - 1, pz0))
		left = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 - 1, pz0));
	if (isValid(px0 + 1, pz0))
		right = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 + 1, pz0));
	if (model->mNeighborType != 0)
	{
		if (isValid(px0 + 1, pz0 + 1))
			topRight = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 + 1, pz0 + 1));
		if (isValid(px0 - 1, pz0 + 1))
			topLeft = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 - 1, pz0 + 1));
		if (isValid(px0 + 1, pz0 - 1))
			downRight = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 + 1, pz0 - 1));
		if (isValid(px0 - 1, pz0 - 1))
			downLeft = pair_int_distance(PAIR_INT(desx0, desz0), PAIR_INT(px0 - 1, pz0 - 1));
	}
	float dis[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	float sort_dis[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(sort_dis, sort_dis + 8);
	vector<PAIR_INT> overallPos;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		int ox = obstacles[o_id].component[i].first;
		int oz = obstacles[o_id].component[i].second;

		if (a_id < 0)
			continue;
	}
	for (int i = 0; i < 2; i++)
	{
		bool movable_ = true;
		/*int dir;
		for (int j = 0; j < 8; j++)
		{
			if (sort_dis[i] == dis[j])
			{
				dir = j;
			}
		}*/
		if (sort_dis[i] == top)
		{
			obstacles[o_id].direction = PAIR_INT(0, 1);
			d = _up;
		}
		if (sort_dis[i] == right)
		{
			obstacles[o_id].direction = PAIR_INT(1, 0);
			d = _right;
		}
		if (sort_dis[i] == down)
		{
			obstacles[o_id].direction = PAIR_INT(0, -1);
			d = _down;
		}
		if (sort_dis[i] == left)
		{
			obstacles[o_id].direction = PAIR_INT(-1, 0);
			d = _left;
			break;
		}
		if (sort_dis[i] == topRight)
		{
			obstacles[o_id].direction = PAIR_INT(1, 1);
			d = _topRight;
			break;
		}
		if (sort_dis[i] == topLeft)
		{
			obstacles[o_id].direction = PAIR_INT(-1, 1);
			d = _topLeft;
			break;
		}
		if (sort_dis[i] == downRight)
		{
			obstacles[o_id].direction = PAIR_INT(1, -1);
			d = _downRight;
			break;
		}
		if (sort_dis[i] == downLeft)
		{
			obstacles[o_id].direction = PAIR_INT(-1, -1);
			d = _downLeft;
			break;
		}
		if (check_obstacle_movement_block(o_id))
		{
			movable_ = false;
			break;
		}
		for (unsigned int j = 0; j < obstacles[o_id].component.size(); j++)
		{
			/*if (sort_dis[i] == top)
			{
				obstacles[o_id].direction = PAIR_INT(0, 1);
				d = _up;
			}
			if (sort_dis[i] == right)
			{
				obstacles[o_id].direction = PAIR_INT(1, 0);
				d = _right;
			}
			if (sort_dis[i] == down)
			{
				obstacles[o_id].direction = PAIR_INT(0, -1);
				d = _down;
			}
			if (sort_dis[i] == left)
			{
				obstacles[o_id].direction = PAIR_INT(-1, 0);
				d = _left;
				break;
			}
			if (sort_dis[i] == topRight)
			{
				obstacles[o_id].direction = PAIR_INT(1, 1);
				d = _topRight;
				break;
			}
			if (sort_dis[i] == topLeft)
			{
				obstacles[o_id].direction = PAIR_INT(-1, 1);
				d = _topLeft;
				break;
			}
			if (sort_dis[i] == downRight)
			{
				obstacles[o_id].direction = PAIR_INT(1, -1);
				d = _downRight;
				break;
			}
			if (sort_dis[i] == downLeft)
			{
				obstacles[o_id].direction = PAIR_INT(-1, -1);
				d = _downLeft;
				break;
			}*/
			/*if (check_obstacle_movement_block(o_id))
			{
				movable_ = false;
				break;
			}*/
			int a_id = obstacles[o_id].volunteer_id[i];
			if (a_id < 0)
				continue;
			if (!agent[a_id].beside_obstacle)
				continue;
			if (check_volunteer_movement_block(a_id))/*|| !check_moveToLowSFF(o_id)*/
			{
				movable_ = false;
				break;
			}
		}
		obstacles[o_id].movable = movable_;
		if (obstacles[o_id].movable)
		{
			return;
		}
	}
	/*for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == obstacles[o_id].block_direction)
				continue;
			switch (j)
			{
			case 0:
				obstacles[o_id].direction = PAIR_INT(0, 1);
				d = _up;
				break;
			case 1:
				obstacles[o_id].direction = PAIR_INT(1, 0);
				d = _right;
				break;
			case 2:
				obstacles[o_id].direction = PAIR_INT(0, -1);
				d = _down;
				break;
			case 3:
				obstacles[o_id].direction = PAIR_INT(-1, 0);
				d = _left;
				break;
			}
			if (!check_obstacle_movement_block(o_id) && !check_volunteer_movement_block(a_id) && check_moveToLowSFF(o_id))
			{
				obstacles[o_id].movable = true;
				//obstacles[o_id].block = false;
				return;
			}
		}
	}*/
	d = local;
	obstacles[o_id].movable = false;
	//obstacles[o_id].block = true;
}

void CS_CELLULAR_AUTOMATA::check_obstacleMovement_blocked_manual(int o_id){

	/*cout << "check route to obstacle destination blocked or not." << endl;
	int clicked_id = obstacles[o_id].final_clicked_o_id;
	PAIR_INT destination = obstacles[o_id].volunteer_destination;
	PAIR_INT com_position = obstacles[o_id].component[clicked_id];
	PAIR_INT direction = obstacles[o_id].direction;
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		int x = agent[a_id].position.first + direction.first;
		int z = agent[a_id].position.second + direction.second;
		// check agent blocked or not
		// if cell occupied by agent 
		if (cell[x][z].occupied == 1)
		{
			if (!check_occupiedAgent_sameTask(x, z, o_id, a_id))
			{
				cout << "blocked by agent" << cell[x][z].occupant_id << endl << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		// if cell occupied by obstacle
		if (cell[x][z].obstacle)
		{
			if (cell[x][z].obstacle_id != o_id)
			{
				cout << "blocked by obstacle" << cell[x][z].obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		// check obstacle blocked or not
		// if cell occupied by agent
		if (cell[o_x][o_z].occupied == 1)
		{
			if (!check_occupiedAgent_sameTask(o_x, o_z, o_id, a_id))
			{
				cout << "blocked by agent" << cell[o_x][o_z].occupant_id << endl << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
		// if cell occupied by obstacle
		if (cell[o_x][o_z].obstacle)
		{
			if (cell[o_x][o_z].obstacle_id != o_id)
			{
				cout << "blocked by obstacle" << cell[o_x][o_z].obstacle_id << endl;
				obstacles[o_id].block = true;
				return;
			}
		}
	}
	obstacles[o_id].block = false;*/
	for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i]; 
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
		if (check_obstacle_movement_block(o_id) || check_volunteer_movement_block(a_id) || !check_moveToLowSFF(o_id))
		{
			//obstacles[o_id].movable = true;
			obstacles[o_id].movable = false;
			return;
		}
	}
	//d = local;
	/*cout << "---------------------------------------------" << endl;
	if (!check_obstacle_movement_block(o_id))
		cout << "obstacle_" << o_id << " is movable." << endl;
	else
		cout << "obstacle_" << o_id << " isn't movable." << endl;
	if (check_moveToLowSFF(o_id))
		cout << "obstacle_" << o_id << " is move to low SFF." << endl;
	else
		cout << "obstacle_" << o_id << " isn't move to low SFF." << endl;
	if (!check_obstacle_movement_block(o_id) && check_moveToLowSFF(o_id))
		cout << "volunteers be blocked." << endl;
	cout << "---------------------------------------------" << endl << endl;*/
	obstacles[o_id].movable = true;
}

void CS_CELLULAR_AUTOMATA::check_volunteer_finish_task(int o_id){

	if (obstacles[o_id].component[obstacles[o_id].final_clicked_o_id] == obstacles[o_id].volunteer_destination)
	{
		for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
		{
			int a_id = obstacles[o_id].volunteer_id[i];
			agent[a_id].volunteer = false;
			agent[a_id].push_obstacle_id = -1;
			agent[a_id].beside_obstacle = false;
			obstacles[o_id].volunteer_id[i] = -1;
			obstacles[o_id].arrive_destination = true;
			obstacles[o_id].block = false;
			//if (cell[obstacles[o_id].component[i].first][obstacles[o_id].component[i].second].obstacle)
			//	cout << "fuck!" << endl;
			//cout << obstacles[o_id].component[i].first << " " << obstacles[o_id].component[i].second << endl;
			cell[obstacles[o_id].component[i].first][obstacles[o_id].component[i].second].obstacle = true;
			cell[obstacles[o_id].component[i].first][obstacles[o_id].component[i].second].obstacle_ = 0;
			cell[obstacles[o_id].component[i].first][obstacles[o_id].component[i].second].obstacle_id = o_id;
			cell[obstacles[o_id].component[i].first][obstacles[o_id].component[i].second].cell_type = 2;
		}
		if (!obstacles[o_id].compute)
		{
			compute_staticFF();
			obstacles[o_id].compute = true;
		}
		add_normal_obstacle(o_id);
		//system("pause");
	}
}

void CS_CELLULAR_AUTOMATA::check_agent_willness(int o_id){

	for (int i = 0; i < (int)obstacles[o_id].volunteer_id.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (agent[a_id].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			//cout << agent[a_id].psychology.willness << endl;
			obstacles[o_id].volunteer_id[i] = -1;
			agent[a_id].volunteer = false;
			agent[a_id].push_obstacle_id = -1;
			agent[a_id].beside_obstacle = false;
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_volunteer_willness(int o_id){

	for (int i = 0; i < (int)obstacles[o_id].volunteer_id.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (agent[a_id].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			//cout << agent[a_id].psychology.willness << endl;
			obstacles[o_id].volunteer_id[i] = -1;
			agent[a_id].volunteer = false;
			agent[a_id].push_obstacle_id = -1;
			agent[a_id].obstacle_component_id = -1;
			agent[a_id].beside_obstacle = false;
		}
	}
}

void CS_CELLULAR_AUTOMATA::check_cell_occupied(){

	for (int x = 0; x < model->size; x++)
	{
		for (int z = 0; z < model->size; z++)
		{
			if (cell[x][z].occupied == 1 && cell[x][z].occupant_id == -1)
			{
				cell[x][z].occupied = 0;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::calculateCellProbality(int x, int y){

	cell[x][y].probability = exp(model->kd * cell[x][y].dFF) * exp(model->ks * cell[x][y].sFF) * (1 - cell[x][y].occupied);
}

void CS_CELLULAR_AUTOMATA::clearCandidateVolunteer(int o_id, int com_id){

	//cout << "clear obstacle_" << o_id << " com_" << com_id << " candidate volunteer" << endl;
	for (unsigned int i = 0; i < obstacles[o_id].candidate_id[com_id].size(); i++)
	{
		int a_id = obstacles[o_id].candidate_id[com_id][i];
		if (a_id == obstacles[o_id].volunteer_id[com_id])
			continue;
		agent[a_id].volunteer = false;
		agent[a_id].blocked_obstacle_id = -1;
		agent[a_id].obstacle_component_id = -1;
		agent[a_id].push_obstacle_id = -1;
		volunteer_id.push_back(a_id);
		//cout << a_id << endl;
	}
	obstacles[o_id].candidate_id[com_id].clear();
	//system("pause");
}

void CS_CELLULAR_AUTOMATA::reset(){

	//cout << "reset cellular simulation." << endl;
	//cout << "seed: " << model->seed << endl;
	//srand(model->seed);
	//cout << "Evacuation time: " << mTimeCounter << endl;
	if (model->mSameSeed)
		srand(model->seed);
	generateRandomAgentOrder();
	mTimeCounter = 0;
	mTimeStep = 0;
	vector<int>().swap(leader_pool);
	vector<int>().swap(strength_agent);
	vector<int>().swap(weakness_agent);
	vector<int>().swap(normal_obstacle);
	vector<int>().swap(blocked_obstacles);
	vector<int>().swap(volunteer_id);
	vector<float>().swap(mDynamicFloorField);
	leader_pool.clear();
	strength_agent.clear();
	weakness_agent.clear();
	normal_obstacle.clear();
	blocked_obstacles.clear();
	volunteer_id.clear();
	mDynamicFloorField.clear();
	mAverageAnxeityAroundObserveAgent.clear();
	mObserveAgentAnxiety.clear();
	if (model->mSameSeed)
	{
		mAverageAnxeityAroundObserveAgent.resize(mObserverAgent.size());
		mObserveAgentAnxiety.resize(mObserverAgent.size());
	}
	// reset agent's parameter
	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].reset();
	}
	gaussian_generate_personality();
	computeAgentLeadership();
	computeAgentPanic();
	model->reset();
	/*for (int i = 0; i < exits.size(); i++)
	{
		cell[exits[i].first][exits[i].second].cell_type = 1;
	}*/
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		for (int j = 0; j < mExit[i].width; j++)
		{
			int ex = mExit[i].position.first + mExit[i].direction.first*j;
			int ez = mExit[i].position.second + mExit[i].direction.second*j;
			cell[ex][ez].cell_type = 1;
		}
	}
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].reset();
		}
	}
	vector<OBSTACLE>().swap(obstacles);
	obstacles.clear();
	set_obstacle();
	set_wall();
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].create_space(obstacles.size());
		}
	}
	cell_manager.specific_max_sFF.resize(obstacles.size());
	compute_staticFF();
	compute_clean_sFF();
	int count = 0;
	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		count += obstacles[i].component.size();
		obstacles[i].moveDestination.resize(obstacles[i].volunteer_id.size());
	}
	volunteer_distance_map.resize(count);
	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		for (unsigned int j = 0; j < obstacles[i].component.size(); j++)
		{
			compute_volunteer_staticFF(i, j);
		}
		//computeNewMapWithoutExit(i);
	}
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_AUTOMATIC_MOVE)
		find_obstacleMustMove();
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_MANUAL_MOVE)
	{
		for (unsigned int i = 0; i < obstacles.size(); i++)
		{
			blocked_obstacles.push_back(i);
			obstacles[i].pool_index = (int)blocked_obstacles.size() - 1;
		}
		if (obstacles.size() == 1)
			obstacles[0].volunteer_destination = PAIR_INT(obstacles[0].component[0].first, obstacles[0].component[0].second + model->mDisToExit);
		//for (int i = 0; i < (int)obstacles.size(); i++)
		//{
		//	normal_obstacle.push_back(i);
		//	obstacles[i].pool_index = normal_obstacle.size() - 1;
		//}
	}
	/*
	regroup the agent
	*/
	set_group();
	/*
	initialize agents' position
	*/
	initialize_agent_position();
	initialize_agent_anxiety();
	for (unsigned int i = 0; i < removed_obstacles.size(); i++)
	{
		removed_obstacles[i].has_volunteer = false;
	}
}

void CS_CELLULAR_AUTOMATA::start(){

	model->start = true;
}

void CS_CELLULAR_AUTOMATA::pause(){

	model->start = false;
}

void CS_CELLULAR_AUTOMATA::setMoveObstacleTime()
{
	model->mMoveObstacleTime += 20;
	mExperimentData.mAverageEvacuationTime.resize(mExit.size());
	mExperimentData.mAverageLastAgentEvacuationTime.resize(mExit.size());
	mExperimentData.mEscapeAgentNumber.resize(mExit.size());
	for (int i = 0; i < model->mRunTimes; i++)
	{
		mExperimentData.mTotalAverageEvaciationTime[i] = 0;
		mExperimentData.mTotalAverageLastAgentEvacuationTime[i] = 0;
	}
	//mExperimentData.mTotalAverageEvaciationTime.resize(100);
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.resize(100);
}

void CS_CELLULAR_AUTOMATA::setMoveObstacleDistance()
{
	model->mMoveDistance += 3;
	mExperimentData.mAverageEvacuationTime.resize(mExit.size());
	mExperimentData.mAverageLastAgentEvacuationTime.resize(mExit.size());
	mExperimentData.mEscapeAgentNumber.resize(mExit.size());
	for (int i = 0; i < model->mRunTimes; i++)
	{
		mExperimentData.mTotalAverageEvaciationTime[i] = 0;
		mExperimentData.mTotalAverageLastAgentEvacuationTime[i] = 0;
	}
}

void CS_CELLULAR_AUTOMATA::setObstacleDestination()
{
	model->mDisToExit += 5;
	mExperimentData.mAverageEvacuationTime.resize(mExit.size());
	mExperimentData.mAverageLastAgentEvacuationTime.resize(mExit.size());
	mExperimentData.mEscapeAgentNumber.resize(mExit.size());
	for (int i = 0; i < model->mRunTimes; i++)
	{
		mExperimentData.mTotalAverageEvaciationTime[i] = 0;
		mExperimentData.mTotalAverageLastAgentEvacuationTime[i] = 0;
	}
}

void CS_CELLULAR_AUTOMATA::addStatisticalArea(){

	cout << model->select_agent_ID << endl;
	if (model->select_agent_ID == -1)
		return;
	mObserverAgent.push_back(model->select_agent_ID);
	mAverageAnxeityAroundObserveAgent.resize(mObserverAgent.size());
	mObserveAgentAnxiety.resize(mObserverAgent.size());
}

void CS_CELLULAR_AUTOMATA::set_cell_exit_segment(int x, int z){

	segment.x = x;
	segment.z = z;
}

void CS_CELLULAR_AUTOMATA::set_specific_group(int ID){

	model->group_ID = ID;
}

void CS_CELLULAR_AUTOMATA::set_numOfgroup_size(int size2_num, int size3_num){

	cout << "set group size." << endl;
	model->size2_num = size2_num;
	model->size3_num = size3_num;
	//cout << model->size2_num << " " << model->size3_num << endl;
}

void CS_CELLULAR_AUTOMATA::set_group_radius(int group_radius){

	cout << "set group radius: " << group_radius << endl;
	model->group_radius = group_radius;
}

void CS_CELLULAR_AUTOMATA::add_obstacles(float minx, float maxx, float minz, float maxz){

	OBSTACLE e;

	e.minx = minx;
	e.maxx = maxx;
	e.minz = minz;
	e.maxz = maxz;

	obstacles.push_back(e);
}

double CS_CELLULAR_AUTOMATA::get_probability(int x, int y, int agent_num){

	if (!agent[agent_num].leader && !agent[agent[agent_num].leader_id].arrival){
		
		//if the length to leader is smaller than certain distance, keep moving follow the formula
		//if not, move according to the direction of leader	
		int leader_x, leader_y;
		leader_x = agent[agent[agent_num].leader_id].position.first;
		leader_y = agent[agent[agent_num].leader_id].position.second;
		if (distanceToLeader(x, y, agent_num) < model->group_radius || !opposite_direction(agent_num, distanceToLeader(x, y, agent_num)))
			return exp((double)-model->kd_member * agent[agent_num].leader_distance) * exp((double)-model->ks_member * model->ks * (cell[x][y].sFF + cell[x][y].aFF)) * (1 - cell[x][y].occupied) * cell[x][y].obstacle_;
		return 0.0f;
	}
	// if agent is a leader block by obstacle and want to escape to another exit
	//if (agent[agent_num].blocked_obstacle_id != -1)
	//{
	//	int o_id = agent[agent_num].blocked_obstacle_id;
	//	return exp((double)model->kd * cell[x][y].dFF) * exp((double)-model->ks * cell[x][y].specific_sFF[o_id]) * (1 - cell[x][y].occupied) * cell[x][y].obstacle_;
	//}
	float anxietyEffect = 1 - agent[agent_num].anxiety;
	float sawExit = 1.0f;
	if (agent[agent_num].compressive_leader && anxietyEffect < agent[agent_num].psychology.leadership)
		anxietyEffect = agent[agent_num].psychology.leadership;
	if (anxietyEffect < 0.3f)
		anxietyEffect = 0.3f;
	// 如果agent看到出口而且出口沒被obstacle擋住，則agent將不再受DFF影響
	if (agent[agent_num].mSawExit)
	{
		anxietyEffect = 1;
		sawExit = 0;
	}
	if (agent[agent_num].blocked_obstacle_id == -1)
	{
		return exp((double)-model->kd * cell[x][y].dFF * sawExit) * exp((double)-model->ks * (cell[x][y].sFF) * anxietyEffect) * exp((double)-model->ka * cell[x][y].aFF) * (1 - cell[x][y].occupied) * cell[x][y].obstacle_;
	}
	int FloorFieldMapID = (int)decodingFloorFieldID(agent[agent_num].blockByExit);
	return exp((double)-model->kd * cell[x][y].dFF * sawExit) * exp((double)-model->ks * cell[x][y].special_sFF[FloorFieldMapID] * anxietyEffect) * exp((double)-model->ka * cell[x][y].aFF) * (1 - cell[x][y].occupied) * cell[x][y].obstacle_;
	
	//return exp((double)model->kd * cell[x][y].dFF) * exp((double)-model->ks * cell[x][y].sFF) * (1 - cell[x][y].occupied) * cell[x][y].obstacle_;
}

float CS_CELLULAR_AUTOMATA::get_probability_volunteer(int x, int z, int agent_num){

	float distanceToTarget;
	if (agent[agent_num].beside_obstacle)
		distanceToTarget = (float)sqrt(pow(x - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
		+ pow(z - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
	if (!agent[agent_num].beside_obstacle)
		distanceToTarget = (float)sqrt(pow(x - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
		+ pow(z - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
	//cout << agent[agent_num].position.first << " " << agent[agent_num].position.second << endl;
	if (!opposite_direction_toObstacle(agent_num, distanceToTarget))
		return exp(model->kd * cell[x][z].normalize_dFF) * exp(model->ks * cell[x][z]._sFF) * (1 - cell[x][z].occupied) * cell[x][z].obstacle_;
	return 0.0f;
}

float CS_CELLULAR_AUTOMATA::get_probability_volunteer(int x, int z, int agent_id, int com_id, int o_id){

	//float distanceToObstacle = (float)sqrt(pow(x - obstacles[o_id].component[com_id].first, 2) + pow(z - obstacles[o_id].component[com_id].second, 2));
	if (!isValid(x, z))
		return 0;
	float distanceToObstacle = pair_int_distance(PAIR_INT(x, z), obstacles[o_id].component[com_id]);
	int id = 0;
	for (int i = 0; i < o_id; i++)
	{
		id += obstacles[i].component.size();
	}
	// id非零
	if (!!id)
		id--;
	//cout << agent_id << endl;
	//cout << o_id << ", " << com_id << endl;
	id += com_id;
	return exp((double)-model->ks * volunteer_distance_map[id][x][z].sFF) * (1 - cell[x][z].occupied) * cell[x][z].obstacle_;
}

int CS_CELLULAR_AUTOMATA::getVolunteerMapID(int o_id, int com_id){

	int id = 0;
	for (int i = 0; i < o_id; i++)
	{
		id += obstacles[i].component.size();
	}
	// id非零
	if (!!id)
		id--;
	id += com_id;
	return id;
}

float CS_CELLULAR_AUTOMATA::get_probability_guider(int id, int x, int z){

	if (!isValid(x, z))
		return 0;
	return exp((double)-model->ks * pair_int_distance(PAIR_INT(x, z), agent[id].guide_pos)) * (1 - cell[x][z].occupied) * cell[x][z].obstacle_;
}

float CS_CELLULAR_AUTOMATA::get_probability_obstacle(int x, int z, int id){

	float distanceToDestination;
	distanceToDestination = (float)sqrt(pow(x - removed_obstacles[id].destination.first, 2)
		+ pow(z - removed_obstacles[id].destination.second, 2));
	if (!opposite_direction_toDestination(id, distanceToDestination))
		return (float)(1 - cell[x][z].occupied) * cell[x][z].obstacle_;
	return 0.0f;
}

float CS_CELLULAR_AUTOMATA::get_normalize(int x, int y){

	float top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f;

	if (isValid(x, y + 1))
		top = cell[x][y + 1].probability;
	if (isValid(x, y - 1))
		down = cell[x][y - 1].probability;
	if (isValid(x - 1, y))
		left = cell[x - 1][y].probability;
	if (isValid(x + 1, y))
		right = cell[x + 1][y].probability;

	return (top + down + left + right);
}

float CS_CELLULAR_AUTOMATA::get_inertia(int x, int y, int agent_number){

	INERTIA inertia = agent[agent_number].inertia;
	float pi = 0.0f;

	switch (inertia){
	case fromBottom:
		if (agent[agent_number].position.second == y - 1)
			pi = model->ki;
		break;
	case fromLeft:
		if (agent[agent_number].position.first == x - 1)
			pi = model->ki;
		break;
	case fromRight:
		if (agent[agent_number].position.first == x + 1)
			pi = model->ki;
		break;
	case fromTop:
		if (agent[agent_number].position.second == y + 1)
			pi = model->ki;
		break;
	case fromRightTop:
		if (agent[agent_number].position.first == x + 1 && agent[agent_number].position.second == y + 1)
			pi = model->ki;
		break;
	case fromLeftTop:
		if (agent[agent_number].position.first == x - 1 && agent[agent_number].position.second == y + 1)
			pi = model->ki;
		break;
	case fromRightBottom:
		if (agent[agent_number].position.first == x + 1 && agent[agent_number].position.second == y - 1)
			pi = model->ki;
		break;
	case fromLeftBottom:
		if (agent[agent_number].position.first == x - 1 && agent[agent_number].position.second == y - 1)
			pi = model->ki;
		break;
	}

	return exp(pi);
}

float CS_CELLULAR_AUTOMATA::get_leader_inertia(int x, int y, int agent_num){

	INERTIA inertia = agent[agent_num].inertia;
	float pi = 0.0f;

	bool arrival = agent[agent[agent_num].leader_id].arrival;

	switch (inertia){
	case fromBottom:
		if (!arrival){
			if (agent[agent[agent_num].leader_id].position.second == y - 1)
				pi = model->ki;
		}
		if (arrival){
			if (agent[agent[agent_num].leader_id].escape_position.second == y - 1)
				pi = model->ki;
		}
		break;
	case fromLeft:
		if (!arrival){
			if (agent[agent[agent_num].leader_id].position.first == x - 1)
				pi = model->ki;
		}
		if (arrival){
			if (agent[agent[agent_num].leader_id].escape_position.first == x - 1)
				pi = model->ki;
		}
		break;
	case fromRight:
		if (!arrival){
			if (agent[agent[agent_num].leader_id].position.first == x + 1)
				pi = model->ki;
		}
		if (arrival){
			if (agent[agent[agent_num].leader_id].escape_position.first == x + 1)
				pi = model->ki;
		}
		break;
	case fromTop:
		if (!arrival){
			if (agent[agent[agent_num].leader_id].position.second == y + 1)
				pi = model->ki;
		}
		if (arrival){
			if (agent[agent[agent_num].leader_id].escape_position.second == y + 1)
				pi = model->ki;
		}
		break;
	case fromRightTop:
		if (!arrival && agent[agent[agent_num].leader_id].position.first == x + 1 && agent[agent[agent_num].leader_id].position.second == y + 1){
			pi = model->ki;
		}
		if (arrival && agent[agent[agent_num].leader_id].escape_position.first == x + 1 && agent[agent[agent_num].leader_id].escape_position.second == y + 1){
			pi = model->ki;
		}
		break;
	case fromLeftTop:
		if (!arrival && agent[agent[agent_num].leader_id].position.first == x - 1 && agent[agent[agent_num].leader_id].position.second == y + 1){
			pi = model->ki;
		}
		if (arrival && agent[agent[agent_num].leader_id].escape_position.first == x - 1 && agent[agent[agent_num].leader_id].escape_position.second == y + 1){
			pi = model->ki;
		}
		break;
	case fromRightBottom:
		if (!arrival && agent[agent[agent_num].leader_id].position.first == x + 1 && agent[agent[agent_num].leader_id].position.second == y - 1){
			pi = model->ki;
		}
		if (arrival && agent[agent[agent_num].leader_id].escape_position.first == x + 1 && agent[agent[agent_num].leader_id].escape_position.second == y - 1){
			pi = model->ki;
		}
		break;
	case fromLeftBottom:
		if (!arrival && agent[agent[agent_num].leader_id].position.first == x - 1 && agent[agent[agent_num].leader_id].position.second == y - 1){
			pi = model->ki;
		}
		if (arrival && agent[agent[agent_num].leader_id].escape_position.first == x - 1 && agent[agent[agent_num].leader_id].escape_position.second == y - 1){
			pi = model->ki;
		}
		break;
	}
	return exp(pi);
}

float CS_CELLULAR_AUTOMATA::get_position_stress(pair<int, int> a_pos, pair<int, int> s_pos){

	float numerator;
	float denominator;
	float cov_determinant;
	float dim1_sd = 1.0f, dim2_sd = 1.0f, cov_value = 0.0f, x1, x2;
	cov_determinant = dim1_sd * dim2_sd - cov_value * cov_value;
	denominator = (float)sqrt(8 * M_PI * M_PI * M_PI * cov_determinant);
	x1 = float(a_pos.first - s_pos.first);
	x2 = float(a_pos.second - s_pos.second);
	cout << x1 << " " << x2 << endl;
	float term_1, term_2, term_3;
	term_1 = dim1_sd * x1 * x1;
	term_2 = dim2_sd * x2 * x2;
	term_3 = (cov_value + cov_value) * x1 * x2;
	cout << term_1 + term_2 + term_3 << endl;
	numerator = exp(-1 / 2 * (term_1 + term_2 + term_3));
	if (denominator == 0.0f)
		0.0f;
	/*if (x1 == 1){
	cout << "numerator: " << numerator << endl;
	cout << "denominator: " << denominator << endl;
	}*/
	return numerator / denominator;
}

float CS_CELLULAR_AUTOMATA::pair_int_distance(pair<int, int>x1, pair<int, int>x2){

	return (float)sqrt(pow(x1.first - x2.first, 2) + pow(x1.second - x2.second, 2));
}

float CS_CELLULAR_AUTOMATA::get_density_threshold(float willness){

	return ((willness - WILLNESS_THRESHOLD) / (1.0f - WILLNESS_THRESHOLD)) * (1.0f - DENSITY_THRESHOLD) + DENSITY_THRESHOLD;
}

float CS_CELLULAR_AUTOMATA::gaussianFunc(float x, float mu, float sigma2)
{
	float a = (float)sqrt(2.0 * M_PI);
	float b = (x - mu)*(x - mu);
	float c = (float)2.0 * sigma2;
	return float(1.0 / (sqrt(sigma2)*a)) * exp(-b / c);
}

float CS_CELLULAR_AUTOMATA::get_angle(pair<float, float> x1, pair<float, float> x2){

	float cos = (x1.first * x2.first + x1.second * x2.second) / (sqrt(x1.first * x1.first + x1.second*x1.second) * sqrt(x2.first*x2.first + x2.second * x2.second));
	return (float)acos(cos) * 180.0 / M_PI;
}

float CS_CELLULAR_AUTOMATA::decodingFloorFieldID(int Exit_block[]){

	int FloorFieldMapID = 0;
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		FloorFieldMapID += int(pow(2, i) * Exit_block[i]);
	}
	return (float)FloorFieldMapID;
}

bool CS_CELLULAR_AUTOMATA::isValid(int x, int y){

	if (x < 0 || y < 0 || x >= model->size || y >= model->size)
		return false;
	return true;
}

bool CS_CELLULAR_AUTOMATA::isGuiderValidArea(int o_id, int x, int z){

	float min_dis = FLT_MAX;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		if (pair_int_distance(obstacles[o_id].component[i], PAIR_INT(x, z)) <= GUIDE_DIS)
			return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::isBlock(int x, int z){

	if (cell[x][z].cell_type == 3)
		return true;
	if (!cell[x][z].obstacle)
		return false;
	int o_id = cell[x][z].obstacle_id;
	if (obstacles[o_id].block_exit)
		return false;
	return true;
}
// obstacle_o is not considered
bool CS_CELLULAR_AUTOMATA::isBlock(int x, int z, int o){

	int o_id = cell[x][z].obstacle_id;
	if (o == o_id)
		return true;
	if (!cell[x][z].obstacle)
		return false;
	if (obstacles[o_id].block_exit)
		return false;
	return true;
}
// check agent's move direction is opposite to leader's whether or not
bool CS_CELLULAR_AUTOMATA::opposite_direction(int agent_num, float distancetoleader){

	float up = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	if (isValid(agent[agent_num].position.first, agent[agent_num].position.second + 1))
	{
		up = distanceToLeader(agent[agent_num].position.first, agent[agent_num].position.second + 1, agent_num);
	}
	if (isValid(agent[agent_num].position.first, agent[agent_num].position.second - 1))
	{
		down = distanceToLeader(agent[agent_num].position.first, agent[agent_num].position.second - 1, agent_num);
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second))
	{
		left = distanceToLeader(agent[agent_num].position.first - 1, agent[agent_num].position.second, agent_num);
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second))
	{
		right = distanceToLeader(agent[agent_num].position.first + 1, agent[agent_num].position.second, agent_num);
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second + 1))
	{
		up = distanceToLeader(agent[agent_num].position.first + 1, agent[agent_num].position.second + 1, agent_num);
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second + 1))
	{
		down = distanceToLeader(agent[agent_num].position.first - 1, agent[agent_num].position.second + 1, agent_num);
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second - 1))
	{
		left = distanceToLeader(agent[agent_num].position.first + 1, agent[agent_num].position.second - 1, agent_num);
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second - 1))
	{
		right = distanceToLeader(agent[agent_num].position.first - 1, agent[agent_num].position.second - 1, agent_num);
	}
	float distance[8] = { up, down, left, right, topRight, topLeft, downRight, downLeft};
	sort(distance, distance + 8);

	for (int i = 0; i < 2; i++)
	{
		if (distancetoleader == distance[i])
			return false;
	}

	return true;
}
// check agent's move direction is opposite to obstacle's whether or not
bool CS_CELLULAR_AUTOMATA::opposite_direction_toObstacle(int agent_num, float distancetoobstacle){

	float up = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	if (isValid(agent[agent_num].position.first, agent[agent_num].position.second + 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			up = (float)sqrt(pow(agent[agent_num].position.first - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			up = (float)sqrt(pow(agent[agent_num].position.first - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first, agent[agent_num].position.second - 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			down = (float)sqrt(pow(agent[agent_num].position.first - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow((agent[agent_num].position.second - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			down = (float)sqrt(pow(agent[agent_num].position.first - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow((agent[agent_num].position.second - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second))
	{
		if (agent[agent_num].beside_obstacle)
		{
			left = (float)sqrt(pow((agent[agent_num].position.first - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow(agent[agent_num].position.second - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			left = (float)sqrt(pow((agent[agent_num].position.first - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow(agent[agent_num].position.second - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second))
	{
		if (agent[agent_num].beside_obstacle)
		{
			right = (float)sqrt(pow((agent[agent_num].position.first + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow(agent[agent_num].position.second - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			right = (float)sqrt(pow((agent[agent_num].position.first + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow(agent[agent_num].position.second - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second + 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			up = (float)sqrt(pow(agent[agent_num].position.first + 1 - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			up = (float)sqrt(pow(agent[agent_num].position.first + 1 - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second + 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			down = (float)sqrt(pow(agent[agent_num].position.first - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			down = (float)sqrt(pow(agent[agent_num].position.first - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow((agent[agent_num].position.second + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first + 1, agent[agent_num].position.second - 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			left = (float)sqrt(pow((agent[agent_num].position.first + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow(agent[agent_num].position.second - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			left = (float)sqrt(pow((agent[agent_num].position.first + 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow(agent[agent_num].position.second - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	if (isValid(agent[agent_num].position.first - 1, agent[agent_num].position.second - 1))
	{
		if (agent[agent_num].beside_obstacle)
		{
			right = (float)sqrt(pow((agent[agent_num].position.first - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
				+ pow(agent[agent_num].position.second - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
		}
		if (!agent[agent_num].beside_obstacle)
		{
			right = (float)sqrt(pow((agent[agent_num].position.first - 1) - removed_obstacles[agent[agent_num].push_obstacle_id].position.first, 2)
				+ pow(agent[agent_num].position.second - 1 - removed_obstacles[agent[agent_num].push_obstacle_id].position.second, 2));
		}
	}
	float distance[8] = { up, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(distance, distance + 8);
	for (int i = 0; i < 1; i++)
	{
		if (distancetoobstacle == distance[i])
			return false;
	}

	return true;
}
// check volunteer' move direction is opposite to obstacle's place whether or not
bool CS_CELLULAR_AUTOMATA::opposite_direction_toOb(int agent_id, float distanceToOb, int com_id, int o_id){

	float up = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	int x = agent[agent_id].position.first;
	int z = agent[agent_id].position.second;
	if (isValid(x, z + 1))
	{
		if ((1 - cell[x][z + 1].occupied) * cell[x][z + 1].obstacle_ == 1)
			up = (float)sqrt(pow(x - obstacles[o_id].component[com_id].first, 2) + pow(z + 1 - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x, z - 1))
	{
		if ((1 - cell[x][z - 1].occupied) * cell[x][z - 1].obstacle_ == 1)
			down = (float)sqrt(pow(x - obstacles[o_id].component[com_id].first, 2) + pow(z - 1 - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x - 1, z))
	{
		if ((1 - cell[x - 1][z].occupied) * cell[x - 1][z].obstacle_ == 1)
			left = (float)sqrt(pow(x - 1 - obstacles[o_id].component[com_id].first, 2) + pow(z - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x + 1, z))
	{
		if ((1 - cell[x + 1][z].occupied) * cell[x + 1][z].obstacle_ == 1)
			right = (float)sqrt(pow(x + 1 - obstacles[o_id].component[com_id].first, 2) + pow(z - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x + 1, z + 1))
	{
		if ((1 - cell[x + 1][z + 1].occupied) * cell[x + 1][z + 1].obstacle_ == 1)
			up = (float)sqrt(pow(x + 1 - obstacles[o_id].component[com_id].first, 2) + pow(z + 1 - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x - 1, z + 1))
	{
		if ((1 - cell[x - 1][z + 1].occupied) * cell[x - 1][z + 1].obstacle_ == 1)
			down = (float)sqrt(pow(x - 1 - obstacles[o_id].component[com_id].first, 2) + pow(z + 1 - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x + 1, z - 1))
	{
		if ((1 - cell[x + 1][z - 1].occupied) * cell[x + 1][z - 1].obstacle_ == 1)
			left = (float)sqrt(pow(x + 1 - obstacles[o_id].component[com_id].first, 2) + pow(z - 1 - obstacles[o_id].component[com_id].second, 2));
	}
	if (isValid(x - 1, z - 1))
	{
		if ((1 - cell[x - 1][z - 1].occupied) * cell[x - 1][z - 1].obstacle_ == 1)
			right = (float)sqrt(pow(x - 1 - obstacles[o_id].component[com_id].first, 2) + pow(z - 1 - obstacles[o_id].component[com_id].second, 2));
	}
	float distance[8] = { up, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(distance, distance + 8);
	for (int i = 0; i < 1; i++)
	{
		if (distanceToOb == distance[i])
			return false;
	}

	return true;
}

bool CS_CELLULAR_AUTOMATA::opposite_direction_toDestination(int id, float distanceToDestination){

	float up = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	int o_x, o_z, d_x, d_z;
	o_x = removed_obstacles[id].position.first;
	o_z = removed_obstacles[id].position.second;
	d_x = removed_obstacles[id].destination.first;
	d_z = removed_obstacles[id].destination.second;
	if (isValid(o_x, o_z + 1))
	{
		up = (float)sqrt(pow(o_x - d_x, 2) + pow(o_z + 1 - d_z, 2));
	}
	if (isValid(o_x, o_z - 1))
	{
		down = (float)sqrt(pow(o_x - d_x, 2) + pow(o_z - 1 - d_z, 2));
	}
	if (isValid(o_x - 1, o_z))
	{
		left = (float)sqrt(pow(o_x - 1 - d_x, 2) + pow(o_z - d_z, 2));
	}
	if (isValid(o_x + 1, o_z))
	{
		right = (float)sqrt(pow(o_x + 1 - d_x, 2) + pow(o_z - d_z, 2));
	}
	if (isValid(o_x + 1, o_z + 1))
	{
		up = (float)sqrt(pow(o_x + 1 - d_x, 2) + pow(o_z + 1 - d_z, 2));
	}
	if (isValid(o_x - 1, o_z + 1))
	{
		down = (float)sqrt(pow(o_x - 1 - d_x, 2) + pow(o_z + 1 - d_z, 2));
	}
	if (isValid(o_x + 1, o_z - 1))
	{
		left = (float)sqrt(pow(o_x + 1 - d_x, 2) + pow(o_z - 1 - d_z, 2));
	}
	if (isValid(o_x - 1, o_z - 1))
	{
		right = (float)sqrt(pow(o_x - 1 - d_x, 2) + pow(o_z - 1 - d_z, 2));
	}
	float distance[8] = { up, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(distance, distance + 8);
	for (int i = 0; i < 1; i++)
	{
		if (distanceToDestination == distance[i])
			return false;
	}

	return true;
}

bool CS_CELLULAR_AUTOMATA::check_member_arrival(int leader_ID){

	int group_ID = agent[leader_ID].group_id;

	for (unsigned int i = 1; i < agent_group[group_ID].member.size(); i++)
	{
		int agent_ID = agent_group[group_ID].member[i];
		/*
		if group didn't arrive to exit
		*/
		if (!agent[agent_ID].arrival)
			return false;
	}
	return true;
}

bool CS_CELLULAR_AUTOMATA::determine_leader_waiting(bool leader, bool member_leave){

	bool wait = false;
	float leader_waiting;
	leader_waiting = (float)rand() / RAND_MAX;
	if (leader_waiting < model->waiting)
		wait = true;
	if (leader && wait && !member_leave)
		return true;
	return false;
}

bool CS_CELLULAR_AUTOMATA::determine_leader_waiting(bool leader, int agent_id){

	if (!leader)
		return false;
	bool wait = false;
	float distance = 0.0f;
	int group_ID = agent[agent_id].group_id;
	for (unsigned int i = 0; i < agent_group[group_ID].member.size(); i++)
	{
		int member_ID = agent_group[group_ID].member[i];
		if (agent[member_ID].leader || agent[member_ID].arrival)
			continue;
		int x = agent[member_ID].position.first;
		int y = agent[member_ID].position.second;
		int leader_x = agent[agent_id].position.first;
		int leader_y = agent[agent_id].position.second;
		//cout << "member: " << cell[x][y].sFF << endl;
		//cout << "leader: " << cell[leader_x][leader_y].sFF << endl;
		
		if (sqrt(pow(x - leader_x, 2) + pow(y - leader_y, 2)) > distance && cell[x][y].sFF > cell[leader_x][leader_y].sFF)
		{
			distance = (float)sqrt(pow(x - leader_x, 2) + pow(y - leader_y, 2));
		}
	}
	if (distance > model->group_radius)
		return true;
	return false;
}

string CS_CELLULAR_AUTOMATA::get_remain_agent(){

	string s = to_string(model->remain_agent);
	return s;
}

string CS_CELLULAR_AUTOMATA::get_through_timeStep(){

	string s = to_string(mTimeCounter);
	return s;
}

string CS_CELLULAR_AUTOMATA::get_through_time(){

	string s = to_string(mTimeStep);
	return s;
}

int CS_CELLULAR_AUTOMATA::getExperimentType(){

	return model->mExperiment;
}

int CS_CELLULAR_AUTOMATA::getRunTimes(){

	return model->mRunTimes;
}

int CS_CELLULAR_AUTOMATA::getTimeCounter(){

	return mTimeCounter;
}

void CS_CELLULAR_AUTOMATA::set_corner_coordinate(int x, int y){

	screen.left_up_corner_x = x;
	screen.left_up_corner_y = y;
}

void CS_CELLULAR_AUTOMATA::set_cur_mouse_position(int mx, int mz){

}

void CS_CELLULAR_AUTOMATA::set_ColorTable_ID(int ID){

	cout << "set Color Table ID = " << ID << "..." << endl;
	model->colorTable_ID = ID;
}

void CS_CELLULAR_AUTOMATA::set_ColorTable_witoutExit_ID(int ID){

	cout << "set Color Table witout Exit ID = " << ID << "..." << endl;
	guiParameter.colorTable_withoutExit_ID = ID;
}

void CS_CELLULAR_AUTOMATA::set_WillingThreshold(float willing){

	cout << "set willing threshold = " << willing << endl;
	guiParameter.willing_threshold = willing;
}

void CS_CELLULAR_AUTOMATA::set_SpecialID(int ID){

	cout << "set Special Table ID = " << ID << endl;
	guiParameter.colorTable_special_ID = ID;
	//cout << "special map size = " <<  cell[0][0].special_sFF.size() << endl;
}

void CS_CELLULAR_AUTOMATA::set_colorTableNormalizeValue(float normalizeValue){

	cout << "set normalize value = " << normalizeValue << endl;
	guiParameter.specailColorTable_normalizeValue = normalizeValue;
}

void CS_CELLULAR_AUTOMATA::set_SameSeed(){

	model->mSameSeed = true;
}

void CS_CELLULAR_AUTOMATA::set_Bias(){
	
	agent_psychology.bias += 0.1f;
}

void CS_CELLULAR_AUTOMATA::set_TimeEffect(){

	agent_psychology.timeEffect += 40;
}

void CS_CELLULAR_AUTOMATA::set_Tendency(){

	agent_psychology.tendency += 0.2f;
}

void CS_CELLULAR_AUTOMATA::set_ObserveAgent(){

	for (unsigned int i = 0; i < model->agent_number; i++)
	{

	}
}

void CS_CELLULAR_AUTOMATA::get_agent_info(int mx, int mz){

	int size = model->size;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int x, z, next_x, next_z;
			x = (int)(i - size / 2) * SCALER - OFFSET;
			next_x = (int)(i + 1 - size / 2) * SCALER - OFFSET;
			z = (int)(j - size / 2) * SCALER - OFFSET;
			next_z = (int)(j + 1 - size / 2) * SCALER - OFFSET;
			//cout << "x: " << x << ", z: " << z << endl;
			//cout << "next_x: " << next_x << ", next_z: " << next_z << endl;
			if (mx >= x && mx < next_x && mz >= z && mz < next_z)
			{
				//cout << "in" << endl;
				get_individual_agent_info(i, j);
				break;
			}
		}
	}

	agent[model->select_agent_ID].cur_click = false;
}

void CS_CELLULAR_AUTOMATA::get_individual_agent_info(int x, int z){

	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].position.first == x && agent[i].position.second == z)
		{
			if (!agent[model->select_agent_ID].added)
				agent[model->select_agent_ID].click = false;
			agent[i].click = true;
			//agent[i].cur_click = true;
			string s = to_string(i);
			string group_id = to_string(agent[i].group_id);
			string group_size = to_string(agent[i].group_size);
			string escape_time = to_string(double(agent[i].escape_time - agent[i].start_time) / CLOCKS_PER_SEC);
			if (!agent[i].volunteer)
				s = "Agent ID: " + s + "\nGroup Size: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time;
			if (agent[i].volunteer)
			{
				if (agent[i].beside_obstacle)
					s = "Agent ID: " + s + "\nGroup Size: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nBeside Obstacle";
				if (!agent[i].beside_obstacle)
					s = "Agent ID: " + s + "\nGroup Size: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nNot Beside Obstacle";
			}
			model->agent_info = s;
			model->select_agent_ID = i;
			break;
		}
		else //record current mouse click position
		{
			//cout << x << " " << z << endl;
			if (isValid(x, z))
			{
				agent[model->select_agent_ID].click = false;
				cell[screen.cur_x][screen.cur_z].click = false;
				screen.cur_x = x;
				screen.cur_z = z;
				cell[screen.cur_x][screen.cur_z].click = true;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::show_agent_info(){

	glPushMatrix();
	glLoadIdentity();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glRasterPos2i(screen.left_up_corner_x, screen.left_up_corner_y);
	update_agent_info();
	if (model->mShowAgentInfo == 1)
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast<const unsigned char*>(model->agent_info.c_str()));
	glPopMatrix();
}

void CS_CELLULAR_AUTOMATA::add_agent_to_group(){

	cout << "add agent" << model->select_agent_ID << " to group" << endl << endl;
	bool exist = false;
	agent[model->select_agent_ID].added = true;
	for (unsigned int i = 0; i < temp_group.member.size(); i++)
	{
		int member_ID = temp_group.member[i];
		if (member_ID == model->select_agent_ID)
		{
			exist = true;
			break;
		}
	}
	if (!exist)
	{
		temp_group.member.push_back(model->select_agent_ID);
	}
	//cout << "group size: " << temp_group.member.size() << endl;
}

void CS_CELLULAR_AUTOMATA::remove_agent_from_group(){

	cout << "remove agent" << model->select_agent_ID << " from group" << endl << endl;
	agent[model->select_agent_ID].click = false;
	agent[model->select_agent_ID].added = false;
	for (unsigned int i = 0; i < temp_group.member.size(); i++)
	{
		int member_ID = temp_group.member[i];
		if (member_ID == model->select_agent_ID)
		{
			temp_group.member.erase(temp_group.member.begin() + i);
		}
	}
}

void CS_CELLULAR_AUTOMATA::add_new_group(){

	cout << "add new group" << endl << endl;
	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		agent[i].click = false;
		agent[i].added = false;
	}
	/*
	mark agents' original group
	*/
	for (unsigned int i = 0; i < temp_group.member.size(); i++)
	{
		int member_ID = temp_group.member[i];
		int group_ID = agent[member_ID].group_id;
		agent_group[group_ID].erase = true;
	}
	/*
	remove agent from its original group
	*/
	for (unsigned int i = 0; i < agent_group.size(); i++)
	{
		if (agent_group[i].erase)
			agent_group.erase(agent_group.begin() + i);
	}
	/*
	update group ID
	*/
	for (unsigned int i = 0; i < agent_group.size(); i++)
	{
		for (unsigned int j = 0; j < agent_group[i].member.size(); j++)
		{
			int agent_ID = agent_group[i].member[j];
			agent[agent_ID].group_id = i;
		}
	}
	/*
	find leader ID
	*/
	int leader_id;
	for (unsigned int i = 0; i < temp_group.member.size(); i++)
	{
		if (agent[temp_group.member[i]].leader)
		{
			leader_id = temp_group.member[i];
			break;
		}
	}
	/*
	collect agents to a group
	*/
	for (unsigned int i = 0; i < temp_group.member.size(); i++)
	{
		int member_ID = temp_group.member[i];
		agent[member_ID].group_size = temp_group.member.size();
		agent[member_ID].group_id = agent_group.size();
		agent[member_ID].leader_id = leader_id;
	}
	AGENT_GROUP group = temp_group;
	agent_group.push_back(group);
	temp_group.member.clear();
	model->group_count++;
	/*
	reset agent group's mark to false
	*/
	for (unsigned int i = 0; i < agent_group.size(); i++)
	{
		agent_group[i].erase = false;
	}
}

void CS_CELLULAR_AUTOMATA::add_stress_source(){

	cout << "add stress source in the scene." << endl;
	stress_area_position.push_back(pair<int, int>(screen.cur_x, screen.cur_z));
	//cout << screen.cur_x << " " << screen.cur_z << endl;
	//cout << stress_area_position.size() << endl;
}

void CS_CELLULAR_AUTOMATA::add_dynamic_stress_source(){

	cout << "add dynamic stress source int the scene." << endl << endl;
	dynamic_stress_area_position.push_back(pair<int, int>(screen.cur_x, screen.cur_z));
}

void CS_CELLULAR_AUTOMATA::set_obstacleMustRemove(){

	if (!isValid(screen.cur_x, screen.cur_z))
		return;
	// check the cell be clicked is obstacle or not
	if (cell[screen.cur_x][screen.cur_z].cell_type != 2)
		return;
	cout << "set removable obstacle." << endl << endl;
	/*REMOVED_OBSTACLE r_o;
	r_o.position = pair<int, int>(screen.cur_x, screen.cur_z);
	r_o.id = removed_obstacles.size();
	removed_obstacles.push_back(r_o);*/
	int o_id = cell[screen.cur_x][screen.cur_z].obstacle_id;
	if (o_id == -1)
		return;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		if (screen.cur_x == obstacles[o_id].component[i].first && screen.cur_z == obstacles[o_id].component[i].second)
			obstacles[o_id].final_clicked_o_id = i;
	}
	obstacles[o_id].arrive_destination = false;
	obstacles[o_id].enough_volunteers = false;
	obstacles[o_id].block = true;
	obstacles[o_id].block_exit = true;
	add_blocked_obstacle(o_id);
	//cout << o_id << endl;
	//blocked_obstacles.push_back(o_id);
}

void CS_CELLULAR_AUTOMATA::set_group_leader(){

	cout << "set agent" << model->select_agent_ID << " to group leader" << endl << endl;
	bool select = false;
	if (temp_group.member.size() == 1)
	{
		agent[model->select_agent_ID].leader = true;
		agent[model->select_agent_ID].group_size = 1;
	}
	if (temp_group.member.size() > 1)
	{
		for (unsigned int i = 0; i < temp_group.member.size(); i++)
		{
			int member_ID = temp_group.member[i];
			if (member_ID == model->select_agent_ID)
			{
				select = true;
				agent[model->select_agent_ID].leader = true;
			}
			else
			{
				agent[member_ID].leader = false;
			}
		}
		if (!select)
			temp_group.member.push_back(model->select_agent_ID);
	}
}

void CS_CELLULAR_AUTOMATA::set_guider(int guider_height){

	cout << "set agent" << model->select_agent_ID << " be a guider" << endl;
	cout << "agent height: " << guider_height << endl;
	agent[model->select_agent_ID].guider = true;
	agent[model->select_agent_ID].height = guider_height;
	model->guider_ID.push_back(model->select_agent_ID);
	agent[model->select_agent_ID].click = false;
	find_nearestExit(model->select_agent_ID);
}

void CS_CELLULAR_AUTOMATA::change_visible_state(){

	guiParameter.visible_count++;
	guiParameter.visible_count %= 2;
	switch (guiParameter.visible_count)
	{
	case 0:
		guiParameter.visibleState = person;
		break;
	case 1:
		guiParameter.visibleState = area;
		break;
	}
}

void CS_CELLULAR_AUTOMATA::choose_obstacle_destination(){

	/*if (removed_obstacles.size() == 0)
	{
	cout << "no obstacle need to be moved." << endl << endl;
	return;
	}*/
	if (blocked_obstacles.size() == 0)
	{
		cout << "no obstacle need to be moved." << endl << endl;
		return;
	}
	if (!isValid(screen.cur_x, screen.cur_z))
	{
		cout << "choosed place can't be a destination." << endl << endl;
		return;
	}
	int o_id = blocked_obstacles[blocked_obstacles.size() - 1];
	obstacles[o_id].volunteer_destination = pair<int, int>(screen.cur_x, screen.cur_z);
	pair<int, int> des = obstacles[o_id].volunteer_destination;
	int offset_x = obstacles[o_id].component[obstacles[o_id].clicked_o_id].first - des.first;
	int offset_z = obstacles[o_id].component[obstacles[o_id].clicked_o_id].second - des.second;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int x = obstacles[o_id].component[i].first;
		int z = obstacles[o_id].component[i].second;
		int des_x = x - offset_x;
		int des_z = z - offset_z;
		if (!isValid(des_x, des_z))
		{
			cout << "choosed place can't be a destination." << endl << endl;
			return;
		}
	}
	cout << "choose a destination for obstacle." << endl << endl;
	//removed_obstacles[removed_obstacles.size() - 1].destination = pair<int, int>(screen.cur_x, screen.cur_z);
	obstacles[o_id].final_clicked_o_id = obstacles[o_id].clicked_o_id;
	cell[screen.cur_x][screen.cur_z].click = false;
	obstacles[o_id].offset_x = (float)offset_x;
	obstacles[o_id].offset_z = (float)offset_z;
}

void CS_CELLULAR_AUTOMATA::choose_large_obstacle_detination(){

	if (blocked_obstacles.size() == 0)
	{
		cout << "no obstacle need to be moved." << endl << endl;
		return;
	}
	//for (int i = 0; i < )
}

void CS_CELLULAR_AUTOMATA::edit_obstacle(){

	//cout << "start editing the obstacles." << endl;
	if (editing_mode){
		if (!isValid(screen.cur_x, screen.cur_z))
		{
			cout << "ilegal cell!" << endl << endl;
			return;
		}
		// if cell be occupied by agent
		if (cell[screen.cur_x][screen.cur_z].occupied)
		{
			cout << "cell is occupied by agent!" << endl << endl;
			return;
		}
		if (cell[screen.cur_x][screen.cur_z].obstacle)
		{
			cout << "cancel obstacle (" << screen.cur_x << "," << screen.cur_z << ")." << endl << endl;
			cell[screen.cur_x][screen.cur_z].obstacle = false;
			cell[screen.cur_x][screen.cur_z].cell_type = 0;
			cell[screen.cur_x][screen.cur_z].obstacle_ = 1;
		}
		else if (!cell[screen.cur_x][screen.cur_z].obstacle)
		{
			cout << "add obstacle (" << screen.cur_x << "," << screen.cur_z << ")." << endl << endl;
			cell[screen.cur_x][screen.cur_z].obstacle = true;
			cell[screen.cur_x][screen.cur_z].cell_type = 2;
			cell[screen.cur_x][screen.cur_z].obstacle_ = 0;
		}
		compute_staticFF();
	}
}

void CS_CELLULAR_AUTOMATA::enabled_edit_obstacle(){

	//editing_mode = !editing_mode;
	//if (editing_mode)
	//	cout << "editing obstacle mode." << endl << endl;;
	guiParameter.mEditingMode = 2;
}

void CS_CELLULAR_AUTOMATA::enabled_edit_exit(){

	guiParameter.mEditingMode = 1;
}

void CS_CELLULAR_AUTOMATA::find_nearestExit(int agent_num){

	int exit_id;
	float e_dis = FLT_MAX;
	/*for (int i = 0; i < exits.size(); i++)
	{
		float distance = sqrt(pow(agent[model->select_agent_ID].position.first - exits[i].first, 2) + pow(agent[model->select_agent_ID].position.second - exits[i].second, 2));
		if (distance < e_dis)
		{
			e_dis = distance;
			exit_id = i;
		}
	}*/
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		for (unsigned int j = 0; j < mExit[i].width; j++)
		{
			int ex = mExit[i].position.first + mExit[i].direction.first*j;
			int ez = mExit[i].position.second + mExit[i].direction.second*j;
			float distance = (float)sqrt(pow(agent[model->select_agent_ID].position.first - ex, 2) + pow(agent[model->select_agent_ID].position.second - ez, 2));
			if (distance < e_dis)
			{
				e_dis = distance;
				exit_id = i;
			}
		}
	}
	agent[agent_num].guide_exit_id = exit_id;
}

void CS_CELLULAR_AUTOMATA::compute_guide_influence(int agent_num){

	int radius = agent[agent_num].height / 10;
	for (int x = -radius; x < radius; x++)
	{
		for (int z = -radius; z < radius; z++)
		{
			if (!isValid(agent[agent_num].position.first + x, agent[agent_num].position.second + z))
				continue;
			if (cell[agent[agent_num].position.first + x][agent[agent_num].position.second + z].cell_type == 2)
				continue;
			if (cell[agent[agent_num].position.first + x][agent[agent_num].position.second + z].occupied == 1)
			{
				int exit_id = agent[agent_num].guide_exit_id;
				int agent_id = cell[agent[agent_num].position.first + x][agent[agent_num].position.second + z].occupant_id;
				if (exits[exit_id].first < agent[agent_id].position.first) //exit at the right side
					agent[agent_id].right = ADDITION_FORCE;
				if (exits[exit_id].first > agent[agent_id].position.first) //exit at the left side
					agent[agent_id].left = ADDITION_FORCE;
				if (exits[exit_id].second < agent[agent_id].position.second) //exit at the top side
					agent[agent_id].top = ADDITION_FORCE;
				if (exits[exit_id].second > agent[agent_id].position.second) //exit at the down side
					agent[agent_id].down = ADDITION_FORCE;
				agent[agent_id].be_leaded = true;
				agent[agent_id].guide_exit_id = exit_id;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::reset_addition_force(){

	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].top = 1.0f;
		agent[i].down = 1.0f;
		agent[i].left = 1.0f;
		agent[i].right = 1.0f;
		agent[i].be_leaded = false;
	}
}

void CS_CELLULAR_AUTOMATA::reset_OccupiedAgentID(){

	
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].occupied_number = 0;
			for (int k = 0; k < 8; k++)
			{
				cell[i][j].m_OccupantID[k] = -1;
			}
			//cell[i][j].m_OccupantID.clear();
		}
	}
}

void CS_CELLULAR_AUTOMATA::reset_AgentStayInPlaceFlg(){

	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].waiting = false;
		agent[i].mStayInPlace = false;
	}
}

void CS_CELLULAR_AUTOMATA::reset_area_stress(){

	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].stressor.area_stress = 0;
	}
}

void CS_CELLULAR_AUTOMATA::translate_perceived_stress(){

	for (int i = 0; i < model->agent_number; i++)
	{
		// translate time stress
		agent[i].stressor.time_stress = stress_model.tranlateStress(agent[i].stressor.time_stress);
		// translate area stress
		agent[i].stressor.area_stress = stress_model.tranlateStress(agent[i].stressor.area_stress);
		// translate position stress
		agent[i].stressor.position_stress = stress_model.tranlateStress(agent[i].stressor.position_stress);
		// translate interpersonal stress
		agent[i].stressor.interpersonal_stress = stress_model.tranlateStress(agent[i].stressor.interpersonal_stress);
	}

	for (int i = 0; i < model->agent_number; i++)
	{
		//cout << agent[i].stressor.interpersonal_stress << endl;
		//cout << agent[i].stressor.area_stress << endl;
	}

}

void CS_CELLULAR_AUTOMATA::push_obstacle(int agent_num){

	if (!agent[agent_num].volunteer)
		return;
}
// check distance from agent to obstacle destination is closer to 
// distance from obstacle to obstacle destination whether or not
bool CS_CELLULAR_AUTOMATA::check_agentDistanceNearestThanObstacle(int removed_obstacle_id){

	float agent_distance;
	float obstacle_distance;
	agent_distance = (float)sqrt(pow(agent[removed_obstacles[removed_obstacle_id].volunteer_id].position.first - removed_obstacles[removed_obstacle_id].destination.first, 2)
		+ pow(agent[removed_obstacles[removed_obstacle_id].volunteer_id].position.second - removed_obstacles[removed_obstacle_id].destination.second, 2));
	obstacle_distance = (float)sqrt(pow(removed_obstacles[removed_obstacle_id].position.first - removed_obstacles[removed_obstacle_id].destination.first, 2)
		+ pow(removed_obstacles[removed_obstacle_id].position.second - removed_obstacles[removed_obstacle_id].destination.second, 2));
	if (agent_distance >= obstacle_distance)
		return false;
	return true;
}

bool CS_CELLULAR_AUTOMATA::check_occupiedAgent_sameTask(int x, int z, int o_id, int a_id){

	//int id = agent[cell[x][z].occupant_id].push_obstacle_id;
	int id = agent[cell[x][z].occupant_id].blocked_obstacle_id;
	if (id == -1)
		return false;
	for (unsigned int i = 0; i < obstacles[id].volunteer_id.size(); i++)
	{
		if (obstacles[id].volunteer_id[i] == -1)
			continue;
		if (obstacles[id].volunteer_id[i] == a_id)
			return true;
	}
	//cout << agent[cell[x][z].occupant_id].obstacle_component_id << endl;
	/*if (o_id == id)
	{
		return true;
	}*/
	/*if (!agent[id].beside_obstacle)
	{
		return false;
	}*/
	//cout << id << endl;
	//cout << o_id << endl;
	if (o_id == id)
	{
		return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::check_volunteer_setup(int o_id){

	for (unsigned int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
	{
		// obstacle not find a volunteer yet
		if (obstacles[o_id].volunteer_id[j] == -1)
		{
			return false;
		}
		// volunteer not beside the obstacle yet
		if (!agent[obstacles[o_id].volunteer_id[j]].beside_obstacle)
		{
			//cout << "volunteer" << obstacles[o_id].volunteer_id[j] << " not beside the obstacle." << endl << endl;
			return false;
		}
	}
	return true;
}

void CS_CELLULAR_AUTOMATA::check_sFF(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (cell[i][j].pre_sFF != cell[i][j].sFF)
			{
				change = true;
				break;
			}
		}
	}
	change = false;
}

void CS_CELLULAR_AUTOMATA::find_direction_towardToObstacleDestination(int o_id, Direction& d){

	float up = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	int x, z, des_x, des_z;
	pair<int, int> destination;
	obstacles[o_id].final_clicked_o_id = 0;
	switch (agent_psychology.obstacle_moveType)
	{
	case CS_OBSTACLE_MANUAL_MOVE:
		x = obstacles[o_id].component[obstacles[o_id].final_clicked_o_id].first;
		z = obstacles[o_id].component[obstacles[o_id].final_clicked_o_id].second;
		destination = obstacles[o_id].volunteer_destination;
		des_x = destination.first;
		des_z = destination.second;
		break;
	case CS_OBSTACLE_AUTOMATIC_MOVE:
		x = obstacles[o_id].component[0].first;
		z = obstacles[o_id].component[0].second;
		des_x = obstacles[o_id].moveDestination[0].first;
		des_z = obstacles[o_id].moveDestination[0].second;
		break;
	}
	if (isValid(x, z + 1))
	{
		up = (float)sqrt(pow(x - des_x, 2) + pow(z + 1 - des_z, 2));
	}
	if (isValid(x, z - 1))
	{
		down = (float)sqrt(pow(x - des_x, 2) + pow(z - 1 - des_z, 2));
	}
	if (isValid(x - 1, z))
	{
		left = (float)sqrt(pow(x - 1 - des_x, 2) + pow(z - des_z, 2));
	}
	if (isValid(x + 1, z))
	{
		right = (float)sqrt(pow(x + 1 - des_x, 2) + pow(z - des_z, 2));
	}
	if (model->mNeighborType != 0)
	{
		if (isValid(x + 1, z + 1))
		{
			topRight = (float)sqrt(pow(x + 1 - des_x, 2) + pow(z + 1 - des_z, 2));
		}
		if (isValid(x - 1, z + 1))
		{
			topLeft = (float)sqrt(pow(x - 1 - des_x, 2) + pow(z + 1 - des_z, 2));
		}
		if (isValid(x + 1, z - 1))
		{
			downRight = (float)sqrt(pow(x + 1 - des_x, 2) + pow(z - 1 - des_z, 2));
		}
		if (isValid(x - 1, z - 1))
		{
			downLeft = (float)sqrt(pow(x - 1 - des_x, 2) + pow(z - 1 - des_z, 2));
		}
	}
	float distance[8] = { up, down, left, right, topRight, topLeft, downRight, downLeft };
	/*for (int i = 0; i < 4; i++)
	{
		cout << distance[i] << " ";
	}
	cout << endl;*/
	sort(distance, distance + 8);
	/*for (int i = 0; i < 4; i++)
	{
		cout << distance[i] << " ";
	}
	cout << endl;*/
	for (int i = 0; i < 2; i++)
	{
		if (abs(up - distance[i]) < 0.000001f)
		{
			d = _up;
			obstacles[o_id].direction = PAIR_INT(0, 1);
			break;
		}
		else if (abs(down - distance[i]) < 0.000001f)
		{
			d = _down;
			obstacles[o_id].direction = PAIR_INT(0, -1);
			break;
		}
		else if (abs(left - distance[i]) < 0.000001f)
		{
			d = _left;
			obstacles[o_id].direction = PAIR_INT(-1, 0);
			break;
		}
		else if (abs(right - distance[i]) < 0.000001f)
		{
			d = _right;
			obstacles[o_id].direction = PAIR_INT(1, 0);
			break;
		}
		else if (abs(topRight - distance[i]) < 0.000001f)
		{
			d = _topRight;
			obstacles[o_id].direction = PAIR_INT(1, 1);
			break;
		}
		else if (abs(topLeft - distance[i]) < 0.000001f)
		{
			d = _topLeft;
			obstacles[o_id].direction = PAIR_INT(-1, 1);
			break;
		}
		else if (abs(downRight - distance[i]) < 0.000001f)
		{
			d = _downRight;
			obstacles[o_id].direction = PAIR_INT(1, -1);
			break;
		}
		else if (abs(downLeft - distance[i]) < 0.000001f)
		{
			d = _downLeft;
			obstacles[o_id].direction = PAIR_INT(-1, -1);
			break;
		}
		else
		{
			d = local;
			obstacles[o_id].direction = PAIR_INT(0, 0);
		}
	}
	return;
}

void CS_CELLULAR_AUTOMATA::disperse_crowd(int o_id, PAIR_INT direction){


}

void CS_CELLULAR_AUTOMATA::reset_agent_parameters(){

	//cout << "reset agent's parameters." << endl;
	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].blockTheRoad = false;
		agent[i].close_obstalce = false;
	}
}

void CS_CELLULAR_AUTOMATA::set_virtual_obstacle(int o_id){

	cout << "set obstacle_" << o_id << " virtual obstacle." << endl;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		// 8-neighbor
		for (int x = -1; x <= 1; x++)
		{
			for (int z = -1; z <= 1; z++)
			{
				if (!isValid(o_x + x, o_z + z))
					continue;
				if (cell[o_x + x][o_z + z].cell_type == 1)
					continue;
				if (!cell[o_x + x][o_z + z].obstacle)
				{
					cell[o_x + x][o_z + z].obstacle = true;
					cell[o_x + x][o_z + z].obstacle_ = 0;
					cell[o_x + x][o_z + z].cell_type = 2;
				}
			}
		}
	}
}

PAIR_INT CS_CELLULAR_AUTOMATA::agent_avoid_obstacle(int a_id){

	cout << "agent_" << a_id << " avoid the obstacle." << endl;
	int o_id = agent[a_id].close_obstacle_id;
	int com_id = agent[a_id].close_obstacle_com_id;
	float top = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, topRight = FLT_MAX, topLeft = FLT_MAX, downRight = FLT_MAX, downLeft = FLT_MAX;
	int x = agent[a_id].position.first;
	int z = agent[a_id].position.second;
	if (isValid(x, z + 1) && cell[x][z + 1].occupied == 0 && !cell[x][z + 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//top = pair_int_distance(PAIR_INT(x, z + 1), PAIR_INT(ob_x, ob_z));
		top = cell[x][z + 1].sFF;
	}
	if (isValid(x, z - 1) && cell[x][z - 1].occupied == 0 && !cell[x][z - 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//down = pair_int_distance(PAIR_INT(x, z - 1), PAIR_INT(ob_x, ob_z));
		down = cell[x][z - 1].sFF;
	}
	if (isValid(x - 1, z) && cell[x - 1][z].occupied == 0 && !cell[x - 1][z].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//left = pair_int_distance(PAIR_INT(x - 1, z), PAIR_INT(ob_x, ob_z));
		left = cell[x - 1][z].sFF;
	}
	if (isValid(x + 1, z) && cell[x + 1][z].occupied == 0 && !cell[x + 1][z].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//right = pair_int_distance(PAIR_INT(x + 1, z), PAIR_INT(ob_x, ob_z));
		right = cell[x + 1][z].sFF;
	}
	if (isValid(x + 1, z + 1) && cell[x + 1][z + 1].occupied == 0 && !cell[x + 1][z + 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//top = pair_int_distance(PAIR_INT(x, z + 1), PAIR_INT(ob_x, ob_z));
		top = cell[x + 1][z + 1].sFF;
	}
	if (isValid(x - 1, z + 1) && cell[x - 1][z + 1].occupied == 0 && !cell[x - 1][z + 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//down = pair_int_distance(PAIR_INT(x, z - 1), PAIR_INT(ob_x, ob_z));
		down = cell[x - 1][z + 1].sFF;
	}
	if (isValid(x + 1, z - 1) && cell[x + 1][z - 1].occupied == 0 && !cell[x + 1][z - 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//left = pair_int_distance(PAIR_INT(x - 1, z), PAIR_INT(ob_x, ob_z));
		left = cell[x + 1][z - 1].sFF;
	}
	if (isValid(x - 1, z - 1) && cell[x - 1][z - 1].occupied == 0 && !cell[x - 1][z - 1].obstacle)
	{
		int ob_x = obstacles[o_id].component[com_id].first;
		int ob_z = obstacles[o_id].component[com_id].second;
		//right = pair_int_distance(PAIR_INT(x + 1, z), PAIR_INT(ob_x, ob_z));
		right = cell[x - 1][z - 1].sFF;
	}
	PAIR_INT direction[8] = { PAIR_INT(x, z + 1), PAIR_INT(x, z - 1), PAIR_INT(x - 1, z), PAIR_INT(x + 1, z), PAIR_INT(x + 1, z + 1), PAIR_INT(x - 1, z + 1), PAIR_INT(x + 1, z - 1), PAIR_INT(x - 1, z - 1) };
	float buffer[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	float distance[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(buffer, buffer + 8);
	if (buffer[0] != FLT_MAX)
	{
		for (int i = 0; i < 8; i++)
		{
			if (buffer[0] == distance[i])
			{
				if (!isValid(direction[i].first, direction[i].second))
				{
					cell[x][z].temp_dFF += DYNAMIC_VALUE;
					return PAIR_INT(x, z);
				}
				cell[x][z].occupied = 0;
				cell[x][z].temp_dFF += DYNAMIC_VALUE;
				cell[direction[i].first][direction[i].second].occupied = 1;
				cell[direction[i].first][direction[i].second].occupant_id = a_id;
				cout << "x: " << direction[i].first << ", z: " << direction[i].second << endl;
				//system("pause");
				return PAIR_INT(direction[i].first, direction[i].second);
			}
		}
	}
	cell[x][z].temp_dFF += DYNAMIC_VALUE;
	return PAIR_INT(x, z);
}

void CS_CELLULAR_AUTOMATA::find_guider(int o_id){

	for (unsigned int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		for (unsigned int v = 0; v < leader_pool.size(); v++)
		{
			if (!agent[leader_pool[v]].strength)
				continue;
			if (agent[leader_pool[v]].volunteer)
				continue;
			if (agent[leader_pool[v]].arrival)
				continue;
			if (obstacles[o_id].volunteer_id[i] != -1)
			{
				// if new volunteer is more faraway to obstacle
				if (pair_int_distance(obstacles[o_id].component[i], agent[leader_pool[v]].position) > pair_int_distance(obstacles[o_id].component[i], agent[obstacles[o_id].volunteer_id[i]].position))
					continue;
				// reset original volunteer's parameter
				delete_volunteer(o_id, i, obstacles[o_id].volunteer_id[i]);
				cout << "leader_" << obstacles[o_id].volunteer_id[i] << " replace by agent_" << leader_pool[v] << endl << endl;
				add_volunteer(o_id, i, leader_pool[v]);
				cout << "leader_" << leader_pool[v] << " be a volunteer." << endl << endl;
				continue;
			}
			// if obstacle is too far to volunteer
			if (pair_int_distance(obstacles[o_id].component[i], agent[leader_pool[v]].position) > sqrt(2 * pow(VISIBILITY_DISTANCE, 2)))
				continue;
			// leader_pool[v] be a volunteer to move obstacle[o_id]
			add_volunteer(o_id, i, leader_pool[v]);
			cout << "leader_" << leader_pool[v] << " be a volunteer." << endl << endl;
		}
	}
	for (unsigned int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		int id = obstacles[o_id].volunteer_id[i];
		if (id == -1)
			continue;
		if (agent[id].leader)
		{
			obstacles[o_id].hasLeader_guider = true;
			break;
		}
	}
}

void CS_CELLULAR_AUTOMATA::reset_cellMark(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			cell[i][j].mark = false;
			cell[i][j].check = false;
			cell[i][j].aFF = 0;
		}
	}
}

void CS_CELLULAR_AUTOMATA::print_illegal_agent_information(string func_name){

	cout << func_name << " function." << endl;
	for (int x = 0; x < model->size; x++)
	{
		for (int z = 0; z < model->size; z++)
		{
			if (cell[x][z].occupied == 0)
				continue;
			int id = cell[x][z].occupant_id;
			if (id == -1)
				continue;
			if (agent[id].position.first != x || agent[id].position.second != z)
			{
				cout << "cell position: (" << x << ", " << z << ")" << endl;
				cout << "agent id: " << id << endl;
				cout << "agnet position: (" << agent[id].position.first << ", " << agent[id].position.second << ")" << endl;
				if (agent[id].volunteer)
					cout << "agent is volunteer." << endl;
				//system("pause");
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::agent_detect_obstalce(){

	for (int i = 0; i < model->agent_number; i++)
	{

	}
}

void CS_CELLULAR_AUTOMATA::setFlgExitColor(bool flg){

	guiParameter.drawExitColor = flg;
}

void CS_CELLULAR_AUTOMATA::gaussian_generate_personality(){

	for (int i = 0; i < model->agent_number * guiParameter.leader_proportion; i++)
	{
		int rnd = rand() % model->agent_number;
		while (agent[rnd].compressive_leader)
		{
			rnd = rand() % model->agent_number;
		}
		agent[rnd].compressive_leader = true;
		leader_pool.push_back(rnd);
	}
	// leader's personality distribution
	for (int ocean_factor = 0; ocean_factor < 5; ocean_factor++)
	{
		double mean, std;
		std = 0.1f;
		switch (ocean_factor)
		{
			//Openness
		case 0:
			mean = 0.0f;
			break;
			//Conscientiousness
		case 1:			
			mean = 0.6f;
			break;
			//Extraversion
		case 2:
			mean = 0.6f;
			break;
			//Agreeableness
		case 3:
			mean = 0.0f;
			break;
			//Neuroticism
		case 4:
			mean = 0.1f;
			break;
		}
		random_device rd;
		mt19937 gen = mt19937(rd());
		normal_distribution<double> d(mean, std);
		for (unsigned int i = 0; i < leader_pool.size(); i++)
		{
			int id = leader_pool[i];
			switch (ocean_factor)
			{
			case 0:
				agent[id].mPersonality.openness = (float)d(gen);
				break;
			case 1:
				agent[id].mPersonality.conscientiousness = (float)d(gen);
				break;
			case 2:
				agent[id].mPersonality.extraversion = (float)d(gen);
				break;
			case 3:
				agent[id].mPersonality.agreeableness = (float)d(gen);
				break;
			case 4:
				agent[id].mPersonality.neuroticism = (float)d(gen);
				break;
			}
		}
	}
	
	int begin;
	int total = 0;
	for (int size = 0; size < guiParameter.group_size; size++)
	{	
		begin = total;
		total = total + (model->agent_number / guiParameter.group_size);
		for (int ocean_factor = 0; ocean_factor < 5; ocean_factor++)
		{
			double mean, std;
			std = 0.1f;
			switch (ocean_factor)
			{
				//Openness
			case 0:
				mean = 0;
				break;
				//Conscientiousness
			case 1:
				mean = 0.2f;
				break;
				//Extraversion
			case 2:
				mean = 0.2f;
				break;
				//Agreeableness
			case 3:
				mean = 0;
				break;
				//Neuroticism
			case 4:
				mean = 0.6f;
				break;
			}
			random_device rd;
			mt19937 gen(rd());
			normal_distribution<double> d(mean, std);
			for (int i = begin; i < total; i++)
			{
				if (agent[i].compressive_leader)
					continue;
				switch (ocean_factor)
				{
				case 0:
					agent[i].mPersonality.openness = (float)d(gen);
					break;
				case 1:
					agent[i].mPersonality.conscientiousness = (float)d(gen);
					break;
				case 2:
					agent[i].mPersonality.extraversion = (float)d(gen);
					break;
				case 3:
					agent[i].mPersonality.agreeableness = (float)d(gen);
					break;
				case 4:
					agent[i].mPersonality.neuroticism = (float)d(gen);
					break;
				}
			}
		}
	}

	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].mPersonality.neuroticism < 0 || agent[i].mPersonality.neuroticism > 1.0f)
			agent[i].mPersonality.neuroticism = 0.01f;
	}
	for (int i = 0; i < model->agent_number; i++)
	{
		if (!agent[i].compressive_leader)
			continue;
		//cout << "(" << agent[i].mPersonality.openness << ", ";
		//cout << agent[i].mPersonality.conscientiousness << ", ";
		//cout << agent[i].mPersonality.extraversion << ", ";
		//cout << agent[i].mPersonality.agreeableness << ", ";
		//cout << agent[i].mPersonality.neuroticism << ")" << endl;
	}
	//cout << "=======================================================================" << endl;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].compressive_leader)
			continue;
		//cout << "(" << agent[i].mPersonality.openness << ", ";
		//cout << agent[i].mPersonality.conscientiousness << ", ";
		//cout << agent[i].mPersonality.extraversion << ", ";
		//cout << agent[i].mPersonality.agreeableness << ", ";
		//cout << agent[i].mPersonality.neuroticism << ")" << endl;
	}
	if (!model->mSameSeed)
		outputAgentPersonality();
	if (model->mSameSeed)
	{
		readAgentPersonality();
		return;
	}
}

void CS_CELLULAR_AUTOMATA::randomGenerateExitColor(){

	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		if (!mExit[i].active)
			continue;
		float r = 0, g = 0, b = 0;
		while (r + g + b < 0.3f)
		{
			r = (float)rand() / RAND_MAX;
			g = (float)rand() / RAND_MAX;
			b = (float)rand() / RAND_MAX;
		}
		//mExitColor.push_back(vector3(r, g, 0.0f));
		switch (i)
		{
		case 0:
			mExitColor.push_back(vector3(1, 0, 0));
			break;
		case 1:
			mExitColor.push_back(vector3(0, 1, 0));
			break;
		case 2:
			mExitColor.push_back(vector3(0, 0, 1));
			break;
		case 3:
			mExitColor.push_back(vector3(1, 1, 0));
			break;
		case 4:
			mExitColor.push_back(vector3(1, 0, 1));
			break;
		case 5:
			mExitColor.push_back(vector3(0, 1, 1));
			break;
		}
	}
}

void CS_CELLULAR_AUTOMATA::detectEditingMode(){

	switch (guiParameter.mEditingMode)
	{
	case 0:
		break;
	case 1:
		editExit();
		break;
	case 2:
		editObstacle();
		break;
	case 3:
		editAgent();
		break;
	}
}

void CS_CELLULAR_AUTOMATA::editExit(){

	cout << screen.cur_x << ", " << screen.cur_z << endl;
	if (!isValid(screen.cur_x, screen.cur_z))
		return;
	if (cell[screen.cur_x][screen.cur_z].cell_type == 1)
	{
		cout << "delete exit." << endl;
		mEditingTools.mNeedChooseAnotherCell = false;
		int eid = cell[screen.cur_x][screen.cur_z].exit_id;
		//mExit[eid].active = false;
		int x = mExit[eid].position.first;
		int z = mExit[eid].position.second;
		int dx = mExit[eid].direction.first;
		int dz = mExit[eid].direction.second;
		for (int i = 0; i < mExit[eid].width; i++)
		{
			int ex = x + dx*i;
			int ez = x + dx*z;
			cell[ex][ez].cell_type = -1;
			cell[ex][ez].exit_id = -1;
		}
		mExit.erase(mExit.begin() + eid);
		regenerate_ExitID();
	}
	if (cell[screen.cur_x][screen.cur_z].cell_type == -1 && mEditingTools.mNeedChooseAnotherCell)
	{
		int x = screen.cur_x;
		int z = screen.cur_z;
		int ex = mEditingTools.mTempExit.position.first;
		int ez = mEditingTools.mTempExit.position.second;
		if (x - ex != 0 && z - ez != 0)
		{
			cout << "can't choose this cell!" << endl;
			return;
		}
		switch (x - ex)
		{
		case 0:
			mEditingTools.mTempExit.width = abs(z - ez) + 1;
			if (z >= ez)
				mEditingTools.mTempExit.direction = PAIR_INT(0, 1);
			else
				mEditingTools.mTempExit.direction = PAIR_INT(0, -1);
			break;
		default:
			mEditingTools.mTempExit.width = abs(x - ex) + 1;
			if (x >= ex)
				mEditingTools.mTempExit.direction = PAIR_INT(1, 0);
			else
				mEditingTools.mTempExit.direction = PAIR_INT(-1, 0);
			break;
		}
		collectExit();
	}
	else if(cell[screen.cur_x][screen.cur_z].cell_type == -1 && !mEditingTools.mNeedChooseAnotherCell)
	{
		cout << "choose " << screen.cur_x << ", " << screen.cur_z << " as exit" << endl;
		mEditingTools.mNeedChooseAnotherCell = true;
		mEditingTools.mTempExit.position = PAIR_INT(screen.cur_x, screen.cur_z);
	}
}

void CS_CELLULAR_AUTOMATA::editObstacle(){

	int x = screen.cur_x;
	int z = screen.cur_z;
	if (cell[x][z].cell_type == 2)
	{
		cout << "delete obstacle." << endl;
		mEditingTools.mNeedChooseAnotherCell = false;
		int oid = cell[screen.cur_x][screen.cur_z].obstacle_id;
		//mExit[eid].active = false;
		for (unsigned int i = 0; i < obstacles[oid].component.size(); i++)
		{
			int x = obstacles[oid].component[i].first;
			int z = obstacles[oid].component[i].second;
			cell[x][z].obstacle = false;
			cell[x][z].cell_type = -1;
			cell[x][z].obstacle_ = 1;
			cell[x][z].obstacle_id = -1;
		}
		obstacles.erase(obstacles.begin() + oid);
		regenerate_ObstacleID();
	}
	if (cell[x][z].cell_type == -1 && mEditingTools.mNeedChooseAnotherCell)
	{
		int ox = mEditingTools.mTempObstacle.component[0].first;
		int oz = mEditingTools.mTempObstacle.component[0].second;
		if (x - ox != 0 && z - oz != 0)
		{
			cout << "can't choose this cell!" << endl;
			return;
		}
		switch (x - ox)
		{
		case 0:
			mEditingTools.mTempObstacle.component.resize(abs(z - oz) + 1);
			mEditingTools.mTempObstacle.highest_x = x;
			mEditingTools.mTempObstacle.lowest_x = x;
			for (int i = 0; i < abs(z - oz) + 1; i++)
			{
				mEditingTools.mTempObstacle.component[i].first = ox;
				if (z >= oz)
					mEditingTools.mTempObstacle.component[i].second = oz + i;
				else
					mEditingTools.mTempObstacle.component[i].second = oz - i;
			}
			if (z >= oz)
			{
				mEditingTools.mTempObstacle.highest_z = z;
				mEditingTools.mTempObstacle.lowest_z = oz;
			}
			else
			{
				mEditingTools.mTempObstacle.highest_z = oz;
				mEditingTools.mTempObstacle.lowest_z = z;
			}
			break;
		default:
			mEditingTools.mTempObstacle.component.resize(abs(x - ox) + 1);
			mEditingTools.mTempObstacle.highest_z = z;
			mEditingTools.mTempObstacle.lowest_z = z;
			for (int i = 0; i < abs(x - ox) + 1; i++)
			{
				if (x >= ox)
					mEditingTools.mTempObstacle.component[i].first = ox + i;
				else
					mEditingTools.mTempObstacle.component[i].first = ox - i;
				mEditingTools.mTempObstacle.component[i].second = oz;
			}
			if (x >= ox)
			{
				mEditingTools.mTempObstacle.highest_x = x;
				mEditingTools.mTempObstacle.lowest_x = ox;
			}
			else
			{
				mEditingTools.mTempObstacle.highest_x = ox;
				mEditingTools.mTempObstacle.lowest_x = x;
			}
			break;
		}
		collectObstacle();
	}
	if (cell[x][z].cell_type == -1 && !mEditingTools.mNeedChooseAnotherCell)
	{
		cout << "choose " << screen.cur_x << ", " << screen.cur_z << " as obstacle" << endl;
		mEditingTools.mNeedChooseAnotherCell = true;
		mEditingTools.mTempObstacle.component.clear();
		mEditingTools.mTempObstacle.component.push_back(PAIR_INT(x, z));
		//mEditingTools.mTempObstacle.
	}
}

void CS_CELLULAR_AUTOMATA::editAgent(){

	int x = screen.cur_x;
	int z = screen.cur_z;
	if (cell[x][z].cell_type == 0)
	{

	}
	if (cell[x][z].cell_type == -1)
	{
		//model->agent_number++;
		//cell[x][z].cell_type = 0;
	}
}

void CS_CELLULAR_AUTOMATA::collectExit(){

	int x = mEditingTools.mTempExit.position.first;
	int z = mEditingTools.mTempExit.position.second;
	int dx = mEditingTools.mTempExit.direction.first;
	int dz = mEditingTools.mTempExit.direction.second;
	int width = mEditingTools.mTempExit.width;
	for (int i = 0; i < width; i++)
	{
		int ex = x + dx*i;
		int ez = z + dz*i;
		cell[ex][ez].cell_type = 1;
		cell[ex][ez].exit_id = mExit.size();
	}
	Exit e = mEditingTools.mTempExit;
	mExit.push_back(e);
	mEditingTools.mNeedChooseAnotherCell = false;
	cout << "collect exit" << endl;
}

void CS_CELLULAR_AUTOMATA::collectObstacle(){

	vector<pair<int, int>> obstacle_component(mEditingTools.mTempObstacle.component.size());
	vector<int> volunteer_id(mEditingTools.mTempObstacle.component.size());
	vector<vector<int>> candidate_id(mEditingTools.mTempObstacle.component.size());
	OBSTACLE o;
	o.block_exit = false;
	for (unsigned int i = 0; i < mEditingTools.mTempObstacle.component.size(); i++)
	{
		int x = mEditingTools.mTempObstacle.component[i].first;
		int z = mEditingTools.mTempObstacle.component[i].second;
		cell[x][z].obstacle = true;
		cell[x][z].cell_type = 2;
		cell[x][z].obstacle_ = 0;
		cell[x][z].obstacle_id = obstacles.size();
		obstacle_component[i] = PAIR_INT(x, z);
		volunteer_id[i] = -1;
	}
	float minx = (float)mEditingTools.mTempObstacle.lowest_x;
	float maxx = (float)mEditingTools.mTempObstacle.highest_x;
	float minz = (float)mEditingTools.mTempObstacle.lowest_z;
	float maxz = (float)mEditingTools.mTempObstacle.highest_z;
	if (minx == maxx && minx == 0)
	{
		o.block_direction = 3;
	}
	if (minx == maxx && minx == model->size - 1)
	{
		o.block_direction = 1;
	}
	if (minz == maxz && minz == 0)
	{
		o.block_direction = 2;
	}
	if (minz == maxz && minz == model->size - 1)
	{
		o.block_direction = 0;
	}
	if (minx == maxx)
		o.type = 0;
	if (minz == maxz)
		o.type = 1;
	o.id = obstacles.size();
	o.size = mEditingTools.mTempObstacle.component.size();
	o.highest_z = (int)maxz;
	o.highest_x = (int)maxx;
	o.lowest_x = (int)minx;
	o.lowest_z = (int)minz;
	o.component = obstacle_component;
	o.volunteer_id = volunteer_id;
	o.candidate_id = candidate_id;
	obstacles.push_back(o);
	mEditingTools.mNeedChooseAnotherCell = false;
	cout << "collect obstacle" << endl;
}

void CS_CELLULAR_AUTOMATA::regenerate_ExitID(){

	cout << "mExit size: " << mExit.size() << endl;
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		cout << "mExit_" << i << " x: " << x << " z: " << z << endl;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			int ex = x + dx*j;
			int ez = z + dz*j;
			cell[ex][ez].cell_type = 1;
			cell[ex][ez].exit_id = i;
		}
	}
}

void CS_CELLULAR_AUTOMATA::regenerate_ObstacleID(){
	
	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].id = i;
		for (unsigned int j = 0; j < obstacles[i].component.size(); j++)
		{
			int x = obstacles[i].component[j].first;
			int z = obstacles[i].component[j].second;
			cell[x][z].obstacle = true;
			cell[x][z].cell_type = 2;
			cell[x][z].obstacle_ = 0;
			cell[x][z].obstacle_id = i;
		}
	}
}

void CS_CELLULAR_AUTOMATA::recordingEvacuationTime(){

	cout << "recording evacuation time." << endl;
	model->seed = (unsigned)time(NULL);
	while (model->remain_agent != 0)
	{

	}
	model->remain_agent = model->agent_number;
}

void CS_CELLULAR_AUTOMATA::saveImage(){

	
}

void CS_CELLULAR_AUTOMATA::generateRandomAgentOrder(){

	//if (mTimeCounter > 300)
	//{
		//cout << "error" << endl;
		//cout << model->remain_agent << endl;
		//cout << blocked_obstacles.size() << endl;
		//system("pause");
		//reset();
	//}
	vector<int> idBuffer;
	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		idBuffer.push_back(i);
	}
	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		int index = rand() % idBuffer.size();
		mRandomAgentOrder[i] = idBuffer[index];
		idBuffer.erase(idBuffer.begin() + index);
	}
	vector<int>().swap(idBuffer);
}

void CS_CELLULAR_AUTOMATA::assignColorProportion(){

	mColorProportion.resize(4);
	float a = 0, b = 0, c = 0, d = 0;
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (cell[i][j].sFF < 25)
				a++;
			if (cell[i][j].sFF >= 25 && cell[i][j].sFF < 50)
				b++;
			if (cell[i][j].sFF >= 50 && cell[i][j].sFF < 75)
				c++;
			if (cell[i][j].sFF >= 75)
				d++;
		}
	}
	a = a / (model->size * model->size);
	b = b / (model->size * model->size);
	c = c / (model->size * model->size);
	d = d / (model->size * model->size);
	//cout << a << endl << b << endl << c << endl << d << endl;
	float aa[4] = { a, b, c, d };
	sort(aa, aa + 4);
	//cout << aa[0] << endl << aa[1] << endl << aa[2] << endl << aa[3] << endl;
	for (int i = 0; i < 4; i++)
	{
		if (a == aa[i])
		{
			mColorProportion[0] = aa[3 - i];
		}
		if (b == aa[i])
		{
			mColorProportion[1] = aa[3 - i];
		}
		if (c == aa[i])
		{
			mColorProportion[2] = aa[3 - i];
		}
		if (d == aa[i])
		{
			mColorProportion[3] = aa[3 - i];
		}
	}
	cout << a << endl << b << endl << c << endl << d << endl;
	cout << mColorProportion[0] << endl << mColorProportion[1] << endl << mColorProportion[2] << endl << mColorProportion[3] << endl;
}

void CS_CELLULAR_AUTOMATA::printDebugInformation(){

	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			continue;
		if (i > 10)
			continue;
		/*if (agent[i].compressive_leader)
			cout << "Leader" << endl;
		else
			cout << "Member" << endl;
		for (unsigned int j = 0; j < agent[i].visible_agentID.size(); j++)
		{
			cout << getContagionStrength(i, agent[i].visible_agentID[j]) << endl;
		}
		cout << "--------------------------------------------------" << endl;*/
	}
}

void CS_CELLULAR_AUTOMATA::changeNeighborType(){

	model->mNeighborType = !model->mNeighborType;
	compute_staticFF();
	compute_clean_sFF();
	for (int j = 0; j < (int)obstacles.size(); j++)
	{
		for (int k = 0; k < (int)obstacles[j].component.size(); k++)
		{
			compute_volunteer_staticFF(j, k);
		}
	}
	int *exit_block = new int[mExit.size()];
	if (obstacles.size() > 0)
	{
		for (int j = 0; j < pow(2, mExit.size()); j++)
		{
			int value = j;
			for (int k = mExit.size() - 1; k >= 0; k--)
			{
				if (value >= pow(2, k))
				{
					value -= (int)pow(2, k);
					exit_block[k] = 1;
				}
				else
				{
					exit_block[k] = 0;
				}
			}
			computeNewMapWithoutExit(exit_block);
		}
	}
}