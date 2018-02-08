#include "../myCellularAutomata/cellular_automata.h"
#include <algorithm>

using namespace std;

void OBSTACLE::reset(){

	block = false;
	block_exit = false;
	arrive_destination = false;
	volunteer_destination = pair<int, int>(-1, -1);
	clicked_o_id = 0;
	enough_volunteers = false;
	direction = pair<int, int>(0, 0);
	finish_task = false;
	weight = 6.0f;
	timer = 0.0f;
	compute = false;
	hasLeader_guider = false;
	density = 0.0f;
	up_count = 0;
	right_count = 0;
	down_count = 0;
	left_count = 0;
	ready = false;
	type = 0;
	block_direction = -1;
	movable = false;
	vector<vector<vector<CELL_BFS>>>().swap(distance_map);
}

void CS_CELLULAR_AUTOMATA::obstacle_findNearestAgent(){

	for (int i = 0; i < removed_obstacles.size(); i++)
	{
		if (removed_obstacles[i].has_volunteer)
			continue;
		for (int x = -removed_obstacles[i].radius; x < removed_obstacles[i].radius; x++)
		{
			for (int z = -removed_obstacles[i].radius; z < removed_obstacles[i].radius; z++)
			{
				if (!isValid(removed_obstacles[i].position.first + x, removed_obstacles[i].position.second + z))
					continue;
				if (cell[removed_obstacles[i].position.first + x][removed_obstacles[i].position.second + z].occupied == 1)
				{
					int agent_id = cell[removed_obstacles[i].position.first + x][removed_obstacles[i].position.second + z].occupant_id;
					// if agent already be a volunteer, then skip
					if (agent[agent_id].volunteer)
						continue;
					// if agent doesn't have willness to move obstacle
					if (agent[agent_id].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
						continue;
					cout << "obstacle" << i << " has find the volunteer agent" << agent_id << "." << endl << endl;;
					agent[agent_id].volunteer = true;
					agent[agent_id].push_obstacle_id = i;
					removed_obstacles[i].has_volunteer = true;
					removed_obstacles[i].volunteer_id = agent_id;
					return;
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::test_large_obstacle1(){

	cout << "larget obstacle scene one..." << endl;
}

void CS_CELLULAR_AUTOMATA::move_obstalce(){

	cout << "moving blocked obstacles." << endl << endl;
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		bool volunteersInPlace = true;
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			if (obstacles[o_id].volunteer_id[j] == -1)
			{
				volunteersInPlace = false;
				break;
			}
		}
		if (!volunteersInPlace)
			continue;

	}
}

void CS_CELLULAR_AUTOMATA::obstacle_volunteer_movement(int o_id, PAIR_INT direction, Direction d){

	//cout << "move volunteers and obstacle_" << o_id << "!!!" << endl << endl;
	disperse_crowd(o_id, direction);
	//pushAgent_createSpace(o_id);
	obstacles[o_id].mMoveStep++;
	//cout << obstacles[o_id].mMoveStep << endl;
	//cout << "obstacle_" << o_id << "'s direction: " << endl;
	if (direction.second == 1)
		obstacles[o_id].up = true;
	if (direction.second == -1)
		obstacles[o_id].down = true;
	if (direction.first == 1)
		obstacles[o_id].right = true;
	if (direction.first == -1)
		obstacles[o_id].left = true;
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		/*int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		int com_id = agent[a_id].obstacle_component_id;*/
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		cell[o_x][o_z].obstacle = false;
		cell[o_x][o_z].obstacle_ = 1;
		cell[o_x][o_z].obstacle_id = -1;
	}
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
		if (agent[a_id].arrival)
			continue;
		agent[a_id].add_sFF = false;
		int com_id = agent[a_id].obstacle_component_id;
		int a_x = agent[a_id].position.first;
		int a_z = agent[a_id].position.second;
		int o_x = obstacles[o_id].component[com_id].first;
		int o_z = obstacles[o_id].component[com_id].second;
		//if (!isValid(a_x + direction.first, a_x + direction.second))
		//	return;
		// agent move
		cell[a_x][a_z].occupied = 0;
		cell[a_x][a_z].occupant_id = -1;
		cell[a_x][a_z].temp_dFF += DYNAMIC_VALUE;
		//cell[a_x + direction.first][a_z + direction.second].occupant_id = a_id;
		//cell[a_x + direction.first][a_z + direction.second].occupied = 1;
		//agent[a_id].position = PAIR_INT(a_x + direction.first, a_z + direction.second);
		//agent[a_id].direction = d;
	}
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
		if (agent[a_id].arrival)
			continue;
		agent[a_id].add_sFF = false;
		int com_id = agent[a_id].obstacle_component_id;
		int a_x = agent[a_id].position.first;
		int a_z = agent[a_id].position.second;
		int o_x = obstacles[o_id].component[com_id].first;
		int o_z = obstacles[o_id].component[com_id].second;
		//if (!isValid(a_x + direction.first, a_x + direction.second))
		//	return;
		// agent move
		//cell[a_x][a_z].occupied = 0;
		//cell[a_x][a_z].occupant_id = -1;
		//cell[a_x][a_z].temp_dFF += DYNAMIC_VALUE;
		cell[a_x + direction.first][a_z + direction.second].occupant_id = a_id;
		cell[a_x + direction.first][a_z + direction.second].occupied = 1;
		agent[a_id].position = PAIR_INT(a_x + direction.first, a_z + direction.second);
		agent[a_id].direction = d;
	}
	// obstacle move
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		cell[o_x + direction.first][o_z + direction.second].obstacle = true;
		cell[o_x + direction.first][o_z + direction.second].obstacle_ = 0;
		cell[o_x + direction.first][o_z + direction.second].obstacle_id = o_id;
		cell[o_x + direction.first][o_z + direction.second].cell_type = 2;
		obstacles[o_id].component[i] = PAIR_INT(o_x + direction.first, o_z + direction.second);
	}
	compute_staticFF();
	/*for (int i = 0; i < pow(2, mExit.size()); i++)
	{
		int value = i;
		for (int j = mExit.size() - 1; j >= 0; j--)
		{
			if (value >= pow(2, j))
			{
				value -= pow(2, j);
				eb[j] = 1;
			}
			else
			{
				eb[j] = 0;
			}
		}
		computeNewMapWithoutExit(eb);
	}*/
	for (int i = 0; i < mExit.size(); i++)
	{
		eb[i] = 0;
	}
	//computeNewMapWithoutExit(eb);
	for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id == -1)
			continue;
		if (!agent[a_id].beside_obstacle)
			continue;
	}
	/*for (int i = 0; i < obstacles.size(); i++)
	{
		computeNewMapWithoutExit(i);
	}*/
}

void CS_CELLULAR_AUTOMATA::move_volunteersAndObstacles(int o_id){


}

