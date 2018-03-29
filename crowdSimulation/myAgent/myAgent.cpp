#include "../myCellularAutomata/cellular_automata.h"
#include <algorithm>

#define SCALER 10.0f
#define OFFSET 5.0f

void AGENT::computeWillness(){

	float alpha = 1.5f;


	psychology.willness = (alpha * (1 - anxiety) + (2 - alpha) * agent_personality) / 2;
}

float AGENT::computePanicConscientiousnessFunction(){

	if (mPersonality.conscientiousness >= 0)
	{
		return -2 * mPersonality.conscientiousness + 2;
	}
	return 0.0f;
}

void AGENT::draw_direction(int s){

	int size = s;
	int x = position.first;
	int z = position.second;
	vector3 ori, dir;
	vector3 left_line_midpoint, right_line_midpoint;
	switch (direction)
	{
	case _up:
		//ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _right:
		//ori = vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _down:
		//ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _left:
		//ori = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _topRight:
		//ori = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.75f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.75f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _topLeft:
		//ori = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.25f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.75f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _downRight:
		//ori = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.75f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.25f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case _downLeft:
		//ori = vector3((x + 1 - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.5f - size / 2) * SCALER - OFFSET);
		dir = vector3((x + 0.25f - size / 2) * SCALER - OFFSET, -0.1f, (z + 0.25f - size / 2) * SCALER - OFFSET);
		left_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET)) / 2;
		right_line_midpoint = (dir + vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z + 1 - size / 2) * SCALER - OFFSET)) / 2;
		break;
	case local:
		break;
	}
	//ori = vector3((x + 0.5f - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET);
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	//ori = vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET);
	glBegin(GL_LINES);
	//ori = vector3((x - size / 2) * SCALER - OFFSET, -0.1f, (z - size / 2) * SCALER - OFFSET);
	pl_glVertex3f(ori);
	pl_glVertex3f(dir);
	//pl_glVertex3f(dir);
	//pl_glVertex3f(left_line_midpoint);
	//pl_glVertex3f(dir);
	//pl_glVertex3f(right_line_midpoint);
	glEnd();
}

void AGENT::receiveWillingFromCrowd(){

	for (int i = 0; i < visible_agentID.size(); i++)
	{
		
	}
}

void AGENT::receiveEmotionFromVisibleAgent(){


}

void AGENT::reset(){

	arrival = false;
	blockTheRoad = false;
	close_obstalce = false;
	start_time = clock();
	escape_time = clock();
	guider = false;
	click = false;
	path.clear();
	stop_guide = false;
	be_leaded = false;
	volunteer = false;
	beside_obstacle = false;
	finish_task = false;
	compressive_leader = false;
	strength = false;
	add_sFF = false;
	bystander = false;
	mVolunteered = false;
	mGuider = false;
	mBlockWay = false;
	mSawExit = false;
	comity = false;
	mStayInPlace = false;
	cur_click = false;
	height = 160;
	obstacle_component_id = -1;
	push_obstacle_id = -1;
	anxiety = 0.0f;
	anxiety_delta = 0.0f;
	escapeTime = 0.0f;
	direction = local;
	blocked_obstacle_id = -1;
	empathy = (double)rand() / RAND_MAX;
	pre_sFF = 0.0f;
	stay_counter = 0;
	vector<int>().swap(visible_agentID);
	vector<PAIR_INT>().swap(visible_area);
	vector<float>().swap(anxiety_variation);
	vector<float>().swap(visible_area_advancesFF);
	visible_agentID.clear();
	visible_area.clear();
	anxiety_variation.clear();
	visible_area_advancesFF.clear();
	mNeighboringAgent.clear();
	mAvoidObstacle.clear();
	memberID.clear();
	for (unsigned int i = 0; i < remain_anxiety.size(); i++)
	{
		remain_anxiety[i] = 0;
	}
	mServeObstacleID = -1;
	block_pos = PAIR_INT(-1, -1);
	mPushTimeCounter = 0;
	currentVisibleBlockedExit = -1;
	mVolunteered = false;
}

void AGENT::insertAnxiety(float anx){

	vector<float> buffer = remain_anxiety;
	for (unsigned int i = 1; i < remain_anxiety.size(); i++)
	{
		buffer[i] = remain_anxiety[i - 1];
	}
	buffer[0] = anx;
	remain_anxiety = buffer;
	buffer.clear();
}

void CS_CELLULAR_AUTOMATA::findGuidePosition(int o_id, int a_id){

	for (unsigned int i = 0; i < obstacles[o_id].moveDestination.size(); i++)
	{
		PAIR_INT pos = obstacles[o_id].moveDestination[i];
		PAIR_INT guide_pos;
		PAIR_INT obstacle_moveDir = obstacles[o_id].mMoveDir;
		float min_dis = FLT_MAX;
		for (int x = -GUIDE_DIS; x <= GUIDE_DIS; x++)
		{
			for (int z = -GUIDE_DIS; z <= GUIDE_DIS; z++)
			{
				if (!isValid(pos.first + x, pos.second + z))
					continue;
				if (!isGuiderValidArea(o_id, pos.first + x, pos.second + z))
					continue;
				if (x == 0 && z == 0)
					continue;
				if (findNearestDistanceToExit(o_id) > cell[pos.first + x][pos.second + z].sFF)
					continue;
				if (cell[pos.first + x][pos.second + z].sFF < min_dis)
				{
					min_dis = cell[pos.first + x][pos.second + z].sFF;
					guide_pos = PAIR_INT(pos.first + x, pos.second + z);
				}
			}
		}
	}
}

int CS_CELLULAR_AUTOMATA::findNearestObstacleComponentID(int o_id, int a_id){

	float min_dis = FLT_MAX;
	int min_com = -1;
	for (unsigned int i = 0; i < obstacles[o_id].component.size(); i++)
	{
		int id = obstacles[o_id].volunteer_id[i];
		if (id != -1)
			continue;
		float dis = pair_int_distance(obstacles[o_id].component[i], agent[a_id].position);
		if (dis < min_dis)
		{
			min_dis = dis;
			min_com = i;
		}
	}
	return min_com;
}

void CS_CELLULAR_AUTOMATA::decideBehavior(int id){

	
}

