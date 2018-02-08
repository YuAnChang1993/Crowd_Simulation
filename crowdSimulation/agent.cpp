//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "agent.h"
#include "../primitives/basic_tools.h"
#include "cs_parameters.h"
using namespace std;

namespace {
	//
// d2 is the shortest distance from the point
// q to the closest path point.
//
// return -1: number of points <=0
//
int bt_computeClosestPathPointIndex( const vector3 &q, const CS_PATH_POINT *p, int numPathPoints, float &d2 )
{
	int vert_index = -1;
	d2 = -1.0;
	if ( numPathPoints <= 0 ) return vert_index;
	vert_index = 0;
	d2 = p[0].p.distance2(q);
	for ( int i = 1; i < numPathPoints; ++i ) 
	{
		float tmp_d2 = p[i].p.distance2(q);

		if ( d2 > tmp_d2 ) {
			d2 = tmp_d2;
			vert_index = i;
			
		}
	}
	return vert_index;
}

//
// d2 is the shortest distance from the point q
// to the closest path edge.
// The first edge index is 0, i.e. (p[0], p[1]).
//
// return -1: number of points <=1
// 
int bt_computeClosestPathEdgeIndex( const vector3 &q, const CS_PATH_POINT *p, int numPathPoints, float &d2 )
{
	int edge_index = -1;
	d2 = -1.0;
	if ( numPathPoints <= 1 ) return edge_index;
	d2 = bt_computeDistance2_LineSegment( q, p[0].p, p[1].p );
	edge_index = 0;
	for ( int i = 1; i < numPathPoints - 1; ++i ) {
		float tmp_d2;
		tmp_d2 = bt_computeDistance2_LineSegment( q, p[i].p, p[i+1].p );
		if ( d2 > tmp_d2 ) {
			d2 = tmp_d2;
			edge_index = i;
		}
	}	
	return edge_index;
}
};

bool isAgentsColliding( const CS_AGENT *agi, const CS_AGENT *agj )
{
	bool flg = false;
	float d2 = agi->p.distance2( agj->p );
	float R = agi->r + agj->r;
	if ( d2 > R*R) {
	} else {
		flg = true;
	}
	return flg;
}

float moveAgentsApart( CS_AGENT *agi, CS_AGENT *agj ) // separation distance
{
	float d2 = agi->p.distance2( agj->p );
	float r = agi->r + agj->r;
	if ( d2 > r*r ) return 0.0;
	float d = sqrt(d2);
	vector3 pij = agi->p - agj->p;
	float SqrLength = pij.SqrLength( );
	vector3 npij;
	if ( SqrLength != 0.0 ) { 
		// very important to check zero or not
		npij = pij.Normalized( );	
	} else {
		//npij = vector3( 1.0, 0.0, 0.0 );
		npij = bt_random_sample_on_circle_boundary_XZPlane( vector3(0.0, 0.0, 0.0), 1.0 );
		//npij.Normalize( );
	}
	float di; 
	float dj; 
	//float f = 0.5;
	float f = 0.10;

	if ( agi->flgHit ) {
		f = 0.0;
	} else if ( agj->flgHit ) {
		f = 1.0;
	}
	if ( agi->flgHit && agj->flgHit ) {
		
	} else {
		di = (r - d)*f;
		dj = (r - d)*( 1 - f);
		agi->p = agi->p + di*npij;
		agj->p = agj->p - dj*npij;
	}
	return (r - d);
}

//
// use the predicted positions of the agents to adjust
// the velocities of the agents
//
float moveAgentsApart_VelocityBased( CS_AGENT *agi, CS_AGENT *agj ) // separation distance
{
	vector3 agi_q = agi->p + agi->sv;
	vector3 agj_q = agj->p + agj->sv;

	float d2 = agi_q.distance2( agj_q );
	float R = agi->r + agj->r;
	if ( d2 >= R*R ) return 0.0;
	float d = sqrt(d2);
	//


//
//Simple collision penalty
//++agi->slow_down_counter;
//++agj->slow_down_counter;
	//end Simple collision penalty

	//consider front and back
	//penalize the agent at the back
	if ( csp_enableSlowDownMechanismDueToAgentCollision ) {
		vector3 pij = agi->p - agj->p;
		vector3 npij = pij.Normalized( );

		vector3 vij = agi->sv - agj->sv;
		/*
		vector3 nvij = vij.Normalized( );
		if ( nvij.x != nvij.x ) {
		nvij.zero( );
		}
		*/
		vector3 nvij = vij;
		float d = npij.dot( nvij );
		float e = 0.05;
		if ( d > e ) {
			++agj->slow_down_counter;
		} else if ( d < -e) {
			++agi->slow_down_counter;
		}

	}
	vector3 qij = agi_q - agj_q;
	//
	float SqrLength = qij.SqrLength( );
	vector3 nqij;
	if ( SqrLength != 0.0 ) { 
		// very important to check zero or not
		nqij = qij.Normalized( );	
	} else {
		//npij = vector3( 1.0, 0.0, 0.0 );
		nqij = bt_random_sample_on_circle_boundary_XZPlane( vector3(0.0, 0.0, 0.0), 1.0 );
		//npij.Normalize( );
	}
	float di; 
	float dj; 
	float f = 0.5;
	if ( agi->flgHit ) {
		f = 0.0;
	} else if ( agj->flgHit ) {
		f = 1.0;
	}

	if ( agi->flgHit && agj->flgHit ) {
		
	} else {
		di = (R - d)*f;
		dj = (R - d)*( 1 - f);
		agi_q = agi_q + di*nqij;
		agj_q = agj_q - dj*nqij;

		agi->sv = agi_q - agi->p;
		agj->sv = agj_q - agj->p;
		//

	}
	return (R - d);
}


