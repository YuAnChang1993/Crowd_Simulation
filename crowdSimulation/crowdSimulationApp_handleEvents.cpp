//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include "crowdSimulationApp.h"
#include "agent_generator_manager.h"
//
using namespace std;

namespace {
	int hitDistanceBasedOnPixels = 15;
};

void CROWD_SIMULATION_APP::clearPickedObj( )
{
	mClickedPickedObj = 0;
	mPassivePickedObj = 0;

}

bool CROWD_SIMULATION_APP::specialFunc(int key, int x, int y)
{
	bool flgHandled = false;

	//cout << "specialFunc:" << key << endl;
	//cout << ">" << endl;
	//switch( key ) {
	//}
	return flgHandled;
}

void CROWD_SIMULATION_APP::handleSpaceBarEvent( )
{
	int mActionType = mCrowdSimulation->getAction( );

	//cout << "CROWD_SIMULATION_APP::mouseFunc mActionType:" << mActionType << endl;
	float x, y, z;
	//mCamera->getCoordinates( x, y, z, mx, my );
	//mSelectedPoint = vector3( x, 0.0, z );
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_SIMULATION:
			mSelectedObjArr.clear( );
			mPassivePickedObj = 0;
			mClickedPickedObj = 0;
			break;
		case CROWD_SIMULATION_ACTION_ADD_ENV_OBJ:
			mPassivePickedObj = 0;
			mClickedPickedObj = 0;

			mCrowdSimulation->addEnvObj( );
			
			break;
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			CS_ENVIRONMENT::instance( )->inverseVertexOrderForSelectedObj( );
			break;
		case CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR:
			mCrowdSimulation->setActionType_AddAgentGenerator( );
			break;
		case CROWD_SIMULATION_ACTION_ADD_PATH_OBJ:
			mPassivePickedObj = 0;
			mClickedPickedObj = 0;
			mCrowdSimulation->setActionType_AddPathObjNow( );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ:
			break;
	}

}