void CS_CELLULAR_AUTOMATA::find_obstacleMustMove(){

	reset_cellMark();
	for (int i = 0; i < obstacles.size(); i++)
	{
		int occupied = 0;
		int total = 0;
		float dis = findNearestDistanceToExit(i);
		//cout << dis << endl;
		for (int j = 0; j < obstacles[i].component.size(); j++)
		{
			int com_x = obstacles[i].component[j].first;
			int com_z = obstacles[i].component[j].second;
			for (int x = -VISIBILITY_DISTANCE; x < VISIBILITY_DISTANCE; x++)
			{
				for (int z = -VISIBILITY_DISTANCE; z < VISIBILITY_DISTANCE; z++)
				{
					if (!isValid(com_x + x, com_z + z))
						continue;
					if (cell[com_x + x][com_z + z].check)
						continue;
					if (cell[com_x + x][com_z + z].mark)
						continue;
					//if (cell[com_x + x][com_z + z].obstacle)
					//	continue;
					if (cell[com_x + x][com_z + z].clean_sFF < cell[com_x][com_z].clean_sFF)
						continue;
					total++;
					cell[com_x + x][com_z + z].check = true;
					if (cell[com_x + x][com_z + z].occupied == 1)
					{
						//obstacles[i].density_map[j][x][z] = true;
						cell[com_x + x][com_z + z].mark = true;
						occupied++;
						if (com_z + z > obstacles[i].highest_z)
							obstacles[i].up_count++;
						else if (com_z + z < obstacles[i].lowest_z)
							obstacles[i].down_count++;
						else if (com_x + x < obstacles[i].lowest_x)
							obstacles[i].left_count++;
						else if (com_x + x > obstacles[i].highest_x)
							obstacles[i].right_count++;
					}
				}
			}
		}
		
		float density = (float)occupied / (total + 0.0001f);
		//cout << "obstacle_" << i << " density: " << density << endl;
		obstacles[i].density = density;
		switch (obstacles[i].block_direction)
		{
		case 0:
			obstacles[i].up_count = -9999;
			break;
		case 1:
			obstacles[i].right_count = -9999;
			break;
		case 2:
			obstacles[i].down_count = -9999;
			break;
		case 3:
			obstacles[i].left_count = -9999;
			break;
		}
		if (obstacles[i].block_exit && check_obstacleID_exist(i))
		{
			obstacles[i].block = true;
			obstacles[i].arrive_destination = false;
			obstacles[i].enough_volunteers = false;
			obstacles[i].pool_index = blocked_obstacles.size();
			blocked_obstacles.push_back(i);
			continue;
		}
		if (density >= DENSITY_THRESHOLD || dis < model->mMoveDistance)
		{
			//cout << obstacles[i].lowest_z << " " << obstacles[i].highest_z << endl;
			obstacles[i].block = true;
			obstacles[i].arrive_destination = false;
			obstacles[i].enough_volunteers = false;
			obstacles[i].pool_index = blocked_obstacles.size();
			blocked_obstacles.push_back(i);
			//cout << blocked_obstacles.size() << endl;
			continue;
		}
		if (density < DENSITY_THRESHOLD)
		{
			obstacles[i].block = false;
			obstacles[i].arrive_destination = false;
			obstacles[i].enough_volunteers = false;
			obstacles[i].pool_index = normal_obstacle.size();
			normal_obstacle.push_back(i);
		}
	}
	//cout << "blocked obstacle size: " << blocked_obstacles.size() << endl;
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		cout << !!obstacles[blocked_obstacles[i]].block << endl;
	}
	//cout << "normal obstacle size: " << normal_obstacle.size() << endl;
	for (int i = 0; i < normal_obstacle.size(); i++)
	{
		//cout << normal_obstacle[i] << endl;
	}
}

float CS_CELLULAR_AUTOMATA::findNearestDistanceToExit(vector<PAIR_INT> v){

	float min_dis = FLT_MAX;
	for (int i = 0; i < v.size(); i++)
	{
		int ex, ez, dx, dz;
		for (int j = 0; j < mExit.size(); j++)
		{
			ex = mExit[j].position.first;
			ez = mExit[j].position.second;
			dx = mExit[j].direction.first;
			dz = mExit[j].direction.second;
			for (int k = 0; k < mExit[j].width; k++)
			{
				int px = ex + dx * k;
				int pz = ez + dz * k;
				if (pair_int_distance(PAIR_INT(px, pz), v[i]) < min_dis)
					min_dis = pair_int_distance(PAIR_INT(px, pz), v[i]);
			}
		}
	}
	return min_dis;
}

float CS_CELLULAR_AUTOMATA::findNearestDistanceToExit(int o_id){

	float min_dis = FLT_MAX;
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int ex, ez, dx, dz;
		for (int j = 0; j < mExit.size(); j++)
		{
			ex = mExit[j].position.first;
			ez = mExit[j].position.second;
			dx = mExit[j].direction.first;
			dz = mExit[j].direction.second;
			for (int k = 0; k < mExit[j].width; k++)
			{
				int px = ex + dx * k;
				int pz = ez + dz * k;
				if (pair_int_distance(PAIR_INT(px, pz), obstacles[o_id].component[i]) < min_dis)
					min_dis = pair_int_distance(PAIR_INT(px, pz), obstacles[o_id].component[i]);
			}
		}
	}
	//cout << min_dis << endl;
	return min_dis;
}

void CS_CELLULAR_AUTOMATA::update_moveable_obstacle(){

	update_obstalce_type();
	reset_cellMark();
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_MANUAL_MOVE)
		return;
	/*for (int i = 0; i < obstacles.size(); i++)
	{
		cout << "obstacle " << i << " is ";
		if (obstacles[i].block)
		{
			cout << "block, index: " << obstacles[i].pool_index << endl;
		}
		else if (obstacles[i].block_exit)
		{
			cout << "block exit, index: " << obstacles[i].pool_index << endl;
		}
		else if (!obstacles[i].block)
		{
			cout << "isn't block, index: " << obstacles[i].pool_index << endl;
		}
	}*/
	for (int i = 0; i < obstacles.size(); i++)
	{
		/*switch (obstacles[i].block_exit)
		{
		case true:
			obstacles[i].density = get_obstacle_density(i);
			if (check_obstacleID_exist(i))
			{
				obstacles[i].arrive_destination = false;
				obstacles[i].enough_volunteers = false;
				obstacles[i].pool_index = blocked_obstacles.size();
				blocked_obstacles.push_back(i);
			}
			break;
		case false:
			float density = get_obstacle_density(i);
			obstacles[i].density = density;
			if (density >= DENSITY_THRESHOLD)
			{
				add_blocked_obstacle(i);
			}
			if (density < DENSITY_THRESHOLD)
			{
				add_normal_obstacle(i);
			}
			break;
		}*/
		float density = get_obstacle_density(i);
		float dis = findNearestDistanceToExit(i);
		//dis = obstacles[i].mMoveStep;
		obstacles[i].density = density;
		//cout << obstacles[i].block_exit << " " << obstacles[i].block << " " << !obstacles[i].moveAlready << " " << !obstacles[i].arrive_destination << " dis:" << dis << endl;
		if (/*density >= DENSITY_THRESHOLD || */(obstacles[i].block_exit || obstacles[i].block || dis < model->mMoveDistance) && !obstacles[i].moveAlready && !obstacles[i].arrive_destination)
		{
			//if (obstacles[i].block_exit)
			//	cout << "block exit" << endl;
			//cout << "density: " << density << endl;
			//cout << "add block obstacle " << i << endl;
			add_blocked_obstacle(i);
			obstacles[i].block = true;
			//cout << obstacles[i].block_exit << " " << !obstacles[i].moveAlready << " " << obstacles[i].block << " " << dis << endl;
		}
		if (/*density < DENSITY_THRESHOLD && */!obstacles[i].block_exit && dis >= model->mMoveDistance)
		{
			//cout << obstacles[i].mMoveStep << endl;
			//cout << "add normal obstacle " << i << endl;
			add_normal_obstacle(i);
		}
		switch (obstacles[i].block_direction)
		{
		case 0:
			obstacles[i].up_count = -9999;
			break;
		case 1:
			obstacles[i].right_count = -9999;
			break;
		case 2:
			obstacles[i].down_count = -9999;
			break;
		case 3:
			obstacles[i].left_count = -9999;
			break;
		}
		//int counter_buffer[4] = { obstacles[i].up_count, obstacles[i].right_count, obstacles[i].down_count, obstacles[i].left_count };
		//cout << "obstacle_" << i << ": (" << counter_buffer[0] << ", " << counter_buffer[1] << ", " << counter_buffer[2] << ", " << counter_buffer[3] << ")" << endl;
	}
	/*//cout << "blocked obstacle size: " << blocked_obstacles.size() << endl;
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		//cout << blocked_obstacles[i] << endl;
	}
	//cout << "normal obstacle size: " << normal_obstacle.size() << endl;
	for (int i = 0; i < normal_obstacle.size(); i++)
	{
		//cout << normal_obstacle[i] << endl;
	}*/
}