bool adjustAgentVelocity_CollisionAvoidance_Overlapped( CS_AGENT *agi, CS_AGENT *agj )
{
	vector3 &pi = agi->p;
	vector3 &pj = agj->p;
	vector3 &vi = agi->sv;
	vector3 &vj = agj->sv;

	vector3 pij = pi - pj;
	vector3 vij = vi - vj;

	//the agents are leaving from each other
	if ( pij.dot( vij ) >= 0.0 ) return false;
	//
	//---
	if ( pij.dot( vi ) >= 0.0 ) {
		// pi is in front of pj
		if ( vi.Length( ) == 0.0 ) {
			agj->sv.zero( );
		} else {
			vector3 nvi = vi.Normalized( );
			float dij = vj.dot(nvi);
			if ( dij > 0.0 ) {
				vector3 new_vj = vj -dij*nvi;
				agj->sv = new_vj + vi;
			}
		}
			
	} else {
		if ( vj.Length( ) == 0.0 ) {
			agi->sv.zero( );
		} else {
			vector3 nvj = vj.Normalized( );
			float dij = vj.dot(nvj);
			if ( dij > 0.0 ) {
				vector3 new_vi = vi - dij*nvj;
				agi->sv = new_vi + vj;
			}
		}
	}

	if ( (agi->sv.x != agi->sv.x)
		||
		(agj->sv.x != agj->sv.x)
		) {
			cout << "adjustAgentVelocity_CollisionAvoidance_Overlapped" << endl;
			cout << "NAN" << endl;
			system("pause");
	}

	return true;
}
//
bool adjustAgentVelocity_CollisionAvoidance( CS_AGENT *agi, CS_AGENT *agj )
{
	bool flg = false;

	float d2 = agi->p.distance2( agj->p );
	float r = agi->r + agj->r;
	float e = r*0.01;
	if ( r*r + e*e >= d2 ) {
		// overlapping
		return adjustAgentVelocity_CollisionAvoidance_Overlapped( agi, agj );
	}
	return false;

	float d = sqrt(d2);
	vector3 &pi = agi->p;
	vector3 &pj = agj->p;
	vector3 &vi = agi->sv;
	vector3 &vj = agj->sv;

	vector3 pij = pi - pj;
	vector3 vij = vi - vj;
	float R = agi->r + agj->r;
	float R2 = R*R;

	float a = vij.SqrLength( );
	float b = 2.0*pij.dot( vij );
	float c = pij.SqrLength( ) - R2;
	
	float D2 = b*b - 4.0*a*c;
	if ( D2 < 0.0 ) return false;
	float D = sqrt(D2);
	float t;
	if ( a == 0.0 ) {
		if ( b != 0.0 ) {
			t = -c/b;
			if ( t >= 0.0 && t <= 1.0 ) {

			}
		}
	} else {
		bool flg = false;
		t = (-b - D)/(2.0*a);
		if ( t >= 0.0 && t <= 1.0 ) {
			flg = true;

		} else {
			t = (-b + D)/(2.0*a);
			if ( t >= 0.0 && t <= 1.0 ) {
				flg = true;
			}	
		}
		if ( flg ) {
			if ( pij.dot( vi ) >= 0.0 ) {
				// pi is in front of pj
				agj->sv = agj->sv*t;
				//cout << ".";

			} else {
				// pj is in front of pi
				agi->sv = agi->sv*t;
				//cout << ".";
			}

		}
	}
	return flg;
}

//
// return 
// false: leaving
// true: colliding
//
bool adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased( CS_AGENT *agi, CS_AGENT *agj )
{
	vector3 &pi = agi->p;
	vector3 &pj = agj->p;
	vector3 &vi = agi->sv;
	vector3 &vj = agj->sv;

	vector3 pij = pi - pj;
	vector3 vij = vi - vj;

	//the agents are leaving from each other
	if ( pij.dot( vij ) >= 0.0 ) return false;
	//
	//---
	if ( pij.dot( vi ) >= 0.0 ) {
		// pi is in front of pj with respect to the pi's moving direction
		if ( vi.Length( ) == 0.0 ) {
			// pi does not move
			// set velocity of agj to zero
			agj->sv.zero( );
		} else {
			// pi moves
			vector3 nvi = vi.Normalized( );
			float dij = vj.dot(nvi);
			if ( dij > 0.0 ) {
				vector3 new_vj = vj - dij*nvi; // eliminate normal direction
				agj->sv = new_vj + vi; // make agj follow agi
			}
		}
	} else {
		if ( vj.Length( ) == 0.0 ) {
			agi->sv.zero( );
		} else {
			vector3 nvj = vj.Normalized( );
			float dij = vj.dot(nvj);
			if ( dij > 0.0 ) {
				vector3 new_vi = vi - dij*nvj; // eliminate normal direction
				agi->sv = new_vi + vj; // make agi follow agj
			}
		}
	}

	if ( (agi->sv.x != agi->sv.x)
		||
		(agj->sv.x != agj->sv.x)
		) {
			cout << "adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased" << endl;
			cout << "NAN" << endl;
			system("pause");
	}

	return true;
}

