//
// Instructor: Yu-An Chang
// Email:	andy123654.cs04g@g2.nctu.edu.tw
//			j60508888@gmail.com
//
// Date: 2017/04/06
//
#ifndef __MY_CROWD_SIMULATOR_H_
#define __MY_CROWD_SIMULATOR_H_
#include <string>
#include <iostream>
#include "../crowdSimulation_Basic/crowdSimulation_Basic.h"

class MY_CROWD_SIMULATOR : public CROWD_SIMULATION_BASIC 
{
public:
	MY_CROWD_SIMULATOR( );
	virtual bool handleKeyEvent(unsigned char key);
	virtual void draw_CelluarAutomata() const;
	virtual void draw_Lattice() const; 
	virtual void draw_Border() const;
	virtual void draw_ColorTable() const;
	virtual void draw_cell_exit_segments(int, int) const;
	virtual void draw_dFF_ColorTable() const;
	virtual void draw_group() const;
	virtual void draw_specific_group() const;
	virtual void draw_group_circle() const;
	virtual void draw_guider() const;
	virtual void draw_agent_path() const;
	virtual void draw_escape_direction() const;
	virtual void draw_stress_area() const;
	virtual void draw_compressive_leader() const;
	virtual void draw_strength_agent() const;
	virtual void draw_selfish_agent() const;
	virtual void draw_neutral_agent() const;
	virtual void draw_brave_agent() const;
	virtual void draw_occupied_cell() const;
	virtual void draw_volunteer_colorTable() const;
	virtual void draw_agent_anxiety() const;
	virtual void draw_agent_willness() const;
	virtual void draw_obstalce_density() const;
	virtual void draw_colorTable_withoutExit() const;
	virtual void draw_special_table() const;
	virtual void draw_CurrentExitBlockAgent() const;
	virtual void draw_AFF() const;
	virtual void draw_TFF() const;
	virtual void draw_AgentEvacuationTime() const;
	virtual void draw_AgentEscapeExit() const;
	virtual void draw() const;
	virtual void sendFlgExitColor(bool) const;
	virtual void show_node_intersectObstacle() const;
	virtual void show_cell_exit_segments() const;
	virtual void show_individual_agent_info() const;
	virtual void set_specific_group(int) const;
	virtual void set_numOfgroup_size(int, int) const;
	virtual void set_group_radius(int) const;
	virtual void set_guider(int) const;
	virtual void reset_CellularAutomata();
	virtual void start_CellularAutomata();
	virtual void stop_CellularAutomata();
	virtual void update_State_Simulation();
	virtual void set_corner_coordinate(int, int) const;
	virtual void get_agent_info(int, int) const;
	virtual void set_cur_mouse_position(int, int) const;
	virtual void add_new_group();
	virtual void set_group_leader();
	virtual void editScene() const;
	virtual void edit_obstacle();
	virtual void enabled_edit_obstacle();
	virtual void enabled_edit_exit();
	virtual void update_scene();
	virtual void load_simulation();
	virtual void set_ColorTable_ID(int);
	virtual void set_ColorTable_WithoutExit_ID(int);
	virtual void set_WillingThreshold(float);
	virtual void set_SpecialID(int);
	virtual void set_colorTableNormalizeValue(float);
	virtual int getMillisecond() const;
	virtual void doExperiment();
	virtual void readExperimentEnvConfig();
protected:
	void show_agent_info();
	void experiment0();
	void experiment1();
	void experiment2();
private:
	int millisecond;
	int times;
	int mExperimentTimes;
	int mExperimentType;
	bool start_simulation;
	bool press_stop;
	bool next_simulation;
	bool mRecordEvacuationTime;
	double total_pause_through_times;
	double pause_through_times;
	clock_t start;
	clock_t through_time;
	clock_t pause_start;
	clock_t pause_through_time;
	clock_t last_time;
	string remain_agent;
	string timeStep;
	string time;
	ifstream file;
};
#endif