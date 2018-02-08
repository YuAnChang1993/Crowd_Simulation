//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __TIMER_INDEX_H__
#define __TIMER_INDEX_H__

/*
	computeAgentMotion( flg_timer );
	adjustAgentMotionBasedOnSeparationDistanceMap( flg_timer );
	//
	modifyAgentVelocity( );
	//
	resolveAgentCollision( );
	//
	updateAgentPosition( );
	//
	applyHardConstraints( );
	//
	computeAgentMovementMap( );
	//
	post_update( );
	//
	handleAgentsReachedDestination( );
*/
#define TIMER_LOG_FILE_NAME		"timer.txt"

#define cTM_IDX_TOTAL_TIME				0
#define	cTM_NAME_TOTAL_TIME				"TOT"

#define cTM_IDX_COMPUTE_AGENT_MOTION	1
#define	cTM_NAME_COMPUTE_AGENT_MOTION	"AGM"

#define cTM_IDX_SEPARATION_ADJUST		2
#define	cTM_NAME_SEPARATION_ADJUST		"SEA"

#define cTM_IDX_MODIFY_AGENT_MOTION		3
#define	cTM_NAME_MODIFY_AGENT_MOTION	"MAM"

#define cTM_IDX_RESOLVE_AGENT_COLLISION		4
#define	cTM_NAME_RESOLVE_AGENT_COLLISION	"RAC"

#define cTM_IDX_UPDATE_AGENT_POSITION		5
#define	cTM_NAME_UPDATE_AGENT_POSITION		"UAP"

#define cTM_IDX_APPY_HARD_CONSTRAINTS		6
#define	cTM_NAME_APPY_HARD_CONSTRAINTS		"AHC"

#define cTM_IDX_COMPUTE_AGENT_MOVEMENT_MAP	7
#define	cTM_NAME_COMPUTE_AGENT_MOVEMENT_MAP	"CMM"

#endif