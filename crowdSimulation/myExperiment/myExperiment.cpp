#include "myExperiment.h"

using namespace std;

Experiment::Experiment()
{
	mType = 0;
	mRunTimes = 0;
	mTimes = 100;
	readConfig();
	ca = new CS_CELLULAR_AUTOMATA();
}

void Experiment::readConfig()
{
	ifstream file;
	file.open("crowd_cellularAutomata/cellular_experiment.txt");
	if (file.fail())
		return;
	while (true)
	{
		string s;
		file >> s;
		if (s == "TYPE")
		{
			int type;
			file >> type;
			mType = type;
		}
		if (s == "TIMES")
		{
			int t;
			file >> t;
			mRunTimes = t;
		}
		if (file.eof())
			break;
	}
}

void Experiment::run()
{
	if (mType == 4)
	{
		ca->set_SameSeed();
		ca->reset();
		ca->set_ObserveAgent();
	}
	while (ca->get_remain_agent() != "0")
	{
		update();
		switch (mType)
		{
		case 0:
			experiment_calculationOfSFF();
			break;
		case 1:
			experiment_obstacle_moveTime();
			break;
		case 2:
			experiment_obstacle_moveDistance();
			break;
		case 3:
			experiment_obstacle_destination();
			break;
		case 4:
			experiment_anxiety();
			break;
		case 5:
			experiment_leader_distribution();
			break;
		case 6:
			experiment_leader_proportion();
			break;
		}
	}
}

void Experiment::experiment_calculationOfSFF()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0)
	{
		mTimes--;
		ca->outputSFFExperiment();
	}
}

void Experiment::experiment_obstacle_moveTime()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0 && mRunTimes > 0)
	{
		ca->outputMoveObstacleDistanceExperiment();
		mRunTimes--;
		mTimes = 100;
		ca->setMoveObstacleTime();
	}
}

void Experiment::experiment_obstacle_moveDistance()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0 && mRunTimes > 0)
	{
		ca->outputMoveObstacleDistanceExperiment();
		mRunTimes--;
		mTimes = 100;
		ca->setMoveObstacleDistance();
	}
}

void Experiment::experiment_obstacle_destination()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0 && mRunTimes > 0)
	{
		ca->outputObstacleLocatedExperiment();
		mRunTimes--;
		mTimes = 100;
		ca->setObstacleDestination();
	}
	if (ca->get_remain_agent() != "0" && ca->getTimeCounter() > 300)
	{
		cout << "reset" << endl;
		ca->reset();
	}
}

void Experiment::experiment_anxiety()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		ca->outputStatisticsOnAnxiety_LeaderAndMember();
		ca->reset();	
	}
}

void Experiment::experiment_leader_distribution()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0 && mRunTimes > 0)
	{
		ca->outputLeaderDistributionExperiment();
		mRunTimes--;
		mTimes = 100;
		ca->setMoveObstacleDistance();
	}
}

void Experiment::experiment_leader_proportion()
{
	if (ca->get_remain_agent() == "0" && mTimes > 0)
	{
		cout << "Times " << mTimes << endl;
		mTimes--;
		ca->reset();
	}
	if (mTimes == 0 && mRunTimes > 0)
	{
		ca->outputLeaderProportionExperiment();
		mRunTimes--;
		mTimes = 100;
		ca->setMoveObstacleDistance();
	}
}

void Experiment::update()
{
	/*ca->generateRandomAgentOrder();
	ca->updateTimeStep();
	ca->check_cell_occupied();
	ca->agent_detect_obstalce();
	ca->update_agent_stress();
	ca->copy_dynamicFF();
	ca->update_distanceToLeader();
	ca->update_leader_influence();
	ca->update_moveable_obstacle();
	ca->update_volunteer_towardToObstacle_action();
	ca->update_blocked_obstacle_position();
	ca->update_agent_position();
	ca->updateVisibleAgent();
	ca->update_agent_psychology();
	ca->reset_agent_parameters();
	ca->update_dynamicFF();*/
	ca->generateRandomAgentOrder();
	ca->collectAverageAnxietyAroundObserveAgent();
	ca->updateTimeStep();
	ca->check_cell_occupied(); //2017/10/28
	//ca->agent_detect_obstalce(); //2017/10/31
	//ca->update_agent_stress(); //2017/08/30
	ca->copy_dynamicFF(); //2017/07/05
	ca->update_distanceToLeader(); //2017/07/31
	//ca->update_agent_psychology(); //2017/10/19
	ca->update_leader_influence(); //2017/09/16
	//ca->update_strength_agent(); //2017/10/22
	ca->update_moveable_obstacle(); //2017/10/24
	//ca->updateDensityAroundObstacle(); //2017/11/16
	//ca->find_obstacleMustMove(); //2017/10/23
	//ca->blocked_obstacle_findVolunteer(); //2017/10/23
	ca->update_blocked_obstacle_position();
	ca->update_volunteer_position(); //2017/09/22
	//ca->update_blocked_obstacle_position(); //2017/09/22
	ca->update_agent_position(); //2017/07/05
	ca->updateVisibleAgent(); //2017/11/06
	ca->update_agent_psychology(); //2017/10/19
	ca->reset_agent_parameters();
	ca->update_dynamicFF(); //2017/07/05
	//ca->update_volunteer_towardToObstacle_action(); //2017/09/22
	//ca->update_blocked_obstacle_position(); //2017/09/22
	//ca->update_agent_closeToObstalce(); //2017/10/11
	ca->updateInformationBetweenAgents();
	ca->printDebugInformation();
}