//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/08/13
//
#include "crowdSimulationApp.h"

using namespace std;

/*
MAXIMUM_AGENT	100000
MAP2D_GRID_DX	5.0
MAP2D_GRID_DZ	5.0
AGENT_COLOR	0.0 0.0 1.0
AGENT_HIGHLIGHT_COLOR	1.0 0.0 0.0
ENVIRONMENT	./config_env/env.txt
PATH_DATA	./config_path/path.txt
AGENT_GENERATOR	./config_agent_gen/gen_circle.txt
TEXTURE_DATA	./config_texture/config_texture.txt
PSO_TRAINING	./pso_training_config/pso.txt
*/


void CROWD_SIMULATION_APP::save( ) const
{
	mCrowdSimulation->save( );
}