void CS_CELLULAR_AUTOMATA::decideToBeVolunteer(int id){

	for (unsigned int i = 0; i < blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		obstacles[o_id].enough_volunteers = true;
		for (unsigned int j = 0; j < obstacles[o_id].volunteer_id.size(); j++)
		{
			if (obstacles[o_id].volunteer_id[j] == -1)
			{
				obstacles[o_id].enough_volunteers = false;
				break;
			}
		}
	}
	//agent[id].volunteer = true;
	int o_id = agent[id].blocked_obstacle_id;
	float min_dis = FLT_MAX;
	int min_com = -1;
	if (agent_psychology.obstacle_moveType == CS_OBSTACLE_MANUAL_MOVE)
	{
		bool flg = false;
		for (unsigned int i = 0; i < blocked_obstacles.size(); i++)
		{
			if (o_id == blocked_obstacles[i])
				flg = true;
		}
		if (!flg)
			return;
	}
	/*for (unsigned int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
	{
		int a_id = obstacles[o_id].volunteer_id[i];
		if (a_id != -1)
			continue;
		float dis = pair_int_distance(obstacles[o_id].component[i], agent[id].position);
		if (dis < min_dis)
		{
			min_dis = dis;
			min_com = i;
		}
	}*/
	min_com = findNearestObstacleComponentID(o_id, id);
	float probability = (float)rand() / RAND_MAX;
	//cout << "willness: " << agent[id].psychology.willness << endl;
	//cout << obstacles[o_id].mWillThreshold << endl;
	if (/*probability < agent[id].psychology.willness*/agent[id].psychology.willness >= obstacles[o_id].mWillThreshold && min_com != -1 && !agent[id].volunteer && check_groupWill(id, o_id))
	{
		if (obstacles[o_id].enough_volunteers)
			return;
		/*agent[id].volunteer = true;
		int o_id = agent[id].blocked_obstacle_id;
		float min_dis = FLT_MAX;
		int min_com = -1;
		for (int i = 0; i < obstacles[o_id].volunteer_id.size(); i++)
		{
			int a_id = obstacles[o_id].volunteer_id[i];
			if (a_id != -1)
				continue;
			float dis = pair_int_distance(obstacles[o_id].component[i], agent[id].position);
			if (dis < min_dis)
			{
				min_dis = dis;
				min_com = i;
			}
		}
		if (min_com == -1)
		{
			//cout << id << endl;
			//system("pause");
			agent[id].volunteer = false;
			agent[id].obstacle_component_id = -1;
			agent[id].blocked_obstacle_id = -1;
			agent[id].push_obstacle_id = -1;
			return;
		}*/
		agent[id].volunteer = true;
		agent[id].obstacle_component_id = min_com;
		obstacles[o_id].volunteer_id[min_com] = id;
		int gID = agent[id].group_id;
		for (unsigned int i = 0; i < agent_group[gID].member.size(); i++)
		{
			int member_min_com = -1;
			int a_id = agent_group[gID].member[i];
			if (a_id == id)
				continue;
			member_min_com = findNearestObstacleComponentID(o_id, a_id);
			if (member_min_com != -1)
			{
				agent[a_id].volunteer = true;
				agent[a_id].obstacle_component_id = member_min_com;
				obstacles[o_id].volunteer_id[member_min_com] = a_id;
			}
			else
			{
				if (agent[a_id].mGuider)
					continue;
				agent[a_id].mGuider = true;
				agent[a_id].blocked_obstacle_id = o_id;
				mGuiderID.push_back(a_id);
				findGuidePosition(o_id, a_id);
			}
		}
		/*bool repeat = false;
		for (int i = 0; i < obstacles[o_id].candidate_id[min_com].size(); i++)
		{
			int a_id = obstacles[o_id].candidate_id[min_com][i];
			if (a_id == id)
				repeat = true;
		}
		if (!repeat)
			obstacles[o_id].candidate_id[min_com].push_back(id);*/
	}
	//if (/*probability > agent[id].psychology.willness*/agent[id].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
	//{
	//	if (agent[id].volunteer)
	//	{
	//		int ob_id = agent[id].blocked_obstacle_id;
	//		int com_id = agent[id].obstacle_component_id;
	//		for (int i = 0; i < obstacles[ob_id].candidate_id[com_id].size(); i++)
	//		{
	//			if (id == obstacles[ob_id].candidate_id[com_id][i])
	//			{
	//				obstacles[ob_id].candidate_id[com_id].erase(obstacles[ob_id].candidate_id[com_id].begin() + i);
	//			}
	//		}
	//	}
	//	//cout << id << endl;
	//	//cout << agent[id].blocked_obstacle_id << " " << agent[id].obstacle_component_id << endl;
	//	agent[id].volunteer = false;
	//	agent[id].obstacle_component_id = -1;
	//	agent[id].blocked_obstacle_id = -1;
	//	agent[id].push_obstacle_id = -1;
	//}
}

void CS_CELLULAR_AUTOMATA::decideToBeBystander(int id){

	/*float rnd = (double)rand() / RAND_MAX;
	if (rnd < 0.5f)
	{
		agent[id].bystander = true;
	}*/
	float bystanderEffect = 1 - pow(agent[id].standerEffect_base, agent[id].visible_bystander);
	//cout << "agent_" << id << "'s bystander effect: " << bystanderEffect << endl;
	//cout << "agent_" << id << "'s willness: " << agent[id].psychology.willness << endl;
	/*if (agent[id].psychology.willness >= guiParameter.willing_threshold && agent[id].psychology.willness - bystanderEffect < guiParameter.willing_threshold)
	{
		agent[id].bystander = true;
		cout << "agent_" << id << " be bystander." << endl;
		system("pause");
	}*/
	if (agent[id].psychology.willness < guiParameter.willing_threshold && agent[id].psychology.willness >= guiParameter.willing_threshold - 0.1f)
	{
		agent[id].bystander = true;
		cout << "agent_" << id << " be bystander." << endl;
	}
	else
	{
		agent[id].bystander = false;
	}
}

void CS_CELLULAR_AUTOMATA::reset_advanced_sFF(int a_id){

	/*for (int i = 0; i < agent[a_id].visible_area.size(); i++)
	{

	}

	for (int j = 0; j < agent[i].visible_area.size(); j++)
	{
	int x = agent[i].visible_area[j].first;
	int z = agent[i].visible_area[j].second;
	if (!isValid(x, z))
	continue;
	if (cell[x][z].obstacle)
	continue;
	float value = agent[i].visible_area_advancesFF[j];
	cell[x][z].sFF -= value;
	cell[x][z].sFF_color = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
	cell[x][z]._sFF = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
	for (int k = 0; k < cell[x][z].specific_sFF.size(); k++)
	{
	cell[x][z].specific_sFF[k] -= value;
	cell[x][z].specific_color_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[x][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
	cell[x][z].specific_normalize_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[z][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
	}
	}*/
}

void CS_CELLULAR_AUTOMATA::updateVisibleAgent(){

	int count = 0;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			continue;
		Direction direction = agent[i].direction;
		PAIR_INT visual_direction;
		switch (direction)
		{
		case _up:
			visual_direction = PAIR_INT(0, 1);
			break;
		case _right:
			visual_direction = PAIR_INT(1, 0);
			break;
		case _down:
			visual_direction = PAIR_INT(0, -1);
			break;
		case _left:
			visual_direction = PAIR_INT(-1, 0);
			break;
		}
		// record last time step visible area
		/*for (int j = 0; j < agent[i].visible_area.size(); j++)
		{
			int x = agent[i].visible_area[j].first;
			int z = agent[i].visible_area[j].second;
			if (!isValid(x, z))
				continue;
			if (cell[x][z].obstacle)
				continue;
			float value = agent[i].visible_area_advancesFF[j];
			cell[x][z].sFF -= value;
			cell[x][z].sFF_color = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
			cell[x][z]._sFF = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
			for (int k = 0; k < cell[x][z].specific_sFF.size(); k++)
			{
				cell[x][z].specific_sFF[k] -= value;
				cell[x][z].specific_color_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[x][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
				cell[x][z].specific_normalize_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[z][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
			}
		}*/
		// compute the visbility grid
		agent[i].visible_agentID.clear();
		agent[i].visible_area.clear();
		agent[i].visible_area_advancesFF.clear();
		collect_visible_bystander(i);
		//if (visual_direction.first == 0 && visual_direction.second == 0)
		//	continue;
		for (int x = -VISIBILITY_GRID_LENGTH; x <= VISIBILITY_GRID_LENGTH; x++)
		{
			for (int z = -VISIBILITY_GRID_LENGTH; z <= VISIBILITY_GRID_LENGTH; z++)
			{
				int a_x = agent[i].position.first;
				int a_z = agent[i].position.second;
				if (!isValid(a_x + x, a_z + z))
					continue;
				float xx = (float)x / sqrt(x*x + z*z);
				float zz = (float)z / sqrt(x*x + z*z);
				float angle = get_angle(pair<float, float>(xx, zz), pair<float, float>(visual_direction));
				if (angle > 60.0f)
				{
					bool _continue = false;
					switch (model->mVisableType)
					{
					case 0:
						break;
					case 1:
						_continue = true;
						break;
					}
					if (_continue)
						continue;
				}
				float length = sqrt(x*x + z*z);
				if (length > sqrt(pow(VISIBILITY_GRID_LENGTH, 2)))
					continue;
				// 當agent看到障礙物
				if (cell[a_x + x][a_z + z].obstacle)
				{
					int o_id = cell[a_x + x][a_z + z].obstacle_id;
					/*if (obstacles[o_id].block_exit && !agent[i].bystander)
					{
					decideToBeBystander(i);
					if (agent[i].bystander)
					{
					agent[i].blocked_obstacle_id = o_id;
					continue;
					}
					}*/
					/*if (obstacles[o_id].block_exit)
					{
					decideToBeBystander(i);
					}
					if (agent[i].bystander)
					{
					agent[i].blocked_obstacle_id = o_id;
					}*/
					if (!obstacles[o_id].block && agent[i].blocked_obstacle_id == o_id && !agent[i].volunteer)
					{
						//agent[i].blockByExit
						agent[i].blocked_obstacle_id = -1;
					}
					//if (a_z + z == 1)
					//	cout << obstacles[o_id].block << endl;
					if (obstacles[o_id].block /*&& agent[i].blocked_obstacle_id == -1 */&& !agent[i].bystander)
					{
						//if (a_z + z == 1)
						//	cout << "yeah" << endl;
						agent[i].blocked_obstacle_id = o_id;
						int e_id = cell[obstacles[o_id].component[0].first][obstacles[o_id].component[0].second].exit_id;
						if (e_id != -1)
						{
							agent[i].blockByExit[e_id] = 1; 
							agent[i].currentVisibleBlockedExit = e_id;
						}
						if (mTimeCounter >= model->mMoveObstacleTime && !agent[i].mVolunteered)
							decideToBeVolunteer(i);
					}
					/*if (agent[i].blocked_obstacle_id != -1 && !agent[i].volunteer)
					{
					if (o_id == agent[i].blocked_obstacle_id && !obstacles[agent[i].blocked_obstacle_id].block)
					{
					agent[i].blocked_obstacle_id = -1;
					agent[i].bystander = false;
					}
					}*/
				}
				// 當agent看到出口
				if (cell[a_x + x][a_z + z].exit)
				{
					int e_id = cell[a_x + x][a_z + z].exit_id;
					int e_x = mExit[e_id].position.first;
					int e_z = mExit[e_id].position.second;
					int d_ex = mExit[e_id].direction.first;
					int d_ez = mExit[e_id].direction.second;
					// 檢查出口有無被obstacle擋住
					for (unsigned int e = 0; e < mExit[e_id].width; e++)
					{
						count++;
						int _x = e_x + d_ex*e;
						int _z = e_z + d_ez*e;
						if (!cell[_x][_z].obstacle)
						{
							agent[i].blockByExit[e_id] = 0;
							agent[i].mSawExit = true;
						}
					}
				}
				agent[i].visible_area.push_back(PAIR_INT(a_x + x, a_z + z));
				if (length < sqrt(pow(VISIBILITY_GRID_LENGTH, 2)) * 0.33f)
				{
					agent[i].visible_area_advancesFF.push_back(6.0f);
				}
				if (length < sqrt(pow(VISIBILITY_GRID_LENGTH, 2)) * 0.66f)
				{
					agent[i].visible_area_advancesFF.push_back(4.0f);
				}
				if (length <= sqrt(pow(VISIBILITY_GRID_LENGTH, 2)))
				{
					agent[i].visible_area_advancesFF.push_back(2.0f);
				}
				int id = cell[a_x + x][a_z + z].occupant_id;
				if (id == -1)
					continue;
				if (agent[id].arrival)
					continue;
				agent[i].visible_agentID.push_back(id);
			}
		}
	}
	//cout << count << endl;
}

