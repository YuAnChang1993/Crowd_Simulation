//
// Instructor: Yu-An Chang
// Email:	andy123654.cs04g@g2.nctu.edu.tw
//			j60508888@gmail.com
//
// Date: 2017/04/08
//
#include "myCrowdSimulation.h"

// 2017/04/08
void MY_CROWD_SIMULATOR::draw_CelluarAutomata() const{ 
	
	mCelluarAutomata->draw_agent();
}

void MY_CROWD_SIMULATOR::draw_group() const{
	
	mCelluarAutomata->draw_group();
}

void MY_CROWD_SIMULATOR::draw_specific_group() const{
	
	mCelluarAutomata->draw_specific_group();
}

void MY_CROWD_SIMULATOR::draw_Lattice() const{
	
	mCelluarAutomata->draw_lattice();
}

void MY_CROWD_SIMULATOR::draw_Border() const{
	
	mCelluarAutomata->draw_border();
}

void MY_CROWD_SIMULATOR::draw_ColorTable() const{

	mCelluarAutomata->draw_colorTable();
}

void MY_CROWD_SIMULATOR::draw_dFF_ColorTable() const{

	mCelluarAutomata->draw_dFF_colorTable();
}

void MY_CROWD_SIMULATOR::draw_cell_exit_segments(int x, int z) const{

	mCelluarAutomata->set_cell_exit_segment(x, z);
}

void MY_CROWD_SIMULATOR::draw_group_circle() const{

	mCelluarAutomata->draw_group_circle();
}

void MY_CROWD_SIMULATOR::draw_guider() const{

	mCelluarAutomata->draw_guider();
}

void MY_CROWD_SIMULATOR::draw_agent_path() const{

	mCelluarAutomata->draw_agent_path();
}

void MY_CROWD_SIMULATOR::draw_escape_direction() const{

	mCelluarAutomata->draw_escape_direction();
}

void MY_CROWD_SIMULATOR::draw_stress_area() const{

	mCelluarAutomata->draw_stress_area();
}

void MY_CROWD_SIMULATOR::draw_compressive_leader() const{

	mCelluarAutomata->draw_compressive_leader();
}

void MY_CROWD_SIMULATOR::draw_strength_agent() const{

	mCelluarAutomata->draw_strength_agent();
}

void MY_CROWD_SIMULATOR::draw_selfish_agent() const{

	mCelluarAutomata->draw_selfish_agent();
}

void MY_CROWD_SIMULATOR::draw_neutral_agent() const{

	mCelluarAutomata->draw_neutral_agent();
}

void MY_CROWD_SIMULATOR::draw_brave_agent() const{

	mCelluarAutomata->draw_brave_agent();
}

void MY_CROWD_SIMULATOR::draw_occupied_cell() const{
	
	mCelluarAutomata->show_cell_beOccupied();
}

void MY_CROWD_SIMULATOR::draw_volunteer_colorTable() const{

	mCelluarAutomata->draw_volunteer_colorTable();
}

void MY_CROWD_SIMULATOR::draw_agent_anxiety() const{

	mCelluarAutomata->draw_agent_anxiety();
}

void MY_CROWD_SIMULATOR::draw_agent_willness() const{

	mCelluarAutomata->draw_agent_willness();
}

void MY_CROWD_SIMULATOR::draw_obstalce_density() const{
	
	mCelluarAutomata->draw_density_around_obstacle();
}

void MY_CROWD_SIMULATOR::draw_colorTable_withoutExit() const{

	mCelluarAutomata->draw_colorTable_withoutExit();
}

void MY_CROWD_SIMULATOR::draw_special_table() const{

	mCelluarAutomata->draw_special_colorTable();
}

void MY_CROWD_SIMULATOR::draw_CurrentExitBlockAgent() const{

	mCelluarAutomata->draw_currentExitBlockAgent();
}

void MY_CROWD_SIMULATOR::draw_AFF() const{

	mCelluarAutomata->draw_AFF();
}

void MY_CROWD_SIMULATOR::draw_TFF() const{

	mCelluarAutomata->draw_totalFloorField();
}

void MY_CROWD_SIMULATOR::draw_AgentEvacuationTime() const{
	
	mCelluarAutomata->draw_AgentEvacuationTime();
}

void MY_CROWD_SIMULATOR::draw_AgentEscapeExit() const{

	mCelluarAutomata->draw_AgentEscapeExit();
}

void MY_CROWD_SIMULATOR::draw() const{

	mCelluarAutomata->draw_clicked_agent();
	mCelluarAutomata->draw_clicked_cell();
	mCelluarAutomata->draw_volunteer();
	mCelluarAutomata->draw_moved_obstacle();
	mCelluarAutomata->draw_idiot_agent();
}

void MY_CROWD_SIMULATOR::sendFlgExitColor(bool flg) const{

	mCelluarAutomata->setFlgExitColor(flg);
}

void MY_CROWD_SIMULATOR::show_node_intersectObstacle() const{
	
	mCelluarAutomata->show_node_intersectObstacle();
}

void MY_CROWD_SIMULATOR::show_cell_exit_segments() const{

	mCelluarAutomata->draw_segmentsBetweenCellAndExits();
}

void MY_CROWD_SIMULATOR::set_specific_group(int ID) const{

	mCelluarAutomata->set_specific_group(ID);
}

void MY_CROWD_SIMULATOR::set_numOfgroup_size(int size2_num, int size3_num) const{

	mCelluarAutomata->set_numOfgroup_size(size2_num, size3_num);
}

void MY_CROWD_SIMULATOR::set_group_radius(int group_radius) const{

	mCelluarAutomata->set_group_radius(group_radius);
}

void MY_CROWD_SIMULATOR::set_guider(int guider_height) const{

	mCelluarAutomata->set_guider(guider_height);
}

void MY_CROWD_SIMULATOR::editScene() const{

	mCelluarAutomata->detectEditingMode();
}

void MY_CROWD_SIMULATOR::edit_obstacle(){

	mCelluarAutomata->edit_obstacle();
}

void MY_CROWD_SIMULATOR::enabled_edit_obstacle(){

	mCelluarAutomata->enabled_edit_obstacle();
}

void MY_CROWD_SIMULATOR::enabled_edit_exit(){

	mCelluarAutomata->enabled_edit_exit();
}

void MY_CROWD_SIMULATOR::update_scene(){

	mCelluarAutomata->updateScene();
}

void MY_CROWD_SIMULATOR::load_simulation(){
	
	mCelluarAutomata->load_simulation();
}

void MY_CROWD_SIMULATOR::set_ColorTable_ID(int ID){

	mCelluarAutomata->set_ColorTable_ID(ID);
}

void MY_CROWD_SIMULATOR::set_ColorTable_WithoutExit_ID(int ID){

	mCelluarAutomata->set_ColorTable_witoutExit_ID(ID);
}

void MY_CROWD_SIMULATOR::set_WillingThreshold(float willing){

	mCelluarAutomata->set_WillingThreshold(willing);
}

void MY_CROWD_SIMULATOR::set_SpecialID(int ID){

	mCelluarAutomata->set_SpecialID(ID);
}

void MY_CROWD_SIMULATOR::set_colorTableNormalizeValue(float normalizeValue){

	mCelluarAutomata->set_colorTableNormalizeValue(normalizeValue);
}