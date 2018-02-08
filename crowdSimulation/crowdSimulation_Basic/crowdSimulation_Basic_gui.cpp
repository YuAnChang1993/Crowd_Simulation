#include "crowdSimulation_Basic.h"
#include "../agent_generator_manager.h"
#include "../cs_parameters.h"

namespace {
	CROWD_SIMULATION_BASIC *_gui_main_obj = 0;

	/** These are the live variables passed into GLUI ***/

	const int OBJ_TYPE_AGENT_GENERATOR = 0;
	const int OBJ_TYPE_ENVIRONMENT_OBJ = 1;
	const int OBJ_TYPE_PATH_OBJ = 2;

	int _mainWinID = 0;

	int   wireframe = 0;
	int   obj_type = OBJ_TYPE_AGENT_GENERATOR;
	int   segments = 8;
	float time_step = 0.03333;
	int num_iteration_per_step = 30;



	// Using a std::string as a live variable is safe.
	GLUI *glui_property_agent_generator = 0;

	std::string text = "Hello World!";
	GLUI_Checkbox   *checkbox;
	GLUI_Spinner    *spinner;
	GLUI_RadioGroup *radio_obj_type;
	GLUI_EditText   *edittext;

	GLUI_RadioGroup *radio_agent_gen_path_type;
	int agent_gen_path_type = 0;
	/*
		double mCurGenerationTime;
		float mRadius;
		float mNumAgentPerSecond;
		float mTimeRequireForOneAgent;
		float mAgentSpeedMin, mAgentSpeedMax;
		int mAgentPathIndex;
	*/
	int agent_generator_objID = -1;
	float agent_generator_Radius = 10.0;
	float agent_generator_NumAgentPerSecond = 1.0;
	float agent_generator_AgentSpeedMin = 1.0;
	float agent_generator_AgentSpeedMax = 1.5;
	float agent_generator_AgentRadiusMin = csp_Generator_AgentRadiusMin;
	float agent_generator_AgentRadiusMax = csp_Generator_AgentRadiusMax;
	float agent_generator_AgentRadiusAvg = 0.35;
	float agent_generator_AgentRadiusStandardDeviation = 0.1;

	float agent_generator_MaxGenerationTime = 0.0;
	int agent_generator_MaxNumOfGeneratedAgents = 1;
	int agent_generator_PathType = CS_AGENT_GEN_PATH_TYPE_CLOSEST;

	CS_AGENT_GENERATOR *_currentPickedAgentGenerator = 0;

