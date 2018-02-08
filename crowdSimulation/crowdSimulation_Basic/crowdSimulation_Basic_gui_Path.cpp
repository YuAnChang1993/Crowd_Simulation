#include "crowdSimulation_Basic.h"
#include "../pathManager.h"

namespace {
	CROWD_SIMULATION_BASIC *_gui_main_obj = 0;
	CS_PATH *_currentPickedPathObj = 0;
	int _mainWinID = 0;
	//
	GLUI_Spinner    *spinner;
	//
	GLUI *glui_property_path_obj = 0;
	//
	int path_objID = 0;
	float path_node_radius = 0.0;



void property_control( int control )
{
	if ( _currentPickedPathObj ) {
		_currentPickedPathObj->setPickedPathNodeRadius( path_node_radius );
	}
}

void control_cb_auto_resize( )
{
}

void control_cb_update( )
{

}


void createGUI_PathObj( )
{
	    GLUI *glui = glui_property_path_obj = GLUI_Master.create_glui( "Path Object Properties",
      0, 50, 500 );

	spinner  = new GLUI_Spinner( glui, "ID:", GLUI_SPINNER_INT, &path_objID,  0, property_control );
	spinner->set_int_limits(-1, 65536 );
	new GLUI_Button( glui, "Auto-resize path node", 0, (GLUI_Update_CB)control_cb_auto_resize );
	spinner  = new GLUI_Spinner( glui, "Path node radius:", GLUI_SPINNER_FLOAT, &path_node_radius,  1, property_control );
	spinner->set_float_limits( 0.001, 100.0 );
	new GLUI_Button( glui, "Update", 0, (GLUI_Update_CB)control_cb_update );

}

};

void CROWD_SIMULATION_BASIC::createGLUI_PathObject( int mainWinID, void (global_idle_func()), void *glui_ptr )
{
	_gui_main_obj = this;
	_mainWinID = mainWinID;

	::createGUI_PathObj( );
}

void CROWD_SIMULATION_BASIC::update_glui_property_path_obj( )
{
	bool flgNew;
	_currentPickedPathObj = CS_PATH_MANAGER::instance( )->getCurPickedObj( flgNew );
	if ( _currentPickedPathObj == 0 ) return;
	path_objID = _currentPickedPathObj->getObjID( );
	path_node_radius = _currentPickedPathObj->getPickedPathNodeRadius( );
	glui_property_path_obj->sync_live( );
}
