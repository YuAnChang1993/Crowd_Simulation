//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "../headers.h"
#include "cs_basic_structs.h"
#include "../openGLbasic/opengl_stuff.h"
#include "agent.h"
#include "cs_parameters.h"
using namespace std;

void CS_AGENT::draw_debug( const vector3 agent_color,  bool used_own_color ) const
{
	if ( used_own_color ) {
		if ( flgReachedDestination ) {
			glLineWidth( 2.0 );
			glColor3f(0.0, 1.0, 0.0);
		} else {
			if ( flgHit ) {
				glColor3f(1.0, 0.0, 0.0);
			} else {
				pl_glColor3f( agent_color );
			}

			
			if ( sv.SqrLength( ) == 0.0 ) {

				glColor3f(0.0, 0.0, 0.0);
			}

			if ( mInvalidPosition ) {
				glColor3f( 1.0, 0.0, 1.0 );
			}

			glLineWidth( 2.0 );
			
		}
	}

	//cout << "r:" << r << endl;
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );

	//------------------------------------------------------
	glColor3f(0.2, 0.2, 0.2);
	vector3 q;
	float L2 = sv.SqrLength( );
	if ( L2 != 0.0 ) {
		q = p + (1.0/sqrt(L2))*sv;
	} else {
		q = p + sv;
	}
	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );

	//------------------------------------------------------
	glColor3f(1.0, 0.2, 0.2);
	
	L2 = desired_v.SqrLength( );
	if ( L2 != 0.0 ) {
		q = p + (1.0/sqrt(L2))*desired_v;
	} else {
		q = p + desired_v;
	}
	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );

	//------------------------------------------------------

	q = p + hitNormal*5.0;
	glColor3f(1.0, 0.0, 0.0);

	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );
	//------------------------------------------------------
	vector3 pre_dir = preferred_dir;
	pre_dir.Normalize( );
	q = p + pre_dir*5.0;
	glColor3f(0.0, 1.0, 0.0);
	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );
}

void CS_AGENT::draw( const vector3 agent_color,  bool used_own_color ) const
{
	if ( used_own_color ) {
		if ( flgReachedDestination ) {
			glLineWidth( 2.0 );
			glColor3f(0.0, 1.0, 0.0);
		} else {
			//if ( flgHit ) {
			//	glColor3f(1.0, 0.0, 0.0);
			//} else {
			if ( slow_down_counter > 0 ) {
				vector3 agent_color_slow( 0.5, 0.5, 0.5 );
				float slow_down_factor = slow_down_counter/max_slow_down_counter;
				agent_color_slow = agent_color_slow*slow_down_factor;
				pl_glColor3f( agent_color_slow );
			} else {
				pl_glColor3f( agent_color );
			}
			//}

			//if ( sv.SqrLength( ) == 0.0 ) {
			//	glColor3f(0.0, 0.0, 0.0);
			//}
			glLineWidth( 2.0 );
			
		}
	}

	csp_ColorTable_TravelLength.updateMaxValue( travelLength );
	csp_ColorTable_TravelTime.updateMaxValue( travelTime );
	float speed_ratio = (sv.Length( )/desired_speed)/0.03;
	csp_ColorTable_AgentSpeedRatio.updateMaxValue( speed_ratio );
	//cout << "speed_ratio:" << speed_ratio << endl;

	if ( csp_flgShowAgentSpeedRatio ) {
		vector3 c = csp_ColorTable_AgentSpeedRatio.getColor_Linear( speed_ratio );
		pl_glColor3f( c );
		ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 12 );
	} 
	else
	if ( csp_flgShowAgentTravelTime ) {
		vector3 c = csp_ColorTable_TravelTime.getColor_Linear( travelTime );
		
		pl_glColor3f( c );
		ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 12 );
	} if ( csp_flgShowAgentTravelLength ) {
		vector3 c = csp_ColorTable_TravelLength.getColor_Linear( travelLength );
		
		pl_glColor3f( c );
		ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 12 );
	} else {
		//cout << "r:" << r << endl;
		if ( flgHit ) {
			//cout << "flgHit:" << flgHit << endl;
			ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 12 );
		} else {
			ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
		}
	}

	glBegin( GL_LINE_STRIP );
	pl_glVertex3f( prev_p );
	pl_glVertex3f( p );
	pl_glVertex3f( predict_p );
	glEnd( );


	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_LINE_STRIP );
	pl_glVertex3f( p );
	pl_glVertex3f( (p + mFeasibleDirection) );
	glEnd( );
	
	
	//ogl_drawSimpleCircleXZ( prev_p.x, prev_p.y, prev_p.z, r, 12 );
	//ogl_drawSimpleCircleXZ( predict_p.x, predict_p.y, predict_p.z, r, 12 );
	
/*
	//------------------------------------------------------
	glColor3f(1.0, 1.0, 1.0);
	vector3 q;
	float L2 = sv.SqrLength( );
	if ( L2 != 0.0 ) {
		q = p + (1.0/sqrt(L2))*sv;
	} else {
		q = p + sv;
	}
	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );

	//------------------------------------------------------
	q = p + hitNormal;
	glColor3f(0.0, 0.0, 0.0);

	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );
	//------------------------------------------------------
	q = p + preferred_dir;
	glColor3f(0.0, 1.0, 0.0);
	ogl_drawSimpleLineSegment( p.x, p.y, p.z, q.x, q.y, q.z );
*/
}

bool CS_AGENT::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	float d2 = p.distance2( vector3(x, 0.0, z) );
	if ( r*r >= d2 ) return true;
	return flg;
}

void CS_AGENT::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
	p.x = global_posx;
	p.z = global_posz;
}

void CS_AGENT::bo_setNewPosition_XZ( float x, float z )
{
	p.x = x;
	p.z = z;

}

void CS_AGENT::bo_drawWithHighLight( ) const
{
	glColor3f(1.0, 1.0, 0.0);
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 6 );
}

void CS_AGENT::bo_drawWithHighLight( float r, float g, float b ) const
{
	glColor3f( r, g, b );
	ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 6 );
}

////////////////////////////////
void CS_AGENT_INFORMATION::draw( ) const
{
	vector3 color_leader( 1.0, 1.0, 0.0 );
	vector3 color_neighbor( 0.5, 0.5, 0.5 );

	if ( leader.size( ) > 0 ) {
		pl_glColor3f( color_leader );

		for ( int i = 0; i < leader.size( ); ++i ) {


		vector3 p = leader[i]->p;
		float r = leader[i]->r;
		ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 6 );
	}

	}

	pl_glColor3f( color_neighbor );

	for ( int i = 0; i < neighbor.size( ); ++i ) {
vector3 p = neighbor[i]->p;
		float r = neighbor[i]->r;
		ogl_drawSimpleSolidCircleXZ( p.x, p.y, p.z, r, 6 );
	}
}
