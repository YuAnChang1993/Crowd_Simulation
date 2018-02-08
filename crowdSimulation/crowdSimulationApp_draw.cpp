//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "crowdSimulationApp.h"
#include "agent_generator_manager.h"

using namespace std;

void CROWD_SIMULATION_APP::drawOrigin( ) const
{
	/*glLineWidth( 7.0 );
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 5.0, 0.0, 0.0 );
	glEnd( );
	//
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 5.0 );
	glEnd( );*/
}

void CROWD_SIMULATION_APP::test_draw( ) const
{
	CS_AGENT g0, g1, g;
	vector3 q0(20.0, 0.0, 0.0);
	vector3 q1(35.0, 0.0, 25.0);

	g0.setPosition( q0 );
	g1.setPosition( q1 );

	glLineWidth( 3.0 );
	glColor3f(0.0, 1.0, 0.0);
	//g0.draw( vector3( 0.0, 1.0, 0.0), false );
	glLineWidth( 3.0 );
	glColor3f(0.0, 0.0, 1.0);
	//g1.draw( vector3( 0.0, 0.0, 1.0), false );
	{
		bool flgHit;
		bool flg_normal;
		vector3 preferred_dir;
		vector3 feasible_direction;
		vector3 s = CS_ENVIRONMENT::instance( )->computeSeparationVector_Circle( q0, q1, 0.7, flgHit, feasible_direction );
		vector3 n = CS_ENVIRONMENT::instance( )->computeNormalDirection_Circle( q0, q1, 0.7, flg_normal, preferred_dir );
		g.setPosition( q1 + s );
		g.hitNormal = n;
	}
	glLineWidth( 3.0 );
	glColor3f(1.0, 0.0, 0.0);
	g.draw( vector3( 1.0, 0.0, 0.0 ), false );
}

void CROWD_SIMULATION_APP::draw( ) const
{
	if ( mEnabledApp == false ) return;

	if (mFlg_Enabled_Draw == 0) return;

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable( GL_LIGHTING );
	FREE_CANVAS_2D::begin_draw( );
	
	glLineWidth(1.0);
	glColor3f(0.5, 0.5, 0.5);

	// 2017/04/14
	if (mFlgShow_ColorTable){
		mCrowdSimulation->draw_ColorTable();
	}

	//2017/08/11
	if (mFlgShow_Group_Circle){
		mCrowdSimulation->draw_group_circle();
	}

	// 2017/07/21
	if (mFlgShow_dFF_ColorTable){
		mCrowdSimulation->draw_dFF_ColorTable();
	}

	// 2017/04/10
	if (mFlgShow_Lattice){
		mCrowdSimulation->draw_Lattice();
	}

	// 2017/09/28
	if (mFlgShow_Occupied_Cell){
		mCrowdSimulation->draw_occupied_cell();
	}

	// 2017/09/29
	if (mFlgShow_Volunteer_ColorTable){
		mCrowdSimulation->draw_volunteer_colorTable();
	}

	// 2017/11/13
	if (mFlgShow_ColorTable_WithoutExit){
		mCrowdSimulation->draw_colorTable_withoutExit();
	}

	if (mFlgShow_Special_ColorTable){
		mCrowdSimulation->draw_special_table();
	}

	if (mFlgShow_CurrentExitBlockAgent){
		mCrowdSimulation->draw_CurrentExitBlockAgent();
	}

	if (mFlgShow_AFF){
		mCrowdSimulation->draw_AFF();
	}

	if (mFlgShow_TFF){
		mCrowdSimulation->draw_TFF();
	}

	if (mFlgShow_EvacuationTime){
		mCrowdSimulation->draw_AgentEvacuationTime();
	}

	if (mFlgShow_AgentEscapeExit){
		mCrowdSimulation->draw_AgentEscapeExit();
	}

	// 2017/07/06
	if (mFlgShow_node_intersectObstacle){
		mCrowdSimulation->show_node_intersectObstacle();
	}

	//2017/07/24
	if (mFlgShow_Group_Member){
		mCrowdSimulation->draw_group();
	}

	//2017/07/24
	if (mFlgShow_Specific_Group_Member){
		mCrowdSimulation->draw_specific_group();
	}

	//2017/08/03
	if (mFlgShow_individual_agent_info){
		mCrowdSimulation->show_individual_agent_info(); 
	}

	// 2017/07/07
	if (mFlgShow_CellExit_Segment){
		mCrowdSimulation->show_cell_exit_segments();
	}

	// 2017/08/18
	if (mFlgShow_Agent_Path){
		mCrowdSimulation->draw_agent_path();
	}

	// 2017/04/13
	if (!mFlgShow_Lattice){
		mCrowdSimulation->draw_Border();
	}

	// 2017/04/08
	if (mFlgShow_CelluarAutomata){
		mCrowdSimulation->draw_CelluarAutomata();
	}

	//2017/08/17
	if (mFlgShow_Guider){
		mCrowdSimulation->draw_guider();
	}

	//2017/09/18
	if (mFlgShow_Selfish_Agent){
		mCrowdSimulation->draw_selfish_agent();
	}

	//2017/09/18
	if (mFlgShow_Neutral_Agent){
		mCrowdSimulation->draw_neutral_agent();
	}

	//2017/09/18
	if (mFlgShow_Brave_Agent){
		mCrowdSimulation->draw_brave_agent();
	}

	//2017/09/16
	if (mFlgShow_Compressive_Leader){
		mCrowdSimulation->draw_compressive_leader();
	}

	//2017/09/18
	if (mFlgShow_Strength_Agent){
		mCrowdSimulation->draw_strength_agent();
	}

	// 2017/10/18
	if (mFlgShow_Agent_Anxiety)
	{
		mCrowdSimulation->draw_agent_anxiety();
	}

	// 2017/10/19
	if (mFlgShow_Agent_Willness){
		mCrowdSimulation->draw_agent_willness();
	}

	//2017/10/25
	if (mFlgShow_Obstalce_Density){
		mCrowdSimulation->draw_obstalce_density();
	}

	//2017/08/21
	if (mFlgShow_Escape_Direction){
		if (mFlgShow_CelluarAutomata)
			glColor3f(1.0f, 1.0f, 1.0f);
		if (!mFlgShow_CelluarAutomata)
			glColor3f(0.0f, 0.0f, 0.0f);
		mCrowdSimulation->draw_escape_direction();
	}

	//2017/09/04
	if (mFlgShow_Stress_Area){
		mCrowdSimulation->draw_stress_area();
	}

	mCrowdSimulation->draw( );
	
	/*if ( mClickedPickedObj ) {
		CS_AGENT_INFORMATION g_info;
		mCrowdSimulation->computeInformation( (CS_AGENT*) mClickedPickedObj, g_info );
		mClickedPickedObj->draw_debug( vector3( 0.0, 1.0, 1.0 ), true );
		g_info.draw( );
	}

	for (int i = 0; i < mSelectedObjArr.size(); ++i) {
		mSelectedObjArr[i]->draw_debug(vector3(0.0, 1.0, 1.0), true);
	}*/
	
	FREE_CANVAS_2D::end_draw( );
	glEnable( GL_LIGHTING );

}