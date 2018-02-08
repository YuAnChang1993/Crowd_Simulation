#ifndef _MY_EXPERIMENT_H_
#define _MY_EXPERIMENT_H_
#include<iostream>
#include<fstream>
#include<string>
#include "../myCellularAutomata/cellular_automata.h"

class Experiment{
public:
	Experiment();
	void update();
	void readConfig();
	void run();
	void experiment_calculationOfSFF();
	void experiment_obstacle_moveTime();
	void experiment_obstacle_moveDistance();
	void experiment_obstacle_destination();
	void experiment_anxiety();
	int mType;
	int mRunTimes;
	int mTimes;
	CS_CELLULAR_AUTOMATA* ca;
};
#endif