void CS_CELLULAR_AUTOMATA::update_obstalce_type(){

	update_ExitID_BlockedByObstacle();
	for (unsigned int j = 0; j < obstacles.size(); j++)
	{
		obstacles[j].block_exit = false;
		obstacles[j].block = false;
	}
	for (unsigned int j = 0; j < mExit.size(); j++)
	{
		int x = mExit[j].position.first;
		int z = mExit[j].position.second;
		int dx = mExit[j].direction.first;
		int dz = mExit[j].direction.second;
		//cout << x << " " << z << endl;
		//cout << dx << " " << dz << endl;
		bool block = true;
		int o_id = -1;
		//cout << mExit[j].width << endl;
		for (int k = 0; k < mExit[j].width; k++)
		{
			int ex = x + dx*k;
			int ez = z + dz*k;
			//cout << ex << " " << ez << " " << !!cell[ex][ez].obstacle << endl;
			if (!cell[ex][ez].obstacle)
			{
				block = false;
				break;
			}
			else
			{
				o_id = cell[ex][ez].obstacle_id;
			}
		}
		if (block == true && o_id != -1)
		{
			obstacles[o_id].block_exit = true;
			obstacles[o_id].block = true;
		}
	}
	/*for (int i = 0; i < obstacles.size(); i++)
	{
		for (int j = 0; j < obstacles[i].component.size(); j++)
		{
			cout << obstacles[i].component[j].first << " " << obstacles[i].component[j].second << " ";
		}
		cout << endl;
	}*/
	/*for (int i = 0; i < obstacles.size(); i++)
	{
	for (int j = 0; j < obstacles[i].component.size(); j++)
	{
	cout << "obstalce " << i << ", volunteer " << j << " = agent " << obstacles[i].volunteer_id[j] << endl;
	}
	if (!obstacles[i].block_exit)
	continue;
	for (int j = 0; j < obstacles[i].component.size(); j++)
	{
	int x = obstacles[i].component[j].first;
	int z = obstacles[i].component[j].second;
	if (cell[x][z].sFF != 0)
	{
	obstacles[i].block_exit = false;
	// update agent blockedByExitID
	return;
	}
	}
	obstacles[i].block_exit = true;
	}*/
}

