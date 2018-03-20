#include "myCrowdSimulation.h"
#include <ctime>

MY_CROWD_SIMULATOR::MY_CROWD_SIMULATOR()
{
	millisecond = 0;
	start_simulation = false;
	press_stop = false;
	total_pause_through_times = 0;
	pause_through_times = 0;
	start = clock();
	pause_start = clock();
	next_simulation = false;
	last_time = clock();
	mRecordEvacuationTime = false;
	times = 100;
	mExperimentType = 0;
	readExperimentEnvConfig();
}

void MY_CROWD_SIMULATOR::reset_CellularAutomata()
{
	mCelluarAutomata->reset();
	start_simulation = false;
	press_stop = false;
}

void MY_CROWD_SIMULATOR::start_CellularAutomata()
{
	mCelluarAutomata->start();
	mCelluarAutomata->copy_preUpdate_time();
	// if already press the 'pause' button in a simulation, then it do not need to reset the start time
	if (!press_stop && !start_simulation)
		start = clock();
	start_simulation = true;
}

void MY_CROWD_SIMULATOR::stop_CellularAutomata()
{
	mCelluarAutomata->pause();
	start_simulation = false;
	press_stop = true;
}
/*
show remain agent and through time on window title 
*/
void MY_CROWD_SIMULATOR::show_agent_info()
{
	remain_agent = mCelluarAutomata->get_remain_agent();
	if (remain_agent != "0")
	{
		time = mCelluarAutomata->get_through_time();
		timeStep = mCelluarAutomata->get_through_timeStep();
	}
	string title = " Residual agent: " + remain_agent + ", Time: " + time + ", TimeStep: " + timeStep;
	const char* title_name = title.c_str();
	glutSetWindowTitle(title_name);
}
/*
show the agent be clicked's ID, group_ID, group size
*/
void MY_CROWD_SIMULATOR::show_individual_agent_info() const
{
	mCelluarAutomata->show_agent_info();
}

void MY_CROWD_SIMULATOR::set_corner_coordinate(int x, int y) const
{
	mCelluarAutomata->set_corner_coordinate(x, y);
}

void MY_CROWD_SIMULATOR::get_agent_info(int mx, int mz) const
{
	mCelluarAutomata->get_agent_info(mx, mz);
}

void MY_CROWD_SIMULATOR::set_cur_mouse_position(int mx, int mz) const
{
	mCelluarAutomata->set_cur_mouse_position(mx, mz);
}

void MY_CROWD_SIMULATOR::add_new_group()
{
	mCelluarAutomata->add_new_group();
}

void MY_CROWD_SIMULATOR::set_group_leader()
{
	mCelluarAutomata->set_group_leader();
}


//void MY_CROWD_SIMULATOR::read

int MY_CROWD_SIMULATOR::getMillisecond() const
{
	return millisecond;
}

bool MY_CROWD_SIMULATOR::handleKeyEvent(unsigned char key)
{
	bool flgHandled = false;
	switch (key){
		case 'a': //2017/08/07
			mCelluarAutomata->add_agent_to_group();
			flgHandled = true;
			break;
		case 'd': //2017/08/07	
			mCelluarAutomata->remove_agent_from_group();
			flgHandled = true;
			break;
		case 'f': //2017/09/01
			mCelluarAutomata->add_stress_source();
			flgHandled = true;
			break;
		case 'g': //2017/09/12
			mCelluarAutomata->add_dynamic_stress_source();
			flgHandled = true;
			break;
		case 'm': //2017/09/07
			mCelluarAutomata->set_obstacleMustRemove();
			flgHandled = true;
			break;
		case ' ': //2017/09/08
			mCelluarAutomata->choose_obstacle_destination();
			flgHandled = true;
			break;
		case '.': //2017/09/12
			next_simulation = true;
			flgHandled = true;
			break;
		case 'c':
			mCelluarAutomata->change_visible_state();
			flgHandled = true;
			break;
		case 's':
			mCelluarAutomata->saveImage();
			flgHandled = true;
			break;
		case 'r':
			mCelluarAutomata->reset();
			flgHandled = true;
			break;
		case 'x':
			mCelluarAutomata->addStatisticalArea();
			flgHandled = true;
			break;
		case 'z':
			flgHandled = true;
			break;
		case 'q':
			mCelluarAutomata->changeNeighborType();
			flgHandled = true;
	}
	return flgHandled;
}

void MY_CROWD_SIMULATOR::doExperiment()
{
	switch (mExperimentType)
	{
	case 0:
		experiment0();
		break;
	case 1:
		experiment1();
		break;
	case 2:
		experiment2();
		break;
	}
}

void MY_CROWD_SIMULATOR::experiment0()
{
	if (remain_agent == "0" && times > 0)
	{
		cout << "Times " << times << endl;
		times--;
		mCelluarAutomata->reset();
	}
	if (times == 0)
	{
		mCelluarAutomata->outputSFFExperiment();
		start_simulation = false;
	}
}

void MY_CROWD_SIMULATOR::experiment1()
{
	if (remain_agent == "0" && times > 0)
	{
		cout << "Times " << times << endl;
		times--;
		mCelluarAutomata->reset();
	}
	if (times == 0 && mExperimentTimes > 0)
	{
		mCelluarAutomata->outputMoveObstacleTimeExperiment();
		mExperimentTimes--;
		times = mCelluarAutomata->getRunTimes();
		mCelluarAutomata->setMoveObstacleTime();
	}
	if (times == 0 && mExperimentTimes == 0)
	{
		start_simulation = false;
	}
}

void MY_CROWD_SIMULATOR::experiment2()
{
	if (remain_agent == "0" && times > 0)
	{
		cout << "Times " << times << endl;
		times--;
		mCelluarAutomata->reset();
	}
	if (times == 0 && mExperimentTimes > 0)
	{
		mCelluarAutomata->outputMoveObstacleDistanceExperiment();
		mExperimentTimes--;
		times = mCelluarAutomata->getRunTimes();
		mCelluarAutomata->setMoveObstacleDistance();
	}
	if (times == 0 && mExperimentTimes == 0)
	{
		start_simulation = false;
	}
}

void MY_CROWD_SIMULATOR::readExperimentEnvConfig()
{
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
			mExperimentType = type;
		}
		if (s == "TIMES")
		{
			int t;
			file >> t;
			mExperimentTimes = t;
		}
		if (file.eof())
			break;
	}
}