void debug_Agents( bool flgHit_computed, const CS_AGENT *agi, const CS_AGENT *agj )
{
	//cout << "." << endl;
	float R = agi->r + agj->r;
	vector3 qi = agi->p;
	vector3 qj = agj->p;

	vector3 qi_predicted = agi->p + agi->sv;
	vector3 qj_predicted = agj->p + agj->sv;	

	float Ldij = qi.distance( qj );
	float Ldij_new = qi_predicted.distance( qj_predicted );

	float e = R*0.01;
	float E = ( Ldij - R );
float E_new = ( Ldij_new - R );
	if ( flgHit_computed == false ) {
		
		if ( E + e >= 0.0 && E_new + e >= 0.0 ) {
		} else {
			cout << "flgHit_computed==false Error debug_Agents:" << E << "\t" << E_new << endl;
		}
	} else {
			
		if ( E <= (0.0 - e) ||  (E_new + e) <= 0.0 ) {
			cout << "flgHit_computed==true Error debug_Agents:" << e << endl;
			cout << "Ldij_new:" << Ldij_new << "\tLdij:" << Ldij << "\tR:" << R << endl;
		} else {
		}
	}

}

bool adjustAgentVelocity_CollisionAvoidance_VelocityBased( CS_AGENT *agi, CS_AGENT *agj )
{
	
	
	float d2 = agi->p.distance2( agj->p );
	float R = agi->r + agj->r;
	float e = R*0.01;
	float R2 = R*R;
	if ( R2 + e*e >= d2 ) {
		// overlapping
		return adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased( agi, agj );
	}
	
	if ( csp_enablePositionBasedAgentCollisionAvoidance ) {
		return false;
	}

	

	float d = sqrt(d2);
	vector3 &pi = agi->p;
	vector3 &pj = agj->p;
	vector3 &vi = agi->sv;
	vector3 &vj = agj->sv;

	vector3 pij = pi - pj;
	vector3 vij = vi - vj;
	

	float a = vij.SqrLength( );
	float b = 2.0*pij.dot( vij );
	float c = pij.SqrLength( ) - R2;
	
	float D2 = b*b - 4.0*a*c;
	if ( D2 < 0.0 ) return false;
	float D = sqrt(D2);
	float t;
	bool flgHit = false;

	if ( a == 0.0 ) {
		if ( b != 0.0 ) {
			t = -c/b;
			if ( t >= 0.0 && t <= 1.0 ) {
				flgHit = true;
			}
		} else {
			// b = 0
			// objects do not move in relative
		}
	} else {
		
		t = (-b - D)/(2.0*a);
		if ( t >= 0.0 && t <= 1.0 ) {
			flgHit = true;

		} else {
			t = (-b + D)/(2.0*a);
			if ( t >= 0.0 && t <= 1.0 ) {
				flgHit = true;
			}	
		}
	}

	
		if ( flgHit ) {
			//agi->sv = agi->sv*t;
			//agj->sv = agj->sv*t;
				
			if ( pij.dot(vj) > 0.0 && pij.dot(vi) < 0.0 ) {
				// head to head collision?
				agi->sv = agi->sv*t;
				agj->sv = agj->sv*t;
			} else 
			if ( pij.dot( vi ) >= 0.0 ) {
				// pi is in front of pj
				agj->sv = agj->sv*t;
				//cout << ".";

			} else {
				// pj is in front of pi
				agi->sv = agi->sv*t;
				//cout << ".";
			}
			

	}

	////////////////////////////////////////////////////////
	//DEBUG purpose
	debug_Agents( flgHit, agi, agj );
	////////////////////////////////////////////////////////


	return flgHit;
}

//
// return 
// false: leaving
// true: colliding
//
// CA: collision avoidance
//
bool adjustAgentVelocity_CA_Overlapped_VelocityBased_Perceived( CS_AGENT *agi, CS_AGENT *agj )
{
	vector3 pi = agi->p;
	vector3 pj = agj->p;
	vector3 vi = agi->sv;
	vector3 vj = agj->sv;
	//
	vector3 vi_perceived = agi->perceived_v;
	vector3 vj_perceived = agj->perceived_v;
	
	vi = vi_perceived;
	vj = vj_perceived;

	//vi_perceived.zero( );
	//vj_perceived.zero( );


	vector3 pij = pi - pj;
	vector3 vij = vi - vj;

	//the agents are leaving from each other
	if ( pij.dot( vij ) >= 0.0 ) return false;
	//
	//---
	if ( pij.dot( vi ) >= 0.0 ) {
		// pi is in front of pj with respect to the pi's moving direction
		if ( vi.Length( ) == 0.0 ) {
			// pi does not move
			// set velocity of agj to zero
			agj->sv.zero( );
		} else {
			// pi moves
			vector3 nvi = vi.Normalized( );
			float dij = vj.dot(nvi);
			if ( dij > 0.0 ) {
				vector3 new_vj = vj - dij*nvi; // eliminate normal direction
				agj->sv = new_vj + vi; // make agj follow agi
			}
		}
	} else {
		if ( vj.Length( ) == 0.0 ) {
			agi->sv.zero( );
		} else {
			vector3 nvj = vj.Normalized( );
			float dij = vj.dot(nvj);
			if ( dij > 0.0 ) {
				vector3 new_vi = vi - dij*nvj; // eliminate normal direction
				agi->sv = new_vi + vj; // make agi follow agj
			}
		}
	}

	if ( (agi->sv.x != agi->sv.x)
		||
		(agj->sv.x != agj->sv.x)
		) {
			cout << "adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased" << endl;
			cout << "NAN" << endl;
			system("pause");
	}

	return true;
}