	CS_AGENT_GENERATOR _agentGenerator_template;


void setAgentGeneratorTemplate( )
{
	_agentGenerator_template.setMaxGenerationTime( -1.0 );

	_agentGenerator_template.setRadius( agent_generator_Radius );
	int pathType = CS_AGENT_GEN_PATH_TYPE_CLOSEST;
	switch( agent_gen_path_type ) 
	{
	case 0:
		pathType = CS_AGENT_GEN_PATH_TYPE_CLOSEST;
		break;
	case 1:
		pathType = CS_AGENT_GEN_PATH_TYPE_RANDOM;
		break;
	default:
		pathType = agent_generator_PathType;
		break;
	}
	_agentGenerator_template.setAgentPathIndex( pathType );
	_agentGenerator_template.setAgentSpeedMin( agent_generator_AgentSpeedMin );
	_agentGenerator_template.setAgentSpeedMax( agent_generator_AgentSpeedMax );
	_agentGenerator_template.setAgentRadiusRange( agent_generator_AgentRadiusMin, agent_generator_AgentRadiusMax );
	_agentGenerator_template.setMaxGenerationTime( agent_generator_MaxGenerationTime );
	_agentGenerator_template.setMaxAgentNumber( agent_generator_MaxNumOfGeneratedAgents );

}

void control_cb( int control )
{
	
	if ( _gui_main_obj ) {
		_gui_main_obj->setNumOfIterationsPerStep( num_iteration_per_step );
	}	
}

void control_radio_createObjs( int control )
{
	agent_gen_path_type = radio_agent_gen_path_type->get_int_val( );
}



void property_control_cb( int control )
{
	 printf( "property_control_cb callback: %d\n", control );
}

void _saveFile( )
{
	if ( _gui_main_obj ) _gui_main_obj->save( );
}

void _addObj( )
{
	
	//if ( _gui_main_obj ) _gui_main_obj->save( );
	int obj_type = radio_obj_type->get_int_val( );
	cout << "radio_obj_type:" << obj_type << endl;
	switch( obj_type ) {
		case OBJ_TYPE_AGENT_GENERATOR:
			if ( _gui_main_obj ) {
				_gui_main_obj->setActionType_AddAgentGenerator( );
				AGENT_GENERATOR_MANAGER::instance( )->setAgentGeneratorTemplate( _agentGenerator_template );
			}
			break;
		case OBJ_TYPE_ENVIRONMENT_OBJ:
			if ( _gui_main_obj ) _gui_main_obj->setActionType_AddEnvObj( );
		break;
		case OBJ_TYPE_PATH_OBJ:
			if ( _gui_main_obj ) _gui_main_obj->setActionType_AddPathObj( );
			break;
	}

	glutPostRedisplay( );
}

void _editObj( )
{
	if ( _gui_main_obj == 0 ) return;
	int obj_type = radio_obj_type->get_int_val( );
	cout << "radio_obj_type:" << obj_type << endl;
	
	if ( _gui_main_obj ) _gui_main_obj->unpickAllObjs( );

	switch( obj_type ) {
		case OBJ_TYPE_AGENT_GENERATOR:
			if ( _gui_main_obj ) _gui_main_obj->setActionType_EditAgentGenerator( );

			break;
		case OBJ_TYPE_ENVIRONMENT_OBJ:
			if ( _gui_main_obj ) _gui_main_obj->setActionType_EditEnvObj( );
		break;
		case OBJ_TYPE_PATH_OBJ:
			if ( _gui_main_obj ) _gui_main_obj->setActionType_EditPathObj( );
			break;
	}
	
	glutPostRedisplay( );

}

void _refreshSimulation( )
{
	if ( _gui_main_obj ) _gui_main_obj->refreshSimulation( );
}

void _simulation( )
{
	if ( _gui_main_obj ) _gui_main_obj->setActionType_Simulation( );
}

void control_radio_agent_gen( int control )
{
}

void property_control_cb_update_agentGen( int control ) {
	if ( _currentPickedAgentGenerator == 0 ) return;
	_currentPickedAgentGenerator->setObjID( agent_generator_objID );
	_currentPickedAgentGenerator->setAgentPathIndex( agent_generator_PathType );
	_currentPickedAgentGenerator->setRadius( agent_generator_Radius);
	_currentPickedAgentGenerator->setAgentSpeedMin( agent_generator_AgentSpeedMin );
	_currentPickedAgentGenerator->setAgentSpeedMax( agent_generator_AgentSpeedMax );
	_currentPickedAgentGenerator->setAgentRadiusRange( agent_generator_AgentRadiusMin, agent_generator_AgentRadiusMax );
	_currentPickedAgentGenerator->setGenerationFrequency( agent_generator_NumAgentPerSecond );
	_currentPickedAgentGenerator->setMaxGenerationTime( agent_generator_MaxGenerationTime );
	_currentPickedAgentGenerator->setMaxAgentNumber( agent_generator_MaxNumOfGeneratedAgents );
	glutPostRedisplay( );

}

void property_control_cb_agent_objID( int control )
{
}

void control_property_agent_generator( )
{

    GLUI *glui = glui_property_agent_generator = GLUI_Master.create_glui( "Agent Generator Properties",
      0, 50, 500 );

    //GLUI_Panel *panel = new GLUI_Panel(glui_property_agent_generator,"", GLUI_PANEL_NONE);
    //new GLUI_Button(panel, "Close", GLUI_CMD_CLOSE_ID, pointer_cb);
    
	spinner  = new GLUI_Spinner( glui, "ID:", GLUI_SPINNER_INT, &agent_generator_objID,  0, property_control_cb_agent_objID );
	spinner->set_int_limits(-1, 65536 );

	GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Generator Path Type" );
	radio_agent_gen_path_type = new GLUI_RadioGroup( obj_panel,&agent_gen_path_type, 0, control_radio_agent_gen );
	new GLUI_RadioButton( radio_agent_gen_path_type, "Closest" );
	new GLUI_RadioButton( radio_agent_gen_path_type, "Random" );
	new GLUI_RadioButton( radio_agent_gen_path_type, "Dedicated" );
	new GLUI_Separator( glui );


	spinner  = new GLUI_Spinner( glui, "MaxGenerationTime:", GLUI_SPINNER_FLOAT, &agent_generator_MaxGenerationTime,  0, property_control_cb );
	spinner->set_float_limits( -1.0, 100000.0 );
	spinner  = new GLUI_Spinner( glui, "MaxNumOfGeneratedAgents:", GLUI_SPINNER_INT, &agent_generator_MaxNumOfGeneratedAgents,  1, property_control_cb );
	spinner->set_int_limits( -1.0, 100000 );
	spinner->set_w(5);
	spinner  = new GLUI_Spinner( glui, "Radius:", GLUI_SPINNER_FLOAT, &agent_generator_Radius,  2, property_control_cb );
	spinner->set_float_limits( 0.01, 1000.0 );
	spinner  = new GLUI_Spinner( glui, "NumAgentPerSecond:", GLUI_SPINNER_FLOAT, &agent_generator_NumAgentPerSecond,  3, property_control_cb );
	spinner->set_float_limits( 0.01, 1000.0 );
	spinner  = new GLUI_Spinner( glui, "AgentSpeedMin:", GLUI_SPINNER_FLOAT, &agent_generator_AgentSpeedMin,  4, property_control_cb );
	spinner->set_float_limits( 0.01, 100.0 );
	spinner  = new GLUI_Spinner( glui, "AgentSpeedMax:", GLUI_SPINNER_FLOAT, &agent_generator_AgentSpeedMax,  5, property_control_cb );
	spinner->set_float_limits( 0.01, 100.0 );
	//
	spinner  = new GLUI_Spinner( glui, "AgentRadiusMin:", GLUI_SPINNER_FLOAT, &agent_generator_AgentRadiusMin,  6, property_control_cb );
	spinner->set_float_limits( 0.01, 10.0 );
	spinner  = new GLUI_Spinner( glui, "AgentRadiusMax:", GLUI_SPINNER_FLOAT, &agent_generator_AgentRadiusMax,  7, property_control_cb );
	spinner->set_float_limits( 0.01, 10.0 );
	//
	spinner  = new GLUI_Spinner( glui, "PathType:", GLUI_SPINNER_INT, &agent_generator_PathType,  6, property_control_cb );
	spinner->set_int_limits(-2, 65536 );
	new GLUI_Button( glui, "Update", 0, property_control_cb_update_agentGen );

    glui_property_agent_generator->set_main_gfx_window( _mainWinID );

    //control->disable();
}

};