bool CROWD_SIMULATION_APP::handleKeyEvent( unsigned char key )
{
	bool flgHandled = false;
	//cout << "key:" << ( int ) key << endl;
	//
	flgHandled = mCrowdSimulation->handleKeyEvent( key );
	if ( flgHandled ) return flgHandled;
	//
	switch( key ) {
		case 't':
			mCrowdSimulation->update_crowd( true );
			mFlgSimulationEnabled = false;
			flgHandled = true;
			
			break;
		case ' ':
			handleSpaceBarEvent( );
			break;
		case '|':
			mFlgShow_LatticePartition = !mFlgShow_LatticePartition;
			break;
		case 'j':
			mFlgShow_Agents = !mFlgShow_Agents;
			flgHandled = true;
			break;
		case 'l':
			mFlgShow_Paths = !mFlgShow_Paths;
			flgHandled = true;
			break;
		case 'g':
			mFlgShow_Grid = !mFlgShow_Grid;
			flgHandled = true;
			break;
		case 'h':
			mFlgShow_AgentGenerators = !mFlgShow_AgentGenerators;
			flgHandled = true;
			break;
		case 'n':
			mFlgShow_SeparationDistanceMap = !mFlgShow_SeparationDistanceMap;
			flgHandled = true;
			break;
		case 'm':
			mFlgShow_AgentMovementMap = !mFlgShow_AgentMovementMap;
			flgHandled = true;
			break;
		case 'r':
			mFlgSimulationEnabled = !mFlgSimulationEnabled;
			flgHandled = true;
			break;
	}
	return flgHandled;
}
///////////////////////////////////////////
bool CROWD_SIMULATION_APP::mouseMotionFunc_State_Simulation( int mx, int my )
{
	//cout << "mMousePressedButton:" << mMousePressedButton << endl;
	//cout << "GLUT_RIGHT_BUTTON:" << GLUT_RIGHT_BUTTON << endl;

	if ( mClickedPickedObj == 0 
		|| mMousePressedButton == GLUT_RIGHT_BUTTON 
		|| mMousePressedButton == GLUT_MIDDLE_BUTTON 
		) {
		return FREE_CANVAS_2D::mouseMotionFunc( mx, my );
	}

	if ( mClickedPickedObj ) {
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	vector3 translation_vector = vector3( x, 0.0, z) - mSelectedPoint;
	mSelectedPoint = vector3( x, 0.0, z);
	mClickedPickedObj->bo_translatePosition_XZ( 
		translation_vector.x, 
		translation_vector.z,
		mSelectedPoint.x, 
		mSelectedPoint.z
		);
	}
	//cout << "Set Position XZ:" << x << "\t" << z << endl;
	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_AddAgentGenerator( int mx, int my )
{
	//cout << "CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj" << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	
	if ( mMousePressedButton == GLUT_LEFT_BUTTON ) {
	} else if ( 
		mMousePressedButton == GLUT_RIGHT_BUTTON 
		|| mMousePressedButton == 3 
		|| mMousePressedButton == 4 ) {
		return FREE_CANVAS_2D::mouseMotionFunc( mx, my );
	}

	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_EditAgentGenerator( int mx, int my )
{
	//cout << "CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj" << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	
	if ( mMousePressedButton == GLUT_LEFT_BUTTON ) {
		AGENT_GENERATOR_MANAGER::instance( )->movePickedObj( vector3(x, y, z) );
	} else if ( 
		mMousePressedButton == GLUT_RIGHT_BUTTON 
		|| mMousePressedButton == 3 
		|| mMousePressedButton == 4 ) {
		return FREE_CANVAS_2D::mouseMotionFunc( mx, my );
	}

	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj( int mx, int my )
{
	//cout << "BGN CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj" << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	
	if ( mMousePressedButton == GLUT_LEFT_BUTTON ) {
		CS_ENVIRONMENT::instance( )->movePickedObj( vector3(x, y, z) );
	} else if ( 
		mMousePressedButton == GLUT_RIGHT_BUTTON 
		|| mMousePressedButton == 3 
		|| mMousePressedButton == 4 ) {
		FREE_CANVAS_2D::mouseMotionFunc( mx, my );
	}

	//cout << "END CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj" << endl;

	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_EditPathObj( int mx, int my )
{
	cout << "CROWD_SIMULATION_APP::mouseMotionFunc_State_EditEnvObj" << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	
	if ( mMousePressedButton == GLUT_LEFT_BUTTON ) {
		CS_PATH_MANAGER::instance( )->movePickedObj( vector3(x, y, z) );
	} else if ( 
		mMousePressedButton == GLUT_RIGHT_BUTTON 
		|| mMousePressedButton == 3 
		|| mMousePressedButton == 4 ) {
		return FREE_CANVAS_2D::mouseMotionFunc( mx, my );
	}

	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_AddEnvObj( int mx, int my )
{
	return true;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc_State_AddPathObj( int mx, int my )
{
	return true;
}


bool CROWD_SIMULATION_APP::mouseFunc_State_AddPathObj( int button, int state, int mx, int my, float hitDistance )
{
	//cout << "button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				CS_PATH_MANAGER::instance( )->mouseClick( vector3( x, 0.0, z ), hitDistance );
				break;
			case GLUT_UP:
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}

bool CROWD_SIMULATION_APP::mouseFunc_State_EditPathObj( int button, int state, int mx, int my, float hitDistance )
{
	//cout << "button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				CS_PATH_MANAGER::instance( )->mouseClick( vector3(x, y, z), hitDistance );
				break;
			case GLUT_UP:
				//CS_PATH_MANAGER::instance( )->unpickSelectedObj( );
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}


bool CROWD_SIMULATION_APP::mouseFunc_State_AddEnvObj( int button, int state, int mx, int my, float hitDistance )
{
	//cout << "button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				CS_ENVIRONMENT::instance( )->mouseClick( vector3( x, 0.0, z ), hitDistance );
				break;
			case GLUT_UP:
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}

bool CROWD_SIMULATION_APP::mouseFunc_State_EditEnvObj( int button, int state, int mx, int my, float hitDistance )
{
	cout << "BGN mouseFunc_State_EditEnvObj button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				CS_ENVIRONMENT::instance( )->mouseClick( vector3(x, y, z), hitDistance );
				break;
			case GLUT_UP:
				//CS_ENVIRONMENT::instance( )->unpickSelectedObj( );
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	cout << "END mouseFunc_State_EditEnvObj button:" << button << endl;

	return true;
}

bool CROWD_SIMULATION_APP::mouseFunc_State_AddAgentGenerator( int button, int state, int mx, int my, float hitDistance )
{
	cout << "CROWD_SIMULATION_APP::mouseFunc_State_AddAgentGenerator button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				AGENT_GENERATOR_MANAGER::instance( )->mouseClick( vector3( x, 0.0, z ), hitDistance );
				break;
			case GLUT_UP:
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}


bool CROWD_SIMULATION_APP::mouseFunc_State_EditAgentGenerator( int button, int state, int mx, int my, float hitDistance )
{
	cout << "CROWD_SIMULATION_APP::mouseFunc_State_EditAgentGenerator button:" << button << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	if ( button == GLUT_LEFT_BUTTON ) {
		switch ( state ) {
			case GLUT_DOWN:
				
				AGENT_GENERATOR_MANAGER::instance( )->mouseClick( vector3( x, 0.0, z ), hitDistance );

				//AGENT_GENERATOR_MANAGER::instance( )->pickObjForEditing( vector3( x, 0.0, z ) );
				
				break;
			case GLUT_UP:
				break;
		}
	} else if ( button == GLUT_RIGHT_BUTTON || button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}


bool CROWD_SIMULATION_APP::mouseFunc_State_Simulation( int button, int state, int mx, int my )
{
	//cout << "button:" << button << endl;
	
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );
		mClickedPickedObj = mCrowdSimulation->pickObj_XZ_BasedOnLattice( x, z );
		//cout << "mClickedPickedObj:" << mClickedPickedObj << endl;
		if ( mClickedPickedObj ) {
			if ( glutGetModifiers( ) == GLUT_ACTIVE_SHIFT ) {
				mSelectedObjArr.push_back( mClickedPickedObj );
			}
		} else {
			mClickedPickedObj = mPassivePickedObj;
			mPassivePickedObj = 0;
		}

		if ( mClickedPickedObj ) {
			mClickedPickedObj->printf_Data( );
		}
		return true;
	}
	if ( ( mClickedPickedObj == 0 ) 
		|| button == GLUT_RIGHT_BUTTON
		|| button == 3 || button == 4 ) {
		return FREE_CANVAS_2D::mouseFunc( button, state, mx, my );
	}
	return true;
}


///////////////////////////////////
bool CROWD_SIMULATION_APP::passiveMouseFunc_State_AddAgentGenerator( int mx, int my )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );		
		AGENT_GENERATOR_MANAGER::instance( )->setTemporaryPointForNewObj( vector3(x, y, z) );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_EditAgentGenerator( int mx, int my, float hitDistance )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );		
		AGENT_GENERATOR_MANAGER::instance( )->passiveMouseFunc( vector3(x, y, z), hitDistance );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_AddPathObj( int mx, int my )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );
		
		CS_PATH_MANAGER::instance( )->setTemporaryPointForNewObj( vector3(x, y, z) );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_EditPathObj( int mx, int my, float hitDistance )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );
		CS_PATH_MANAGER::instance( )->passiveMouseFunc( vector3(x, y, z), hitDistance );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_AddEnvObj( int mx, int my )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );
		
		CS_ENVIRONMENT::instance( )->setTemporaryPointForNewObj( vector3(x, y, z) );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_EditEnvObj( int mx, int my, float hitDistance )
{
		float x, y, z;
		mCamera->getCoordinates( x, y, z, mx, my );
		
		CS_ENVIRONMENT::instance( )->passiveMouseFunc( vector3(x, y, z), hitDistance );
		return true;
}

