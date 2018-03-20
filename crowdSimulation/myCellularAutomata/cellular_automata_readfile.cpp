#include"cellular_automata.h"
#include <iomanip>

void CS_CELLULAR_AUTOMATA::read_data(){

	cout << "start read the data from cellular_environment..." << endl;
	file.open("crowd_cellularAutomata/cellular_environment.txt");

	if (file.fail())
	{
		cout << "CROWD_SIMULATION::cellular_environment. Cannot open data file." << endl;
		return;
	}

	model = new FFMODEL();

	while (true)
	{
		string dataType;
		file >> dataType;
		if (dataType == "MAP_SIZE")
		{
			file >> model->size;
		}
		if (dataType == "DENSITY_OF_AGENT")
		{
			float density;
			file >> density;
			model->agent_number = (int)model->size * model->size * density;
		}
		if (dataType == "NUMBER_OF_GROUP_SIZE_TWO")
		{
			file >> model->size2_num;
		}
		if (dataType == "NUMBER_OF_GROUP_SIZE_THREE")
		{
			file >> model->size3_num;
		}
		if (dataType == "DECAY")
		{
			file >> model->decay;
		}
		if (dataType == "DIFFUSION")
		{
			file >> model->diffusion;
		}
		if (dataType == "KS")
		{
			file >> model->ks;
		}
		if (dataType == "KD")
		{
			file >> model->kd;
		}
		if (dataType == "KA")
		{
			file >> model->ka;
		}
		if (dataType == "KI")
		{
			file >> model->ki;
		}
		if (dataType == "KW")
		{
			file >> model->kw;
		}
		if (dataType == "KSS")
		{
			file >> model->ks_member;
		}
		if (dataType == "KDD")
		{
			file >> model->kd_member;
		}
		if (dataType == "WAITING")
		{
			file >> model->waiting;
		}
		if (dataType == "MEU")
		{
			file >> model->meu;
		}
		if (dataType == "DENSITY")
		{
			file >> model->density;
		}
		if (dataType == "D_MAX")
		{
			file >> model->d_max;
		}
		if (dataType == "exit")
		{
			int exit_x, exit_y;
			file >> exit_x >> exit_y;
			exits.push_back(pair<int, int>(exit_x, exit_y));
		}
		if (dataType == "EXIT")
		{
			int x, z, width;
			file >> x >> z >> width;
			Exit e;
			e.position = PAIR_INT(x, z);
			e.width = width;
			string s;
			file >> s;
			if (s == "Up"){
				e.direction = PAIR_INT(0, 1); 
			}
			else if (s == "Right"){
				e.direction = PAIR_INT(1, 0);
			}
			else if (s == "Down"){
				e.direction = PAIR_INT(0, -1);
			}
			else if (s == "Left"){
				e.direction = PAIR_INT(-1, 0);
			}
			mExit.push_back(e);
		}
		if (dataType == "MANUAL")
		{
			agent_psychology.obstacle_moveType = 0;
		}
		if (dataType == "AUTOMATIC")
		{
			agent_psychology.obstacle_moveType = 1;
		}
		if (dataType == "LEADER_PROPORTION")
		{
			float p;
			file >> p;
			guiParameter.leader_proportion = p;
		}
		if (dataType == "LEADER_DISTRUBUTION")
		{
			int type;
			file >> type;
			model->mLeaderDistrubution = type;
		}
		if (dataType == "SFF_TYPE")
		{
			int type;
			file >> type;
			model->mType = type;
		}
		if (dataType == "NEW_SEED")
		{
			int type;
			file >> type;
			model->mNewSeed = type;
		}
		if (dataType == "SHOW_AGENT_INFO")
		{
			int type;
			file >> type;
			model->mShowAgentInfo = type;
		}
		if (dataType == "PARALLEL_UPDATE")
		{
			int type;
			file >> type;
			model->mParallelUpdate = type;
		}
		if (dataType == "FRICTION")
		{
			float v;
			file >> v;
			model->mFriction = v;
		}
		if (dataType == "EXPERIMENT")
		{
			int type;
			file >> type;
			model->mExperiment = type;
		}
		if (dataType == "MOVE_OBSTACLE_TIME")
		{
			int t;
			file >> t;
			model->mMoveObstacleTime = t;
		}
		if (dataType == "MOVE_OBSTACLE_DISTANCE")
		{
			int dis;
			file >> dis;
			model->mMoveDistance = dis;
		}
		if (dataType == "RUN_TIMES")
		{
			int t;
			file >> t;
			model->mRunTimes = t;
		}
		if (dataType == "RECORD_VOLUNTEER_EVACUATION_TIME")
		{
			int t;
			file >> t;
			model->mRecordVolunteerEvacuationTime = !!t;
		}
		if (dataType == "DISTOEXIT")
		{
			file >> model->mDisToExit;
		}
		if (dataType == "VISABLE_TYPE")
		{
			file >> model->mVisableType;
		}
		if (dataType == "ANXIETY_EXPERIMENT")
		{
			file >> model->mAnxietyExperiment;
		}
		if (dataType == "TIME_EFFECT_EXPERIMENT")
		{
			file >> model->mTimeEffectExperiment;
		}
		if (dataType == "TENDENCY_EXPERIMENT")
		{
			file >> model->mTendencyExperiment;
		}
		if (dataType == "ENABLE_AGENT_COMMUNICATION")
		{
			file >> model->mCommunication;
		}
		if (dataType == "COMMUNICATION_PROB")
		{
			file >> model->mCommunicationProbability;
		}
		if (dataType == "VOLUNTEER_PRIORITY 1")
		{
			file >> model->mVolunteerPriority;
		}
		if (file.eof())
			break;
	}
	//agent->map_size = model->size;
	file.close();

	agent = new AGENT[model->agent_number];

	cout << "start read the data from psychology_model..." << endl;
	file.open("crowd_psychology_model/psychology_model.txt");
	if (file.fail())
	{
		cout << "CROWD_SIMULATION::psychology_model. Cannot open data file." << endl;
		return;
	}

	while (true)
	{
		string dataType;
		file >> dataType;
		if (dataType == "stress_accumulation_rate")
		{
			file >> stress_model.stress_accumulation_rate;
		}
		if (dataType == "max_stress")
		{
			file >> stress_model.max_stress;
		}
		if (dataType == "power_law_coefficient")
		{
			file >> stress_model.k;
		}
		if (dataType == "power_law_exponent")
		{
			file >> stress_model.n;
		}
		if (dataType == "standard_deviation")
		{
			file >> stress_model.standard_deviation;
		}
		if (dataType == "interpersonal_stress_radius")
		{
			file >> stress_model.stress_radius;
		}
		if (dataType == "area_stress_intensity")
		{
			file >> stress_model.area_stress_intensity;
		}
		if (dataType == "TENDENCY")
		{
			file >> agent_psychology.tendency;
		}
		if (dataType == "TIME_EFFECT")
		{
			file >> agent_psychology.timeEffect;
		}
		if (dataType == "ANXIETY_WEIGHT")
		{
			file >> agent_psychology.anxiety_weight;
		}
		if (dataType == "WILL_WEIGHT")
		{
			file >> agent_psychology.will_weight;
		}
		if (dataType == "GROUP_WILL_WEIGHT")
		{
			file >> agent_psychology.groupWill_weight;
		}
		if (file.eof())
		{
			break;
		}
	}
	file.close();

	file.open("crowd_cellularAutomata/cellular_evacuation_time.txt");
	while (true)
	{
		string s;
		file >> s;
		if (s == "SEED")
		{
			unsigned seed;
			file >> seed;
			model->seed = seed;
			cout << seed << endl;
		}
		if (s == "AGENT")
		{
			int id, time, escapeExit;
			file >> id >> time >> escapeExit;
			agent[id].escapeTime = (float)time;
			agent[id].mEscapeExit = escapeExit;
		}
		if (s == "MAX_TIME")
		{
			int m;
			file >> m;
			model->maxEvacuationTime = m;
		}
		if (file.eof())
			break;
	}
	file.close();

}

