#include <Windows.h>
#include "myCrowdSimulation.h"
#include "../cs_parameters.h"

void MY_CROWD_SIMULATOR::update_State_Simulation(){

	if (start_simulation || next_simulation)
	{
		mCelluarAutomata->generateRandomAgentOrder();
		mCelluarAutomata->collectAverageAnxietyAroundObserveAgent();
		mCelluarAutomata->updateTimeStep();
		show_agent_info(); //2017/07/27
		mCelluarAutomata->check_cell_occupied(); //2017/10/28
		//mCelluarAutomata->agent_detect_obstalce(); //2017/10/31
		//mCelluarAutomata->update_agent_stress(); //2017/08/30
		mCelluarAutomata->copy_dynamicFF(); //2017/07/05
		mCelluarAutomata->update_distanceToLeader(); //2017/07/31
		//mCelluarAutomata->update_agent_psychology(); //2017/10/19
		mCelluarAutomata->update_leader_influence(); //2017/09/16
		//mCelluarAutomata->update_strength_agent(); //2017/10/22
		mCelluarAutomata->update_moveable_obstacle(); //2017/10/24
		//mCelluarAutomata->updateDensityAroundObstacle(); //2017/11/16
		//mCelluarAutomata->find_obstacleMustMove(); //2017/10/23
		//mCelluarAutomata->blocked_obstacle_findVolunteer(); //2017/10/23
		mCelluarAutomata->update_blocked_obstacle_position();
		mCelluarAutomata->update_volunteer_position(); //2017/09/22
		//mCelluarAutomata->update_blocked_obstacle_position(); //2017/09/22
		mCelluarAutomata->update_agent_position(); //2017/07/05
		mCelluarAutomata->updateVisibleAgent(); //2017/11/06
		mCelluarAutomata->update_agent_psychology(); //2017/10/19
		mCelluarAutomata->reset_agent_parameters();
		mCelluarAutomata->update_dynamicFF(); //2017/07/05
		//mCelluarAutomata->update_volunteer_towardToObstacle_action(); //2017/09/22
		//mCelluarAutomata->update_blocked_obstacle_position(); //2017/09/22
		//mCelluarAutomata->update_agent_closeToObstalce(); //2017/10/11
		mCelluarAutomata->updateInformationBetweenAgents();
		mCelluarAutomata->printDebugInformation();
		if (mCelluarAutomata->getExperimentType() == 1)
			doExperiment();
		if (next_simulation)
			next_simulation = false;
		else
			Sleep(milliseconds);
		//mCelluarAutomata->outputTimeInfluenceStrength();
	}
	else
	{
		//mCelluarAutomata->detectEditingMode();
	}
	if (!start_simulation && press_stop) //2017/08/06
	{
		pause_through_time = clock();
		pause_through_times = double(pause_through_time - pause_start);
	}
	mCelluarAutomata->update_pause_time(); //2017/08/06
}