void CS_CELLULAR_AUTOMATA::update_ExitID_BlockedByObstacle(){

	for (int i = 0; i < mExit.size(); i++)
	{
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			int e_x = x + dx*j;
			int e_z = z + dz*j;
			if (!cell[e_x][e_z].obstacle)
			{
				for (int a = 0; a < model->agent_number; a++)
				{
					//agent[a].blockByExit[i] = 0;
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::find_volunteers(int o_id){

	for (int j = 0; j < obstacles[o_id].component.size(); j++)
	{
		int com_x = obstacles[o_id].component[j].first;
		int com_z = obstacles[o_id].component[j].second;
		for (int x = -VISIBILITY_DISTANCE; x < VISIBILITY_DISTANCE; x++)
		{
			for (int z = -VISIBILITY_DISTANCE; z < VISIBILITY_DISTANCE; z++)
			{
				if (!isValid(com_x + x, com_z + z))
					continue;
				if (cell[com_x + x][com_z + z].occupied == 0)
					continue;
				if (cell[com_x + x][com_z + z].obstacle)
					continue;
				if (cell[com_x + x][com_z + z].clean_sFF < cell[com_x][com_z].clean_sFF)
					continue;
				int id = cell[com_x + x][com_z + z].occupant_id;
				if (!agent[id].strength)
					continue;
				if (agent[id].volunteer)
					continue;
				int original_volunteer_id = obstacles[o_id].volunteer_id[j];
				float willness = agent[id].psychology.willness;
				float d_threshold = get_density_threshold(willness);
				// if agent has enough willing to move obstacle
				if (willness > d_threshold)
				{
					// try to find another better choice
					if (original_volunteer_id != -1)
					{
						cout << "obstacle_" << o_id << " find better choice." << endl;
						// if new volunteer is more faraway to obstacle
						if (pair_int_distance(obstacles[o_id].component[j], agent[id].position) > pair_int_distance(obstacles[o_id].component[j], agent[original_volunteer_id].position))
							continue;
						// leader has priority
						//if (agent[original_volunteer_id].leader)
						//	continue;
						// reset original volunteer's parameter
						delete_volunteer(o_id, j, original_volunteer_id);
						cout << "agent_" << original_volunteer_id << " replace by agent_" << id << endl;
					}
					add_volunteer(o_id, j, id);
					cout << "obstacle_" << o_id << " com_" << j << " find volunteer_" << id << endl;
					continue;
				}
				// if agent is volunteer but want to esacpe first
				/*if (willness <= d_threshold && agent[id].push_obstacle_id != -1)
				{
				cout << "agent_" << i << " don't wanna be volunteer." << endl;
				delete_volunteer(o_id, j, id);
				}*/
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::blocked_obstacle_findVolunteer(){

	cout << "blocked obstacle find volunteer." << endl;
	switch (agent_psychology.obstacle_moveType)
	{
	case CS_OBSTACLE_MANUAL_MOVE:
		manual_findVolunteers();
		break;
	case CS_OBSTACLE_AUTOMATIC_MOVE:
		automatic_findVolunteers();
		break;
	}
}

void CS_CELLULAR_AUTOMATA::automatic_findVolunteers(){

	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		obstacles[o_id].enough_volunteers = true;
		//check_volunteer_willness(o_id);
		// if obstacle has not enough volunteer
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			if (obstacles[o_id].volunteer_id[j] == -1)
			{
				obstacles[o_id].enough_volunteers = false;
				break;
			}
		}
		find_volunteers(o_id);
		/*find_guider(o_id);
		if (obstacles[o_id].hasLeader_guider)
		{
			cout << "obstacle_" << o_id << " has a leader guider." << endl;
			find_volunteers(o_id);
		}*/
	}
}

void CS_CELLULAR_AUTOMATA::manual_findVolunteers(){

	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		if (obstacles[o_id].arrive_destination)
			continue;
		obstacles[o_id].enough_volunteers = true;
		// if obstacle has not enough volunteer
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			if (obstacles[o_id].volunteer_id[j] == -1)
			{
				obstacles[o_id].enough_volunteers = false;
				break;
			}
		}
		find_guider(o_id);
		if (obstacles[o_id].hasLeader_guider)
		{
			for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
			{
				//cout << "leader find strength agent." << endl;
				if (!obstacles[o_id].hasLeader_guider)
				{
					//cout << "obstacle_" << o_id << "no leader" << endl;
					//system("pause");
				}
				//cout << "strength size: " << strength_agent.size() << endl;
				for (int v = 0; v < strength_agent.size(); v++)
				{
					//cout << v << endl;
					if (agent[strength_agent[v]].volunteer)
						continue;
					if (agent[strength_agent[v]].arrival)
						continue;
					// if obstacle already has a volunteer
					if (obstacles[o_id].volunteer_id[j] != -1)
					{
						// if obstacle have not enough volunteers
						if (!obstacles[o_id].enough_volunteers)
						{
							//cout << "obstacle_" << o_id << " has enough volunteers." << endl;
							continue;
						}
						// if new volunteer is more faraway to obstacle
						if (pair_int_distance(obstacles[o_id].component[j], agent[strength_agent[v]].position) > pair_int_distance(obstacles[o_id].component[j], agent[obstacles[o_id].volunteer_id[j]].position))
							continue;
						// leader has priority
						if (agent[strength_agent[v]].leader)
							continue;
						// reset original volunteer's parameter
						//cout << "obstacle_" << o_id << " has better choice." << endl;
						//system("pause");
						//obstacles[o_id].volunteer_id[j] = -1;
						agent[obstacles[o_id].volunteer_id[j]].push_obstacle_id = -1;
						agent[obstacles[o_id].volunteer_id[j]].volunteer = false;
						agent[obstacles[o_id].volunteer_id[j]].obstacle_component_id = -1;
					}
					// if obstacle is too far to volunteer
					if (pair_int_distance(obstacles[o_id].component[j], agent[strength_agent[v]].position) > VISIBILITY_DISTANCE)
						continue;
					// strength_agent[v] be a volunteer to move obstacle[o_id]
					obstacles[o_id].volunteer_id[j] = strength_agent[v];
					agent[strength_agent[v]].push_obstacle_id = o_id;
					agent[strength_agent[v]].volunteer = true;
					agent[strength_agent[v]].obstacle_component_id = j;
				}
			}
		}
	}
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		for (int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			cout << "volunteer: " << obstacles[o_id].volunteer_id[j] << endl;
			cout << "component id: " << agent[obstacles[o_id].volunteer_id[j]].obstacle_component_id << endl;
			cout << "willness: " << agent[obstacles[o_id].volunteer_id[j]].psychology.willness << endl;
		}
		cout << endl;
	}
}
// add agent a_id to obstacle o_id
void CS_CELLULAR_AUTOMATA::add_volunteer(int o_id, int vol_id, int a_id){

	obstacles[o_id].volunteer_id[vol_id] = a_id;
	agent[a_id].push_obstacle_id = o_id;
	agent[a_id].obstacle_component_id = vol_id;
	agent[a_id].volunteer = true;
	agent[a_id].beside_obstacle = false;
}
// delete agent a_id from obstacle o_id
// o_id, volunteer_id, agent_id
void CS_CELLULAR_AUTOMATA::delete_volunteer(int o_id, int vol_id, int a_id){

	obstacles[o_id].volunteer_id[vol_id] = -1;
	agent[a_id].push_obstacle_id = -1;
	agent[a_id].blocked_obstacle_id = -1;
	agent[a_id].obstacle_component_id = -1;
	agent[a_id].volunteer = false;
	agent[a_id].beside_obstacle = false;
	agent[a_id].mVolunteered = true;
}

void CS_CELLULAR_AUTOMATA::pushAgent_createSpace(int o_id){

	cout << "obstacle" << o_id << " push other agents to create the space." << endl;
	enum DIRECTION{ lateral, longitude };
	DIRECTION avoid_direction;
	if (obstacles[o_id].direction.first == 0)
	{
		avoid_direction = lateral;
	}
	if (obstacles[o_id].direction.second == 0)
	{
		avoid_direction = longitude;
	}
	// volunteers push agent
	for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		pair<int, int> direction = obstacles[o_id].direction;
		int dir_x = direction.first;
		int dir_z = direction.second;
		int v_id = obstacles[o_id].volunteer_id[i];
		int x = agent[v_id].position.first;
		int z = agent[v_id].position.second;
		int pos_x = x + dir_x;
		int pos_z = z + dir_z;
		int pre_agent_id;
		cout << "volunteer_x: " << pos_x << " volunteer_z: " << pos_z << endl;
		bool start = false;
		if (isValid(pos_x, pos_z))
		{
			while (true)
			{
				if (cell[pos_x][pos_z].occupied == 0)
				{
					if (start)
					{
						agent[pre_agent_id].position = PAIR_INT(pos_x, pos_z);
						agent[pre_agent_id].blockTheRoad = true;
						cell[pos_x][pos_z].occupied = 1;
						cell[pos_x][pos_z].occupant_id = pre_agent_id;
					}
					break;
				}
				cell[pos_x][pos_z].occupied = 0;
				int agent_id = cell[pos_x][pos_z].occupant_id;
				cout << "Agent, agent id: " << agent_id << endl;
				cout << "pos_x: " << pos_x << " pos_z: " << pos_z << endl;
				if (agent[agent_id].push_obstacle_id == o_id)
					break;
				pos_x += dir_x;
				pos_z += dir_z;
				if (cell[pos_x][pos_z].occupied == 0)
				{
					agent[agent_id].position = PAIR_INT(pos_x, pos_z);
					agent[agent_id].blockTheRoad = true;
					cell[pos_x][pos_z].occupied = 1;
					cell[pos_x][pos_z].occupant_id = agent_id;
					break;
				}
				if (cell[pos_x][pos_z].occupied == 1)
				{
					if (isValid(pos_x + dir_x, pos_z + dir_z))
					{
						pre_agent_id = cell[pos_x][pos_z].occupant_id;
						agent[agent_id].position = PAIR_INT(pos_x, pos_z);
						agent[agent_id].blockTheRoad = true;
						cell[pos_x][pos_z].occupied = 1;
						cell[pos_x][pos_z].occupant_id = agent_id;
						pos_x += dir_x;
						pos_z += dir_z;
					}
				}
				start = true;
			}
			/*while (cell[pos_x][pos_z].occupied == 1)
			{
			cell[pos_x][pos_z].occupied = 0;
			int agent_id = cell[pos_x][pos_z].occupant_id;
			cout << "Agent, agent id: " << agent_id << endl;
			cout << "pos_x: " << pos_x << " pos_z: " << pos_z << endl;
			if (agent[agent_id].push_obstacle_id == o_id)
			break;
			pos_x += dir_x;
			pos_z += dir_z;
			agent[agent_id].position = pair<int, int>(pos_x, pos_z);
			agent[agent_id].blockTheRoad = true;
			cell[pos_x][pos_z].occupied = 1;
			cell[pos_x][pos_z].occupant_id = agent_id;
			if (isValid(pos_x + dir_x, pos_z + dir_z))
			{
			pos_x += dir_x;
			pos_z += dir_z;
			}
			// if conflict with the wall
			if (!isValid(pos_x + dir_x, pos_z + dir_z))
			{
			switch (avoid_direction)
			{
			case lateral:
			if (pos_x > model->size / 2)
			dir_x = -1;
			else
			dir_x = 1;
			dir_z = 0;
			break;
			case longitude:
			if (pos_z > model->size / 2)
			dir_z = -1;
			else
			dir_z = 1;
			dir_x = 0;
			break;
			}
			pos_x += dir_x;
			pos_z += dir_z;
			}
			}*/
		}
	}
	// obstacle push agent
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		pair<int, int> direction = obstacles[o_id].direction;
		int dir_x = direction.first;
		int dir_z = direction.second;
		int x = obstacles[o_id].component[i].first;
		int z = obstacles[o_id].component[i].second;
		int pos_x = x + dir_x;
		int pos_z = z + dir_z;
		int pre_agent_id;
		cout << "obstalce_x: " << pos_x << " obstacle_z: " << pos_z << endl;
		bool start = false;
		if (isValid(pos_x, pos_z))
		{
			while (true)
			{
				if (cell[pos_x][pos_z].occupied == 0)
				{
					if (start)
					{
						agent[pre_agent_id].position = PAIR_INT(pos_x, pos_z);
						agent[pre_agent_id].blockTheRoad = true;
						cell[pos_x][pos_z].occupied = 1;
						cell[pos_x][pos_z].occupant_id = pre_agent_id;
					}
					break;
				}
				cell[pos_x][pos_z].occupied = 0;
				int agent_id = cell[pos_x][pos_z].occupant_id;
				cout << "Agent, agent id: " << agent_id << endl;
				cout << "pos_x: " << pos_x << " pos_z: " << pos_z << endl;
				if (agent[agent_id].push_obstacle_id == o_id)
					break;
				pos_x += dir_x;
				pos_z += dir_z;
				if (cell[pos_x][pos_z].occupied == 0)
				{
					agent[agent_id].position = PAIR_INT(pos_x, pos_z);
					agent[agent_id].blockTheRoad = true;
					cell[pos_x][pos_z].occupied = 1;
					cell[pos_x][pos_z].occupant_id = agent_id;
					break;
				}
				if (cell[pos_x][pos_z].occupied == 1)
				{
					if (isValid(pos_x + dir_x, pos_z + dir_z))
					{
						pre_agent_id = cell[pos_x][pos_z].occupant_id;
						agent[agent_id].position = PAIR_INT(pos_x, pos_z);
						agent[agent_id].blockTheRoad = true;
						cell[pos_x][pos_z].occupied = 1;
						cell[pos_x][pos_z].occupant_id = agent_id;
						pos_x += dir_x;
						pos_z += dir_z;
					}
				}
				start = true;
			}
			/*while (cell[pos_x][pos_z].occupied == 1)
			{
			cell[pos_x][pos_z].occupied = 0;
			int agent_id = cell[pos_x][pos_z].occupant_id;
			cout << "obstacle, agent id: " << agent_id << endl;
			cout << "pos_x: " << pos_x << " pos_z: " << pos_z << endl;
			if (agent[agent_id].push_obstacle_id == o_id)
			break;
			pos_x += dir_x;
			pos_z += dir_z;
			agent[agent_id].position = pair<int, int>(pos_x, pos_z);
			agent[agent_id].blockTheRoad = true;
			cell[pos_x][pos_z].occupied = 1;
			cell[pos_x][pos_z].occupant_id = agent_id;
			if (isValid(pos_x + dir_x, pos_z + dir_z))
			{
			pos_x += dir_x;
			pos_z += dir_z;
			}
			// if conflict with the wall
			if (!isValid(pos_x + dir_x, pos_z + dir_z))
			{
			switch (avoid_direction)
			{
			case lateral:
			if (pos_x > model->size / 2)
			dir_x = -1;
			else
			dir_x = 1;
			dir_z = 0;
			break;
			case longitude:
			if (pos_z > model->size / 2)
			dir_z = -1;
			else
			dir_z = 1;
			dir_x = 0;
			break;
			}
			pos_x += dir_x;
			pos_z += dir_z;
			}
			}*/
		}
	}
}

void CS_CELLULAR_AUTOMATA::obstacle_timer_accumulation(int o_id){

	//cout << "obstacle_" << o_id << " timer accumulation." << endl;
	//obstacles[o_id].timer++;
	//cout << obstacles[o_id].timer << endl;
	for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		if (obstacles[o_id].volunteer_id[i] == -1)
			continue;
		if (!agent[obstacles[o_id].volunteer_id[i]].beside_obstacle)
			continue;
		if (agent[obstacles[o_id].volunteer_id[i]].arrival)
			continue;
		agent[obstacles[o_id].volunteer_id[i]].mPushTimeCounter++;
		//cout << agent[i].mPushTimeCounter << endl;
		obstacles[o_id].timer++;
		//cout << obstacles[o_id].timer << endl;
	}
}

