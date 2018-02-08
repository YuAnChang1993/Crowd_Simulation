//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2015/12/10
//
#include <iostream>
#include <fstream>
#include "agent_generator.h"
#include "./crowdSimulation_constants.h"
#include "agent_generator_data_format.h"
using namespace std;

void CS_AGENT_GENERATOR::save( ofstream *fp ) const
{
	*fp << AGENT_GEN_DF_TAG_BEGIN << endl;
*fp << AGENT_GEN_DF_TAG_OBJ_ID << " \t" << mObjID << endl;
*fp << AGENT_GEN_DF_TAG_AGENT_PATH_INDEX << " \t" << mAgentPathIndex << endl;
 		*fp << AGENT_GEN_DF_TAG_GEOMETRY << " \t" << AGENT_GEN_DF_TAG_GEOMETRY_CIRCLE << endl;
*fp << AGENT_GEN_DF_TAG_POSITION << " \t" << mPosition.x << " \t" << mPosition.y << " \t" << mPosition.z << endl;
*fp << AGENT_GEN_DF_TAG_RADIUS << " \t" << mRadius << endl;
*fp << AGENT_GEN_DF_TAG_PLANE_XZ << endl;
 *fp << AGENT_GEN_DF_TAG_MAX_GENERATION_TIME << " \t" << mMaxGenerationTime << endl;
 *fp << AGENT_GEN_DF_TAG_MAX_AGENT_NUMBER << " \t" << mMaxNumOfGeneratedAgents << endl;

 *fp << AGENT_GEN_DF_TAG_AGENT_SPEED << " \t" << mAgentSpeedMin << " \t" << mAgentSpeedMax << endl;
 *fp << AGENT_GEN_DF_TAG_FREQUENCY << " \t" << mNumAgentPerSecond << endl;
 *fp << AGENT_GEN_DF_TAG_METHOD << " \t" << AGENT_GEN_DF_TAG_METHOD_RANDOM << endl;
 *fp << AGENT_GEN_DF_TAG_END << endl;

}