void CROWD_SIMULATION_BASIC::update_glui( )
{
	update_glui_property_agent_generator( );
	update_glui_property_environment( );
	update_glui_property_path_obj( );


}

void CROWD_SIMULATION_BASIC::update_glui_property_agent_generator( )
{
	bool flgNew;
	CS_AGENT_GENERATOR *obj = _currentPickedAgentGenerator = AGENT_GENERATOR_MANAGER::instance( )->getCurPickedObj( flgNew );
	if ( flgNew == false ) return;

vector3 p;
agent_generator_objID = obj->getObjID( );
	 agent_generator_Radius = obj->getRadius( );
	 obj->getAgentSpeedRange( agent_generator_AgentSpeedMin, agent_generator_AgentSpeedMax );
	 
	 obj->getAgentRadiusRange( agent_generator_AgentRadiusMin, agent_generator_AgentRadiusMax );

	agent_generator_PathType = obj->getPathType( );
	agent_generator_NumAgentPerSecond = obj->getNumAgentPerSecond( );
	agent_generator_MaxGenerationTime = obj->getMaxGenerationTime( );
	agent_generator_MaxNumOfGeneratedAgents = obj->getMaxNumOfGeneratedAgents( );
	glui_property_agent_generator->sync_live( );

}

void CROWD_SIMULATION_BASIC::createGLUI( int mainWinID, void (global_idle_func()), void *glui_ptr )
{
	_gui_main_obj = this;
	_mainWinID = mainWinID;
	//
	cout << "BGN CROWD_SIMULATION_BASIC::createGLUI" << endl;

	GLUI *glui = (GLUI*)glui_ptr;

	//GLUI_SPINNER_INT

	//spinner  = new GLUI_Spinner( glui, "Time step:", &time_step, 2, control_cb );
	spinner  = new GLUI_Spinner( glui, "#Iteration per step:", GLUI_SPINNER_INT, &num_iteration_per_step,  0, control_cb );
	spinner->set_int_limits( 1, 100 );
	spinner  = new GLUI_Spinner( glui, "Time step:", GLUI_SPINNER_FLOAT, &time_step,  2, control_cb );
	spinner->set_float_limits( 0.0001, 0.1 );
	//spinner->set_int_limits( 3, 60 );

	//edittext = new GLUI_EditText( glui, "Text:", text, 3, control_cb );

	GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Create Object Type" );
	radio_obj_type = new GLUI_RadioGroup( obj_panel,&obj_type, 4, control_radio_createObjs );
	new GLUI_RadioButton( radio_obj_type, "Agent generator" );
	new GLUI_RadioButton( radio_obj_type, "Environment object" );
	new GLUI_RadioButton( radio_obj_type, "Path" );
	new GLUI_Button( glui, "Simulation", 0, (GLUI_Update_CB)_simulation );
	new GLUI_Button( glui, "Create/Finish Object", 0, (GLUI_Update_CB)_addObj );
	new GLUI_Button( glui, "Edit Object", 0, (GLUI_Update_CB)_editObj );
	new GLUI_Button( glui, "Refresh Simulation", 0, (GLUI_Update_CB)_refreshSimulation );
	new GLUI_Button( glui, "Save", 0, (GLUI_Update_CB)_saveFile );
	new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

	glui->set_main_gfx_window( mainWinID );

	//control_property_agent_generator( );

	glutIdleFunc( NULL );
	GLUI_Master.set_glutIdleFunc( global_idle_func );

	//createGLUI_EnvironmentObject( mainWinID, global_idle_func, 0 );
	//createGLUI_PathObject( mainWinID, global_idle_func, 0 );

	cout << "END CROWD_SIMULATION_BASIC::createGLUI" << endl;
}

/*
//GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 ); // name, flags,
//															//x, and y
new GLUI_StaticText( glui, "Crowd Simulation Basic" );
new GLUI_Separator( glui );
checkbox = new GLUI_Checkbox( glui, "Wireframe", &wireframe, 1, control_cb );
spinner  = new GLUI_Spinner( glui, "Segments:", &segments, 2, control_cb );
spinner->set_int_limits( 3, 60 );
edittext = new GLUI_EditText( glui, "Text:", text, 3, control_cb );
GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Object Type" );
radio = new GLUI_RadioGroup( obj_panel,&obj,4,control_cb );
new GLUI_RadioButton( radio, "Sphere" );
new GLUI_RadioButton( radio, "Torus" );
new GLUI_RadioButton( radio, "Teapot" );
new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

glui->set_main_gfx_window( mainWinID );

//We register the idle callback with GLUI, *not* with GLUT
//GLUI_Master.set_glutIdleFunc( myGlutIdle );

glutIdleFunc( NULL );
GLUI_Master.set_glutIdleFunc( global_idle_func );

//glutIdleFunc( gb_idle );

system("pause");
*/