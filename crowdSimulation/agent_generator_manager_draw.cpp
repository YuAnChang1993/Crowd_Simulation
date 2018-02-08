//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include "agent_generator_manager.h"
#include "../openGLbasic/opengl_stuff.h"

using namespace std;

namespace {
void drawAgentGenerator( const CS_AGENT_GENERATOR *g ) 
{

	vector3 p;
	float r;

	g->getGenerationPosition( p, r );
	ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
}

};

void AGENT_GENERATOR_MANAGER::drawAgentGenerators( ) const
{
	glLineWidth(3.0);
	glColor3f(0.0, 1.0, 0.0);
	

	mAgentGenProvider.resetIterator( );
	CS_AGENT_GENERATOR *g;
	while ( g = mAgentGenProvider.getNext( ) ) {
	
	//for ( int i = 0; i < mCurNumOfAgentGenerators; ++i ) 
	//{
		vector3 p;
		float r;
		//int index = mUIObjArr[ i ];
		//mAgentGeneratorArr[ index ].getGenerationPosition( p, r );
		g->getGenerationPosition( p, r );
		ogl_drawSimpleCircleXZ( p.x, p.y, p.z, r, 12 );
	}
}

void AGENT_GENERATOR_MANAGER::draw( ) const
{
	if ( m_bo_CurAction == CS_EDIT_ACTION_ADD_OBJ ) {
		glLineWidth(3.0);
		glColor3f(0.8, 0.8, 0.8);
		drawAgentGenerator( &mTmpObject );
	}

	drawAgentGenerators( );

	if ( mCurPickedObj && mCurPickedObj->getUsedFlg( ) && mCurPickedObj->getActivatedFlg( ) ) {
		glLineWidth(3.0);
		glColor3f(0.8, 0.0, 0.0);
		drawAgentGenerator( mCurPickedObj );
	}

	if ( mCurHitObj && (mCurHitObj != mCurPickedObj ) 
		&& mCurHitObj->getUsedFlg( ) && mCurHitObj->getActivatedFlg( )
		) {
		glLineWidth(3.0);
		glColor3f(0.2, 0.2, 0.2);
		drawAgentGenerator( mCurHitObj );
	}
}