bool adjustAgentVelocity_CA_VelocityBased_Perceived( CS_AGENT *agi, CS_AGENT *agj )
{
	
	
	float d2 = agi->p.distance2( agj->p );
	float R = agi->r + agj->r;
	float e = R*0.01;
	float R2 = R*R;
	if ( R2 + e*e >= d2 ) {
		// overlapping
		adjustAgentVelocity_CA_Overlapped_VelocityBased_Perceived( agi, agj );
		return adjustAgentVelocity_CollisionAvoidance_Overlapped_VelocityBased( agi, agj );
	}
	
	if ( csp_enablePositionBasedAgentCollisionAvoidance ) {
		return false;
	}

	

	float d = sqrt(d2);
	vector3 &pi = agi->p;
	vector3 &pj = agj->p;
	vector3 &vi = agi->sv;
	vector3 &vj = agj->sv;

	vector3 pij = pi - pj;
	vector3 vij = vi - vj;
	

	float a = vij.SqrLength( );
	float b = 2.0*pij.dot( vij );
	float c = pij.SqrLength( ) - R2;
	
	float D2 = b*b - 4.0*a*c;
	if ( D2 < 0.0 ) return false;
	float D = sqrt(D2);
	float t;
	bool flgHit = false;

	if ( a == 0.0 ) {
		if ( b != 0.0 ) {
			t = -c/b;
			if ( t >= 0.0 && t <= 1.0 ) {
				flgHit = true;
			}
		} else {
			// b = 0
			// objects do not move in relative
		}
	} else {
		
		t = (-b - D)/(2.0*a);
		if ( t >= 0.0 && t <= 1.0 ) {
			flgHit = true;

		} else {
			t = (-b + D)/(2.0*a);
			if ( t >= 0.0 && t <= 1.0 ) {
				flgHit = true;
			}	
		}
	}

	
		if ( flgHit ) {
			//agi->sv = agi->sv*t;
			//agj->sv = agj->sv*t;
				
			if ( pij.dot(vj) > 0.0 && pij.dot(vi) < 0.0 ) {
				// head to head collision?
				agi->sv = agi->sv*t;
				agj->sv = agj->sv*t;
			} else 
			if ( pij.dot( vi ) >= 0.0 ) {
				// pi is in front of pj
				agj->sv = agj->sv*t;
				//cout << ".";

			} else {
				// pj is in front of pi
				agi->sv = agi->sv*t;
				//cout << ".";
			}
			

	}

	////////////////////////////////////////////////////////
	//DEBUG purpose
	debug_Agents( flgHit, agi, agj );
	////////////////////////////////////////////////////////


	return flgHit;
}

CS_AGENT::CS_AGENT( )
{
	group_id = 0;
	flgReachedDestination = false;
	mFlgUsed = true;
	flgInitPosition = false;
	mInvalidPosition = false;
	p.zero( );
	destP.zero( );
	//desired_v = 1.5;
	desired_speed = 1.0;
	cur_desired_speed = desired_speed;
	destR = 1.0;
	destR2 = destR*destR;
	r = csp_default_AgentRadius;

	flgHit = false;

	maxNumOfPathPoints = 16;
	mNumOfPathPoints = 0;
	mPathPointArr = new CS_PATH_POINT[maxNumOfPathPoints];

	simType = CS_AGENT_SIM_TYPE_POINT_TO_POINT;
	destPathPointIndex = 1;
	//
	travelTime = 0.0;
	travelLength = 0.0;
	//
	max_slow_down_counter = 100;
	slow_down_counter = 0;
	//
	curAccumulatedResponseTime = 0.0;
	maxResponseTime = csp_MaxAgentResponseTime;
	//
	external_path = 0;
}

void CS_AGENT::setRadius( float r )
{
	this->r = r;
}

float CS_AGENT::getRadius2( ) const
{
	return r*r;
}

void CS_AGENT::pre_update( )
{
	prev_p = p;
	prev_v = v;
}