void CS_CELLULAR_AUTOMATA::follow_behavior(int id){

	int leader_id = agent_group[agent[id].group_id].member[0];
	if (agent[leader_id].volunteer)
	{
		agent[id].volunteer = true;
		agent[id].blocked_obstacle_id = agent[leader_id].blocked_obstacle_id;
		agent[id].obstacle_component_id = agent[leader_id].obstacle_component_id;
	}
}

void CS_CELLULAR_AUTOMATA::collect_visible_bystander(int a_id){

	Direction direction = agent[a_id].direction;
	PAIR_INT visual_direction;
	switch (direction)
	{
	case _up:
		visual_direction = PAIR_INT(0, 1);
		break;
	case _right:
		visual_direction = PAIR_INT(1, 0);
		break;
	case _down:
		visual_direction = PAIR_INT(0, -1);
		break;
	case _left:
		visual_direction = PAIR_INT(-1, 0);
		break;
	}
	// compute the visbility grid
	agent[a_id].visible_bystander = 0;
	if (visual_direction.first == 0 && visual_direction.second == 0)
		return;
	// longitude direction
	if (visual_direction.first == 0)
	{
		int z = 0;
		for (int j = 0; j < VISIBILITY_GRID_LENGTH; j++)
		{
			z += visual_direction.second;
			for (int x = -VISIBILITY_GRID_LENGTH; x < VISIBILITY_GRID_LENGTH; x++)
			{
				int a_x = agent[a_id].position.first;
				int a_z = agent[a_id].position.second;
				if (!isValid(a_x + x, a_z + z))
					continue;
				float xx = (float)x / sqrt(x*x + z*z);
				float zz = (float)z / sqrt(x*x + z*z);
				float angle = get_angle(pair<float, float>(xx, zz), pair<float, float>(visual_direction));
				if (angle > 60.0f)
				{
					continue;
				}
				float length = sqrt(x*x + z*z);
				if (length > sqrt(pow(VISIBILITY_GRID_LENGTH, 2)))
					continue;
				int id = cell[a_x + x][a_z + z].occupant_id;
				if (id == -1)
					continue;
				if (agent[id].arrival)
					continue;
				if (agent[id].bystander)
					agent[a_id].visible_bystander++;
			}
		}
	}
	// lateral direction
	if (visual_direction.second == 0)
	{
		int x = 0;
		for (int j = 0; j < VISIBILITY_GRID_LENGTH; j++)
		{
			x += visual_direction.first;
			for (int z = -VISIBILITY_GRID_LENGTH; z < VISIBILITY_GRID_LENGTH; z++)
			{
				int a_x = agent[a_id].position.first;
				int a_z = agent[a_id].position.second;
				if (!isValid(a_x + x, a_z + z))
					continue;
				float xx = (float)x / sqrt(x*x + z*z);
				float zz = (float)z / sqrt(x*x + z*z);
				float angle = get_angle(pair<float, float>(xx, zz), pair<float, float>(visual_direction));
				if (angle > 60.0f)
				{
					continue;
				}
				float length = sqrt(x*x + z*z);
				if (length > sqrt(pow(VISIBILITY_GRID_LENGTH, 2)))
					continue;
				int id = cell[a_x + x][a_z + z].occupant_id;
				if (id == -1)
					continue;
				if (agent[id].arrival)
					continue;
				if (agent[id].bystander)
					agent[a_id].visible_bystander++;
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::addsFF_resolveCounterFlow(int a_id){

	for (int i = 0; i < agent[a_id].visible_area.size(); i++)
	{
		int x = agent[a_id].visible_area[i].first;
		int z = agent[a_id].visible_area[i].second;
		if (!isValid(x, z))
			continue;
		if (cell[x][z].obstacle)
			continue;
		float value = agent[a_id].visible_area_advancesFF[i];
		cell[x][z].sFF += value;
		cell[x][z].sFF_color = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
		cell[x][z]._sFF = (cell_manager.max_sFF - cell[x][z].sFF) / cell_manager.max_sFF;
		for (int k = 0; k < cell[x][z].specific_sFF.size(); k++)
		{
			cell[x][z].specific_sFF[k] += value;
			cell[x][z].specific_color_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[x][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
			cell[x][z].specific_normalize_sFF[k] = (cell_manager.specific_max_sFF[k] - cell[x][z].specific_sFF[k]) / cell_manager.specific_max_sFF[k];
		}
	}
	//system("pause");
}

PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction(int x, int y, int agent_number){

	PAIR_INT temp_pos = PAIR_INT(x, y);
	double top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f, topRight = 0.0f, topLeft = 0.0f, downRight = 0.0f, downLeft = 0.0f, total;
	int com_id = agent[agent_number].obstacle_component_id;
	int o_id = agent[agent_number].blocked_obstacle_id;
	// if agent is a guider
	if (agent[agent_number].guider && !agent[agent_number].stop_guide)
	{
		update_guider_density(agent_number);
		cout << "agent" << agent_number << "'s density = " << agent[agent_number].guide_area_density << endl;
		compute_guide_influence(agent_number);

		if (agent[agent_number].guide_area_density < model->density_thresold)
		{
			agent[agent_number].stop_guide = true;
		}
		cell[x][y].temp_dFF += DYNAMIC_VALUE;
		cell[x][y].occupant_id = agent_number;
		return temp_pos;
	}
	// if agent has a leader	
	if (!agent[agent_number].leader)
	{
		if (isValid(x, y + 1))
			top = get_probability(x, y + 1, agent_number) * get_leader_inertia(x, y + 1, agent_number);
		if (isValid(x, y - 1))
			down = get_probability(x, y - 1, agent_number) * get_leader_inertia(x, y - 1, agent_number);
		if (isValid(x - 1, y))
			left = get_probability(x - 1, y, agent_number) * get_leader_inertia(x - 1, y, agent_number);
		if (isValid(x + 1, y))
			right = get_probability(x + 1, y, agent_number) * get_leader_inertia(x + 1, y, agent_number);
		if (model->mNeighborType)
		{
			if (isValid(x + 1, y + 1))
				topRight = get_probability(x + 1, y + 1, agent_number) * get_leader_inertia(x + 1, y + 1, agent_number);
			if (isValid(x - 1, y + 1))
				topLeft = get_probability(x - 1, y + 1, agent_number) * get_leader_inertia(x - 1, y + 1, agent_number);
			if (isValid(x + 1, y - 1))
				downRight = get_probability(x + 1, y - 1, agent_number) * get_leader_inertia(x + 1, y - 1, agent_number);
			if (isValid(x - 1, y - 1))
				downLeft = get_probability(x - 1, y - 1, agent_number) * get_leader_inertia(x - 1, y - 1, agent_number);
		}
	}
	// if agent is a leader
	if (agent[agent_number].leader)
	{
		if (isValid(x, y + 1))
			top = get_probability(x, y + 1, agent_number) * get_inertia(x, y + 1, agent_number);// * agent[agent_number].top;
		if (isValid(x, y - 1))
			down = get_probability(x, y - 1, agent_number) * get_inertia(x, y - 1, agent_number);// * agent[agent_number].down;
		if (isValid(x - 1, y))
			left = get_probability(x - 1, y, agent_number) * get_inertia(x - 1, y, agent_number);// * agent[agent_number].left;
		if (isValid(x + 1, y))
			right = get_probability(x + 1, y, agent_number) * get_inertia(x + 1, y, agent_number);// * agent[agent_number].right;
		if (model->mNeighborType)
		{
			if (isValid(x + 1, y + 1))
				topRight = get_probability(x + 1, y + 1, agent_number) * get_inertia(x + 1, y + 1, agent_number);
			if (isValid(x - 1, y + 1))
				topLeft = get_probability(x - 1, y + 1, agent_number) * get_inertia(x - 1, y + 1, agent_number);
			if (isValid(x + 1, y - 1))
				downRight = get_probability(x + 1, y - 1, agent_number) * get_inertia(x + 1, y - 1, agent_number);
			if (isValid(x - 1, y - 1))
				downLeft = get_probability(x - 1, y - 1, agent_number) * get_inertia(x - 1, y - 1, agent_number);
		}
	}
	if (agent[agent_number].volunteer)
	{
		top = get_probability_volunteer(x, y + 1, agent_number, com_id, o_id);
		down = get_probability_volunteer(x, y - 1, agent_number, com_id, o_id);
		left = get_probability_volunteer(x - 1, y, agent_number, com_id, o_id);
		right = get_probability_volunteer(x + 1, y, agent_number, com_id, o_id);
		topRight = get_probability_volunteer(x + 1, y + 1, agent_number, com_id, o_id);
		topLeft = get_probability_volunteer(x - 1, y + 1, agent_number, com_id, o_id);
		downRight = get_probability_volunteer(x + 1, y - 1, agent_number, com_id, o_id);
		downLeft = get_probability_volunteer(x - 1, y - 1, agent_number, com_id, o_id);
	}
	//leader decide to wait for other members of not
	/*if (determine_leader_waiting(agent[agent_number].leader, check_member_arrival(agent_number)))
	{
	cout << "agent" << agent_number << " waiting for other members" << endl;
	agent[agent_number].waiting = true;
	cell[x][y].temp_dFF += DYNAMIC_VALUE;
	return temp_pos;
	}*/
	if (determine_leader_waiting(agent[agent_number].leader, agent_number))
	{
		agent[agent_number].waiting = true;
		cell[x][y].occupant_id = agent_number;
		cell[x][y].temp_dFF += DYNAMIC_VALUE;
		agent[agent_number].direction = local;
		agent[agent_number].pre_sFF = cell[x][y].sFF;
		return temp_pos;
	}
	//if (agent_number < 10)
	//{
		//cout << top << " " << down << " " << left << " " << right << " " << topRight << " " << topLeft << " " << downRight << " " << downLeft << endl;
	//}

	// normalize each direction, their sum are equal to one	
	total = top + down + left + right + topRight + topLeft + downRight + downLeft;

	// each side have been occupied, stay in place 	
	if (total == 0)
	{
		cell[x][y].temp_dFF += DYNAMIC_VALUE;
		cell[x][y].occupant_id = agent_number;
		agent[agent_number].direction = local;
		agent[agent_number].pre_sFF = cell[x][y].sFF;
		return temp_pos;
	}
	top /= total;
	down /= total;
	left /= total;
	right /= total;
	topRight /= total;
	topLeft /= total;
	downRight /= total;
	downLeft /= total;

	down += top;
	left += down;
	right += left;
	topRight += right;
	topLeft += topRight;
	downRight += topLeft;

	float random_num = (double)rand() / RAND_MAX;
	//choose four direction
	if (random_num <= top)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x, y + 1);
		agent[agent_number].inertia = fromBottom;
		agent[agent_number].direction = _up;
	}
	else if (random_num > top && random_num <= down)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x, y - 1);
		agent[agent_number].inertia = fromTop;
		agent[agent_number].direction = _down;
	}
	else if (random_num > down && random_num <= left)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y);
		agent[agent_number].inertia = fromRight;
		agent[agent_number].direction = _left;
	}
	else if (random_num > left && (model->mNeighborType == 0 || (model->mNeighborType != 0 && random_num <= right)))
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y);
		agent[agent_number].inertia = fromLeft;
		agent[agent_number].direction = _right;
	}
	else if (random_num > right && random_num <= topRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y + 1);
		agent[agent_number].inertia = fromLeftBottom;
		agent[agent_number].direction = _topRight;
	}
	else if (random_num > topRight && random_num <= topLeft)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y + 1);
		agent[agent_number].inertia = fromRightBottom;
		agent[agent_number].direction = _topLeft;
	}
	else if (random_num > topLeft && random_num <= downRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y - 1);
		agent[agent_number].inertia = fromLeftTop;
		agent[agent_number].direction = _downRight;
	}
	else if (random_num > downRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y - 1);
		agent[agent_number].inertia = fromRightTop;
		agent[agent_number].direction = _downLeft;
	}
	agent[agent_number].waiting = false;
	cell[x][y].temp_dFF += DYNAMIC_VALUE;
	cell[temp_pos.first][temp_pos.second].occupant_id = agent_number;
	agent[agent_number].pre_sFF = cell[x][y].sFF;
	//agent[agent_number].path.push_back(temp_pos);

	return temp_pos;
}

PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_HighValueBased(int x, int y, int agent_num){

	float top = FLT_MAX, down = FLT_MAX, left = FLT_MAX, right = FLT_MAX, top_left = FLT_MAX, top_right = FLT_MAX, down_left = FLT_MAX, down_right = FLT_MAX;
	int com_id = agent[agent_num].obstacle_component_id;
	int o_id = agent[agent_num].blocked_obstacle_id;
	if (isValid(x, y + 1))
		top = get_probability_volunteer(x, y + 1, agent_num, com_id, o_id);
	if (isValid(x, y - 1))
		down = get_probability_volunteer(x, y - 1, agent_num, com_id, o_id);
	if (isValid(x - 1, y))
		left = get_probability_volunteer(x - 1, y, agent_num, com_id, o_id);
	if (isValid(x + 1, y))
		right = get_probability_volunteer(x + 1, y, agent_num, com_id, o_id);
	if (isValid(x - 1, y + 1))
		top_left = get_probability_volunteer(x - 1, y + 1, agent_num, com_id, o_id);
	if (isValid(x + 1, y + 1))
		top_right = get_probability_volunteer(x + 1, y + 1, agent_num, com_id, o_id);
	if (isValid(x - 1, y - 1))
		down_left = get_probability_volunteer(x - 1, y - 1, agent_num, com_id, o_id);
	if (isValid(x + 1, y - 1))
		down_right = get_probability_volunteer(x + 1, y - 1, agent_num, com_id, o_id);

	PAIR_INT direction[8] = { PAIR_INT(x, y + 1), PAIR_INT(x, y - 1), PAIR_INT(x - 1, y), PAIR_INT(x + 1, y),
		PAIR_INT(x - 1, y + 1), PAIR_INT(x + 1, y + 1), PAIR_INT(x - 1, y - 1), PAIR_INT(x + 1, y - 1) };
	float buffer[8] = { top, down, left, right, top_left, top_right, down_left, down_right };
	float distance[8] = { top, down, left, right, top_left, top_right, down_left, down_right };
	sort(distance, distance + 8);

	int count = 7;
	while (distance[count] == FLT_MAX)
	{
		if (count == 0)
		{
			cout << "volunteer stay at ori place." << endl;
			cell[x][y].temp_dFF += DYNAMIC_VALUE;
			return PAIR_INT(x, y);
		}
		count--;
	}
	for (int i = 0; i < 8; i++)
	{
		if (distance[7] == buffer[i])
		{
			/*if (cell[direction[i].first][direction[i].second].occupied == 1)
			{
			cell[x][y].temp_dFF += DYNAMIC_VALUE;
			cell[x][y].occupant_id = agent_num;
			return pair<int, int>(x, y);
			}*/
			if (!isValid(direction[i].first, direction[i].second))
				return PAIR_INT(x, y);
			cell[x][y].occupied = 0;
			cell[x][y].temp_dFF += DYNAMIC_VALUE;
			cell[direction[i].first][direction[i].second].occupied = 1;
			cell[direction[i].first][direction[i].second].occupant_id = agent_num;
			return direction[i];
		}
	}
}

PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_HighValueBased_fourDirection(int x, int y, int agent_num){

	float top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f, topRight = -0.0f, topLeft = 0.0f, downRight = 0.0f, downLeft = 0.0f;
	int com_id = agent[agent_num].obstacle_component_id;
	int o_id = agent[agent_num].blocked_obstacle_id;
	/*int o_id = agent[agent_num].blocked_obstacle_id;
	if (isValid(x, y + 1) && cell[x][y + 1].occupied != 1 && !isObstacle(x, y + 1))
		top = pair_int_distance(PAIR_INT(x, y + 1), obstacles[o_id].component[com_id]);
	if (isValid(x, y - 1) && cell[x][y - 1].occupied != 1 && !isObstacle(x, y - 1))
		down = pair_int_distance(PAIR_INT(x, y - 1), obstacles[o_id].component[com_id]);
	if (isValid(x - 1, y) && cell[x - 1][y].occupied != 1 && !isObstacle(x - 1, y))
		left = pair_int_distance(PAIR_INT(x - 1, y), obstacles[o_id].component[com_id]);
	if (isValid(x + 1, y) && cell[x + 1][y].occupied != 1 && !isObstacle(x + 1, y))
		right = pair_int_distance(PAIR_INT(x + 1, y), obstacles[o_id].component[com_id]);*/
	top = get_probability_volunteer(x, y + 1, agent_num, com_id, o_id);
	down = get_probability_volunteer(x, y - 1, agent_num, com_id, o_id);
	left = get_probability_volunteer(x - 1, y, agent_num, com_id, o_id);
	right = get_probability_volunteer(x + 1, y, agent_num, com_id, o_id);
	if (model->mNeighborType != 0)
	{
		topRight = get_probability_volunteer(x + 1, y + 1, agent_num, com_id, o_id);
		topLeft = get_probability_volunteer(x - 1, y + 1, agent_num, com_id, o_id);
		downRight = get_probability_volunteer(x + 1, y - 1, agent_num, com_id, o_id);
		downLeft = get_probability_volunteer(x - 1, y - 1, agent_num, com_id, o_id);
	}

	double r = (double)rand() / RAND_MAX;
	// normalize each direction, their sum are equal to one	
	float total = top + down + left + right + topRight + topLeft + downRight + downLeft;

	// each side have been occupied, stay in place 	
	if (total == 0)
	{
		//cout << top << " " << down << " " << left << " " << right << endl;
		cell[x][y].temp_dFF += DYNAMIC_VALUE;
		cell[x][y].occupant_id = agent_num;
		cell[x][y].occupied = 1;
		agent[agent_num].direction = local;
		agent[agent_num].pre_sFF = cell[x][y].sFF;
		return PAIR_INT(x,y);
	}
	top /= total;
	down /= total;
	left /= total;
	right /= total;
	topRight /= total;
	topLeft /= total;
	downRight /= total;
	downLeft /= total;

	down += top;
	left += down;
	right += left;
	topRight += right;
	topLeft += topRight;
	downRight += topLeft;
	PAIR_INT temp_pos;
	float random_num = (double)rand() / RAND_MAX;
	//choose four direction
	if (random_num <= top)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x, y + 1);
		agent[agent_num].inertia = fromBottom;
		agent[agent_num].direction = _up;
	}
	else if (random_num > top && random_num <= down)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x, y - 1);
		agent[agent_num].inertia = fromTop;
		agent[agent_num].direction = _down;
	}
	else if (random_num > down && random_num <= left)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y);
		agent[agent_num].inertia = fromRight;
		agent[agent_num].direction = _left;
	}
	else if (random_num > left && (model->mNeighborType == 0 || (model->mNeighborType != 0 && random_num <= right)))
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y);
		agent[agent_num].inertia = fromLeft;
		agent[agent_num].direction = _right;
	}
	else if (random_num > right && random_num <= topRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y + 1);
		agent[agent_num].inertia = fromLeftBottom;
		agent[agent_num].direction = _topRight;
	}
	else if (random_num > topRight && random_num <= topLeft)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y + 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y + 1);
		agent[agent_num].inertia = fromRightBottom;
		agent[agent_num].direction = _topLeft;
	}
	else if (random_num > topLeft && random_num <= downRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x + 1][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, y - 1);
		agent[agent_num].inertia = fromLeftTop;
		agent[agent_num].direction = _downRight;
	}
	else if (random_num > downRight)
	{
		cell[x][y].occupied = 0;
		cell[x][y].occupant_id = -1;
		cell[x - 1][y - 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, y - 1);
		agent[agent_num].inertia = fromRightTop;
		agent[agent_num].direction = _downLeft;
	}
	agent[agent_num].waiting = false;
	cell[x][y].temp_dFF += DYNAMIC_VALUE;
	cell[temp_pos.first][temp_pos.second].occupant_id = agent_num;
	agent[agent_num].pre_sFF = cell[x][y].sFF;
	agent[agent_num].pre_pos = PAIR_INT(x, y);
	if (temp_pos.first == -1 || temp_pos.second == -1)
		system("pause");
	return temp_pos;

	PAIR_INT direction[8] = { PAIR_INT(x, y + 1), PAIR_INT(x, y - 1), PAIR_INT(x - 1, y), PAIR_INT(x + 1, y), PAIR_INT(x + 1, y + 1), PAIR_INT(x - 1, y + 1), PAIR_INT(x + 1, y - 1), PAIR_INT(x - 1, y - 1) };
	float buffer[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	float distance[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(distance, distance + 8);
	int count = 7;
	while (distance[count] == -FLT_MAX)
	{
		if (count == 0)
		{
			cout << "volunteer stay at ori place. " << x << " " << y << endl;
			cell[x][y].temp_dFF += DYNAMIC_VALUE;
			agent[agent_num].direction = local;
			return PAIR_INT(x, y);
		}
		count--;
	}
	for (int i = 0; i < 8; i++)
	{
		if (distance[7] == buffer[i])
		{
			if (!isValid(direction[i].first, direction[i].second))
			{
				cout << -FLT_MAX << endl;
				cout << buffer[i] << endl;
				cout << i << endl;
				agent[agent_num].direction = local;
				return PAIR_INT(x, y);
			}
			cell[x][y].occupied = 0;
			cell[x][y].temp_dFF += DYNAMIC_VALUE;
			cell[direction[i].first][direction[i].second].occupied = 1;
			cell[direction[i].first][direction[i].second].occupant_id = agent_num;
			switch (i)
			{
			case 0:
				agent[agent_num].direction = _up;
				break;
			case 1:
				agent[agent_num].direction = _down;
				break;
			case 2:
				agent[agent_num].direction = _left;
				break;
			case 3:
				agent[agent_num].direction = _right;
				break;
			case 4:
				agent[agent_num].direction = _topRight;
				break;
			case 5:
				agent[agent_num].direction = _topLeft;
				break;
			case 6:
				agent[agent_num].direction = _downRight;
				break;
			case 7:
				agent[agent_num].direction = _downLeft;
				break;
			}
			return direction[i];
		}
	}

	return PAIR_INT(0, 0);
}

PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_guider(int id){

	float top = -FLT_MAX, down = -FLT_MAX, left = -FLT_MAX, right = -FLT_MAX, topRight = -FLT_MAX, topLeft = -FLT_MAX, downRight = -FLT_MAX, downLeft = -FLT_MAX;
	int x = agent[id].position.first;
	int z = agent[id].position.second;
	top = get_probability_guider(id, x, z + 1);
	down = get_probability_guider(id, x, z - 1);
	left = get_probability_guider(id, x - 1, z);
	right = get_probability_guider(id, x + 1, z);
	topRight = get_probability_guider(id, x + 1, z + 1);
	topLeft = get_probability_guider(id, x - 1, z + 1);
	downRight = get_probability_guider(id, x + 1, z - 1);
	downLeft = get_probability_guider(id, x - 1, z - 1);

	double r = (double)rand() / RAND_MAX;
	// normalize each direction, their sum are equal to one	
	float total = top + down + left + right + topRight + topLeft + downRight + downLeft;

	// each side have been occupied, stay in place 	
	if (total == 0)
	{
		cell[x][z].temp_dFF += DYNAMIC_VALUE;
		cell[x][z].occupant_id = id;
		cell[x][z].occupied = 1;
		agent[id].direction = local;
		agent[id].pre_sFF = cell[x][z].sFF;
		return PAIR_INT(x, z);
	}
	top /= total;
	down /= total;
	left /= total;
	right /= total;
	topRight /= total;
	topLeft /= total;
	downRight /= total;
	downLeft /= total;

	down += top;
	left += down;
	right += left;
	topRight += right;
	topLeft += topRight;
	downRight += topLeft;
	PAIR_INT temp_pos;
	float random_num = (double)rand() / RAND_MAX;
	//choose four direction
	if (random_num <= top)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x][z + 1].occupied = 1;
		temp_pos = PAIR_INT(x, z + 1);
		agent[id].inertia = fromBottom;
		agent[id].direction = _up;
	}
	else if (random_num > top && random_num <= down)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x][z - 1].occupied = 1;
		temp_pos = PAIR_INT(x, z - 1);
		agent[id].inertia = fromTop;
		agent[id].direction = _down;
	}
	else if (random_num > down && random_num <= left)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x - 1][z].occupied = 1;
		temp_pos = PAIR_INT(x - 1, z);
		agent[id].inertia = fromRight;
		agent[id].direction = _left;
	}
	else if (random_num > left && isValid(x + 1, z) && (model->mNeighborType == 0 || (model->mNeighborType != 0 && random_num <= right)))
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x + 1][z].occupied = 1;
		temp_pos = PAIR_INT(x + 1, z);
		agent[id].inertia = fromLeft;
		agent[id].direction = _right;
	}
	else if (random_num > right && random_num <= topRight)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x + 1][z + 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, z + 1);
		agent[id].inertia = fromLeftBottom;
		agent[id].direction = _topRight;
	}
	else if (random_num > topRight && random_num <= topLeft)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x - 1][z + 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, z + 1);
		agent[id].inertia = fromRightBottom;
		agent[id].direction = _topLeft;
	}
	else if (random_num > topLeft && random_num <= downRight)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x + 1][z - 1].occupied = 1;
		temp_pos = PAIR_INT(x + 1, z - 1);
		agent[id].inertia = fromLeftTop;
		agent[id].direction = _downRight;
	}
	else if (random_num > downRight)
	{
		cell[x][z].occupied = 0;
		cell[x][z].occupant_id = -1;
		cell[x - 1][z - 1].occupied = 1;
		temp_pos = PAIR_INT(x - 1, z - 1);
		agent[id].inertia = fromRightTop;
		agent[id].direction = _downLeft;
	}
	agent[id].waiting = false;
	cell[x][z].temp_dFF += DYNAMIC_VALUE;
	cell[temp_pos.first][temp_pos.second].occupant_id = id;
	agent[id].pre_sFF = cell[x][z].sFF;
	agent[id].pre_pos = PAIR_INT(x, z);
	if (temp_pos.first == -1 || temp_pos.second == -1)
		system("pause");
	return temp_pos;

	PAIR_INT direction[8] = { PAIR_INT(x, z + 1), PAIR_INT(x, z - 1), PAIR_INT(x - 1, z), PAIR_INT(x + 1, z), PAIR_INT(x + 1, z + 1), PAIR_INT(x - 1, z + 1), PAIR_INT(x + 1, z - 1), PAIR_INT(x - 1, z - 1) };
	float buffer[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	float distance[8] = { top, down, left, right, topRight, topLeft, downRight, downLeft };
	sort(distance, distance + 8);
	int count = 7;
	while (distance[count] == FLT_MAX)
	{
		if (count == 0)
		{
			cout << "volunteer stay at ori place. " << x << " " << z << endl;
			cell[x][z].temp_dFF += DYNAMIC_VALUE;
			agent[id].direction = local;
			return PAIR_INT(x, z);
		}
		count--;
	}
	for (int i = 0; i < 8; i++)
	{
		if (distance[7] == buffer[i])
		{
			if (!isValid(direction[i].first, direction[i].second))
			{
				cout << FLT_MAX << endl;
				cout << buffer[i] << endl;
				cout << i << endl;
				agent[id].direction = local;
				return PAIR_INT(x, z);
			}
			cell[x][z].occupied = 0;
			cell[x][z].temp_dFF += DYNAMIC_VALUE;
			cell[direction[i].first][direction[i].second].occupied = 1;
			cell[direction[i].first][direction[i].second].occupant_id = id;
			switch (i)
			{
			case 0:
				agent[id].direction = _up;
				break;
			case 1:
				agent[id].direction = _down;
				break;
			case 2:
				agent[id].direction = _left;
				break;
			case 3:
				agent[id].direction = _right;
				break;
			case 4:
				agent[id].direction = _topRight;
				break;
			case 5:
				agent[id].direction = _topLeft;
				break;
			case 6:
				agent[id].direction = _downRight;
				break;
			case 7:
				agent[id].direction = _downLeft;
				break;
			}
			return direction[i];
		}
	}

	return PAIR_INT(0, 0);
}
// volunteer choose the closer route to destination
PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_volunteer(int x, int z, int agent_num){

	pair<int, int> temp_pos = pair<int, int>(x, z);
	float top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f, total;
	bool push = false;
	float odisTodestination;
	float adisTodestination;
	odisTodestination = sqrt(pow(removed_obstacles[agent[agent_num].push_obstacle_id].position.first - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
		+ pow(removed_obstacles[agent[agent_num].push_obstacle_id].position.second - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
	adisTodestination = sqrt(pow(x - removed_obstacles[agent[agent_num].push_obstacle_id].destination.first, 2)
		+ pow(z - removed_obstacles[agent[agent_num].push_obstacle_id].destination.second, 2));
	if (odisTodestination < adisTodestination)
		push = true;
	enum force{ push_, pull };
	force force_direction;
	if (push && agent[agent_num].beside_obstacle)
		force_direction = push_;
	else
		force_direction = pull;
	if (push && agent[agent_num].beside_obstacle)
	{
		int o_x, o_z;
		o_x = removed_obstacles[agent[agent_num].push_obstacle_id].position.first;
		o_z = removed_obstacles[agent[agent_num].push_obstacle_id].position.second;
		if (isValid(o_x, o_z + 1))
			top = get_probability_obstacle(o_x, o_z + 1, agent[agent_num].push_obstacle_id);
		if (isValid(o_x, o_z - 1))
			down = get_probability_obstacle(o_x, o_z - 1, agent[agent_num].push_obstacle_id);
		if (isValid(o_x - 1, o_z))
			left = get_probability_obstacle(o_x - 1, o_z, agent[agent_num].push_obstacle_id);
		if (isValid(o_x + 1, o_z))
			right = get_probability_obstacle(o_x + 1, o_z, agent[agent_num].push_obstacle_id);
	}
	else
	{
		if (isValid(x, z + 1))
			top = get_probability_volunteer(x, z + 1, agent_num);
		if (isValid(x, z - 1))
			down = get_probability_volunteer(x, z - 1, agent_num);
		if (isValid(x - 1, z))
			left = get_probability_volunteer(x - 1, z, agent_num);
		if (isValid(x + 1, z))
			right = get_probability_volunteer(x + 1, z, agent_num);
	}

	total = top + down + left + right;
	// each side have been occupied, stay in place 	
	if (total == 0)
	{
		cell[x][z].temp_dFF += DYNAMIC_VALUE;
		cell[x][z].occupant_id = agent_num;
		agent[agent_num].inertia = standStatic;
		return temp_pos;
	}
	if (total != 0)
	{
		top /= total;
		down /= total;
		left /= total;
		right /= total;
	}

	down += top;
	left += down;
	float random_num = (double)rand() / RAND_MAX;

	switch (force_direction)
	{
		//push mode
	case push_:
		int o_x, o_z;
		if (agent[agent_num].beside_obstacle)
		{
			cout << "agent" << agent_num << " push the obstacle." << endl;
			cell[x][z].occupied = 0;
			o_x = removed_obstacles[agent[agent_num].push_obstacle_id].position.first;
			o_z = removed_obstacles[agent[agent_num].push_obstacle_id].position.second;
			cell[o_x][o_z].cell_type = 0;
			cell[o_x][o_z].obstacle = false;
			cell[o_x][o_z].obstacle_ = 1;
			cell[o_x][o_z].occupied = 1;
			temp_pos = removed_obstacles[agent[agent_num].push_obstacle_id].position;
		}
		//choose four direction
		if (random_num <= top)
		{
			cell[o_x][o_z + 1].obstacle = true;
			cell[o_x][o_z + 1].obstacle_ = 0;
			cell[o_x][o_z + 1].cell_type = 2;
			removed_obstacles[agent[agent_num].push_obstacle_id].position = pair<int, int>(o_x, o_z + 1);
		}
		else if (random_num > top && random_num <= down)
		{
			cell[o_x][o_z - 1].obstacle = true;
			cell[o_x][o_z - 1].obstacle_ = 0;
			cell[o_x][o_z - 1].cell_type = 2;
			removed_obstacles[agent[agent_num].push_obstacle_id].position = pair<int, int>(o_x, o_z - 1);
		}
		else if (random_num > down && random_num <= left)
		{
			cell[o_x - 1][o_z].obstacle = true;
			cell[o_x - 1][o_z].obstacle_ = 0;
			cell[o_x - 1][o_z].cell_type = 2;
			removed_obstacles[agent[agent_num].push_obstacle_id].position = pair<int, int>(o_x - 1, o_z);
		}
		else if (random_num > left)
		{
			if (isValid(x, z)){
				cell[o_x + 1][o_z].obstacle = true;
				cell[o_x + 1][o_z].obstacle_ = 0;
				cell[o_x + 1][o_z].cell_type = 2;
				removed_obstacles[agent[agent_num].push_obstacle_id].position = pair<int, int>(o_x + 1, o_z);
			}
		}
		break;
		//pull mode
	case pull:
		if (agent[agent_num].beside_obstacle)
		{
			cout << "agent" << agent_num << " pull the obstacle." << endl;
			cell[x][z].obstacle_ = 0;
			cell[x][z].obstacle = true;
			cell[x][z].cell_type = 2;
			cell[removed_obstacles[agent[agent_num].push_obstacle_id].position.first][removed_obstacles[agent[agent_num].push_obstacle_id].position.second].cell_type = 0;
			cell[removed_obstacles[agent[agent_num].push_obstacle_id].position.first][removed_obstacles[agent[agent_num].push_obstacle_id].position.second].obstacle = false;
			cell[removed_obstacles[agent[agent_num].push_obstacle_id].position.first][removed_obstacles[agent[agent_num].push_obstacle_id].position.second].obstacle_ = 1;
			removed_obstacles[agent[agent_num].push_obstacle_id].position = pair<int, int>(x, z);
			//cout << top << " " << down << " " << left << " " << right << endl;
		}
		//choose four direction
		if (random_num <= top)
		{
			cell[x][z].occupied = 0;
			cell[x][z + 1].occupied = 1;
			temp_pos = pair<int, int>(x, z + 1);
			agent[agent_num].inertia = fromBottom;
		}
		else if (random_num > top && random_num <= down)
		{
			cell[x][z].occupied = 0;
			cell[x][z - 1].occupied = 1;
			temp_pos = pair<int, int>(x, z - 1);
			agent[agent_num].inertia = fromTop;
		}
		else if (random_num > down && random_num <= left)
		{
			cell[x][z].occupied = 0;
			cell[x - 1][z].occupied = 1;
			temp_pos = pair<int, int>(x - 1, z);
			agent[agent_num].inertia = fromRight;
		}
		else if (random_num > left)
		{
			if (isValid(x, z)){
				cell[x][z].occupied = 0;
				cell[x + 1][z].occupied = 1;
				temp_pos = pair<int, int>(x + 1, z);
				agent[agent_num].inertia = fromLeft;
			}
		}
		break;
	}

	agent[agent_num].waiting = false;
	cell[x][z].temp_dFF += DYNAMIC_VALUE;
	cell[temp_pos.first][temp_pos.second].occupant_id = agent_num;

	agent[agent_num].path.push_back(temp_pos);

	return temp_pos;
}
// volunteer choose the closer route to target
PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_volunteer(int agent_id){
	int x = agent[agent_id].position.first;
	int z = agent[agent_id].position.second;
	pair<int, int> temp_pos = pair<int, int>(x, z);
	float top = 0.0f, down = 0.0f, left = 0.0f, right = 0.0f, total;
	int com_id = agent[agent_id].obstacle_component_id;
	int o_id = agent[agent_id].push_obstacle_id;

	if (isValid(x, z + 1))
		top = get_probability_volunteer(x, z + 1, agent_id, com_id, o_id);
	if (isValid(x, z - 1))
		down = get_probability_volunteer(x, z - 1, agent_id, com_id, o_id);
	if (isValid(x - 1, z))
		left = get_probability_volunteer(x - 1, z, agent_id, com_id, o_id);
	if (isValid(x + 1, z))
		right = get_probability_volunteer(x + 1, z, agent_id, com_id, o_id);

	total = top + down + left + right;
	/*cout << "top: " << top << endl;
	cout << "down: " << down << endl;
	cout << "left: " << left << endl;
	cout << "right: " << right << endl;
	cout << "total: " << total << endl;
	cout << endl;*/
	// each side have been occupied, stay in place 	
	if (total == 0)
	{
		cout << "agent id: " << agent_id << " stay in ori." << endl << endl;
		cell[x][z].temp_dFF += DYNAMIC_VALUE;
		cell[x][z].occupant_id = agent_id;
		agent[agent_id].inertia = standStatic;
		return temp_pos;
	}
	if (total != 0)
	{
		top /= total;
		down /= total;
		left /= total;
		right /= total;
	}

	down += top;
	left += down;
	float random_num = (double)rand() / RAND_MAX;
	if (random_num <= top)
	{
		cell[x][z].occupied = 0;
		cell[x][z + 1].occupied = 1;
		temp_pos = pair<int, int>(x, z + 1);
		agent[agent_id].inertia = fromBottom;
	}
	else if (random_num > top && random_num <= down)
	{
		cell[x][z].occupied = 0;
		cell[x][z - 1].occupied = 1;
		temp_pos = pair<int, int>(x, z - 1);
		agent[agent_id].inertia = fromTop;
	}
	else if (random_num > down && random_num <= left)
	{
		cell[x][z].occupied = 0;
		cell[x - 1][z].occupied = 1;
		temp_pos = pair<int, int>(x - 1, z);
		agent[agent_id].inertia = fromRight;
	}
	else if (random_num > left)
	{
		if (isValid(x, z)){
			cell[x][z].occupied = 0;
			cell[x + 1][z].occupied = 1;
			temp_pos = pair<int, int>(x + 1, z);
			agent[agent_id].inertia = fromLeft;
		}
	}

	agent[agent_id].waiting = false;
	cell[x][z].temp_dFF += DYNAMIC_VALUE;
	cell[temp_pos.first][temp_pos.second].occupant_id = agent_id;

	agent[agent_id].path.push_back(temp_pos);

	return temp_pos;
}
// obstacle choose the closer route to destination
PAIR_INT CS_CELLULAR_AUTOMATA::choose_direction_obstacle(int o_id, int com_id){


}
// emotion influenced by other agents
float CS_CELLULAR_AUTOMATA::getImpactFromOtherAgents(int id){

	float impact = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int a_id = agent[id].visible_agentID[i];
		impact = impact + agent[a_id].psychology.leadership * agent[id].getDistanceWeight(agent[a_id].position) * (agent[a_id].anxiety - agent[id].anxiety);
		//impact = impact + agent[a_id].psychology.leadership * agent[id].getDistanceWeight(agent[a_id].position) * exp(agent[a_id].anxiety - agent[id].anxiety);
		//cout << "distanceWeight: " << agent[id].getDistanceWeight(agent[a_id].position) << endl;
		//cout << "leaderShip: " << agent[i].psychology.leadership << endl;
		//cout << "anxA: " << agent[id].anxiety << endl;
		//cout << "anxB: " << agent[a_id].anxiety << endl;
		//cout << "anxDiff: " << agent[id].anxiety - agent[a_id].anxiety << endl;
		//cout << "impact: " << impact << endl;
	}
	//cout << impact / (agent[id].visible_agentID.size() + 0.00001f) << endl;
	//return impact;
	return impact / (agent[id].visible_agentID.size() + 0.00001f);
}

float CS_CELLULAR_AUTOMATA::getContagionStrengthFromContactAgents(int id){

	float overallContagionStrength = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int a_id = agent[id].visible_agentID[i];
		//contagionStrength += agent[a_id].psychology.leadership * agent[id].getDistanceWeight(agent[id].position) * agent[id].mPersonality.neuroticism;
		float contagionStrength = getContagionStrength(id, a_id);
		overallContagionStrength += contagionStrength;
	}
	return overallContagionStrength;
}

float CS_CELLULAR_AUTOMATA::getContagionStrength(int receiverID, int senderID){

	return agent[senderID].psychology.leadership * agent[receiverID].getDistanceWeight(agent[senderID].position) * agent[receiverID].mPersonality.neuroticism;
}

float CS_CELLULAR_AUTOMATA::getInfluenceStrengthFromContactAgents(int id){

	float influenceStrength = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int a_id = agent[id].visible_agentID[i];
		float contagionStrength = getContagionStrength(id, a_id);
		influenceStrength += (contagionStrength * agent[a_id].anxiety);
	}
	return influenceStrength / (getContagionStrengthFromContactAgents(id) + 0.00001f);
}