void CS_CELLULAR_AUTOMATA::reset_obstacle_counter(int o_id){

	obstacles[o_id].up_count = 0;
	obstacles[o_id].right_count = 0;
	obstacles[o_id].down_count = 0;
	obstacles[o_id].left_count = 0;
}

void CS_CELLULAR_AUTOMATA::add_blocked_obstacle(int o_id){
	
	// update normal obstacle pool, pop obstalce id from normal vector
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		if (blocked_obstacles[i] == o_id)
		{
			/*for (unsigned int j = 0; j < obstacles[blocked_obstacles[i]].volunteer_id.size(); j++)
			{
				int id = obstacles[blocked_obstacles[i]].volunteer_id[j];
				if (id == -1)
					continue;
				delete_volunteer(o_id, j, id);
			}
			obstacles[o_id].block = false;
			obstacles[o_id].block_exit = false;
			obstacles[o_id].arrive_destination = false;
			obstacles[o_id].enough_volunteers = false;
			//obstacles[o_id].moveAlready = true;*/
			return;
		}
	}
	int pop_id = normal_obstacle[normal_obstacle.size() - 1];
	normal_obstacle.pop_back();
	if (normal_obstacle.size() > 0)
	{
		int new_index = obstacles[o_id].pool_index;
		normal_obstacle[new_index] = pop_id;
		//cout << "new index: " << new_index << endl;
		//cout << "pop id: " << pop_id << endl;
		obstacles[pop_id].pool_index = new_index;
	}
	// check obstacle in pool
	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int id = blocked_obstacles[i];
		if (id == o_id)
			return;
	}
	// update blocked obstacle pool
	obstacles[o_id].block = true;
	obstacles[o_id].arrive_destination = false;
	obstacles[o_id].enough_volunteers = false;
	//cout << "normal_obstacle size: " << normal_obstacle.size() << endl;
	//cout << "obstalce pool index: " << obstacles[o_id].pool_index << endl;
	//normal_obstacle.erase(normal_obstacle.begin() + obstacles[o_id].pool_index);
	obstacles[o_id].pool_index = blocked_obstacles.size();
	blocked_obstacles.push_back(o_id);
}