void CS_AGENT::post_update( float time_step ) 
{

	vector3 s = p - prev_p;
	travelTime += time_step;
	travelLength += s.Length( );
	//
	perceived_v = s;
	perceived_s = perceived_v.Length( );

	/////////////////////////////////////////
	/*
	//Debug
	float L_sv = sv.Length( );
	float ratio = perceived_s / L_sv;
	if ( ratio > 2.0 ) {
		cout << "ratio:" << ratio << "\tperceived_s:" << perceived_s << endl;
	}

	ratio = L_sv / (desired_v*0.03);
	if ( ratio > 2.0 ) {
		cout << "L_sv / desired_v ratio:" << ratio << "\L_sv:" << L_sv << endl;
	}
	*/
	/////////////////////////////////////////

}
void CS_AGENT::reset( )
{
	//
	//external_path = 0;
	//numOfPathPoints = 0;
	//
	max_slow_down_counter = 100;
	slow_down_counter = 0;

	travelTime = 0.0;
	travelLength = 0.0;

	flgReachedDestination = false;
	mFlgUsed = true;

	switch( simType ) 
	{
	case CS_AGENT_SIM_TYPE_POINT_TO_POINT:
		if ( flgInitPosition ) {
			p = init_p;
			v = init_v;
		}
		break;
	case CS_AGENT_SIM_TYPE_ALONG_PATH:

		if ( external_path ) {
prepareBasedonSimulationType( 
							 external_path->getNumPoints( ),
							 external_path->getVertArr( )
							 );
		} else {
			/*
		prepareBasedonSimulationType( 
			numOfPathPoints,
	path_points,
	path_radii
			);
			*/
		}
		break;
	}
	
}

void CS_AGENT::setSimulationType( int type )
{
	simType = type;
	switch( simType ) 
	{
	case CS_AGENT_SIM_TYPE_POINT_TO_POINT:
		break;
	case CS_AGENT_SIM_TYPE_ALONG_PATH:
		curPathPointIndex = 0;
		destPathPointIndex = 1;
		break;
	}
}

void CS_AGENT::prepareBasedonSimulationType( )
{
	prepareBasedonSimulationType( 
		external_path->getNumPoints( ),
		external_path->getVertArr( )
		);
}


void CS_AGENT::prepareBasedonSimulationType( 
	int numOfPoints,
	CS_PATH_POINT *pathPointsArr
	)
{

	curPathEdgeIndex = -1;

	switch( simType ) 
	{
	case CS_AGENT_SIM_TYPE_POINT_TO_POINT:
		break;
	case CS_AGENT_SIM_TYPE_ALONG_PATH:
		v.zero( );
		if ( numOfPoints > 1 ) 
		{
			curPathPointIndex = 0;
			destPathPointIndex = 1;
			
			if ( flgInitPosition == false ) {
				p = pathPointsArr[curPathPointIndex].p;
			}
			destP = pathPointsArr[destPathPointIndex].p;
			destR = pathPointsArr[destPathPointIndex].r;
		} else if ( numOfPoints == 1) {
			curPathPointIndex = 0;
			destPathPointIndex = 0;
			p = pathPointsArr[curPathPointIndex].p;
			destP = pathPointsArr[destPathPointIndex].p;
			destR = pathPointsArr[destPathPointIndex].r;
		} else {
			curPathPointIndex = 0;
			destPathPointIndex = 0;
			p.zero( );
			destP.zero( );
			destR = 0.0;
		}
		break;
	}
	destR2 = destR*destR;
}

void CS_AGENT::setExternalPath( const CS_PATH *path )
{
	external_path = path;
	if ( path == 0 ) {
		begin_pathPoints( );
		end_pathPoints( );
		return;
	}
	if ( external_path ) {
		mNumOfPathPoints = external_path->getNumPoints( );
	} else {
		setPath( *external_path );
	}
}

void CS_AGENT::setPath( const CS_PATH &path )
{
	begin_pathPoints( );
	for (int i = 0; i < path.numPoints; ++i ) {
		addPathPoint( path.ptArr[i].p, path.ptArr[i].r );
	}
	end_pathPoints( );
}

void CS_AGENT::begin_pathPoints( )
{
	mNumOfPathPoints = 0;
}

void CS_AGENT::addPathPoint( const vector3 &p, float path_radius )
{
	if ( mNumOfPathPoints >= maxNumOfPathPoints ) return;
	mPathPointArr[mNumOfPathPoints].p = p;
	mPathPointArr[mNumOfPathPoints].r = path_radius;
	++mNumOfPathPoints;
}

void CS_AGENT::end_pathPoints( )
{
}

void CS_AGENT::addPathLoad( )
{
	if ( external_path && curPathEdgeIndex >= 0 ) {
		float w = r*r*csp_path_with_weight_for_agent;
		((CS_PATH*)external_path)->addLoadRecord( curPathEdgeIndex, w );
	}
}

void CS_AGENT::addProjectedDistanceToPath( )
{
	if ( external_path && curPathEdgeIndex >= 0 ) {
		float d_to_next_pathPoint = 0.0;
		float proj_d = 0.0;
		vector3 p0, p1;
		((CS_PATH*)external_path)->getPathPoints( curPathEdgeIndex, p0, p1 );
		d_to_next_pathPoint = p.distance( p1 );
		vector3 q = bt_computeNearestPoint_LineSegment( p, p0, p1 );
		proj_d = q.distance( p ) + r*2.0;
		((CS_PATH*)external_path)->addMaxProjectedDistance( curPathEdgeIndex, d_to_next_pathPoint, proj_d );
	}
}

void CS_AGENT::save( )
{
	init_p = p;
	init_v = v;
}