float CS_CELLULAR_AUTOMATA::combinationFunction(int id){

	float influenceStrength = getInfluenceStrengthFromContactAgents(id);
	float anxiety = agent[id].anxiety;
	float tendency = agent[id].mPersonality.neuroticism;
	//tendency = tendency + getContagionStrengthFromContactLeader(id) *(getLeaderEffect(id) - agent[id].mPersonality.neuroticism);
	tendency = tendency + (getLeaderEffect(id) - agent[id].mPersonality.neuroticism);
	//if ((getLeaderEffect(id) - agent[id].mPersonality.neuroticism) != 0)
	//	cout << (getLeaderEffect(id) - agent[id].mPersonality.neuroticism) << endl;
	if (tendency < 0.1f)
		tendency = 0.1f;
	agent[id].mPersonality.neuroticism = tendency;
	float t = getContagionStrengthFromContactLeader(id) *(getLeaderEffect(id) - agent[id].mPersonality.neuroticism);
	//tendency = getAverageLeaderNeuroticism(id);
	//if (t != 0)
	//{
	//	if (id < 10 && agent[id].position.first < 50 && !agent[id].compressive_leader)
	//		cout << "id: " << id << ": " << tendency << endl;
	//}
	//tendency = 1 - agent[id].psychology.leadership;
	//tendency = 0.5f;
	//tendency = agent_psychology.tendency;
	float bias = agent[id].mBias;
	bias = agent_psychology.bias;
	bias = getTimeInfluence(tendency, id);
	//bias = 0.0f;
	return tendency * (bias * (1 - (1 - influenceStrength) * (1 - anxiety)) + (1 - bias) * influenceStrength * anxiety) + (1 - tendency) * influenceStrength;
	//return (1 - tendency) * (bias * (1 - (1 - influenceStrength) * (1 - anxiety)) + (1 - bias) * influenceStrength * anxiety) + tendency * influenceStrength;
}

