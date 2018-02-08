#include"cellular_automata.h"
//#include<boost/random/mersenne_twister.hpp>
//#include<boost/random/normal_distribution.hpp>

using namespace std;

void CS_CELLULAR_AUTOMATA::update_dynamicFF(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			float top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f;
			if (isValid(i, j + 1))
				top = cell[i][j + 1].temp_dFF;
			if (isValid(i, j - 1))
				down = cell[i][j - 1].temp_dFF;
			if (isValid(i - 1, j))
				left = cell[i - 1][j].temp_dFF;
			if (isValid(i + 1, j))
				right = cell[i + 1][j].temp_dFF;
			cell[i][j].dFF = (1 - model->diffusion) * (1 - model->decay) * cell[i][j].temp_dFF + (model->diffusion * ((1 - model->decay) / 4)) * (top + down + left + right);
		}
	}
	// scale the dynamic floor field value to 0~13
	scale_dFF();
	for (int i = 0; i < model->agent_number; i++)
	{
		for (int j = 0; j < model->agent_number; j++)
		{
			if (i == j)
				continue;
			if (agent[i].arrival)
				continue;
			if (agent[i].position == agent[j].position)
			{
				//if (agent[i].volunteer)
					//cout << "volunteer" << ", obstacle: " << agent[i].blocked_obstacle_id << ", com: " << agent[i].obstacle_component_id << endl;
				//cout << "id: " << i << ", id: " << j << " position: " << agent[i].position.first << " " << agent[i].position.second << endl;
				//system("pause");
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_pause_time(){

	if (model->start){
		model->pause_start_time = clock();
		model->pause_end_time = clock();
	}
	if (!model->start){
		model->pause_end_time = clock();
		model->pause_through_time = double(model->pause_end_time - model->pause_start_time);
	}
	//cout << model->total_pause_through_time << endl;
}

void CS_CELLULAR_AUTOMATA::update_distanceToLeader(){

	for (int i = 0; i < model->agent_number; i++){
		if (!agent[i].leader){
			int x = agent[i].position.first;
			int z = agent[i].position.second;
			int leader_ID = agent[i].leader_id;
			agent[i].leader_distance = distanceToLeader(x, z, leader_ID);
		}
	}

	/*float max = -FLT_MAX;

	for (int i = 0; i < model->agent_number; i++){
	if (!agent[i].leader){
	if (agent[i].leader_distance > max)
	max = agent[i].leader_distance;
	}
	}*/

	for (int i = 0; i < model->agent_number; i++)
	{
		if (!agent[i].leader)
		{
			if (agent[i].leader_distance > model->group_radius)
				agent[i].leader_distance = (float)model->group_radius;
			if (!agent[i].leader_distance > model->group_radius)
				agent[i].leader_distance = agent[i].leader_distance / model->group_radius;
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_agent_position(){

	//check_task_finish();
	check_arrival();
	reset_addition_force();
	//obstacle_findNearestAgent();
	//blocked_obstacle_findVolunteer();
	// update volunteer's position first
	switch (model->mParallelUpdate)
	{
	case 0:
		update_AgentPositionDisordered();
		break;
	case 1:
		update_AgentPositionParallel();
		break;
	}
	/*for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			continue;
		if (agent[i].position.first < 0 || agent[i].position.first >= model->size || agent[i].position.second < 0 || agent[i].position.second >= model->size)
		{
			cout << "agent_" << i << " 's position is illegal." << endl;
			cout << "position: (" << agent[i].position.first << ", " << agent[i].position.second << ")." << endl;
			system("pause");
		}
	}*/
	/*for (int i = 0; i < model->agent_number; i++)
	{
		cout << "agent_" + i << ": (" << agent[i].position.first << ", " << agent[i].position.second << ")." << endl;
	}*/
	/*if (model->remain_agent == 0)
	{
		vector<int> time;
		vector<int> amount;
		time.resize(mExit.size()); 
		amount.resize(mExit.size());
		for (int i = 0; i < model->agent_number; i++)
		{
			int eID = agent[i].mEscapeExit;
			time[eID] += agent[i].escapeTime;
			amount[eID]++;
		}
		for (int i = 0; i < time.size(); i++)
		{
			//cout << "mExit_" << i << ": " << (float)time[i] / amount[i] << endl;
			cout << "mExit_" << i << ": " << (float)amount[i] / mExit[i].width << endl;
		}
	}*/
	if (model->remain_agent == 0 && !model->out /*&& model->mNewSeed == 1*/)
	{
		model->out = true;
		outputInformation();
	}
	if (model->remain_agent == 0 && !model->out_anxiety)
	{
		model->out_anxiety = true;
		outputAnxietyVariation();
		outputStatisticInfo();
		outputMaxDFF();
		outputStatisticsOnAnxiety_LeaderAndMember();
		outputVolunteerEvacuation();
	}
}

void CS_CELLULAR_AUTOMATA::update_AgentPositionDisordered(){
	
	for (unsigned int index = 0; index < mRandomAgentOrder.size(); index++)
	{
		int i = mRandomAgentOrder[index];
		// if agent has to push obstacle
		if (agent[i].push_obstacle_id != -1)
			continue;
		// if agent block the obstacle's road
		if (agent[i].blockTheRoad)
			continue;
		if (agent[i].volunteer)
			continue;
		if (agent[i].bystander)
			continue;
		if (agent[i].arrival == false){
			agent[i].position = choose_direction(agent[i].position.first, agent[i].position.second, i);
		}
		else{
			if (isValid(agent[i].position.first, agent[i].position.second))
			{
				cell[agent[i].position.first][agent[i].position.second].occupant_id = -1;
				cell[agent[i].position.first][agent[i].position.second].occupied = 0;
			}
			PAIR_INT temp_pos = PAIR_INT(-1, -1);
			agent[i].position = temp_pos;
			agent[i].arrival = true;
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_AgentPositionParallel(){

	reset_AgentStayInPlaceFlg();
	reset_OccupiedAgentID();
	update_Cell_AgentWantOccpied();
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			bool s = true;
			for (int k = 0; k < 4; k++)
			{
				if (cell[i][j].m_OccupantID[k] != -1)
				{
					s = false;
					break;
				}
				//cout << cell[i][j].m_OccupantID[k] << " ";
			}
			//cout << endl;
			if (s && cell[i][j].occupied_number >= 1)
			{
				//cout << i << " " << j << endl;
				for (int k = 0; k < 4; k++)
				{
					//cout << cell[i][j].m_OccupantID[k] << " ";
				}
				//cout << endl;
				cell[i][j].occupied_number = 0;
				//cout << cell[i][j].occupied_number << endl;
			}
		}
	}
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			//if (cell[i][j].m_OccupantID.size() == 0)
			//	continue;
			if (cell[i][j].occupied_number == 0)
				continue;
			if (cell[i][j].occupied_number > 4)
			{
				//cout << "ff" << endl;
				//system("pause");
			}
			double p = (double)rand() / RAND_MAX;
			int id, x, z;
			// if the cell has more than one pedestrian want to occupy 
			if (p < model->mFriction /*&& cell[i][j].m_OccupantID.size() > 1*/)
			{
				cell[i][j].occupant_id = -1;
				cell[i][j].occupied = 0;
				for (int k = 0; k < (int)cell[i][j].m_OccupantID.size(); k++)
				{
					id = cell[i][j].m_OccupantID[k];
					if (id == -1)
						continue;
					x = agent[id].position.first;
					z = agent[id].position.second;
					agent[id].position = PAIR_INT(x, z);
					agent[id].direction = local;
					agent[id].pre_sFF = cell[x][z].sFF;
					cell[x][z].occupant_id = id;
					cell[x][z].occupied = 1;
					cell[x][z].temp_dFF += DYNAMIC_VALUE;
				}
				continue;
			}
			// random choose a agent to occupy the cell
			int index = rand() % cell[i][j].occupied_number;
			//int index = rand() % cell[i][j].m_OccupantID.size();
			id = cell[i][j].m_OccupantID[index];
			x = agent[id].position.first;
			z = agent[id].position.second;
			agent[id].position = PAIR_INT(i, j);
			agent[id].pre_sFF = cell[i][j].sFF;
			//cout << i << " " << j << endl;
			//cout << cell[i][j].occupied_number << endl;
			//cout << id << endl;
			cell[x][z].occupant_id = -1;
			cell[x][z].occupied = 0;
			cell[i][j].occupant_id = id;
			cell[i][j].occupied = 1;
			cell[i][j].temp_dFF += DYNAMIC_VALUE;
			if (i > x && j == z)//right
			{
				agent[id].direction = _right;
				agent[id].inertia = fromLeft;
			}
			else if (i < x && j == z)//left
			{
				agent[id].direction = _left;
				agent[id].inertia = fromRight;
			}
			else if (i == x && j > z)//up
			{
				agent[id].direction = _up;
				agent[id].inertia = fromBottom;
			}
			else if (i == x && j < z)//down
			{
				agent[id].direction = _down;
				agent[id].inertia = fromTop;
			}
			for (int k = 0; k < (int)cell[i][j].m_OccupantID.size(); k++)
			{
				if (k == index)
					continue;
				id = cell[i][j].m_OccupantID[k];
				if (id == -1)
					continue;
				x = agent[id].position.first;
				z = agent[id].position.second;
				agent[id].position = PAIR_INT(x, z);
				agent[id].direction = local;
				agent[id].pre_sFF = cell[x][z].sFF;
				cell[x][z].occupant_id = id;
				cell[x][z].occupied = 1;
				cell[x][z].temp_dFF += DYNAMIC_VALUE;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_Cell_AgentWantOccpied(){

	for (int index = 0; index < model->agent_number; index++)
	{
		int i = mRandomAgentOrder[index];
		if (agent[i].push_obstacle_id != -1)
			continue;
		if (agent[i].blockTheRoad)
			continue;
		if (agent[i].volunteer)
			continue;
		if (agent[i].bystander)
			continue;
		if (agent[i].arrival)
			continue;
		double top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f, total;
		int x = agent[i].position.first;
		int z = agent[i].position.second;
		if (!agent[i].leader)
		{
			//cout << "inintitnint" << endl;
			if (isValid(x, z + 1))
				top = get_probability(x, z + 1, i) * get_leader_inertia(x, z + 1, i);
			if (isValid(x, z - 1))
				down = get_probability(x, z - 1, i) * get_leader_inertia(x, z - 1, i);
			if (isValid(x - 1, z))
				left = get_probability(x - 1, z, i) * get_leader_inertia(x - 1, z, i);
			if (isValid(x + 1, z))
				right = get_probability(x + 1, z, i) * get_leader_inertia(x + 1, z, i);
		}
		// if agent is a leader
		if (agent[i].leader)
		{
			if (isValid(x, z + 1))
				top = get_probability(x, z + 1, i) * get_inertia(x, z + 1, i); //* agent[i].top;
			if (isValid(x, z - 1))
				down = get_probability(x, z - 1, i) * get_inertia(x, z - 1, i); //* agent[i].down;
			if (isValid(x - 1, z))
				left = get_probability(x - 1, z, i) * get_inertia(x - 1, z, i); //* agent[i].left;
			if (isValid(x + 1, z))
				right = get_probability(x + 1, z, i) * get_inertia(x + 1, z, i); //* agent[i].right;
			if (model->remain_agent == 1)
			{
				//cout << top << " " << cell[x][z + 1].dFF << " " << cell[x][z + 1].sFF << endl;
				//cout << down << " " << cell[x][z - 1].dFF << " " << cell[x][z - 1].sFF << endl;
				//cout << left << " " << cell[x - 1][z].dFF << " " << cell[x - 1][z].sFF << endl;
				//cout << right << " " << cell[x + 1][z].dFF << " " << cell[x + 1][z].sFF << endl;
			}
		}

		total = top + down + left + right;
		/*if (determine_leader_waiting(agent[i].leader, i))
		{
			agent[i].waiting = true;
			//cell[x][z].m_OccupantID.push_back(i);
			agent[i].position = PAIR_INT(x, z);
			agent[i].direction = local;
			agent[i].pre_sFF = cell[x][z].sFF;
			cell[x][z].occupant_id = i;
			cell[x][z].occupied = 1;
			cell[x][z].temp_dFF += DYNAMIC_VALUE;
		}*/
		if (total == 0)
		{
			agent[i].mStayInPlace = true;
			//cell[x][z].m_OccupantID.push_back(i);
			agent[i].position = PAIR_INT(x, z);
			//cout << x << " " << z << endl;
			agent[i].direction = local;
			agent[i].pre_sFF = cell[x][z].sFF;
			cell[x][z].occupant_id = i;
			cell[x][z].occupied = 1;
			cell[x][z].temp_dFF += DYNAMIC_VALUE;
			//if (i == 467)
			//	system("pause");
			continue;
		}

		top /= total;
		down /= total;
		left /= total;
		right /= total;
		down += top;
		left += down;
		double random_num = (double)rand() / RAND_MAX;
		random_num += 0.00000001f;
		if (random_num <= top && isValid(x, z + 1))
		{
			//cout << cell[x][z + 1].occupied_number << " " << cell[x][z + 1].m_OccupantID[cell[x][z + 1].occupied_number] << " ";
			//if (cell[x][z + 1].occupied_number >= 3)
			//	cout << cell[x][z + 1].occupied_number << endl;
			int c_id = cell[x][z + 1].occupied_number;
			cell[x][z + 1].m_OccupantID[c_id] = i;
			cell[x][z + 1].occupied_number++;
			//cout << cell[x][z + 1].occupied_number << " " << cell[x][z + 1].m_OccupantID[cell[x][z + 1].occupied_number] << endl;
		}
		else if (random_num > top && random_num <= down && isValid(x, z - 1))
		{
			//cout << cell[x][z - 1].occupied_number << " " << cell[x][z - 1].m_OccupantID[cell[x][z - 1].occupied_number] << " ";
			cell[x][z - 1].m_OccupantID[cell[x][z - 1].occupied_number] = i;
			cell[x][z - 1].occupied_number++;
			//cout << cell[x][z-1].occupied_number << " " << cell[x][z - 1].m_OccupantID[cell[x][z - 1].occupied_number] << endl;
		}
		else if (random_num > down && random_num <= left && isValid(x - 1, z))
		{
			//cout << cell[x - 1][z].occupied_number << " " << cell[x - 1][z].m_OccupantID[cell[x - 1][z].occupied_number] << " ";
			cell[x - 1][z].m_OccupantID[cell[x - 1][z].occupied_number] = i;
			cell[x - 1][z].occupied_number++; 
			//cout << cell[x - 1][z].occupied_number << " " << cell[x - 1][z].m_OccupantID[cell[x - 1][z].occupied_number] << endl;
		}
		else if (random_num > left && isValid(x + 1, z))
		{
			//cout << cell[x + 1][z].occupied_number << " " << cell[x + 1][z].m_OccupantID[cell[x + 1][z].occupied_number] << " ";
			cell[x + 1][z].m_OccupantID[cell[x + 1][z].occupied_number] = i;
			cell[x + 1][z].occupied_number++;
			//cout << cell[x + 1][z].occupied_number << " " << cell[x + 1][z].m_OccupantID[cell[x + 1][z].occupied_number] << endl;
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_virtual_agent_position(){


}

void CS_CELLULAR_AUTOMATA::update_agent_stress(){

	update_area_stress();
	update_neighbors_count();
	update_positional_stress();
	update_time_stress();

	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			continue;
		if (!agent[i].arrival)
		{
			// update interpersonal stress
			update_interpersonal_stress(i);
		}
	}

	translate_perceived_stress();
}

void CS_CELLULAR_AUTOMATA::update_agent_info(){

	string s = to_string(model->select_agent_ID);
	string group_id = to_string(agent[model->select_agent_ID].group_id);
	string leader_id = to_string(agent[model->select_agent_ID].leader_id);
	string group_size = to_string(agent[model->select_agent_ID].group_size);
	string escape_time = to_string((double(agent[model->select_agent_ID].escape_time - agent[model->select_agent_ID].start_time) - model->total_pause_through_time) / CLOCKS_PER_SEC);
	string push_obId = to_string((agent[model->select_agent_ID].obstacle_component_id));
	//cout << double(agent[model->select_agent_ID].escape_time - agent[model->select_agent_ID].start_time) << endl;
	//cout << model->total_pause_through_time << endl;
	if ((double(agent[model->select_agent_ID].escape_time - agent[model->select_agent_ID].start_time) - model->total_pause_through_time) < 0)
		escape_time = "0";
	if (agent[model->select_agent_ID].leader)
	{
		//s = "Agent ID: " + s + "\nLeader: Yes" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time;
		if (!agent[model->select_agent_ID].volunteer)
			s = "Agent ID: " + s + "\nLeader: Yes" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time;
		if (agent[model->select_agent_ID].volunteer)
		{
			if (agent[model->select_agent_ID].beside_obstacle)
				s = "Agent ID: " + s + "\nLeader: Yes" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nBeside Obstacle" + "\npush_OBID: " + push_obId;
			if (!agent[model->select_agent_ID].beside_obstacle)
				s = "Agent ID: " + s + "\nLeader: Yes" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nNot Beside Obstacle" + "\npush_OBID: " + push_obId;
		}
		if (agent[model->select_agent_ID].bystander)
			s = s + "\n Agent is bystander. Block by obstacle " + to_string(agent[model->select_agent_ID].blocked_obstacle_id);
	}
	if (!agent[model->select_agent_ID].leader)
	{
		//s = "Agent ID: " + s + "\nLeader: No" + "\nLeader ID: " + leader_id + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time;
		if (!agent[model->select_agent_ID].volunteer)
			s = "Agent ID: " + s + "\nLeader: No" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time;
		if (agent[model->select_agent_ID].volunteer)
		{
			if (agent[model->select_agent_ID].beside_obstacle)
				s = "Agent ID: " + s + "\nLeader: No" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nBeside Obstacle" + "\npush_OBID: " + push_obId;
			if (!agent[model->select_agent_ID].beside_obstacle)
				s = "Agent ID: " + s + "\nLeader: No" + "\nGroup ID: " + group_id + "\nGroup Size: " + group_size + "\nEscape Time: " + escape_time + "\nNot Beside Obstacle" + "\npush_OBID: " + push_obId;
		}
		if (agent[model->select_agent_ID].bystander)
			s = s + "\n Agent is bystander. Block by obstacle " + to_string(agent[model->select_agent_ID].blocked_obstacle_id);
	}
	model->agent_info = s;
}

void CS_CELLULAR_AUTOMATA::update_guider_density(int agent_num){

	int radius = agent[agent_num].height / 10;
	int total = 0;
	int occupant = 0;
	for (int x = -radius; x < radius; x++)
	{
		for (int z = -radius; z < radius; z++)
		{
			if (!isValid(agent[agent_num].position.first + x, agent[agent_num].position.second + z))
				continue;
			if (cell[agent[agent_num].position.first + x][agent[agent_num].position.second + z].cell_type == 2)
				continue;
			total++;
			if (cell[agent[agent_num].position.first + x][agent[agent_num].position.second + z].occupied == 1)
				occupant++;
		}
	}
	agent[agent_num].guide_area_density = (float)occupant / total;
}

void CS_CELLULAR_AUTOMATA::update_neighbors_count(){

	//cout << "update neighbors counts..." << endl;
	int radius = stress_model.stress_radius;
	for (int i = 0; i < model->agent_number; i++)
	{
		agent[i].stressor.pref_neighbor_number = agent[i].stressor.cur_neighbor_number;
		for (int x = -radius; x < radius; x++)
		{
			for (int z = -radius; z < radius; z++)
			{
				if (!isValid(agent[i].position.first + x, agent[i].position.second + z))
					continue;
				if (cell[agent[i].position.first + x][agent[i].position.second + z].cell_type == 2)
					continue;
				if (cell[agent[i].position.first + x][agent[i].position.second + z].occupied == 1)
					agent[i].stressor.cur_neighbor_number++;
			}
		}
	}
}
// check agent in the stress area whether or not
void CS_CELLULAR_AUTOMATA::update_area_stress(){

	//cout << "update area stress..." << endl;
	reset_area_stress();

	for (unsigned int i = 0; i < stress_area_position.size(); i++)
	{
		//cout << "area" + to_string(i) << endl;
		for (int x = -stress_model.stress_radius; x < stress_model.stress_radius; x++)
		{
			for (int z = -stress_model.stress_radius; z < stress_model.stress_radius; z++)
			{
				if (!isValid(stress_area_position[i].first + x, stress_area_position[i].second + z))
					continue;
				if (cell[stress_area_position[i].first + x][stress_area_position[i].second + z].cell_type == 2)
					continue;
				if (cell[stress_area_position[i].first + x][stress_area_position[i].second + z].occupied == 1)
				{
					int agent_id = cell[stress_area_position[i].first + x][stress_area_position[i].second + z].occupant_id;
					// update area stress
					agent[agent_id].stressor.area_stress = stress_model.area_stress_intensity;
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_interpersonal_stress(int agent_id){

	agent[agent_id].stressor.interpersonal_stress = agent[agent_id].stressor.compute_interpersonal_stress();
}

void CS_CELLULAR_AUTOMATA::update_positional_stress(){

	//cout << "update positional stress." << endl;

	for (int i = 0; i < (int)dynamic_stress_area_position.size(); i++)
	{
		//cout << "stress pos: " << dynamic_stress_area_position[i].first << ", " << dynamic_stress_area_position[i].second << endl;
		for (int p = 0; p < model->agent_number; p++)
		{
			if (agent[p].arrival)
				continue;
			agent[p].stressor.position_stress = get_position_stress(agent[p].position, dynamic_stress_area_position[i]);
			//cout << "pos: " << agent[p].position.first << ", " << agent[p].position.second << ",stress: " << agent[p].stressor.position_stress << endl;
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_time_stress(){

	//cout << "update time stress." << endl;
	for (int i = 0; i < model->agent_number; i++)
	{

	}
}

void CS_CELLULAR_AUTOMATA::update_leader_influence(){

	//update_agent_anxiety();
	//cout << "update leader influence." << endl << endl;
	for (unsigned int i = 0; i < leader_pool.size(); i++)
	{
		int id = leader_pool[i];
		int pos_x = agent[id].position.first;
		int pos_z = agent[id].position.second;
		float leader_effect = 0.8f;
		for (int x = -LEADER_INFLUENCE_RANGE; x < LEADER_INFLUENCE_RANGE; x++)
		{
			for (int z = -LEADER_INFLUENCE_RANGE; z < LEADER_INFLUENCE_RANGE; z++)
			{
				if (!isValid(pos_x + x, pos_z + z))
					continue;
				if (cell[pos_x + x][pos_z + z].occupied == 0)
					continue;
				int agent_id = cell[pos_x + x][pos_z + z].occupant_id;
				//if (agent[agent_id].push_obstacle_id != agent[cell[pos_x + x][pos_z + z].occupant_id].push_obstacle_id)
				//	continue;
				if (agent_id == -1)
					continue;
				if (!agent[agent_id].leader)
					continue;
				//cout << agent[agent_id].anxiety * TIME_STEP * agent[i].psychology.leadership << endl;
				//agent[agent_id].anxiety -= agent[agent_id].anxiety * TIME_STEP * agent[i].psychology.leadership * 10;
				//cout << agent[agent_id].anxiety << endl;
				/*float leaderShip_weight;
				//cout << agent_id << endl;
				switch (agent[agent_id].personality)
				{
				case selfish:
				leaderShip_weight = 0.1f;
				break;
				case littleSelfish:
				leaderShip_weight = 0.3f;
				break;
				case neutral:
				leaderShip_weight = 0.5f;
				break;
				case littleBrave:
				leaderShip_weight = 0.7f;
				break;
				case brave:
				leaderShip_weight = 0.9f;
				break;
				}
				agent[agent_id].psychology.willness += agent[id].psychology.leadership * leaderShip_weight - agent[agent_id].psychology.resistance;
				//cout << agent[agent_id].psychology.willness << endl;
				/f (!agent[agent_id].strength && agent[agent_id].psychology.willness >= WILLNESS_THRESHOLD)
				{
				agent[agent_id].strength = true;
				strength_agent.push_back(agent_id);
				}
				*/
			}
		}
		for (unsigned int j = 0; j < agent[id].visible_agentID.size(); j++)
		{
			
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_volunteer_towardToObstacle_action(){

	//cout << "update volunteer toward to obstacle action." << endl;
	/*check_volunteer_besideObstacle();
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			int a_id = obstacles[o_id].volunteer_id[j];
			if (a_id == -1)
				continue;
			if (!agent[a_id].beside_obstacle)
			{
				cout << "agent_" << a_id << " toward to obstacle_" << agent[a_id].push_obstacle_id << "'s com_" << agent[a_id].obstacle_component_id << endl;
				cout << "-------------------------------------------------------------------------" << endl;
				agent[a_id].position = choose_direction_HighValueBased_fourDirection(agent[a_id].position.first, agent[a_id].position.second, a_id);
			}
		}
	}
	print_illegal_agent_information("volunteer_towardToObstacle_action");*/
	check_volunteer_position();
	check_volunteer_beside_obstacle();
	for (int i = 0; i < (int)blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		for (int j = 0; j < (int)obstacles[o_id].volunteer_id.size(); j++)
		{
			//cout << obstacles[o_id].volunteer_id[j] << endl;
			//cout << "obstacle_" << o_id << " com_" << j << endl;
			//cout << "volunteerid: " << obstacles[o_id].volunteer_id[j] << endl;
			/*if (obstacles[o_id].volunteer_id[j] != -1)
				continue;
			int size = obstacles[o_id].candidate_id[j].size(); 
			for (int k = 0; k < (int)obstacles[o_id].candidate_id[j].size(); k++)
			{
				int a_id = obstacles[o_id].candidate_id[j][k];
				//處理候選人ID的時候沒處理好，所以強制跳過
				if (agent[a_id].blocked_obstacle_id == -1)
					continue;
				if (!agent[a_id].beside_obstacle)
				{
					//cout << "aDFSF@FDSF" << endl;
					agent[a_id].position = choose_direction_HighValueBased_fourDirection(agent[a_id].position.first, agent[a_id].position.second, a_id);
				}
			}*/
			if (obstacles[o_id].volunteer_id[j] == -1)
				continue;
			int id = obstacles[o_id].volunteer_id[j];
			//cout << agent[id].position.first << " " << agent[id].position.second << endl;
			if (agent[id].beside_obstacle)
				continue;
			if (agent[id].arrival)
				continue;
			if (agent[id].obstacle_component_id == -1)
				continue;
			//cout << obstacles[i].component[j].first << " " << obstacles[i].component[j].second << endl;
			//cout << agent[id].position.first << " " << agent[id].position.second << endl;
			if (agent[id].position.first == -1 || agent[id].position.second == -1)
			{
				cout << "ilegal position" << endl;
				cout << agent[id].pre_pos.first << " " << agent[id].pre_pos.second << endl;
				cout << agent[id].position.first << " " << agent[id].position.second << endl;
				system("pause");
			}
			agent[id].position = choose_direction_HighValueBased_fourDirection(agent[id].position.first, agent[id].position.second, id);
			//cout << agent[id].position.first << " " << agent[id].position.second << endl;
			//cout << "---------------" << endl;
		}
		//cout << "=================" << endl;
	}
}

void CS_CELLULAR_AUTOMATA::update_blocked_obstacle_position(){

	//cout << "update blocked obstacle position." << endl << endl;
	//cout << "block obstacle size: " << blocked_obstacles.size() << endl;
	//cout << "normal obstacle size: " << normal_obstacle.size() << endl;
	switch (agent_psychology.obstacle_moveType)
	{
	case CS_OBSTACLE_MANUAL_MOVE:
		manual_update_blocked_obstacle_position();
		break;
	case CS_OBSTACLE_AUTOMATIC_MOVE:
		automatic_update_blocked_obstacle_position();
		break;
	}
	//print_illegal_agent_information("blocked_obstacle_position");
}

void CS_CELLULAR_AUTOMATA::automatic_update_blocked_obstacle_position(){

	/*for (int i = 0; i < model->agent_number; i++)
	{
		for (int j = 0; j < model->agent_number; j++)
		{
			if (agent[i].position == agent[j].position && i != j && !agent[i].arrival)
			{
				if (agent[i].volunteer && agent[j].volunteer)
				{
					cout << "agent " << i << " is volunteer" << ", agent " << j << " is volunteer" << endl;
				}
				if (!agent[i].volunteer)
				{
					cout << "agent " << i << " is common agent." << endl;
				}
				if (!agent[j].volunteer)
				{
					cout << "agent " << j << " is common agent." << endl;
				}
				//if (agent[j].volunteer)
				//{
				//	cout << "agent " << j << " is volunteer" << endl;
				//}
			}
		}
	}*/
	
	vector<int> removal_obstacle;
	//cout << "block size: " << blocked_obstacles.size() << endl;
	for (int i = 0; i < (int)blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		//cout << !!obstacles[o_id].block << endl;
		check_obstacle_arrive(o_id);
		if (obstacles[o_id].arrive_destination)
		{
			//add_normal_obstacle(o_id);
			//cout << normal_obstacle.size() << endl;
			//cout << blocked_obstacles.size() << endl;
			//cout << "obstacle " << o_id << " arrive destination" << endl;
			//system("pause");
			removal_obstacle.push_back(o_id);
			continue;
		}
		/*if (!check_volunteer_setup(o_id))
		{
			cout << "obstacle" << o_id << "'s volunteers didn't set up!." << endl << endl;
			continue;
		}*/
		bool s = false;
		for (unsigned int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			if (obstacles[o_id].volunteer_id[j] != -1 && agent[obstacles[o_id].volunteer_id[j]].beside_obstacle)
			{
				s = true;
				break;
			}
		}
		if (!s)
			continue;
		//cout << s << endl;
		if (!obstacles[o_id].mHaveDes && s)
			findOptimalDestination(o_id);
		for (int j = 0; j < obstacles[o_id].component.size(); j++)
		{
			//cout << obstacles[o_id].volunteer_id[j] << endl;
		}
		Direction move_direction;
		if (isBeoccupied(o_id))
		{
			/*cout << "obstacle " << o_id << " destination be occupied." << endl;
			for (unsigned int j = 0; j < obstacles[o_id].moveDestination.size(); j++)
			{
				int _x = obstacles[o_id].moveDestination[j].first;
				int _z = obstacles[o_id].moveDestination[j].second;
				cout << _x << " " << _z << ": " << !!cell[_x][_z].obstacle << " " << cell[_x][_z].obstacle_id << " " << o_id << endl;
			}*/
			//move_direction = obstalce_move_direction(o_id);
			findOptimalDestination(o_id);
			find_direction_towardToObstacleDestination(o_id, move_direction);
			/*for (unsigned int j = 0; j < obstacles[o_id].moveDestination.size(); j++)
			{
				int _x = obstacles[o_id].moveDestination[j].first;
				int _z = obstacles[o_id].moveDestination[j].second;
				cout << _x << " " << _z << ": " << !!cell[_x][_z].obstacle << " " << cell[_x][_z].obstacle_id << " " << o_id << endl;
			}*/
		}
		for (unsigned int j = 0; j < obstacles[o_id].moveDestination.size(); j++)
		{
			int _x = obstacles[o_id].moveDestination[j].first;
			int _z = obstacles[o_id].moveDestination[j].second;
			//cout << _x << " " << _z << ": " << !!cell[_x][_z].obstacle << " " << cell[_x][_z].obstacle_id << " " << o_id << endl;
		}
		if (!isBeoccupied(o_id))
		{
			find_direction_towardToObstacleDestination(o_id, move_direction);
		}
		//cout << "obstacle " << o_id << " destination: "<< endl;
		for (unsigned int j = 0; j < obstacles[o_id].moveDestination.size(); j++)
		{
			int _x = obstacles[o_id].moveDestination[j].first;
			int _z = obstacles[o_id].moveDestination[j].second;
			//cout << "obstacle " << o_id << endl;
			//cout << _x << " " << _z << endl; //": " << !!cell[_x][_z].obstacle << " " << cell[_x][_z].obstacle_id << " " << o_id << endl << endl;
		}
		//cout << "obstacle " << o_id << " direction " << obstacles[o_id].direction.first << " " << obstacles[o_id].direction.second << endl;
		check_obstacleMovement_blocked(o_id, move_direction);
		//cout << "obstacle " << o_id << " direction " << obstacles[o_id].direction.first << " " << obstacles[o_id].direction.second << endl;
		bool set = false;
		for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
		{
			if (obstacles[o_id].volunteer_id[i] != -1 && agent[obstacles[o_id].volunteer_id[i]].beside_obstacle)
			{
				set = true;
				break;
			}
		}
		//float first_level = 12, second_level = 8, third_level = 4;
		//float first_level_dis = 2.0f, second_level_dis = 4.0f, third_level_dis = 6.0f;
		//再次確認志願者和障礙物的移動沒有越界
		for (int j = 0; j < (int)obstacles[o_id].component.size(); j++)
		{
			int ox = obstacles[o_id].component[j].first;
			int oz = obstacles[o_id].component[j].second;
			int dx = obstacles[o_id].direction.first;
			int dz = obstacles[o_id].direction.second;
			if (set)
			{
				/*for (int _x = -third_level_dis; _x <= third_level_dis; _x++)
				{
					for (int _z = -third_level_dis; _z <= third_level_dis; _z++)
					{
						int x = ox + _x;
						int z = oz + _z;
						if (!isValid(x, z))
							continue;
						//float dis = (float)sqrt(_x*_x + _z*_z);
						float dis = abs(_x) + abs(_z);
						if (dis <= first_level_dis && first_level > cell[x][z].aFF)
						{
							cell[x][z].aFF = first_level;
						}
						else if (dis > first_level_dis && dis <= second_level_dis && second_level > cell[x][z].aFF)
						{
							cell[x][z].aFF = second_level;
						}
						else if (dis > second_level_dis && dis <= third_level_dis && third_level > cell[x][z].aFF)
						{
							cell[x][z].aFF = third_level;
						}
					}
				}*/
				constructAFF(o_id);
			}
			int id = obstacles[o_id].volunteer_id[j];
			if (id == -1)
				continue;
			if (!agent[id].beside_obstacle)
				continue;
			if (agent[id].arrival)
				continue;
			int level = 0;
			/*for (int lat = 1; lat <= 3; lat++)
			{
				level = level + dx + dz;
				for (int lon = -3; lon <= 3; lon++)
				{
					int x = level * abs(dx) + lon * abs(dz);
					int z = level * abs(dz) + lon * abs(dx);
					//cout << x << " " << z << endl;
					if (id == -1)
						continue;
					if (!isValid(agent[id].position.first + x, agent[id].position.second + z))
						continue;
					float dis = (float)sqrt(x*x + z*z);
					if (dis < 1.5f)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 6;
					}
					else if (dis >= 1.5f && dis < 3)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 4;
					}
					else if (dis >= 3 && dis < 4.5f)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 2;
					}
				}
			}*/
			/*for (int x = -3; x <= 3; x++)
			{
				for (int z = -3; z <= 3; z++)
				{
					if (id == -1)
						continue;
					if (!isValid(ox + x, oz + z))
						continue;
					float dis = sqrt(x*x + z*z);
					if (dis < 1.5f)
					{
						cell[ox + x][oz + z].aFF += 6;
					}
					else if (dis >= 1.5f && dis < 3)
					{
						cell[ox + x][oz + z].aFF += 4;
					}
					else if (dis >= 3 && dis < 4.5f)
					{
						cell[ox + x][oz + z].aFF += 2;
					}
				}
			}
			for (int x = -3; x <= 3; x++)
			{
				for (int z = -3; z <= 3; z++)
				{
					if (id == -1)
						continue;
					if (!isValid(agent[id].position.first + x, agent[id].position.second + z))
						continue;
					float dis = sqrt(x*x + z*z);
					if (dis < 1.5f)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 6;
					}
					else if (dis >= 1.5f && dis < 3)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 4;
					}
					else if (dis >= 3 && dis < 4.5f)
					{
						cell[agent[id].position.first + x][agent[id].position.second + z].aFF += 2;
					}
				}
			}*/
			if (!isValid(ox + dx, oz + dz))
			{
				cout << "ox " << ox << " oz " << oz << endl;
				cout << dx << " " << dz << endl;
				cout << ox + dx << " " << oz + dz << endl;
				obstacles[o_id].movable = false;
				break;
			}
			if (id != -1 && !isValid(agent[id].position.first + dx, agent[id].position.second + dz))
			{
				cout << "ax " << agent[id].position.first << " az " << agent[id].position.second << endl;
				cout << dx << " " << dz << endl;
				cout << agent[id].position.first + dx << " " << agent[id].position.second + dz << endl;
				delete_volunteer(o_id, j, id);
				obstacles[o_id].movable = false;
				//system("pause");
				reset();
			}
		}
		if (/*!obstacles[o_id].block*/ obstacles[o_id].movable && check_obstacle_weight(o_id))
		{
			//cout << "obstacle " << o_id << " direction ";
			switch (move_direction)
			{
			case _up:
				//cout << "(0,1)" << endl;
				obstacle_volunteer_movement(o_id, PAIR_INT(0, 1), move_direction);
				break;
			case _right:
				//cout << "(1,0)" << endl;
				obstacle_volunteer_movement(o_id, PAIR_INT(1, 0), move_direction);
				break;
			case _down:
				//cout << "(0,-1)" << endl;
				obstacle_volunteer_movement(o_id, PAIR_INT(0, -1), move_direction);
				break;
			case _left:
				//cout << "(-1,0)" << endl;
				obstacle_volunteer_movement(o_id, PAIR_INT(-1, 0), move_direction);
				break;
			}
			obstacles[o_id].stuck_time = 0;
		}
		obstacle_timer_accumulation(o_id);
		obstacles[o_id].stuck_time++;
		//cout << "===========================" << endl;
	}
	for (unsigned int i = 0; i < removal_obstacle.size(); i++)
	{
		//cout << "removal " << removal_obstacle[i] << endl;
		obstacles[removal_obstacle[i]].arrive_destination = true;
		add_normal_obstacle(removal_obstacle[i]);
	}
	//vector<int> t = blocked_obstacles;
	//for (unsigned int i = 0; i < t.size(); i++)
	//{
	//	int o_id = t[i];
	//	if (obstacles[o_id].stuck_time >(int)obstacles[o_id].component.size())
	//		add_normal_obstacle(o_id);
	//}
}

void CS_CELLULAR_AUTOMATA::manual_update_blocked_obstacle_position(){

	for (int i = 0; i < (int)blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		bool agent_setup = true;
		// volunteer finish the task
		if (obstacles[o_id].arrive_destination)
			continue;
		//check_agent_willness(o_id);
		//find_direction_towardToObstacleDestination(o_id);
		Direction move_direction;
		find_direction_towardToObstacleDestination(o_id, move_direction);
		check_obstacleMovement_blocked(o_id, move_direction);
		bool set = false;
		for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
		{
			if (obstacles[o_id].volunteer_id[i] != -1 && agent[obstacles[o_id].volunteer_id[i]].beside_obstacle)
			{
				set = true;
				break;
			}
		}
		//再次確認志願者和障礙物的移動沒有越界
		for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
		{
			int id = obstacles[o_id].volunteer_id[i];
			int ox = obstacles[o_id].component[i].first;
			int oz = obstacles[o_id].component[i].second;
			int dx = obstacles[o_id].direction.first;
			int dz = obstacles[o_id].direction.second;
			if (set)
			{
				constructAFF(o_id);
			}
			if (!isValid(ox + dx, oz + dz))
			{
				obstacles[o_id].movable = false;
				cout << ox + dx << ", " << oz + dz << endl;
				break;
			}
			if (id != -1 && !isValid(agent[id].position.first + dx, agent[id].position.second + dz) && !agent[id].arrival)
			{
				cout << agent[id].position.first + dx << ", " << agent[id].position.second + dz << endl;
				obstacles[o_id].movable = false;
			}
		}
		check_volunteer_finish_task(o_id);
		/*if (!obstacles[o_id].arrive_destination && check_obstacle_weight(o_id))
		{
			//obstacle_volunteer_movement(o_id, )
			//pushAgent_createSpace(o_id);
			//move_volunteersAndObstacles(o_id);
			//set_virtual_obstacle(o_id);
		}*/
		if (!obstacles[o_id].arrive_destination && obstacles[o_id].movable && check_obstacle_weight(o_id))
		{
			switch (move_direction)
			{
			case _up:
				obstacle_volunteer_movement(o_id, PAIR_INT(0, 1), move_direction);
				break;
			case _right:
				obstacle_volunteer_movement(o_id, PAIR_INT(1, 0), move_direction);
				break;
			case _down:
				obstacle_volunteer_movement(o_id, PAIR_INT(0, -1), move_direction);
				break;
			case _left:
				obstacle_volunteer_movement(o_id, PAIR_INT(-1, 0), move_direction);
				break;
			}
		}
		/*else
		{
			cout << "-------------------------------------------" << endl;
			if (obstacles[o_id].arrive_destination)
				cout << "obstacle" << o_id << " is arrive destination." << endl;
			else
				cout << "obstacle" << o_id << " isn't arrive destination." << endl;
			if (obstacles[o_id].movable)
				cout << "obstacle" << o_id << " is movable!" << endl;
			else
				cout << "obstacle" << o_id << " isn't movable!" << endl;
			if (check_obstacle_weight(o_id))
				cout << "obstacle" << o_id << "'s weight is ok." << endl;
			else
				cout << "obstacle" << o_id << "'s weight isn't ok." << endl;
			cout << "-------------------------------------------" << endl;
		}*/
		obstacle_timer_accumulation(o_id);
	}
}

void CS_CELLULAR_AUTOMATA::update_agent_closeToObstalce(){

	for (int i = 0; i < (int)obstacles.size(); i++)
	{
		for (int j = 0; j < (int)obstacles[i].component.size(); j++)
		{
			int o_x = obstacles[i].component[j].first;
			int o_z = obstacles[i].component[j].second;
			for (int x = -OBSTACLE_AVOID_DIS; x < OBSTACLE_AVOID_DIS; x++)
			{
				for (int z = -OBSTACLE_AVOID_DIS; z < OBSTACLE_AVOID_DIS; z++)
				{
					if (!isValid(o_x + x, o_z + z))
						continue;
					if (cell[o_x + x][o_z + z].occupied == 0)
						continue;
					if (cell[o_x + x][o_z + z].obstacle)
						continue;
					int a_id = cell[o_x + x][o_z + z].occupant_id;
					if (agent[a_id].volunteer)
						continue;
					if (agent[a_id].arrival)
						continue;
					agent[a_id].close_obstalce = true;
					agent[a_id].close_obstacle_id = i;
					agent[a_id].close_obstacle_com_id = j;
				}
			}
		}
		for (int j = 0; j < (int)obstacles[i].volunteer_id.size(); j++)
		{
			int id = obstacles[i].volunteer_id[j];
			int a_x = agent[id].position.first;
			int a_z = agent[id].position.second;
			for (int x = -OBSTACLE_AVOID_DIS; x < OBSTACLE_AVOID_DIS; x++)
			{
				for (int z = -OBSTACLE_AVOID_DIS; z < OBSTACLE_AVOID_DIS; z++)
				{
					if (!isValid(a_x + x, a_z + z))
						continue;
					if (cell[a_x + x][a_z + z].occupied == 0)
						continue;
					if (cell[a_x + x][a_z + z].obstacle)
						continue;
					int a_id = cell[a_x + x][a_z + z].occupant_id;
					if (agent[a_id].volunteer)
						continue;
					if (agent[a_id].arrival)
						continue;
					agent[a_id].close_obstalce = true;
					agent[a_id].close_obstacle_id = i;
					agent[a_id].close_obstacle_com_id = j;
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_strength_agent(){

	for (int i = 0; i < model->agent_number; i++)
	{
		// agent is strength if he/she has enough willness
		if (!agent[i].strength && agent[i].psychology.willness >= guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			// update weakness agent pool
			int weak_agent_id = weakness_agent[weakness_agent.size() - 1];
			weakness_agent.pop_back();
			int new_index = agent[i].pool_index;
			weakness_agent[new_index] = weak_agent_id;
			agent[weak_agent_id].pool_index = new_index;
			// update strength agent pool
			agent[i].pool_index = strength_agent.size();
			agent[i].strength = true;
			strength_agent.push_back(i);
			continue;
		}
		// agent is weak if he/she has not enough willness
		if (agent[i].strength && agent[i].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
		{
			int strength_agent_id = strength_agent[strength_agent.size() - 1];
			strength_agent.pop_back();
			int new_index = agent[i].pool_index;
			strength_agent[new_index] = strength_agent_id;
			agent[strength_agent_id].pool_index = new_index;
			agent[i].pool_index = weakness_agent.size();
			agent[i].strength = false;
			/*if (agent[i].volunteer)
			{
			int o_id = agent[i].push_obstacle_id;
			int com_id = agent[i].obstacle_component_id;
			obstacles[o_id].volunteer_id[com_id] = -1;
			agent[i].volunteer = false;
			agent[i].push_obstacle_id = -1;
			agent[i].obstacle_component_id = -1;
			}*/
			weakness_agent.push_back(i);
		}
	}
}

void CS_CELLULAR_AUTOMATA::update_agent_psychology(){

	update_agent_anxiety();
	update_agent_willness();
}

void CS_CELLULAR_AUTOMATA::update_agent_anxiety(){

	//clock_t cur_time_step = clock();
	//model->travel_time_step += (double(cur_time_step - model->last_time_step) / CLOCKS_PER_SEC);
	//cout << double(cur_time_step - model->last_time_step) / CLOCKS_PER_SEC << endl;
	//model->last_time_step = cur_time_step;
	//cout << model->travel_time_step << endl;
	//system("pause");
	model->travel_time_step += TIME_STEP;
	float time_step_weight;
	std::vector<float> aa(model->agent_number);
	for (int index = 0; index < model->agent_number; index++)
	{
		int i = mRandomAgentOrder[index];
		if (agent[i].arrival)
			continue;
		switch (agent_psychology.actionType)
		{
		case CROWD_SIMULATION_WITH_AGENT_PERSONALITY:
			time_step_weight = agent[i].agent_personality;
			break;
		case CROWD_SIMULATION_WITHOUT_AGENT_PERSONALITY:
			time_step_weight = 5.0f;
			//agent[i].receiveEmotionFromVisibleAgent();
			break;
		}
		//int x = agent[i].position.first;
		//int z = agent[i].position.second;
		//float visible_anxiety = 0.0f;
		//float total_weight = 0.0f;
		//for (int j = 0; j < agent[i].visible_agentID.size(); j++)
		//{
			//int id = agent[i].visible_agentID[j];
			//float distance = pair_int_distance(agent[i].position, agent[id].position);
			//visible_anxiety = visible_anxiety + (gaussianFunc(distance, 0, 10.0f) / gaussianFunc(0, 0, 10.0f)) * agent[id].anxiety;
			//total_weight = total_weight + gaussianFunc(distance, 0, (float)VISIBILITY_GRID_LENGTH) / gaussianFunc(0,0,(float)VISIBILITY_GRID_LENGTH);
			//visible_anxiety = visible_anxiety + (agent[id].anxiety - agent[i].anxiety) * (distance / sqrt(pow(VISIBILITY_GRID_LENGTH, 2)));
			//total_weight = total_weight + (distance / sqrt(pow(VISIBILITY_GRID_LENGTH, 2)));
			//visible_anxiety = visible_anxiety + agent[id].psychology.leadership * (agent[id].anxiety - agent[i].anxiety);
			//total_weight = total_weight + agent[id].psychology.leadership;
			//visible_anxiety = visible_anxiety + ((VISIBILITY_GRID_LENGTH - distance) / VISIBILITY_GRID_LENGTH) * (agent[id].anxiety - agent[i].anxiety);
			//visible_anxiety = visible_anxiety + (agent[id].anxiety - agent[i].anxiety);
			//total_weight = total_weight + ((VISIBILITY_GRID_LENGTH - distance) / VISIBILITY_GRID_LENGTH);
		//}
		//agent[i].anxiety = model->travel_time_step * time_step_weight / (cell[x][z]._sFF + 0.0001f);
		//float receive_emotion;
		//receive_emotion = (1 - agent[i].empathy) * (model->travel_time_step * time_step_weight / (cell[x][z]._sFF + 0.0001f)) + agent[i].empathy * ((visible_anxiety + 0.0001f) / (total_weight + 0.0001f));
		//receive_emotion = (visible_anxiety / (total_weight + 0.0001f));
		//receive_emotion = visible_anxiety;
		//cout << receive_emotion << endl;
		//float alpha, beta, gama;
		//alpha = guiParameter.pos_weight;
		//beta = guiParameter.stayInPlace_weight;
		//gama = guiParameter.visibleAnxiety_weight;
		//agent[i].anxiety_delta = alpha * (cell[x][z].sFF - agent[i].pre_sFF) + beta * agent[i].stay_counter + gama * receive_emotion;
		//float through_time = mTimeStep / (cell_manager.max_sFF - cell[x][z]._sFF + 0.0001f);
		//float rate = mTimeStep / (cell_manager.max_sFF - cell[x][z].sFF + 0.0001f);
		//float rate = mTimeStep / (30- cell[x][z].sFF + 0.0001f);
		//receive_emotion = visible_anxiety * agent[i].mPersonality.neuroticism + (1 - agent[i].psychology.leadership) * rate * 10;
		//agent[i].anxiety += agent[i].psychology.leadership * (through_time + receive_emotion);
		//agent[i].anxiety += (guiParameter.sFF_weight * mTimeStep + (0.1f - guiParameter.sFF_weight) * receive_emotion);
		//agent[i].anxiety += agent[i].anxiety_delta;
		//if (agent[i].anxiety > 1.0f)
		//	agent[i].anxiety = 1.0f;
		//cout << through_time << endl;
		//cout << receive_emotion << endl;
		//agent[i].anxiety_delta = agent[i].psychology.panic * 0.1f * (cell[x][z].sFF - agent[i].pre_sFF + agent[i].stay_counter) + 0.1f * receive_emotion + through_time;
		//cout << receive_emotion << endl;
		//agent[i].anxiety_delta = agent[i].psychology.panic * receive_emotion * TIME_STEP;
		//if (agent[i].volunteer)
		//	agent[i].anxiety_delta = receive_emotion * TIME_STEP - agent[i].mPersonality.conscientiousness * TIME_STEP;
		//else
		//agent[i].anxiety_delta = receive_emotion * TIME_STEP /*+ rate * (cell[x][z].sFF - agent[i].pre_sFF + agent[i].stay_counter)*/;
		//agent[i].anxiety += agent[i].anxiety_delta;
		//if (!agent[i].leader && agent[i].anxiety > 0.7f)
		//{
		//	agent[i].anxiety -= agent[i].anxiety * agent[agent_group[agent[i].group_id].member[0]].psychology.leadership * TIME_STEP * 10;
		//}
		//if (agent[i].anxiety < 0.0f)
		//	agent[i].anxiety = 0.0f;
		//if (agent[i].anxiety > 1)
		//	agent[i].anxiety = 1;
		float n = agent[i].mPersonality.neuroticism;
		float anx = agent[i].anxiety;
		float escapeTime = 1 / (1 + exp(-mTimeStep));
		float leaderEffect = getLeaderEffect(i);
		aa[i] = anx + getContagionStrengthFromContactAgents(i) * (combinationFunction(i) - anx);
		//aa[i] = anx + (0.02f* exp(-0.02f * (mTimeCounter)));
		//aa[i] = anx + (-0.02f * exp(-0.02f * (anx - combinationFunction(i))));
		//aa[i] = anx + getContagionStrengthFromContactAgents(i) * (anx - combinationFunction(i));
		if (i == 0)
		{
			//cout << "contagion strength: " << getContagionStrengthFromContactAgents(i) << endl;
			//cout << "combination function: " << anx - combinationFunction(i) << endl;
			//cout << "combination function: " << combinationFunction(i) - anx << endl;
			//cout << "tendency = 0.1 " << getContagionStrengthFromContactAgents(i) * (combinationFunction(i) - anx) << endl;
			//cout << "tendency = 0.5 " << getContagionStrengthFromContactAgents(i) * (testCombinationFunction(i, 0.5f, 0.1f) - anx) << endl;
			//cout << "tendency = 0.9 " << getContagionStrengthFromContactAgents(i) * (testCombinationFunction(i, 0.9f, 0.1f) - anx) << endl << endl;
		}
		//aa[i] = anx + n * getImpactFromOtherAgents(i) + leaderEffect * agent[i].averageResidualAnxiety();
		//aa[index] = (1+(1-exp(-mTimeCounter / 50))) * anx + n * getImpactFromOtherAgents(i);
		//cout << "impact: " << getImpactFromOtherAgents(i) << endl;
		//cout << "aver: " << agent[i].averageResidualAnxiety() << endl;
		//cout << "an: " << aa[index] << endl;
		//double eee = aa[i] - 0.2f;
		//aa[i] = 1 - 1 / (1 + exp(-5 * eee));
		//if (agent[i].anxiety < 0)
		//	agent[i].anxiety = 0;
		//cout << anx + n * (getImpactFromOtherAgents(i) - anx) * TIME_STEP << endl;
		if (aa[i] > 1)
			aa[i] = 1;
		if (aa[i] < 0)
			aa[i] = 0;
		if (agent[i].compressive_leader)
		{
			agent[i].anxiety_variation.push_back(anx);
		}
		if (i < 10)
		{
			agent[i].anxiety_variation.push_back(anx);
		}
	}
	float total = 0;
	int num = 0;
	for (int index = 0; index < model->agent_number; index++)
	{
		int i = mRandomAgentOrder[index];
		if (i < 0 || i > model->agent_number)
		{
			cout << i << endl;
			system("pause");
		}
		if (agent[i].arrival)
			continue;
		//cout << index << endl;
		//cout << aa[index] << endl;
		//cout << agent[i].anxiety << endl;
		agent[i].anxiety = aa[i];
		//cout << aa[i] << endl;
		//cout << i << endl;
		//cout << agent[i].anxiety << endl;
		agent[i].insertAnxiety(aa[i]);
		total += (float)agent[i].anxiety;
		num++;
	}
	//vector<float>().swap(aa);
	//cout << "average anxiety: " << total / (num + 0.000001f) << endl;
}

void CS_CELLULAR_AUTOMATA::update_agent_willness(){

	float alpha = 1.5f;
	for (int i = 0; i < model->agent_number; i++)
	{
		//agent[i].psychology.willness = (agent_psychology.anxiety_weight * (1 - agent[i].anxiety)) / model->travel_time_step;
		//agent[i].psychology.willness = agent_psychology.anxiety_weight * (1 - agent[i].anxiety) + agent[i].agent_personality;
		//agent[i].psychology.willness = exp(agent_psychology.anxiety_weight * (1 - agent[i].anxiety)) + agent[i].agent_personality;
		//agent[i].psychology.willness = (alpha * (1 - agent[i].anxiety) + (2 - alpha) * agent[i].agent_personality) / 2;
		//float alpha;
		//alpha = guiParameter.anxiety_weight;
		//agent[i].psychology.willness = alpha * (1 - agent[i].anxiety) + (1 - alpha);
		//agent[i].psychology.willness = 0.5f * (1 - agent[i].anxiety) + model->travel_time_step;
		//cout << agent[i].psychology.willness << endl;
		//agent[i].receiveWillingFromCrowd();
		//agent[i].receiveEmotionFromVisibleAgent();
		//agent[i].psychology.willness = (agent[i].mPersonality.conscientiousness * (1 - agent[i].anxiety) * model->travel_time_step - (1-agent[i].psychology.leadership));
		//agent[i].psychology.willness = (agent[i].mPersonality.conscientiousness * (1 - agent[i].anxiety) * model->travel_time_step /*- (1-agent[i].psychology.leadership)*/);
		//if (agent[i].psychology.willness > 1)
		//	agent[i].psychology.willness = 1;
		//if (agent[i].psychology.willness < 0)
		//	agent[i].psychology.willness = 0;
		//agent[i].psychology.willness = (agent[i].mPersonality.conscientiousness + 1 - agent[i].anxiety) * model->travel_time_step /*- (1-agent[i].psychology.leadership)*/;
		//cout << agent[i].psychology.willness << endl;
		//agent[i].psychology.willness = 1 - (1 / (1 + exp(-1 * (agent[i].anxiety - (1 - guiParameter.willing_threshold)))));
		agent[i].psychology.willness = float(1 - agent[i].anxiety);
		agent[i].psychology.willness = 1.0f;
		//cout << agent[i].psychology.willness << endl;
		//if (i < 10 && !agent[i].arrival)
		//	cout << agent[i].anxiety << " " << agent[i].psychology.willness << ",  ";
	}
	//cout << endl;
}

void CS_CELLULAR_AUTOMATA::update_agent_blockedByObstacle(){

	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].blocked_obstacle_id != -1)
		{
			if (!obstacles[agent[i].blocked_obstacle_id].block_exit)
			{
				agent[i].blocked_obstacle_id = -1;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::updateTimeStep(){

	mTimeStep += TIME_STEP;
	mTimeCounter++;
}

void CS_CELLULAR_AUTOMATA::updateScene(){

	randomGenerateExitColor();
	//reset();
	compute_staticFF();
	compute_clean_sFF();
	guiParameter.mEditingMode = 0;
}

void CS_CELLULAR_AUTOMATA::collectAverageAnxietyAroundObserveAgent(){

	//cout << mAverageAnxeityAroundObserveAgent.size() << endl;
	for (unsigned int i = 0; i < mObserverAgent.size(); i++)
	{
		int id = mObserverAgent[i];
		if (agent[id].arrival)
			continue;
		float total = 0;
		int num = 0;
		int _x = agent[id].position.first;
		int _z = agent[id].position.second;
		for (int x = -VISIBILITY_GRID_LENGTH; x <= VISIBILITY_GRID_LENGTH; x++)
		{
			for (int z = -VISIBILITY_GRID_LENGTH; z <= VISIBILITY_GRID_LENGTH; z++)
			{
				if (!isValid(_x + x, _z + z))
					continue;
				if (cell[_x + x][_z + z].occupied == 0)
					continue;
				if (cell[_x + x][_z + z].occupant_id == -1)
					continue;
				if (cell[_x + x][_z + z].obstacle)
					continue;
				int a_id = cell[_x + x][_z + z].occupant_id;
				total += agent[a_id].anxiety;
				num++;
			}
		}
		total /= (num + 0.00001f);
		//for (unsigned int j = 0; j < agent[i].visible_agentID.size(); j++)
		//{
		//	int a_id = agent[i].visible_agentID[j];
		//	total += agent[a_id].anxiety;
		//}
		//total /= (agent[i].visible_agentID.size() + 0.00001f);
		mAverageAnxeityAroundObserveAgent[i].push_back(total);
		mObserveAgentAnxiety[i].push_back(agent[id].anxiety);
	}
}