#include "crowdSimulation_Basic.h"
#include "../environment.h"

namespace {
	CROWD_SIMULATION_BASIC *_gui_main_obj = 0;
	GLUI *glui_property_environment_obj = 0;
	int _mainWinID = 0;
	//
	GLUI_Spinner    *spinner;
	//
	int environment_objID = -1;
	CS_ENV_OBJECT *_currentPickedEnvironmentObj = 0;
	int flgActionData = false;
	float enabledDuration = 15.0;
	float disabledDuration = 10.0;
	//
	GLUI_Spinner    *spinner_enabledDuration;
	GLUI_Spinner    *spinner_disabledDuration;


	//
	
};

void property_control_envObj( int control )
{

}

void control_cb( int control )
{
}

void control_cb_update( )
{
	if ( _currentPickedEnvironmentObj == 0) return;
	_currentPickedEnvironmentObj->enableActionData( flgActionData );
}

void control_cb_add_working_cycle( )
{
	if ( _currentPickedEnvironmentObj == 0) return;
_currentPickedEnvironmentObj->addWorkingCycle( enabledDuration, disabledDuration );
}

void createGUI_EnvironmentObj( )
{

    GLUI *glui = glui_property_environment_obj = GLUI_Master.create_glui( "Environment Object Properties",
      0, 50, 500 );

    //GLUI_Panel *panel = new GLUI_Panel(glui_property_agent_generator,"", GLUI_PANEL_NONE);
    //new GLUI_Button(panel, "Close", GLUI_CMD_CLOSE_ID, pointer_cb);
    
	spinner  = new GLUI_Spinner( glui, "ID:", GLUI_SPINNER_INT, &environment_objID,  0, property_control_envObj );
	spinner->set_int_limits(-1, 65536 );

	new GLUI_Checkbox( glui, "Use Action Data", &flgActionData, 0, control_cb );

	new GLUI_Button( glui, "Add Working Cycle", 0, (GLUI_Update_CB) control_cb_add_working_cycle );
	spinner_enabledDuration  = new GLUI_Spinner( glui, "Enabled Duration:", GLUI_SPINNER_FLOAT, &enabledDuration,  1, control_cb );
	spinner_enabledDuration->set_int_limits(0.0, 100.0 );
	spinner_disabledDuration  = new GLUI_Spinner( glui, "Disabled Duration:", GLUI_SPINNER_FLOAT, &disabledDuration,  2, control_cb );
	spinner_disabledDuration->set_int_limits(0.0, 100.0 );

	new GLUI_Button( glui, "Update", 0, (GLUI_Update_CB)control_cb_update );

}

void CROWD_SIMULATION_BASIC::update_glui_property_environment( )
{
		bool flgNew;
	CS_ENV_OBJECT *obj = _currentPickedEnvironmentObj = CS_ENVIRONMENT::instance( )->getCurPickedObj( flgNew );
	if ( flgNew == false ) return;

vector3 p;
environment_objID = obj->getObjID( );
flgActionData = obj->getFlg_UseActionData( );
glui_property_environment_obj->sync_live( );
}

void CROWD_SIMULATION_BASIC::createGLUI_EnvironmentObject( int mainWinID, void (global_idle_func()), void *glui_ptr )
{
		_gui_main_obj = this;
	_mainWinID = mainWinID;

	createGUI_EnvironmentObj( );
}