float CS_CELLULAR_AUTOMATA::getWillInfluenceStrengthFromContactAgents(int id){

	float influenceStrength = 0;
	float overallInfluenceStrength = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int a_id = agent[id].visible_agentID[i];
		float contagionStrength = getContagionStrength(id, a_id);
		influenceStrength += (contagionStrength * agent[a_id].psychology.willness);
	}
	influenceStrength = influenceStrength / (getContagionStrengthFromContactAgents(id) + 0.00001f);
	overallInfluenceStrength =
		agent_psychology.will_weight * influenceStrength +
		agent_psychology.anxiety_weight * (1 - agent[id].anxiety) +
		agent_psychology.groupWill_weight * getGroupEffect(id);
	return overallInfluenceStrength;
}

float CS_CELLULAR_AUTOMATA::willCombinationFunction(int id){

	float influenceStrength = getWillInfluenceStrengthFromContactAgents(id);
	float will = agent[id].psychology.willness;
	float tendency = agent[id].mPersonality.neuroticism;
	//tendency = tendency + getContagionStrengthFromContactLeader(id) * (getLeaderEffect(id) - agent[id].mPersonality.neuroticism);
	//tendency = getAverageLeaderNeuroticism(id);
	float bias = agent[id].mBias;
	bias = agent_psychology.bias;
	bias = getTimeInfluence(tendency, id);
	return tendency * (bias * (1 - (1 - influenceStrength) * (1 - will)) + (1 - bias) * influenceStrength * will) + (1 - tendency) * influenceStrength;
}

