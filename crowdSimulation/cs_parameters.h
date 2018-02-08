#ifndef __CS_PARAMETERS_H__
#define __CS_PARAMETERS_H__

#include "../primitives/vector3d.h"
#include "../openGLBasic/ogl_basic_tools.h"

//////////////////////////////////////////////////////
//Numbers
extern int csp_maxNum_Agents;
extern int csp_maxNum_Paths;
extern int csp_maxNum_AgentGenerators;
extern int csp_maxNum_EvironmentObjs;
extern int csp_maxNum_Teleporters;
extern int csp_maxNum_CrowdMonitorManagers;
extern float csp_CrowdMonitorRadius;
//////////////////////////////////////////////////////
extern float csp_default_AgentRadius;
extern float csp_Generator_AgentRadiusMin;
extern float csp_Generator_AgentRadiusMax;

//////////////////////////////////////////////////////
extern int csp_enableSlowDownMechanismDueToAgentCollision;
extern float csp_cd_penalty_factor;
//////////////////////////////////////////////////////
extern int csp_enableAgentSeparationMap;
extern int csp_numOfStepsForAgentVelocityAdjustmentDueToAgents;
extern int csp_enablePositionBasedAgentCollisionAvoidance;
extern int csp_enableAgentPerceivedVelocity;
extern int csp_enableAgentLowestSpeed;
extern float csp_lowestAgentSpeedRatio;
extern int csp_enableAdaptivePaths;
extern int csp_enableAgentCornerMovement;

//////////////////////////////////////////////////////
extern float csp_agent_radius;
extern float csp_max_agent_speed;
//////////////////////////////////////////////////////

extern int csp_enableAgentResponseTime;
extern float csp_MaxAgentResponseTime;
//////////////////////////////////////////////////////
extern float csp_path_with_weight_for_agent;
//////////////////////////////////////////////////////
extern int csp_flgShowAgentTravelTime;
extern int csp_flgShowAgentTravelLength;
extern int csp_flgShowAgentSpeedRatio;
extern int csp_flgShowPathLoad;
//////////////////////////////////////////////////////

extern int csp_zlayer_map;
extern int csp_zlayer_agents;
extern int csp_zlayer_agent_generator;
extern int csp_zlayer_environment;
extern int csp_zlayer_path;

extern vector3 csp_color_agent_generator;
extern vector3 csp_color_agent;
extern vector3 csp_color_path;
extern vector3 csp_color_env_obj;

extern float csp_ClosestDistance;
//
extern COLOR_TABLE csp_ColorTable_TravelTime;
extern COLOR_TABLE csp_ColorTable_TravelLength;
extern COLOR_TABLE csp_ColorTable_AgentSpeedRatio;
extern COLOR_TABLE csp_ColorTable_PathLoad;

extern void csp_init( );

extern int milliseconds;
#endif