void CS_CELLULAR_AUTOMATA::output_data(){

	outputFile.open("crowd_cellularAutomata/cellular_staticFloorField.txt");
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			outputFile << std::left << setw(5) << setprecision(3) << setfill('0') << cell[i][j]._sFF << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::output_data(int id){

	outputFile.open("crowd_cellularAutomata/cellular_staticFloorField.txt");
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			outputFile << std::left << setw(5) << setprecision(3) << setfill('0') << volunteer_distance_map[id][i][j].sFF << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::output_colorTableWithoutExit(int o_id){

	outputFile.open("crowd_cellularAutomata/cellular_staticFloorField.txt");
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			outputFile << std::left << setw(5) << setprecision(3) << setfill('0') << cell[i][j].specific_color_sFF[o_id] << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::output_specialColorTable(int Exit_block[]){

	outputFile.open("crowd_cellularAutomata/cellular_specialFloorField.txt");
	int FloorFieldMapID = 0;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		FloorFieldMapID += (int)(pow(2, i) * Exit_block[i]);
	}
	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			outputFile << std::left << setw(5) << setprecision(3) << setfill('0') << cell[i][j].special_sFF[FloorFieldMapID] << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputInformation(){

	outputFile.open("crowd_cellularAutomata/cellular_evacuation_time.txt");
	outputFile << "SEED " << model->seed << endl;
	float max = -FLT_MAX;
	for (int i = 0; i < model->agent_number; i++)
	{
		outputFile << "AGENT " << i << " " << agent[i].escapeTime << " " << agent[i].mEscapeExit << endl;
		if (agent[i].escapeTime > max)
			max = agent[i].escapeTime;
	}
	outputFile << "MAX_TIME " << max;
	outputFile.close();
}


void CS_CELLULAR_AUTOMATA::outputAnxietyVariation(){

	outputFile.open("crowd_cellularAutomata/cellular_anxiety.txt");
	for (int i = 0; i < 10; i++)
	{
		int id = leader_pool[i];
		outputFile << "AGENT " << id;
		outputFile << " Leader, Neuroticism = " << agent[id].mPersonality.neuroticism << endl;
		//cout << agent[id].anxiety_variation.size() << endl;
		for (int j = 0; j < (int)agent[id].anxiety_variation.size(); j++)
		{
			outputFile << agent[id].anxiety_variation[j] << endl;
		}
	}
	int count = 0;
	for (int i = 0; i < 10; i++)
	{
		outputFile << "AGENT " << i;
		if (agent[i].compressive_leader)
			outputFile << " Leader, Neuroticism = " << agent[i].mPersonality.neuroticism << endl;
		else
			outputFile << " Memember. Neuroticism = " << agent[i].mPersonality.neuroticism << endl;
		//cout << agent[i].anxiety_variation.size() << endl;
		for (int j = 0; j < (int)agent[i].anxiety_variation.size(); j++)
		{
			outputFile << agent[i].anxiety_variation[j] << endl;
		}
		count++;
		if (count == 5)
			break;
	}
	outputFile << "===========================================================" << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputStatisticInfo(){

	outputFile.open("crowd_cellularAutomata/cellular_statistic.txt");
	float average0 = 0, average1 = -FLT_MAX;
	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		average0 += (int)agent[i].escapeTime;
		if (agent[i].escapeTime > average1)
			average1 = agent[i].escapeTime;
	}
	average0 /= model->agent_number;
	//cout << average0 << endl;
	//cout << average1 << endl;
	mExperimentData.mTotalAverageEvaciationTime[model->mTimes] = average0;
	mExperimentData.mTotalAverageLastAgentEvacuationTime[model->mTimes] = average1;
	model->mTimes++;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		outputFile << "EXIT_" << i << " WIDTH = " << mExit[i].width << endl;
	}
	float total = 0;
	for (int i = 0; i < model->agent_number; i++)
	{
		total += agent[i].escapeTime;
	}
	float average = total / model->agent_number;
	//outputFile << "Average Evacuation Time: " << total << endl;
	outputFile << endl;
	outputFile << "AVERAGE EVACUATION TIME: " << average << endl;
	vector<float> averageExitEvacuationTime(mExit.size());
	vector<int> mTotal(mExit.size());
	for (int i = 0; i < model->agent_number; i++)
	{
		int id = agent[i].mEscapeExit;
		averageExitEvacuationTime[id] += agent[i].escapeTime;
		mTotal[id]++;
	}
	for (int i = 0; i < (int)averageExitEvacuationTime.size(); i++)
	{
		averageExitEvacuationTime[i] = (float)(averageExitEvacuationTime[i]) / (mTotal[i] + 0.00001f);
		outputFile << "EXIT_" << i << " Average Evacuation Time: " << averageExitEvacuationTime[i] << endl;
		outputFile << "EXIT_" << i << " AVERAGE EVACUATION TIME: " << averageExitEvacuationTime[i] << endl;
		mExperimentData.mAverageEvacuationTime[i].push_back(averageExitEvacuationTime[i]);
		//cout << averageExitEvacuationTime[i] << endl;
	}
	outputFile << endl;
	outputFile << "TOTAL ESCAPE AGENT NUMBER: " << model->agent_number << endl;
	for (int i = 0; i < (int)averageExitEvacuationTime.size(); i++)
	{
		outputFile << "EXIT_" << i << " ESCAPE AGENT NUMBER: " << mTotal[i] << endl;
		mExperimentData.mEscapeAgentNumber[i].push_back(mTotal[i]);
		//cout << mTotal[i] << endl;
	}

	outputFile << endl;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		outputFile << "EXIT_" << i << " LAST EVACUATION TIME: " << mExit[i].mEscapeTime << endl;
		mExperimentData.mAverageLastAgentEvacuationTime[i].push_back(mExit[i].mEscapeTime);
		//cout << mExit[i].mEscapeTime << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputMaxDFF(){

	outputFile.open("crowd_cellularAutomata/cellular_dFF.txt", std::ios_base::app);
	//outputFile << "" << endl;
	float average = 0;
	for (int i = 0; i < (int)mDynamicFloorField.size(); i++)
	{
		average += mDynamicFloorField[i];
	}
	average /= mTimeCounter;
	outputFile << average << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputSeed(){

	outputFile.open("crowd_cellularAutomata/cellular_seed.txt", std::ios_base::app);
	outputFile << model->seed << endl;
	outputFile << "===================" << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputVolunteerEvacuation(){

	outputFile.open("crowd_cellularAutomata/cellular_volunteer_evacuation_time.txt", std::ios_base::app);
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputAgentPersonality(){

	outputFile.open("crowd_cellularAutomata/cellular_agent_personality.txt");
	//outputFile.open("crowd_cellularAutomata/cellular_seed.txt", std::ios_base::app);
	//if (outputFile.fail())
	//	return;
	for (unsigned int i = 0; i < model->agent_number; i++)
	{
		//cout << i << endl;
		//cout << agent[i].mPersonality.neuroticism << endl;
		outputFile << i << " " << agent[i].mPersonality.openness << " " << agent[i].mPersonality.conscientiousness << " " << agent[i].mPersonality.extraversion << " ";
		outputFile << agent[i].mPersonality.agreeableness << " " << agent[i].mPersonality.neuroticism << endl;
	}
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::readAgentPersonality(){

	file.open("crowd_cellularAutomata/cellular_agent_personality.txt");
	while (true)
	{
		for (int i = 0; i < model->agent_number; i++)
		{
			int id;
			file >> id;
			for (int j = 0; j < 5; j++)
			{
				switch (j)
				{
				case 0:
					file >> agent[id].mPersonality.openness;
					break;
				case 1:
					file >> agent[id].mPersonality.conscientiousness;
					break;
				case 2:
					file >> agent[id].mPersonality.extraversion;
					break;
				case 3:
					file >> agent[id].mPersonality.agreeableness;
					break;
				case 4:
					file >> agent[id].mPersonality.neuroticism;
					break;
				}
			}
		}
		if (file.eof())
			break;
	}
	file.close();
}

void CS_CELLULAR_AUTOMATA::outputStatisticsOnAnxiety_LeaderAndMember(){

	//outputFile.open("crowd_cellularAutomata/cellular_experiment/experiment_analysis_anxiety/cellular_statistical_anxiety.txt", std::ios_base::app);
	outputFile.open("crowd_cellularAutomata/cellular_experiment_anxiety.txt", ios_base::app);
	if (outputFile.fail())
		return;
	//cout << mAverageAnxeityAroundObserveAgent.size() << endl;
	if (model->mAnxietyExperiment == 1)
	{
		outputFile << "bias: " << agent_psychology.bias << endl;
		outputFile << "tendency: " << agent_psychology.tendency << endl;
	}
	if (model->mTimeEffectExperiment == 1)
	{
		outputFile << "timeEffect: " << agent_psychology.timeEffect << endl;
	}
	for (unsigned int j = 0; j < mObserverAgent.size(); j++)
	{
		outputFile << "AGENT_" << to_string(j) << endl;
		for (unsigned int i = 0; i < mObserveAgentAnxiety[j].size(); i++)
		{
			outputFile << mObserveAgentAnxiety[j][i] << endl;
		}
	}
	//cout << mAverageAnxeityAroundObserveAgent.size() << endl;
	for (unsigned int i = 0; i < mAverageAnxeityAroundObserveAgent.size(); i++)
	{
		outputFile << "STATISTICAL_AREA_" << to_string(i) << endl;
		for (unsigned int j = 0; j < mAverageAnxeityAroundObserveAgent[i].size(); j++)
		{
			outputFile << mAverageAnxeityAroundObserveAgent[i][j] << endl;
		}
	}
	outputFile << "===========================================================" << endl;
	if (!!model->mAnxietyExperiment)
		set_Bias();
	if (!!model->mTimeEffectExperiment)
		set_TimeEffect();
	if (!!model->mTendencyExperiment)
		set_Tendency();
	//cout << agent_psychology.bias * 10 << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputSFFExperiment(){

	outputFile.open("crowd_cellularAutomata/cellular_experiment_result.txt", std::ios_base::app);
	if (model->mType == 0)
		outputFile << "NOT CONSIDER EXIT WIDTH" << endl;
	else
		outputFile << "CONSIDER EXIT WIDTH" << endl;
	outputFile << "===========================================================" << endl;
	for (int i = 0; i < (int)mExit.size(); i++)
	{
		//cout << "Exit" << i << " Average Evacuation Time      Average Last Agent Escape Time      Std" << endl;
		int times = mExperimentData.mAverageEvacuationTime[i].size();
		float average0 = 0, average1 = 0, average2 = 0;
		float std0 = 0, std1 = 0, std2 = 0;
		for (int j = 0; j < times; j++)
		{
			average0 += mExperimentData.mAverageEvacuationTime[i][j];
			average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
			average2 += mExperimentData.mEscapeAgentNumber[i][j];
		}
		average0 /= times;
		average1 /= times;
		average2 /= times;
		for (int j = 0; j < times; j++)
		{
			std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
			std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
			std2 += pow(mExperimentData.mEscapeAgentNumber[i][j] - average2, 2);
		}
		std0 /= times;
		std1 /= times;
		std2 /= times;
		std0 = sqrt(std0);
		std1 = sqrt(std1);
		std2 = sqrt(std2);
		//mExperimentData.mStandardDeviation0.push_back(std0);
		//mExperimentData.mStandardDeviation1.push_back(std1);
		//mExperimentData.mStamderdDeviation2.push_back(std2);
		cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
		outputFile << "EXIT " << i << " WIDTH " << mExit[i].width << " POSITION " << mExit[i].position.first << " " << mExit[i].position.second << " DIRECTION " << mExit[i].direction.first << " " << mExit[i].direction.second << endl;
		outputFile << "AVERAGE EVACUATION TIME " << average0 << endl;
		outputFile << "STD " << std0 << endl;
		outputFile << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
		outputFile << "STD " << std1 << endl;
		outputFile << "AVERAGE ESCAPE AGENT NUMBER " << average2 << endl;
		outputFile << "STD " << std2 << endl;
		outputFile << "===========================================================" << endl;
	}
	//float total = 0;
	//for (int i = 0; i < model->agent_number; i++)
	//{
	//	total += agent[i].escapeTime;
	//}
	//outputFile << "TOTAL AVERAGE EVACUATION TIME " << total / model->agent_number << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputMoveObstacleTimeExperiment()
{
	FILE *fp;
	fp = fopen("crowd_cellularAutomata/cellular_experiment_result.txt", "a");
	std::ofstream fout("crowd_cellularAutomata/cellular_experiment_result.txt", std::ios_base::app);
	float average0 = 0, average1 = 0;
	float std0 = 0, std1 = 0;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageEvaciationTime.size()*/; i++)
	{
		average0 += mExperimentData.mTotalAverageEvaciationTime[i];
		average1 += mExperimentData.mTotalAverageLastAgentEvacuationTime[i];
	}
	average0 /= model->mRunTimes;
	average1 /= model->mRunTimes;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageLastAgentEvacuationTime.size()*/; i++)
	{
		std0 += pow(mExperimentData.mTotalAverageEvaciationTime[i] - average0, 2);
		std1 += pow(mExperimentData.mTotalAverageLastAgentEvacuationTime[i] - average1, 2);
	}
	std0 /= model->mRunTimes;
	std1 /= model->mRunTimes;
	std0 = sqrt(std0);
	std1 = sqrt(std1);
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int size = mExperimentData.mAverageEvacuationTime[i].size();
		for (int j = 0; j < size; j++)
		{
			average0 += mExperimentData.mAverageEvacuationTime[i][j];
			average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
		}
	}
	average0 /= model->agent_number;
	average1 /= model->agent_number;
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int size = mExperimentData.mAverageEvacuationTime[i].size(); 
		for (int j = 0; j < size; j++)
		{
			std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
			std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
		}
	}
	std0 /= model->agent_number;
	std1 /= model->agent_number;*/
	//mExperimentData.mStandardDeviation0.push_back(std0);
	//mExperimentData.mStandardDeviation1.push_back(std1);
	cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
	fout << "MOVE OBSTACLE TIME " << model->mMoveObstacleTime << endl;
	fout << "AVERAGE EVACUATION TIME " << average0 << endl;
	fout << "STD " << std0 << endl;
	fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
	fout << "STD " << std1 << endl;
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int times = mExperimentData.mAverageEvacuationTime[i].size();
		float average0 = 0, average1 = 0, average2 = 0;
		float std0 = 0, std1 = 0, std2 = 0;
		for (int j = 0; j < times; j++)
		{
			average0_ += mExperimentData.mAverageEvacuationTime[i][j];
			average1_ += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
			average0 += mExperimentData.mAverageEvacuationTime[i][j];
			average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
			average2 += mExperimentData.mEscapeAgentNumber[i][j];
		}
		average0 /= times;
		average1 /= times;
		average2 /= times;
		for (int j = 0; j < times; j++)
		{
			std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
			std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
			std2 += pow(mExperimentData.mEscapeAgentNumber[i][j] - average2, 2);
		}
		std0 /= times;
		std1 /= times;
		std2 /= times;
		mExperimentData.mStandardDeviation0.push_back(std0);
		mExperimentData.mStandardDeviation1.push_back(std1);
		mExperimentData.mStamderdDeviation2.push_back(std2);
		cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
		fout << "EXIT " << i << endl;
		fout << "AVERAGE EVACUATION TIME " << average0 << endl;
		fout << "STD " << std0 << endl;
		fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
		fout << "STD " << std1 << endl;
		fout << "AVERAGE ESCAPE AGENT NUMBER " << average2 << endl;
		fout << "STD " << std2 << endl << endl;
	}*/
	fout << "===========================================================" << endl;
	mExperimentData.mAverageEvacuationTime.clear();
	mExperimentData.mAverageLastAgentEvacuationTime.clear();
	mExperimentData.mEscapeAgentNumber.clear();
	mExperimentData.mStandardDeviation0.clear();
	mExperimentData.mStandardDeviation1.clear();
	mExperimentData.mStamderdDeviation2.clear();
	//mExperimentData.mTotalAverageEvaciationTime.clear();
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.clear();
	vector<vector<float>>().swap(mExperimentData.mAverageEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mAverageLastAgentEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mEscapeAgentNumber);
	vector<float>().swap(mExperimentData.mStandardDeviation0);
	vector<float>().swap(mExperimentData.mStandardDeviation1);
	vector<float>().swap(mExperimentData.mStamderdDeviation2);
	//mExperimentData.mTotalAverageEvaciationTime.swap(mt0);
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.swap(mt1);
	model->mTimes = 0;
}

void CS_CELLULAR_AUTOMATA::outputMoveObstacleDistanceExperiment(){

	std::ofstream fout("crowd_cellularAutomata/cellular_experiment_move_obstacle_dis.txt", std::ios_base::app);
	float average0 = 0, average1 = 0;
	float std0 = 0, std1 = 0;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageEvaciationTime.size()*/; i++)
	{
		average0 += mExperimentData.mTotalAverageEvaciationTime[i];
		average1 += mExperimentData.mTotalAverageLastAgentEvacuationTime[i];
	}
	average0 /= model->mRunTimes;
	average1 /= model->mRunTimes;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageLastAgentEvacuationTime.size()*/; i++)
	{
		std0 += pow(mExperimentData.mTotalAverageEvaciationTime[i] - average0, 2);
		std1 += pow(mExperimentData.mTotalAverageLastAgentEvacuationTime[i] - average1, 2);
	}
	std0 /= model->mRunTimes;
	std1 /= model->mRunTimes;
	std0 = sqrt(std0);
	std1 = sqrt(std1);
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int size = mExperimentData.mAverageEvacuationTime[i].size();
	for (int j = 0; j < size; j++)
	{
	average0 += mExperimentData.mAverageEvacuationTime[i][j];
	average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	}
	}
	average0 /= model->agent_number;
	average1 /= model->agent_number;
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int size = mExperimentData.mAverageEvacuationTime[i].size();
	for (int j = 0; j < size; j++)
	{
	std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
	std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
	}
	}
	std0 /= model->agent_number;
	std1 /= model->agent_number;*/
	//mExperimentData.mStandardDeviation0.push_back(std0);
	//mExperimentData.mStandardDeviation1.push_back(std1);
	cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
	fout << "MOVE OBSTACLE DISTANCE " << model->mMoveDistance << endl;
	fout << "AVERAGE EVACUATION TIME " << average0 << endl;
	fout << "STD " << std0 << endl;
	fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
	fout << "STD " << std1 << endl;
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int times = mExperimentData.mAverageEvacuationTime[i].size();
	float average0 = 0, average1 = 0, average2 = 0;
	float std0 = 0, std1 = 0, std2 = 0;
	for (int j = 0; j < times; j++)
	{
	average0_ += mExperimentData.mAverageEvacuationTime[i][j];
	average1_ += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	average0 += mExperimentData.mAverageEvacuationTime[i][j];
	average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	average2 += mExperimentData.mEscapeAgentNumber[i][j];
	}
	average0 /= times;
	average1 /= times;
	average2 /= times;
	for (int j = 0; j < times; j++)
	{
	std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
	std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
	std2 += pow(mExperimentData.mEscapeAgentNumber[i][j] - average2, 2);
	}
	std0 /= times;
	std1 /= times;
	std2 /= times;
	mExperimentData.mStandardDeviation0.push_back(std0);
	mExperimentData.mStandardDeviation1.push_back(std1);
	mExperimentData.mStamderdDeviation2.push_back(std2);
	cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
	fout << "EXIT " << i << endl;
	fout << "AVERAGE EVACUATION TIME " << average0 << endl;
	fout << "STD " << std0 << endl;
	fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
	fout << "STD " << std1 << endl;
	fout << "AVERAGE ESCAPE AGENT NUMBER " << average2 << endl;
	fout << "STD " << std2 << endl << endl;
	}*/
	fout << "===========================================================" << endl;
	mExperimentData.mAverageEvacuationTime.clear();
	mExperimentData.mAverageLastAgentEvacuationTime.clear();
	mExperimentData.mEscapeAgentNumber.clear();
	mExperimentData.mStandardDeviation0.clear();
	mExperimentData.mStandardDeviation1.clear();
	mExperimentData.mStamderdDeviation2.clear();
	//mExperimentData.mTotalAverageEvaciationTime.clear();
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.clear();
	vector<vector<float>>().swap(mExperimentData.mAverageEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mAverageLastAgentEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mEscapeAgentNumber);
	vector<float>().swap(mExperimentData.mStandardDeviation0);
	vector<float>().swap(mExperimentData.mStandardDeviation1);
	vector<float>().swap(mExperimentData.mStamderdDeviation2);
	//mExperimentData.mTotalAverageEvaciationTime.swap(mt0);
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.swap(mt1);
	model->mTimes = 0;
}