bool CS_AGENT::computeDesireVelocity_AlongEdge(const vector3 &q0, const vector3 &q1, float time_step )
{
	vector3 qq = q1 - q0;
	float L = qq.Normalize( );

	if ( L == 0.0 ) return false;

		v = qq;
		v = v*cur_desired_speed;
		sv = v*time_step;
	return true;
}

bool CS_AGENT::computeDesireVelocity_AlongPath( 
	int numPoints,
	CS_PATH_POINT *pointArr, 
	int edgeIndex,
	float time_step )
{
	bool flg_reached = false;
	float d2;
	d2 = p.distance2( destP );

	if (  destR2 >= d2 ) {
		// = is used so that zero radius is acceptable
		v.zero( );
		sv.zero( );
		flg_reached = true;
		//
	} else {
		// here pp must not be a zero vector
		vector3 pp = pointArr[ edgeIndex + 1 ].p - pointArr[ edgeIndex ].p;
		pp.Normalize( );
		v = pp;
		v = v*cur_desired_speed;
		sv = v*time_step;
	}
	return flg_reached;
}

bool CS_AGENT::computeDesireVelocity_PointToPoint( 
	int numPoints,
	CS_PATH_POINT *pointArr, 
	float time_step )
{
	bool flg_reached = false;
	float d2;
	d2 = p.distance2( destP );

	if (  destR2 >= d2 ) {
		// = is used so that zero radius is acceptable
		v.zero( );
		sv.zero( );
		flg_reached = true;
		//
	} else {
		// here pp must not be a zero vector
		vector3 pp = destP - p;
		pp.Normalize( );
		v = pp;
		v = v*cur_desired_speed;
		sv = v*time_step;
	}
	return flg_reached;
}

//
// go to path point and then the next path point, etc, 
// in a consecutive manner.
// Must reach the region of the path point 
// before going to the next path point.
//
bool CS_AGENT::computeDesireVelocity_AlongPath_PoinToPoint( 
	int numPoints,
	CS_PATH_POINT *pointArr, 
	float time_step )
{
	bool flg_reached = false;
	bool flg;
	flg = computeDesireVelocity_PointToPoint( 
		numPoints,
		pointArr,
		time_step 
		);
	if ( flg == false ) return flg_reached;
	if ( numPoints > destPathPointIndex ) {
		++curPathPointIndex;
		++destPathPointIndex;
	} else {
		flgReachedDestination = true;
		return true;
	}
	if ( numPoints > destPathPointIndex ) {
		destP = pointArr[destPathPointIndex].p;
		destR = pointArr[destPathPointIndex].r;
		destR2 = destR*destR;
	}

	return flg_reached;
}

/*
//computeDesireVelocity_AlongPath_VoronoiRegions
Steps:
1. check if there is any path point.
2. check if destination is reached.
3. compute the closest path edge
4. the desired direction of the agent points to the second end point of
the path edge.
*/
bool CS_AGENT::computeDesireVelocity_AlongPath_VoronoiRegions( 
	int numPoints,
	CS_PATH_POINT *pointArr, 
	float time_step 
	)
{
	bool flg_reached = false;
	if ( numPoints <= 0 ) {
		flgReachedDestination = true;
		return true; // no point
	}
	float d2 = pointArr[numPoints-1].p.distance2( p );
	float r = pointArr[numPoints-1].r;
	float r2 = r*r;
	if ( r2 >= d2 ) {
		v.zero( );
		sv.zero( );
		flgReachedDestination = true;
		flg_reached = true;
		destPathPointIndex = numPoints;
	} else {
		if ( numPoints >= 2 ) {
			int pathPointIndex;
			float pathPoint_d2;
			float pathEdge_d2;
			int pIndex = bt_computeClosestPathPointIndex( p, pointArr, numPoints, pathPoint_d2 );
			int eIndex = bt_computeClosestPathEdgeIndex( p, pointArr, numPoints, pathEdge_d2 );

			pathPointIndex = eIndex+1;
			destP = pointArr[eIndex+1].p; // get the second end point of the edge
			destR = pointArr[eIndex+1].r;
			destR2 = destR*destR;
			float d2 = destP.distance2( p );
			if ( destR2 >= d2 ) {
				if ( numPoints > eIndex+2 ) {
					pathPointIndex = eIndex+2;
					destP = pointArr[eIndex+2].p; 
					destR = pointArr[eIndex+2].r;
					destR2 = destR*destR;
				} 
			}

			bool flg_along_edge = false;
			if ( (pathPointIndex >= 1) && (numPoints + 1 > pathPointIndex) ) {
				vector3 q0 = pointArr[pathPointIndex - 1].p;
				vector3 q1 = pointArr[pathPointIndex].p;

				 flg_along_edge = computeDesireVelocity_AlongEdge( 
					 q0, q1, 
					 time_step );

			}
			
			if ( flg_along_edge == false) {
				computeDesireVelocity_PointToPoint( 
					numPoints,
					pointArr,
					time_step 
					);
			}

		} else {
			// only one point
			destP = pointArr[0].p;
			destR = pointArr[0].r;
			destR2 = destR*destR;
		}
	}

	desired_v = sv;

	//if ( flg_reached) cout << "flg_reached:" << flg_reached << endl;
	return flg_reached;
}