bool CROWD_SIMULATION_APP::passiveMouseFunc_State_Simulation( int mx, int my )
{
		float x, y, z;
		//cout << "passiveMouseFunc_State_Simulation" << endl;
		mCamera->getCoordinates( x, y, z, mx, my );
		BASE_OBJ *tmpObj = mPassivePickedObj;
		mPassivePickedObj = mCrowdSimulation->pickObj_XZ_BasedOnLattice( x, z );
		if ( mPassivePickedObj ) {
			if ( tmpObj != mPassivePickedObj ) {
				mPassivePickedObj->printf_Data( );
			}
		}
		return true;
}

/////////////////////////////////////////////////////////////
bool CROWD_SIMULATION_APP::passiveMouseFunc( int mx, int my )
{
	bool flg = true;
	int mActionType = mCrowdSimulation->getAction( );
	//cout << "CROWD_SIMULATION_APP::passiveMouseFunc:" << mActionType << endl;
	//
	bool flgHitObj = false;
	float screenHitDistance = mCamera->getAbsoluteDistanceForPixels( hitDistanceBasedOnPixels );
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_SIMULATION:
			passiveMouseFunc_State_Simulation( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_ADD_ENV_OBJ:
			passiveMouseFunc_State_AddEnvObj( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			passiveMouseFunc_State_EditEnvObj( mx, my, screenHitDistance );
			flgHitObj = CS_ENVIRONMENT::instance( )->isHitObj( );

			break;
		case CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR:
			passiveMouseFunc_State_AddAgentGenerator( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR:
			passiveMouseFunc_State_EditAgentGenerator( mx, my, screenHitDistance );
			flgHitObj = AGENT_GENERATOR_MANAGER::instance( )->isHitObj( );
			break;
		case CROWD_SIMULATION_ACTION_ADD_PATH_OBJ:
			passiveMouseFunc_State_AddPathObj( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ:
			passiveMouseFunc_State_EditPathObj( mx, my, screenHitDistance );
			flgHitObj = CS_PATH_MANAGER::instance( )->isHitObj( );
			break;
	}
	if ( flgHitObj ) {
		glutSetCursor( GLUT_CURSOR_CROSSHAIR ); 
	} else {
		glutSetCursor( GLUT_CURSOR_LEFT_ARROW ); 
	}

	return flg;
}

//
bool CROWD_SIMULATION_APP::mouseFunc( int button, int state, int mx, int my )
{
	bool flg = true;
	
	if ( state == GLUT_DOWN ) {
		mMousePressedButton = button;
	} else {
		mMousePressedButton = 0;
	}

	int mActionType = mCrowdSimulation->getAction( );

	float hitDistance = mCamera->getAbsoluteDistanceForPixels( hitDistanceBasedOnPixels );

	//cout << "CROWD_SIMULATION_APP::mouseFunc mActionType:" << mActionType << endl;
	float x, y, z;
	mCamera->getCoordinates( x, y, z, mx, my );
	mSelectedPoint = vector3( x, 0.0, z );
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_SIMULATION:
			mouseFunc_State_Simulation( button, state, mx, my );
			break;
		case CROWD_SIMULATION_ACTION_ADD_ENV_OBJ:
			clearPickedObj( );
			
			mouseFunc_State_AddEnvObj( button, state, mx, my, hitDistance );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			mouseFunc_State_EditEnvObj( button, state, mx, my, hitDistance );
			break;
		case CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR:
			mouseFunc_State_AddAgentGenerator( button, state, mx, my, hitDistance );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR:
			mouseFunc_State_EditAgentGenerator( button, state, mx, my, hitDistance );
			break;
		case CROWD_SIMULATION_ACTION_ADD_PATH_OBJ:
			clearPickedObj( );

			mouseFunc_State_AddPathObj( button, state, mx, my, hitDistance );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ:
			mouseFunc_State_EditPathObj( button, state, mx, my, hitDistance );
			break;
	}

	mCrowdSimulation->update_glui( );

	return flg;
}

bool CROWD_SIMULATION_APP::mouseMotionFunc( int mx, int my )
{
	bool flg = true;
	//cout << "CROWD_SIMULATION_APP::mouseMotionFunc" << endl;
	int mActionType = mCrowdSimulation->getAction( );
	switch( mActionType ) {
		case CROWD_SIMULATION_ACTION_SIMULATION:
			mouseMotionFunc_State_Simulation( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_ADD_ENV_OBJ:
			mouseMotionFunc_State_AddEnvObj( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_ENV_OBJ:
			mouseMotionFunc_State_EditEnvObj( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_ADD_AGENT_GENERATOR:
			mouseMotionFunc_State_AddAgentGenerator( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_AGENT_GENERATOR:
			mouseMotionFunc_State_EditAgentGenerator( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_ADD_PATH_OBJ:
			mouseMotionFunc_State_AddPathObj( mx, my );
			break;
		case CROWD_SIMULATION_ACTION_EDIT_PATH_OBJ:
			mouseMotionFunc_State_EditPathObj( mx, my );
			break;
	}
	return flg;
}