void CS_CELLULAR_AUTOMATA::outputObstacleLocatedExperiment(){

	std::ofstream fout("crowd_cellularAutomata/cellular_experiment_obstacle_located.txt", std::ios_base::app);
	float average0 = 0, average1 = 0;
	float std0 = 0, std1 = 0;
	fout << "MOVE DISTANCE " << model->mDisToExit << endl;
	fout << "**********************************************************" << endl;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageEvaciationTime.size()*/; i++)
	{
		average0 += mExperimentData.mTotalAverageEvaciationTime[i];
		average1 += mExperimentData.mTotalAverageLastAgentEvacuationTime[i];
	}
	average0 /= model->mRunTimes;
	average1 /= model->mRunTimes;
	for (unsigned int i = 0; i < model->mRunTimes/*mExperimentData.mTotalAverageLastAgentEvacuationTime.size()*/; i++)
	{
		std0 += pow(mExperimentData.mTotalAverageEvaciationTime[i] - average0, 2);
		std1 += pow(mExperimentData.mTotalAverageLastAgentEvacuationTime[i] - average1, 2);
	}
	std0 /= model->mRunTimes;
	std1 /= model->mRunTimes;
	std0 = sqrt(std0);
	std1 = sqrt(std1);
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int size = mExperimentData.mAverageEvacuationTime[i].size();
	for (int j = 0; j < size; j++)
	{
	average0 += mExperimentData.mAverageEvacuationTime[i][j];
	average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	}
	}
	average0 /= model->agent_number;
	average1 /= model->agent_number;
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int size = mExperimentData.mAverageEvacuationTime[i].size();
	for (int j = 0; j < size; j++)
	{
	std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
	std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
	}
	}
	std0 /= model->agent_number;
	std1 /= model->agent_number;*/
	//mExperimentData.mStandardDeviation0.push_back(std0);
	//mExperimentData.mStandardDeviation1.push_back(std1);
	cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
	fout << "MOVE OBSTACLE DISTANCE " << model->mMoveDistance << endl;
	fout << "AVERAGE EVACUATION TIME " << average0 << endl;
	fout << "STD " << std0 << endl;
	fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
	fout << "STD " << std1 << endl;
	/*for (unsigned int i = 0; i < mExit.size(); i++)
	{
	int times = mExperimentData.mAverageEvacuationTime[i].size();
	float average0 = 0, average1 = 0, average2 = 0;
	float std0 = 0, std1 = 0, std2 = 0;
	for (int j = 0; j < times; j++)
	{
	average0_ += mExperimentData.mAverageEvacuationTime[i][j];
	average1_ += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	average0 += mExperimentData.mAverageEvacuationTime[i][j];
	average1 += mExperimentData.mAverageLastAgentEvacuationTime[i][j];
	average2 += mExperimentData.mEscapeAgentNumber[i][j];
	}
	average0 /= times;
	average1 /= times;
	average2 /= times;
	for (int j = 0; j < times; j++)
	{
	std0 += pow(mExperimentData.mAverageEvacuationTime[i][j] - average0, 2);
	std1 += pow(mExperimentData.mAverageLastAgentEvacuationTime[i][j] - average1, 2);
	std2 += pow(mExperimentData.mEscapeAgentNumber[i][j] - average2, 2);
	}
	std0 /= times;
	std1 /= times;
	std2 /= times;
	mExperimentData.mStandardDeviation0.push_back(std0);
	mExperimentData.mStandardDeviation1.push_back(std1);
	mExperimentData.mStamderdDeviation2.push_back(std2);
	cout << std::left << setw(5) << setprecision(3) << average0 << std::left << setw(5) << setprecision(3) << average1 << std::left << setw(5) << setprecision(3) << std0 << " " << std1 << endl;
	fout << "EXIT " << i << endl;
	fout << "AVERAGE EVACUATION TIME " << average0 << endl;
	fout << "STD " << std0 << endl;
	fout << "AVERAGE LAST AGENT ESCAPE TIME " << average1 << endl;
	fout << "STD " << std1 << endl;
	fout << "AVERAGE ESCAPE AGENT NUMBER " << average2 << endl;
	fout << "STD " << std2 << endl << endl;
	}*/
	fout << "===========================================================" << endl;
	mExperimentData.mAverageEvacuationTime.clear();
	mExperimentData.mAverageLastAgentEvacuationTime.clear();
	mExperimentData.mEscapeAgentNumber.clear();
	mExperimentData.mStandardDeviation0.clear();
	mExperimentData.mStandardDeviation1.clear();
	mExperimentData.mStamderdDeviation2.clear();
	//mExperimentData.mTotalAverageEvaciationTime.clear();
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.clear();
	vector<vector<float>>().swap(mExperimentData.mAverageEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mAverageLastAgentEvacuationTime);
	vector<vector<float>>().swap(mExperimentData.mEscapeAgentNumber);
	vector<float>().swap(mExperimentData.mStandardDeviation0);
	vector<float>().swap(mExperimentData.mStandardDeviation1);
	vector<float>().swap(mExperimentData.mStamderdDeviation2);
	//mExperimentData.mTotalAverageEvaciationTime.swap(mt0);
	//mExperimentData.mTotalAverageLastAgentEvacuationTime.swap(mt1);
	model->mTimes = 0;
}