bool CS_AGENT::computeDesireVelocity_AlongPath_ParallelPathEdge_CornerAware( 
	int numPoints,
	CS_PATH_POINT *pointArr,
	float time_step 
	)
{
	//cout << "computeDesireVelocity_AlongPath_ParallelPathEdge_CornerAware:" << endl;

	curPathEdgeIndex = -1;

	bool flg_reached = false;
	if ( numPoints <= 0 ) {
		flgReachedDestination = true;
		return true; // no point
	}
	float d2 = pointArr[numPoints-1].p.distance2( p );
	float r = pointArr[numPoints-1].r;
	float r2 = r*r;
	if ( r2 >= d2 ) {
		// reach destination
		v.zero( );
		sv.zero( );
		flgReachedDestination = true;
		flg_reached = true;
		destPathPointIndex = numPoints;
		curPathEdgeIndex = numPoints - 2;
	} else {
		if ( numPoints >= 2 ) {
			float pathPoint_d2;
			float pathEdge_d2;
			int pIndex = bt_computeClosestPathPointIndex( p, pointArr, numPoints, pathPoint_d2 );
			int eIndex = bt_computeClosestPathEdgeIndex( p, pointArr, numPoints, pathEdge_d2 );

			curPathEdgeIndex = eIndex;
			destP = pointArr[eIndex+1].p; // get the second end point of the edge
			destR = pointArr[eIndex+1].r;
			destR2 = destR*destR;
			float d2 = destP.distance2( p );
			
			vector3 q = bt_computeNearestPoint_LineSegment( p, pointArr[ eIndex ].p, pointArr[ eIndex + 1 ].p );
			
			float r_pq2 = q.distance2( p );
			float ep = 1.001;
			bool flgClosed = ( d2*ep >= r_pq2 );

			flgClosed = (( pointArr[ eIndex ].p - pointArr[ eIndex + 1 ].p ).dot( p - pointArr[ eIndex + 1 ].p ) < 0.0);
			//flgClosed = flgClosed && ( !(destR2 >= d2) );
			//flgClosed = true;
			if ( destR2 >= d2 
				//&& flgClosed 
				&& (numPoints > eIndex+2)
				) 
			{
					destP = pointArr[eIndex+2].p; 
					destR = pointArr[eIndex+2].r;
					destR2 = destR*destR;
			}
			/*
			if ( (flgClosed == false) && ( destR2 >= d2 ) ) {
				computeDesireVelocity_AlongPath(
					numPoints,
					pointArr,
					eIndex,
					time_step
					);
			} else {
			*/
				computeDesireVelocity_PointToPoint( 
					numPoints,
					pointArr,
					time_step 
				);
				float dpp = prev_v.dot(v);
				float L0 = prev_v.Length( );
				float L1 = v.Length( );
				vector3 n0 = prev_v.Normalized( );
				vector3 n1 = v.Normalized( );

				dpp = dpp/(L0*L1);
				float angle;
				if ( dpp >= 1.0 ) angle = 0.0;
				else angle = acos( dpp );
				float k = 5.0;
				if ( angle == angle ) {
					float n = 3.0*angle/(3.14*30/180.0*time_step);
					if ( ( n > 1.0 ) 
						//&& angle > 3.14/90 
						) 
					{
						//cout << "angle:" << angle/3.14*180.0 << endl;
						//v = n1*time_step/k + n0*(1.0 - time_step/k); 
						float f = 1.0/n;
						v = n1*f + n0*(1.0 - f); 
						v.Normalize( );
						v = v*L1;
						sv = v*time_step;
					}
				}

			//}

		} else {
			// only one point
			destP = pointArr[0].p;
			destR = pointArr[0].r;
			destR2 = destR*destR;
		}
	}

	//if ( flg_reached) cout << "flg_reached:" << flg_reached << endl;
	return flg_reached;
}

//computeDesireVelocity_AlongPath_VoronoiRegions_ParallelPathEdge
bool CS_AGENT::computeDesireVelocity_AlongPath_ParallelPathEdge( 
	int numPoints,
	CS_PATH_POINT *pointArr,
	float time_step 
	)
{
	curPathEdgeIndex = -1;

	bool flg_reached = false;
	if ( numPoints <= 0 ) {
		flgReachedDestination = true;
		return true; // no point
	}
	float d2 = pointArr[numPoints-1].p.distance2( p );
	float r = pointArr[numPoints-1].r;
	float r2 = r*r;
	if ( r2 >= d2 ) {
		v.zero( );
		sv.zero( );
		flgReachedDestination = true;
		flg_reached = true;
		destPathPointIndex = numPoints-1;
		curPathEdgeIndex = numPoints - 2;
	} else {
		if ( numPoints >= 2 ) {
			float pathPoint_d2;
			float pathEdge_d2;
			int pIndex = bt_computeClosestPathPointIndex( p, pointArr, numPoints, pathPoint_d2 );
			int eIndex = bt_computeClosestPathEdgeIndex( p, pointArr, numPoints, pathEdge_d2 );

			curPathEdgeIndex = eIndex;
			destP = pointArr[eIndex+1].p; // get the second end point of the edge
			destR = pointArr[eIndex+1].r;
			destR2 = destR*destR;
			float d2 = destP.distance2( p );
			if ( destR2 >= d2 ) {
				if ( numPoints > eIndex+2 ) {
					destP = pointArr[eIndex+2].p; 
					destR = pointArr[eIndex+2].r;
					destR2 = destR*destR;
				} 
			}
			computeDesireVelocity_PointToPoint( 
				numPoints,
				pointArr,
				time_step 
				);

		} else {
			// only one point
			destP = pointArr[0].p;
			destR = pointArr[0].r;
			destR2 = destR*destR;
		}
	}

	//if ( flg_reached) cout << "flg_reached:" << flg_reached << endl;
	return flg_reached;
}

