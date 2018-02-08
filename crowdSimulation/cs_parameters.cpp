#include "cs_parameters.h"

int csp_maxNum_Agents = 100000;
int csp_maxNum_Paths = 128;
int csp_maxNum_AgentGenerators = 128;
int csp_maxNum_EvironmentObjs = 128;
int csp_maxNum_Teleporters = 128;
int csp_maxNum_CrowdMonitorManagers = 128;
//
float csp_CrowdMonitorRadius = 10.0;
//
float csp_default_AgentRadius = 0.7;
float csp_Generator_AgentRadiusMin = 0.3;
float csp_Generator_AgentRadiusMax = 0.7;
//
int csp_flgShowAgentTravelTime = false;
int csp_flgShowAgentTravelLength = false;
int csp_flgShowAgentSpeedRatio = false;
int csp_flgShowPathLoad = true;
//
int csp_enableAgentSeparationMap = true;
int csp_enableAgentPerceivedVelocity = false;
int csp_enableAgentLowestSpeed = false;
int csp_enableAdaptivePaths = false;
float csp_lowestAgentSpeedRatio = 0.25;
int csp_enableAgentCornerMovement = false;
//
int csp_enableSlowDownMechanismDueToAgentCollision = false;
float csp_cd_penalty_factor = 0.9;
//
int csp_enableAgentResponseTime = false;
float csp_MaxAgentResponseTime = 0.25; // second
//
float csp_path_with_weight_for_agent = 4.0;
//
int csp_numOfStepsForAgentVelocityAdjustmentDueToAgents = 1;
int csp_enablePositionBasedAgentCollisionAvoidance = true;
//
float csp_agent_radius = 0.75;
float csp_max_agent_speed = 1.5; // per second
//
int csp_zlayer_map = 5;
int csp_zlayer_agents = 0;
int csp_zlayer_agent_generator = 1;
int csp_zlayer_environment = 4;
int csp_zlayer_path = 3;

int csp_selected_line_thickness_path = 3;
int csp_selected_line_thickness_agent = 3;
int csp_selected_line_thickness_env_obj = 3;

int milliseconds = 0;

vector3 csp_color_agent_generator = vector3( 0.0, 1.0, 0.0 );
vector3 csp_color_agent = vector3( 0.0, 0.0, 1.0 );
vector3 csp_color_path  = vector3( 0.9, 0.5, 0.5 );
vector3 csp_color_env_obj = vector3( 0.5, 0.5, 0.5 );

float csp_ClosestDistance = 0.2;
//
COLOR_TABLE csp_ColorTable_TravelTime;
COLOR_TABLE csp_ColorTable_TravelLength;
COLOR_TABLE csp_ColorTable_AgentSpeedRatio;
COLOR_TABLE csp_ColorTable_PathLoad;

//
void csp_init( )
{
	csp_ColorTable_PathLoad.enableAutoScale( true );
	float Pv = 40;
	csp_ColorTable_PathLoad.addEntry( 0.0, vector3(0.5, 0.5, 0.5) );
	csp_ColorTable_PathLoad.addEntry( Pv*1.0, vector3(0.0, 0.0, 1.0) );
	csp_ColorTable_PathLoad.addEntry( Pv*2.0, vector3(0.0, 1.0, 0.0) );
	csp_ColorTable_PathLoad.addEntry( Pv*4.0, vector3(1.0, 1.0, 0.0) );
	csp_ColorTable_PathLoad.addEntry( Pv*8.0, vector3(1.0, 0.0, 0.0) );
	csp_ColorTable_PathLoad.addEntry( Pv*10.0, vector3(1.0, 0.0, 1.0) );

	//
	csp_ColorTable_AgentSpeedRatio.enableAutoScale( false );
	float Rv = 0.5;
	csp_ColorTable_AgentSpeedRatio.addEntry( 0.0, vector3(0.5, 0.5, 0.5) );
	csp_ColorTable_AgentSpeedRatio.addEntry( Rv*1.0, vector3(0.0, 0.0, 1.0) );
	csp_ColorTable_AgentSpeedRatio.addEntry( Rv*2.0, vector3(0.0, 1.0, 0.0) );
	csp_ColorTable_AgentSpeedRatio.addEntry( Rv*4.0, vector3(1.0, 1.0, 0.0) );
	csp_ColorTable_AgentSpeedRatio.addEntry( Rv*8.0, vector3(1.0, 0.0, 0.0) );
	csp_ColorTable_AgentSpeedRatio.addEntry( Rv*10.0, vector3(1.0, 0.0, 1.0) );
	//
	float Tv = 100.0;
	csp_ColorTable_TravelTime.addEntry( 0.0, vector3(0.5, 0.5, 0.5) );
	csp_ColorTable_TravelTime.addEntry( Tv*1.0, vector3(0.0, 0.0, 1.0) );
	csp_ColorTable_TravelTime.addEntry( Tv*2.0, vector3(0.0, 1.0, 0.0) );
	csp_ColorTable_TravelTime.addEntry( Tv*3.0, vector3(1.0, 1.0, 0.0) );
	csp_ColorTable_TravelTime.addEntry( Tv*4.0, vector3(1.0, 0.0, 0.0) );
	csp_ColorTable_TravelTime.addEntry( Tv*5.0, vector3(1.0, 0.0, 1.0) );
	//
	float Lv = 300.0;
	csp_ColorTable_TravelLength.addEntry( 0.0, vector3(0.5, 0.5, 0.5) );
	csp_ColorTable_TravelLength.addEntry( Lv*1.0, vector3(0.0, 0.0, 1.0) );
	csp_ColorTable_TravelLength.addEntry( Lv*2.0, vector3(0.0, 1.0, 0.0) );
	csp_ColorTable_TravelLength.addEntry( Lv*3.0, vector3(1.0, 1.0, 0.0) );
	csp_ColorTable_TravelLength.addEntry( Lv*4.0, vector3(1.0, 0.0, 0.0) );
	csp_ColorTable_TravelLength.addEntry( Lv*5.0, vector3(1.0, 0.0, 1.0) );
	
	//milliseconds = 500;
}