float CS_CELLULAR_AUTOMATA::testCombinationFunction(int id, float _tendency, float _bias){

	float influenceStrength = getInfluenceStrengthFromContactAgents(id);
	float anxiety = agent[id].anxiety;
	float tendency = _tendency;
	//tendency = 1 - agent[id].psychology.leadership;
	//tendency = 0.9f;
	float bias = _bias;
	//bias = 0.5f;
	return tendency * (bias * (1 - (1 - influenceStrength) * (1 - anxiety)) + (1 - bias) * influenceStrength * anxiety) + (1 - tendency) * influenceStrength;
}

float CS_CELLULAR_AUTOMATA::getContagionStrengthFromContactLeader(int id){

	float contagionStrength = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int e = agent[id].visible_agentID[i];
		if (!agent[e].compressive_leader)
			continue;
		contagionStrength += getContagionStrength(id, e);
	}
	return contagionStrength;
}

float CS_CELLULAR_AUTOMATA::getLeaderEffect(int id){

	if (agent[id].compressive_leader)
		return agent[id].mPersonality.neuroticism;

	float contagionStrength = getContagionStrengthFromContactLeader(id);
	float leaderEffect = 0;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int e = agent[id].visible_agentID[i];
		if (!agent[e].compressive_leader)
			continue;
		float s = getContagionStrength(id, e);
		//leaderEffect += (agent[e].mPersonality.neuroticism * (s / contagionStrength));
		leaderEffect += getContagionStrength(id, e) * agent[e].mPersonality.neuroticism;
	}
	if (leaderEffect == 0)
	{
		return agent[id].mPersonality.neuroticism;
	}
	return leaderEffect / (getContagionStrengthFromContactLeader(id) + 0.00001f);
}

float CS_CELLULAR_AUTOMATA::getAverageLeaderNeuroticism(int id){

	if (agent[id].compressive_leader)
		return agent[id].mPersonality.neuroticism;
	float totalNeuro = agent[id].mPersonality.neuroticism;
	int totalNum = 1;
	for (unsigned int i = 0; i < agent[id].visible_agentID.size(); i++)
	{
		int a_id = agent[id].visible_agentID[i];
		if (!agent[a_id].compressive_leader)
			continue;
		totalNeuro += agent[a_id].mPersonality.neuroticism;
		totalNum++;
	}
	totalNeuro /= totalNum;
	return totalNeuro;
}

float CS_CELLULAR_AUTOMATA::getGroupEffect(int id){

	float overallEffect = 0;
	float totalLeadership = 0;
	for (unsigned int i = 0; i < agent_group[agent[id].group_id].member.size(); i++)
	{
		int a_id = agent_group[agent[id].group_id].member[i];
		if (id == a_id)
			continue;
		totalLeadership += agent[a_id].psychology.leadership;
	}
	for (unsigned int i = 0; i < agent_group[agent[id].group_id].member.size(); i++)
	{
		int a_id = agent_group[agent[id].group_id].member[i];
		if (id == a_id)
			continue;
		overallEffect += (agent[a_id].psychology.willness * (agent[a_id].psychology.leadership / (totalLeadership+0.00001f)));
	}
	return overallEffect;
}

float CS_CELLULAR_AUTOMATA::getTimeInfluence(float neuroticism, int id){

	//if (agent[id].compressive_leader)
	//	return 1 - exp((double)-mTimeCounter / (agent_psychology.timeEffect + 100));
	return 1 - exp((double)-mTimeCounter / agent_psychology.timeEffect);
	return 1 - exp((double)-mTimeCounter / (agent_psychology.timeEffect * (1 - neuroticism)));
}

float CS_CELLULAR_AUTOMATA::getWeightedSum_EmotionIntention(int id){

	//return 0.3f *  
}

float AGENT::getDistanceWeight(PAIR_INT pos){

	float dis = sqrt(pow(position.first - pos.first, 2) + pow(position.second - pos.second, 2));
	//dis = abs(position.first - pos.first) + abs(position.second - pos.second);
	//return (1 / 1 + exp(dis));
	//cout << dis << endl;
	//cout << 1 - 1 / (1 + exp(-w * (dis - VISIBILITY_GRID_LENGTH))) << endl;
	return 1 - (1 / (1 + exp(-w * (dis - 3))));
}

float AGENT::averageResidualAnxiety(){

	float total = 0;
	for (int i = 0; i < remain_anxiety.size(); i++)
	{
		total += remain_anxiety[i];
	}
	return total / (remain_anxiety.size()+0.0001f) / 100;
}

bool CS_CELLULAR_AUTOMATA::check_groupWill(int id, int o_id){

	int gID = agent[id].group_id;
	for (unsigned int i = 0; i < agent_group[gID].member.size(); i++)
	{
		int a_id = agent_group[gID].member[i];
		if (agent[a_id].psychology.willness < obstacles[o_id].mWillThreshold)
		{
			return false;
		}
	}
	return true;
}