void CS_CELLULAR_AUTOMATA::outputTimeInfluenceStrength(){

	outputFile.open("crowd_cellularAutomata/cellular_exponential_timeInfluence.txt", std::ios_base::app);
	//outputFile << getTimeInfluence(1) << endl;
	//cout << getTimeInfluence(1) << endl;
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::outputTendencyInfluence(){

	outputFile.open("crowd_cellularAutomata/cellular_experiment_tendency.txt", std::ios_base::app);
	
	outputFile.close();
}

void CS_CELLULAR_AUTOMATA::set_obstacle(){

	//cout << "start read the data from cellular_obstacle..." << endl;
	file.open("crowd_cellularAutomata/cellular_scene.txt");

	if (file.fail()){
		cout << "CROWD_SIMULATION::cellular_obstacle. Cannot open data file." << endl;
		return;
	}

	while (true)
	{
		string s;
		file >> s;
		if (s == "/")
		{
			break;
		}
		if (file.eof())
			break;
		if (s == "OBSTACLE")
		{
			vector<pair<int, int>> obstacle_component;
			vector<int> volunteer_id;
			vector<vector<int>> candidate_id;
			OBSTACLE o;
			o.block_exit = false;
			int id, size = 0;
			id = obstacles.size();
			int minx, maxx, minz, maxz;
			file >> minx >> maxx >> minz >> maxz;
			for (int i = 0; i < model->size; i++)
			{
				for (int j = 0; j < model->size; j++)
				{
					if (i >= minx && i <= maxx && j >= minz && j <= maxz)
					{
						if (cell[i][j].obstacle)
							continue;
						size++;
						PAIR_INT pos = PAIR_INT(i, j);
						obstacle_component.push_back(pos);
						if (cell[i][j].cell_type == 1)
							o.block_exit = true;
						cell[i][j].obstacle = true;
						cell[i][j].cell_type = 2;
						cell[i][j].obstacle_ = 0;
						cell[i][j].obstacle_id = id;
					}
				}
			}
			volunteer_id.resize(size);
			candidate_id.resize(size);
			for (int i = 0; i < size; i++)
			{
				volunteer_id[i] = -1;
			}
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
			o.id = id;
			o.size = size;
			o.highest_z = maxz;
			o.highest_x = maxx;
			o.lowest_x = minx;
			o.lowest_z = minz;
			o.component = obstacle_component;
			o.volunteer_id = volunteer_id;
			o.candidate_id = candidate_id;
			o.mWillThreshold = (float)obstacle_component.size() * OBSTACLE_UNIT;
			//o.mWillThreshold = 0;
			obstacles.push_back(o);
		}
	}
	file.close();
	mRemovalObstacleDestinationAFF.resize(obstacles.size());
	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		mRemovalObstacleDestinationAFF[i].resize(model->size);
		for (unsigned int j = 0; j < model->size; j++)
		{
			mRemovalObstacleDestinationAFF[i][j].resize(model->size, 0.0f);
		}
	}
	/*cout << "start read the data from cellular_test_obstacle..." << endl;
	file.open("crowd_cellularAutomata/cellular_test_obstacle.txt");

	if (file.fail()){
		cout << "CROWD_SIMULATION::cellular_test_obstacle. Cannot open data file." << endl;
		return;
	}

	while (true)
	{
		vector<pair<int, int>> obstacle_component;
		vector<int> volunteer_id;
		vector<bool> component_removable;
		int id, size;
		id = obstacles.size();
		file >> size;
		cout << size << endl;
		for (int i = 0; i < size; i++)
		{
			int x, z;
			pair<int, int> pos;
			file >> x >> z;
			if (!isValid(x, z))
			{
				//size--; illegal
				continue;
			}
			pos = pair<int, int>(x, z);
			obstacle_component.push_back(pos);
			cell[x][z].obstacle = true;
			cell[x][z].cell_type = 2;
			cell[x][z].obstacle_ = 0;
			cell[x][z].obstacle_id = id;
		}
		volunteer_id.resize(size);
		component_removable.resize(size);
		for (int i = 0; i < size; i++)
		{
			volunteer_id[i] = -1;
			component_removable[i] = false;
		}
		//
		OBSTACLE o;
		o.id = id;
		o.size = size;
		o.component = obstacle_component;
		o.volunteer_id = volunteer_id;
		//o.clicked_o_id = 1; //test
		//o.block_exit = true; //test
		o.block_exit = false;
		obstacles.push_back(o);
		if (file.eof())
			break;

	}*/
	/*for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i].density_map.resize(obstacles[i].component.size());
		for (int j = 0; j < obstacles[i].density_map.size(); j++)
		{
			obstacles[i].density_map[j].resize(VISIBILITY_DISTANCE);
			for (int k = 0; k < VISIBILITY_DISTANCE; k++)
			{
				obstacles[i].density_map[j][k].resize(VISIBILITY_DISTANCE);
				for (int z = 0; z < VISIBILITY_DISTANCE; z++)
				{
					obstacles[i].density_map[j][k][z] = false;
				}
			}
		}
	}*/
	//print information
	/*for (int i = 0; i < obstacles.size(); i++)
	{
		cout << "id: " << obstacles[i].id << endl;
		cout << "size: " << obstacles[i].size << endl;
		for (int j = 0; j < obstacles[i].component.size(); j++)
		{
			cout << "component" << j << ": " << obstacles[i].component[j].first << ", " << obstacles[i].component[j].second << endl;
		}
		for (int j = 0; j < obstacles[i].volunteer_id.size(); j++)
		{
			cout << "volunteer id: " << obstacles[i].volunteer_id[j] << endl;
		}
	}*/
}