void CS_CELLULAR_AUTOMATA::add_normal_obstacle(int o_id){

	// update blocked obstalce pool
	for (int i = 0; i < normal_obstacle.size(); i++)
	{
		if (normal_obstacle[i] == o_id)
		{
			return;
		}
	}
	int pop_id = blocked_obstacles[blocked_obstacles.size() - 1];
	//cout << blocked_obstacles.size() << endl;
	blocked_obstacles.pop_back();
	//cout << blocked_obstacles.size() << endl;
	if (blocked_obstacles.size() > 0)
	{
		int new_index = obstacles[o_id].pool_index;
		blocked_obstacles[new_index] = pop_id;
		//cout << "new index: " << new_index << endl;
		//cout << "pop id: " << pop_id << endl;
		obstacles[pop_id].pool_index = new_index;
	}
	// delete volunteer
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int id = obstacles[o_id].volunteer_id[i];
		if (id == -1)
			continue;
		agent[id].blocked_obstacle_id = -1;
		agent[id].volunteer = false;
		agent[id].obstacle_component_id = -1;
		agent[id].push_obstacle_id = -1;
		agent[id].beside_obstacle = false;
	}
	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		for (int j = 0; j < obstacles[o_id].candidate_id[i].size(); j++)
		{
			int id = obstacles[o_id].candidate_id[i][j];
			agent[id].blocked_obstacle_id = -1;
			agent[id].volunteer = false;
			agent[id].obstacle_component_id = -1;
			agent[id].push_obstacle_id = -1;
		}
	}
	/*blocked_obstacles.pop_back();
	if (blocked_obstacles.size() > 0)
	{
		// delete volunteer
		for (int i = 0; i < obstacles[o_id].component.size(); i++)
		{
			int id = obstacles[o_id].volunteer_id[i];
			if (id == -1)
				continue;
			agent[id].blocked_obstacle_id = -1;
			agent[id].volunteer = false;
			agent[id].obstacle_component_id = -1;
			agent[id].push_obstacle_id = -1;
			agent[id].beside_obstacle = false;
		}
		for (int i = 0; i < obstacles[o_id].component.size(); i++)
		{
			for (int j = 0; j < obstacles[o_id].candidate_id[i].size(); j++)
			{
				int id = obstacles[o_id].candidate_id[i][j];
				agent[id].blocked_obstacle_id = -1;
				agent[id].volunteer = false;
				agent[id].obstacle_component_id = -1;
				agent[id].push_obstacle_id = -1;
			}
			system("pause");
		}
		int new_index = obstacles[o_id].pool_index;
		blocked_obstacles[new_index] = pop_id;
		obstacles[pop_id].pool_index = new_index;
	}
	// check obstacle in pool
	for (int i = 0; i < normal_obstacle.size(); i++)
	{
		int id = normal_obstacle[i];
		if (id == o_id)
			return;
	}*/
	// update normal obstacle pool
	obstacles[o_id].block = false;
	obstacles[o_id].block_exit = false;
	obstacles[o_id].arrive_destination = false;
	obstacles[o_id].enough_volunteers = false;
	obstacles[o_id].moveAlready = true;
	//blocked_obstacles.erase(blocked_obstacles.begin() + obstacles[o_id].pool_index);
	obstacles[o_id].pool_index = normal_obstacle.size();
	normal_obstacle.push_back(o_id);
}