void CS_AGENT::computeDesireVelocity( float time_step )
{

	switch( simType )
	{
	case CS_AGENT_SIM_TYPE_POINT_TO_POINT:
		if ( external_path) {
		computeDesireVelocity_PointToPoint( 
			external_path->getNumPoints( ),
				external_path->getVertArr( ),
			time_step );
		}
		break;
	case CS_AGENT_SIM_TYPE_ALONG_PATH:
		//computeDesireVelocity_AlongPath_PoinToPoint( time_step );
		//computeDesireVelocity_AlongPath_VoronoiRegions( time_step );
		if ( external_path) {
			computeDesireVelocity_AlongPath_ParallelPathEdge( 
				external_path->getNumPoints( ),
				external_path->getVertArr( ),
			time_step 
			);
			if ( csp_enableAgentCornerMovement ) {
				computeDesireVelocity_AlongPath_ParallelPathEdge_CornerAware( 
					external_path->getNumPoints( ),
					external_path->getVertArr( ),
					time_step 
				);
			} else {
				computeDesireVelocity_AlongPath_ParallelPathEdge( 
					external_path->getNumPoints( ),
					external_path->getVertArr( ),
					time_step 
				);
			}
		} else {
			
		computeDesireVelocity_AlongPath_ParallelPathEdge( 
			mNumOfPathPoints,
			mPathPointArr,
			time_step 
			);
		}
		break;
	}

	if ( csp_enableSlowDownMechanismDueToAgentCollision ) {
		--slow_down_counter;
		if ( slow_down_counter > max_slow_down_counter ) {
			slow_down_counter = max_slow_down_counter;
		} else if ( slow_down_counter < 0 ) {
			slow_down_counter = 0;
		}
		float sd_factor = slow_down_counter/(float)max_slow_down_counter*csp_cd_penalty_factor;
		
		//if ( slow_down_counter > 0 ) {
		//	sd_factor = 1.0;
		//}
		float scale_down_factor = 1.0 - sd_factor;
		if ( scale_down_factor < 0.0 ) scale_down_factor = 0.0;
		sv = sv*scale_down_factor;
	} else {
		slow_down_counter = 0;
	}
}

void CS_AGENT::setPosition( const vector3 &p )
{
	setPosition(p.x, p.y, p.z );
}

void CS_AGENT::setPosition( float x, float y, float z )
{
	flgInitPosition = true;
	p.x = x;
	p.y = y;
	p.z = z;
	prev_p = p;
	predict_p = p;
	init_p = p;
}

void CS_AGENT::setDestinationPosition( float x, float y, float z )
{
	destP.x = x;
	destP.y = z;
	destP.z = z;
}

void CS_AGENT::setDestinationPosition( float x, float y, float z, float r )
{
	destP.x = x;
	destP.y = z;
	destP.z = z;
	destR = r;
	destR2 = destR*destR;
}

void CS_AGENT::setDestinationPosition( const vector3 &dest_p, float dest_r )
{
	destP = dest_p;
	destR = dest_r;
	destR2 = destR*destR;
}

bool CS_AGENT::isAtDestination( ) const
{
	bool flg = false;
	switch( simType )
	{
	case CS_AGENT_SIM_TYPE_POINT_TO_POINT:
		flg = ( destR2 >= p.distance2( destP ) );
		break;
	case CS_AGENT_SIM_TYPE_ALONG_PATH:
		if ( destPathPointIndex >= mNumOfPathPoints ) {
			flg = true;
		}
		break;
	}
	return flg;
}

void CS_AGENT::chooseStrategy( )
{
}

void CS_AGENT::updatePosition( float time_step )
{
	p = p + sv;
	predict_p = p;
}

void CS_AGENT::printf_Data( ) const
{
	cout << "Agent----" << endl;
	cout << "flgHit:" << flgHit << endl;
	printf_vector3("p", p);
	printf_vector3("v", v);
	printf_vector3("sv", sv);
	cout << "Walk step size:" << sv.Length( ) << endl;
	cout << "Travel time:" << travelTime << endl;
	cout << "TravelLength:" << travelLength << endl;

}

/*
void CS_AGENT::printf( ) const
{
	cout << "Agent index:" << _index << endl;
	cout << "travelTime:\t" << travelTime << endl;
	cout << "travelLength:\t" << travelLength << endl;
}
*/

float CS_AGENT::computeDistance2_XZ( const vector3 & p ) const
{

	return ( this->p.x - p.x )*( this->p.x - p.x ) 
		+ 
		( this->p.z - p.z )*( this->p.z - p.z );
}