void CS_CELLULAR_AUTOMATA::set_wall(){

	file.open("crowd_cellularAutomata/cellular_scene.txt");
	wall.clear();
	while (true)
	{
		string s;
		file >> s;
		if (s == "WALL")
		{
			int minx, maxx, minz, maxz;
			file >> minx >> maxx >> minz >> maxz;
			for (int i = 0; i < model->size; i++)
			{
				for (int j = 0; j < model->size; j++)
				{
					if (i >= minx && i <= maxx && j >= minz && j <= maxz)
					{
						if (cell[i][j].obstacle)
							continue;
						wall.push_back(PAIR_INT(i, j));
						cell[i][j].cell_type = 3;
						cell[i][j].obstacle_ = 0;
					}
				}
			}
		}
		if (file.eof())
			break;
	}
	for (int i = -1; i < model->size + 1; i++)
	{
		for (int j = -1; j < model->size + 1; j++)
		{
			if (i == -1 || j == -1 || i == model->size || j == model->size)
			{
				wall.push_back(PAIR_INT(i, j));
			}
		}
	}
	for (auto &w : wall){
		//cout << w.first << " " << w.second << endl;
		if (isValid(w.first, w.second))
		{
			cell[w.first][w.second].obstacle_ = 0;
			cell[w.first][w.second].cell_type = 3;
		}
	}
}

void CS_CELLULAR_AUTOMATA::load_simulation(){

	file.open("crowd_cellularAutomata/cellular_save.txt");
	while (true)
	{
		string dataType;
		file >> dataType;
		if (dataType == "SEED")
		{
			unsigned seed;
			file >> seed;
			model->seed = seed;
		}
		if (file.eof())
			break;
	}
}