void CS_CELLULAR_AUTOMATA::updateDensityAroundObstacle(){

	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		reset_obstacle_counter(o_id);
		for (int j = 0; j < obstacles[o_id].component.size(); j++)
		{
			int com_x = obstacles[o_id].component[j].first;
			int com_z = obstacles[o_id].component[j].second;
			for (int x = -VISIBILITY_DISTANCE; x < VISIBILITY_DISTANCE; x++)
			{
				for (int z = -VISIBILITY_DISTANCE; z < VISIBILITY_DISTANCE; z++)
				{
					if (!isValid(com_x + x, com_z + z))
						continue;
					if (cell[com_x + x][com_z + z].check)
						continue;
					if (cell[com_x + x][com_z + z].mark)
						continue;
					//if (cell[com_x + x][com_z + z].obstacle)
					//	continue;
					if (cell[com_x + x][com_z + z].clean_sFF < cell[com_x][com_z].clean_sFF)
						continue;
					cell[com_x + x][com_z + z].check = true;
					if (cell[com_x + x][com_z + z].occupied == 0)
					{
						cell[com_x + x][com_z + z].mark = true;
						if (com_z + z > obstacles[o_id].highest_z)
						{
							obstacles[o_id].up_count++;
						}
						else if (com_z + z < obstacles[o_id].lowest_z)
						{
							obstacles[o_id].down_count++;
						}
						else if (com_x + x < obstacles[o_id].lowest_x)
						{
							obstacles[o_id].left_count++;
						}
						else if (com_x + x > obstacles[o_id].highest_x)
						{
							obstacles[o_id].right_count++;
						}
					}
					if (cell[com_x + x][com_z + z].occupied == 1)
					{
						cell[com_x + x][com_z + z].mark = true;
					}
					/*if (cell[com_x + x][com_z + z].obstacle)
					{
					cell[com_x + x][com_z + z].mark = true;
					occupied++;
					if (com_z + z > obstacles[o_id].highest_z)
					obstacles[o_id].up_count -= 5;
					else if (com_z + z < obstacles[o_id].lowest_z)
					obstacles[o_id].down_count -= 5;
					else if (com_z + x < obstacles[o_id].lowest_x)
					obstacles[o_id].left_count -= 5;
					else if (com_z + x > obstacles[o_id].highest_x)
					obstacles[o_id].right_count -= 5;
					}*/
				}
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::constructAFF(int o_id){

	float first_level = 12, second_level = 8, third_level = 4;
	float first_level_dis = 3.0f, second_level_dis = 6.0f, third_level_dis = 9.0f;
	for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
	{
		int ox = obstacles[o_id].component[i].first;
		int oz = obstacles[o_id].component[i].second;
		int dx = obstacles[o_id].direction.first;
		int dz = obstacles[o_id].direction.second;
		for (int _x = -first_level_dis; _x <= first_level_dis; _x++)
		{
			for (int _z = -first_level_dis; _z <= first_level_dis; _z++)
			{
				int x = ox + _x;
				int z = oz + _z;
				if (!isValid(x, z))
					continue;
				float dis = abs(_x) + abs(_z);
				switch (obstacles[o_id].type)
				{
				case 0:
					
					break;
				case 1:
					break;
				}
			}
		}
		for (int _x = -third_level_dis; _x <= third_level_dis; _x++)
		{
			for (int _z = -third_level_dis; _z <= third_level_dis; _z++)
			{
				int x = ox + _x;
				int z = oz + _z;
				if (!isValid(x, z))
					continue;
				int dx = obstacles[o_id].direction.first;
				int dz = obstacles[o_id].direction.second;
				//cout << dx << " " << dz << endl;
				if (dx < 0)
				{
					if (_x > 0)
						continue;
				}
				if (dx > 0)
				{
					if (_x < 0)
						continue;
				}
				if (dz < 0)
				{
					if (_z > 0)
						continue;
				}
				if (dz > 0)
				{
					if (_z < 0)
						continue;
				}
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
		}
	}
}

void CS_CELLULAR_AUTOMATA::findOptimalDestination(int o_id){

	int px0 = obstacles[o_id].component[0].first;
	int pz0 = obstacles[o_id].component[0].second;
	vector<PAIR_INT> candidateDes;
	vector<float> density;
	vector<PAIR_INT> o(obstacles[o_id].component.size());
	for (int x = -model->mMoveDistance; x <= model->mMoveDistance; x++)
	{
		for (int z = -model->mMoveDistance; z <= model->mMoveDistance; z++)
		{
			if (isValid(px0 + x, pz0 + z) && isBesideWall(px0 + x, pz0 + z)/* && findNearestDistanceToExit(o) >= model->mMoveDistance*/ && !isObstacle(px0 + x, pz0 + z) && !isExit(px0 + x, pz0 + z))
			{
				for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
				{
					int ox = obstacles[o_id].component[i].first + x;
					int oz = obstacles[o_id].component[i].second + z;
					o[i] = PAIR_INT(ox, oz);
				}
				bool goodCandidate = true;
				for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
				{
					int ox = obstacles[o_id].component[i].first + x;
					int oz = obstacles[o_id].component[i].second + z;
					if (isObstacle(ox, oz) && !isBelongSameObstacle(o_id, ox, oz) && findNearestDistanceToExit(o) >= model->mMoveDistance)
					{
						goodCandidate = false;
						break;
					}
					o[i] = PAIR_INT(ox, oz);
				}
				if (goodCandidate)
					candidateDes.push_back(PAIR_INT(x, z));
				/*for (unsigned int i = 0; i < obstacles[o_id]..size(); i++)
				{
					obstacles[o_id].moveDestination[i].first = obstacles[o_id].component[i].first + x;
					obstacles[o_id].moveDestination[i].second = obstacles[o_id].component[i].second + z;
					cout << obstacles[o_id].moveDestination[i].first << " " << obstacles[o_id].moveDestination[i].second << endl;
				}
				obstacles[o_id].mHaveDes = true;
				return;*/
			}
		}
	}
	//cout << "obstacle " << o_id << " candidateDes: " << candidateDes.size() << endl;
	for (unsigned int j = 0; j < candidateDes.size(); j++)
	{
		float count = 0, total = 0;
		reset_cellMark();
		for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
		{
			for (int x = -6; x <= 6; x++)
			{
				for (int z = -6; z <= 6; z++)
				{
					int ox = obstacles[o_id].component[i].first + x + candidateDes[j].first;
					int oz = obstacles[o_id].component[i].second + z + candidateDes[j].second;
					if (!isValid(ox, oz))
						continue;
					if (cell[ox][oz].mark)
						continue;
					if (isObstacle(ox, oz))
						continue;
					cell[ox][oz].mark = true;
					if (cell[ox][oz].occupied == 1)
						count++;
					if (isObstacle(ox, oz) && isBelongSameObstacle(o_id, ox, oz))
						count++;
					total++;
				}
			}
		}
		density.push_back(count / total);
		//cout << "pos: " << candidateDes[j].first << " " << candidateDes[j].second << endl;
		//cout << "density: " << count / total << endl;
	}
	int min_index = -1;
	float min_density = FLT_MAX;
	for (unsigned int i = 0; i < density.size(); i++)
	{
		if (density[i] < min_density)
		{
			min_density = density[i];
			min_index = i;
		}
	}
	if (min_index == -1)
		return;
	for (unsigned int i = 0; i < obstacles[o_id].moveDestination.size(); i++)
	{
		obstacles[o_id].moveDestination[i].first = obstacles[o_id].component[i].first;// -model->mDisToExit / 2;
		obstacles[o_id].moveDestination[i].second = obstacles[o_id].component[i].second + (model->mDisToExit );
		//obstacles[o_id].moveDestination[i].first = obstacles[o_id].component[i].first + candidateDes[min_index].first;
		//obstacles[o_id].moveDestination[i].second = obstacles[o_id].component[i].second + candidateDes[min_index].second;
		//cout << obstacles[o_id].moveDestination[i].first << " " << obstacles[o_id].moveDestination[i].second << endl;
	}
	obstacles[o_id].mHaveDes = true;
	return;
}

bool CS_CELLULAR_AUTOMATA::isBesideWall(int x, int z){

	for (unsigned int i = 0; i < wall.size(); i++)
	{
		int dis_x = abs(x - wall[i].first);
		int dis_z = abs(z - wall[i].second);
		if (dis_x + dis_z == 1)
			return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::isFarEnough(int dis_x, int dis_z){

	if (abs(dis_x) + abs(dis_z) >= model->mMoveDistance)
	{
		return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::isObstacle(int x, int z){

	if (cell[x][z].obstacle)
		return true;
	return false;
}

bool CS_CELLULAR_AUTOMATA::isBelongSameObstacle(int o_id, int x, int z){

	if (cell[x][z].obstacle_id == o_id)
		return true;
	return false;
}

bool CS_CELLULAR_AUTOMATA::isBeoccupied(int o_id){

	if (o_id == -1)
		return false;
	for (unsigned int i = 0; i < obstacles[o_id].moveDestination.size(); i++)
	{
		int px = obstacles[o_id].moveDestination[i].first;
		int pz = obstacles[o_id].moveDestination[i].second;
		if (isObstacle(px, pz) && !isBelongSameObstacle(o_id, px, pz))
			return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::isExit(int x, int z){
	
	if (cell[x][z].exit)
		return true;
	return false;
}

bool CS_CELLULAR_AUTOMATA::check_obstacle_weight(int o_id){

	for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		int id = obstacles[o_id].volunteer_id[i];
		if (id == -1)
			continue;
		if (!agent[id].beside_obstacle)
			continue;
		agent[id].direction = local;
	}
	int timer = 0;
	for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		int id = obstacles[o_id].volunteer_id[i];
		//cout << id << endl;
		if (id == -1)
			continue;
		if (!agent[id].beside_obstacle)
			continue;
		if (agent[id].arrival)
			continue;
		timer += agent[id].mPushTimeCounter;
	}
	//int timer = obstacles[o_id].timer;
	int weight = obstacles[o_id].weight;
	if (timer >= weight)
	{
		//cout << "timer: " << timer << endl;
		//cout << "weight: " << weight << endl;
		timer -= weight;
		for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
		{
			int id = obstacles[o_id].volunteer_id[i];
			if (id == -1)
				continue;
			if (!agent[id].beside_obstacle)
				continue;
			if (agent[id].arrival)
				continue;
			agent[id].mPushTimeCounter = 0;
		}
		obstacles[o_id].timer = timer;
		return true;
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::check_obstacleID_exist(int o_id){

	for (int i = 0; i < blocked_obstacles.size(); i++)
	{
		if (blocked_obstacles[i] == o_id)
			return false;
	}
	return true;
}

bool CS_CELLULAR_AUTOMATA::check_obstacle_movement_block(int o_id){

	for (int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int o_x = obstacles[o_id].component[i].first;
		int o_z = obstacles[o_id].component[i].second;
		int d_x = obstacles[o_id].direction.first;
		int d_z = obstacles[o_id].direction.second;
		if (!isValid(o_x + d_x, o_z + d_z))
			return true;
		if (cell[o_x + d_x][o_z + d_z].obstacle)
		{
			if (cell[o_x + d_x][o_z + d_z].obstacle_id != o_id)
				return true;
		}
		if (cell[o_x + d_x][o_z + d_z].occupied == 1)
		{
			int a_id = cell[o_x + d_x][o_z + d_z].occupant_id;
			if (a_id == -1)
				continue;
			if (agent[a_id].blocked_obstacle_id != o_id || !agent[a_id].beside_obstacle)
				return true;
		}
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::check_volunteer_movement_block(int id){

	int x = agent[id].position.first;
	int z = agent[id].position.second;
	int d_x = obstacles[agent[id].blocked_obstacle_id].direction.first;
	int d_z = obstacles[agent[id].blocked_obstacle_id].direction.second;
	if (!isValid(x + d_x, z + d_z))
	{
		return true;
	}
	if (cell[x + d_x][z + d_z].obstacle)
	{
		if (cell[x + d_x][z + d_z].obstacle_id != agent[id].blocked_obstacle_id)
		{
			//cout << "OBBBBB: " << cell[x + d_x][z + d_z].obstacle_id << endl;
			return true;
		}
	}
	if (cell[x + d_x][z + d_z].occupied == 1)
	{
		int a_id = cell[x + d_x][z + d_z].occupant_id;
		if (agent[a_id].blocked_obstacle_id != agent[id].blocked_obstacle_id || !agent[a_id].beside_obstacle)
		{
			//cout << "AGGGGG: " << a_id << endl;
			return true;
		}
	}
	return false;
}

bool CS_CELLULAR_AUTOMATA::check_moveToLowSFF(int o_id){

	int o_x = obstacles[o_id].component[0].first;
	int o_z = obstacles[o_id].component[0].second;
	int d_x = obstacles[o_id].direction.first;
	int d_z = obstacles[o_id].direction.second;
	if (d_x == 1 && obstacles[o_id].left)
		return false;
	if (d_x == -1 && obstacles[o_id].right)
		return false;
	if (d_z == 1 && obstacles[o_id].down)
		return false;
	if (d_z == -1 && obstacles[o_id].up)
		return false;
	//if (cell[o_x][o_z].clean_sFF > cell[o_x + d_x][o_z + d_z].clean_sFF)
	//	return false;
	return true;
}

Direction CS_CELLULAR_AUTOMATA::obstalce_move_direction(int o_id){

	switch (obstacles[o_id].type)
	{
		// vertical
	case 0:
		
		break;
		// horizontal
	case 1:
		break;
	}
	int counter_buffer[4] = { obstacles[o_id].up_count, obstacles[o_id].right_count, obstacles[o_id].down_count, obstacles[o_id].left_count };
	cout << "obstacle_" << o_id << ": (" << counter_buffer[0] << ", " << counter_buffer[1] << ", " << counter_buffer[2] << ", " << counter_buffer[3] << ")" << endl;
	sort(counter_buffer, counter_buffer + 4);
	cout << "sort obstacle_" << o_id << ": (" << counter_buffer[0] << ", " << counter_buffer[1] << ", " << counter_buffer[2] << ", " << counter_buffer[3] << ")" << endl;
	Direction direction = local;
	obstacles[o_id].pre_direction = obstacles[o_id].direction;
	if (counter_buffer[3] == obstacles[o_id].up_count)
	{
		direction = _up;
		obstacles[o_id].direction = PAIR_INT(0, 1);
	}
	else if (counter_buffer[3] == obstacles[o_id].right_count)
	{
		direction = _right;
		obstacles[o_id].direction = PAIR_INT(1, 0);
	}
	else if (counter_buffer[3] == obstacles[o_id].down_count)
	{
		direction = _down;
		obstacles[o_id].direction = PAIR_INT(0, -1);
	}
	else if (counter_buffer[3] == obstacles[o_id].left_count)
	{
		direction = _left;
		obstacles[o_id].direction = PAIR_INT(-1, 0);
	}
	else
	{
		cout << "move count error" << endl;;
	}
	// detect which direction block
	if (counter_buffer[0] == obstacles[o_id].up_count)
	{
		obstacles[o_id].block_direction = 0;
	}
	else if (counter_buffer[0] == obstacles[o_id].right_count)
	{
		obstacles[o_id].block_direction = 1;
	}
	else if (counter_buffer[0] == obstacles[o_id].down_count)
	{
		obstacles[o_id].block_direction = 2;
	}
	else if (counter_buffer[0] == obstacles[o_id].left_count)
	{
		obstacles[o_id].block_direction = 3;
	}
	return direction;
}

float CS_CELLULAR_AUTOMATA::get_obstacle_density(int o_id){


	int occupied = 0;
	int total = 0;
	reset_obstacle_counter(o_id);
	for (int j = 0; j < obstacles[o_id].component.size(); j++)
	{
		int com_x = obstacles[o_id].component[j].first;
		int com_z = obstacles[o_id].component[j].second;
		for (int x = -VISIBILITY_DISTANCE; x < VISIBILITY_DISTANCE; x++)
		{
			for (int z = -VISIBILITY_DISTANCE; z < VISIBILITY_DISTANCE; z++)
			{
				if (!isValid(com_x + x, com_z + z))
					continue;
				if (cell[com_x + x][com_z + z].check)
					continue;
				if (cell[com_x + x][com_z + z].mark)
					continue;
				//if (cell[com_x + x][com_z + z].obstacle)
				//	continue;
				if (cell[com_x + x][com_z + z].clean_sFF < cell[com_x][com_z].clean_sFF)
				{
					if (com_z + z > obstacles[o_id].highest_z)
					{
						obstacles[o_id].up_count -= 999;
					}
					else if (com_z + z < obstacles[o_id].lowest_z)
					{
						obstacles[o_id].down_count -= 999;
					}
					else if (com_x + x < obstacles[o_id].lowest_x)
					{
						obstacles[o_id].left_count -= 999;
					}
					else if (com_x + x > obstacles[o_id].highest_x)
					{
						obstacles[o_id].right_count -= 999;
					}
					continue;
				}
				total++;
				cell[com_x + x][com_z + z].check = true;
				if (cell[com_x + x][com_z + z].occupied == 0)
				{
					cell[com_x + x][com_z + z].mark = true;
					if (com_z + z > obstacles[o_id].highest_z)
					{
						obstacles[o_id].up_count++;
					}
					else if (com_z + z < obstacles[o_id].lowest_z)
					{
						obstacles[o_id].down_count++;
					}
					else if (com_x + x < obstacles[o_id].lowest_x)
					{
						obstacles[o_id].left_count++;
					}
					else if (com_x + x > obstacles[o_id].highest_x)
					{
						obstacles[o_id].right_count++;
					}
				}
				if (cell[com_x + x][com_z + z].occupied == 1)
				{
					cell[com_x + x][com_z + z].mark = true;
					occupied++;
				}
				//int counter_buffer[4] = { obstacles[o_id].up_count, obstacles[o_id].right_count, obstacles[o_id].down_count, obstacles[o_id].left_count };
				//cout << "obstacle_" << o_id << ": (" << counter_buffer[0] << ", " << counter_buffer[1] << ", " << counter_buffer[2] << ", " << counter_buffer[3] << ")" << endl;
				/*if (cell[com_x + x][com_z + z].obstacle)
				{
					cell[com_x + x][com_z + z].mark = true;
					occupied++;
					if (com_z + z > obstacles[o_id].highest_z)
						obstacles[o_id].up_count -= 5;
					else if (com_z + z < obstacles[o_id].lowest_z)
						obstacles[o_id].down_count -= 5;
					else if (com_z + x < obstacles[o_id].lowest_x)
						obstacles[o_id].left_count -= 5;
					else if (com_z + x > obstacles[o_id].highest_x)
						obstacles[o_id].right_count -= 5;
				}*/
			}
		}
	}
	//cout << float(occupied) / total << endl;
	return float(occupied) / (total+0.0001f);
}