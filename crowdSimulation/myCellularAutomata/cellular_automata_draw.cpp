#include"cellular_automata.h"
#include"../../openGLbasic/ogl_basic_tools.h"
#include<algorithm>
#define SCALER 10.0f
#define OFFSET 5.0f

void CS_CELLULAR_AUTOMATA::draw_segmentsBetweenCellAndExits(){

	if (isValid(segment.x, segment.z))
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		/*for (auto &e : exits){
		glVertex3f((segment.x - model->size / 2) * 10.0f, 0.0f, (segment.z - model->size / 2) * 10.0f);
		glVertex3f((e.first - model->size / 2) * 10.0f, 0.0f, (e.second - model->size / 2) * 10.0f);
		}*/
		for (unsigned int i = 0; i < exits.size(); i++)
		{
			//if (cell[segment.x][segment.z].intersect_obstacle_segment[i])
			//	glColor3f(1, 0, 0);
			//if (!cell[segment.x][segment.z].intersect_obstacle_segment[i])
			//	glColor3f(0, 0, 0);
			glVertex3f((segment.x - model->size / 2) * SCALER, 0.0f, (segment.z - model->size / 2) * SCALER);
			glVertex3f((exits[i].first - model->size / 2) * SCALER, 0.0f, (exits[i].second - model->size / 2) * SCALER);
		}
		glEnd();
	}
}

void CS_CELLULAR_AUTOMATA::draw_agent(){

	int agentNumber = model->agent_number;
	int size = model->size;
	glColor3f(0, 0, 0);
	for (int i = 0; i < agentNumber; i++){
		if (agent[i].position.first == -1)
			continue;
		glColor3f(0, 0, 0);
		if (agent[i].click)
			glColor3f(0, 1, 0);
		//if (agent[i].cur_click)
		//glColor3f(1, 0, 0);
		//ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
	//draw_agent_face_direction();
	// draw obstacles
	draw_obstacle();
	for (int i = 0; i < mObserverAgent.size(); i++)
	{
		int id = mObserverAgent[i];
		glColor3f(1, 0, 0);
		ogl_drawSimpleSolidCircleXZ((agent[id].position.first - size / 2) * SCALER, 0.0f, (agent[id].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_group(){

	int size = model->size;
	int group_num = agent_group.size();
	// draw obstacles
	draw_obstacle();
	// draw agent group
	glLineWidth(1.0f);
	for (int i = 0; i < group_num; i++){
		vector<int> group_member = agent_group[i].member;
		const int group_size = group_member.size();
		/*
		sign the leader waiting for members to blue
		*/
		switch (group_size){
		case 1:
			glColor3f(0, 0, 0);
			if (agent[group_member[0]].position.first == -1)
				continue;
			ogl_drawSimpleSolidCircleXZ((agent[group_member[0]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[0]].position.second - size / 2) * SCALER, 5, 30);
			break;
		case 2:
			for (int num = 0; num < group_size; num++){
				if (agent[group_member[num]].position.first == -1)
					continue;
				if (agent[group_member[num]].waiting){
					glColor3f(0, 1, 0);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
				else if (agent[group_member[num]].leader){
					glColor3f(1, 0, 1);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
				else{
					glColor3f(1, 0, 0);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
			}
			break;
		case 3:
			for (int num = 0; num < group_size; num++){
				if (agent[group_member[num]].position.first == -1)
					continue;
				if (agent[group_member[num]].waiting){
					glColor3f(0, 1, 0);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
				else if (agent[group_member[num]].leader){
					glColor3f(1, 0, 1);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
				else{
					glColor3f(0, 0, 1);
					ogl_drawSimpleSolidCircleXZ((agent[group_member[num]].position.first - size / 2) * SCALER, 0.0f, (agent[group_member[num]].position.second - size / 2) * SCALER, 5, 30);
				}
			}
			break;
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_specific_group(){

	draw_obstacle();
	show_specific_group();
}

void CS_CELLULAR_AUTOMATA::draw_lattice(){

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	int size = model->size;
	for (int i = 0; i <= size; i++){
		glVertex3f((i - size / 2) * SCALER - OFFSET, 0.0f, -size / 2 * SCALER - OFFSET);
		glVertex3f((i - size / 2) * SCALER - OFFSET, 0.0f, size / 2 * SCALER - OFFSET);
	}
	for (int i = 0; i <= size; i++){
		glVertex3f(-size / 2 * SCALER - OFFSET, 0.0f, (i - size / 2) * SCALER - OFFSET);
		glVertex3f(size / 2 * SCALER - OFFSET, 0.0f, (i - size / 2) * SCALER - OFFSET);
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);;
	for (int i = -1; i < model->size + 1; i++)
	{
		for (int j = -1; j < model->size + 1; j++)
		{
			if (i == -1 || j == -1 || i == model->size || j == model->size)
				ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	/*for (int i = 0; i < exits.size(); i++){
		if (exits[i].first == 0)
			ogl_drawFilledRectXZ((exits[i].first - 1 - size / 2) * SCALER - OFFSET, (exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].second == 0)
			ogl_drawFilledRectXZ((exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].second - 1 - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].second == size - 1)
			ogl_drawFilledRectXZ((exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, (exits[i].second + 2 - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].first == size - 1)
			ogl_drawFilledRectXZ((exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].first + 2 - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, 1);
	}*/
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			if (guiParameter.drawExitColor)
				pl_glColor3f(mExitColor[i]);
			int ex = x + dx*j;
			int ez = z + dz*j;
			if (ex == 0)
				ogl_drawFilledRectXZ((ex - 1 - size / 2) * SCALER - OFFSET, (ex - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, 1);
			else if (ez == 0)
				ogl_drawFilledRectXZ((ex - size / 2) * SCALER - OFFSET, (ex + 1 - size / 2) * SCALER - OFFSET, (ez - 1 - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, 1);
			else if (ez == size - 1)
				ogl_drawFilledRectXZ((ex - size / 2) * SCALER - OFFSET, (ex + 1 - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, (ez + 2 - size / 2) * SCALER - OFFSET, 1);
			else if (ex == size - 1)
				ogl_drawFilledRectXZ((ex + 1 - size / 2) * SCALER - OFFSET, (ex + 2 - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
	// draw obstacles
	draw_obstacle();
}

void CS_CELLULAR_AUTOMATA::draw_border(){

	int size = model->size;
	glColor3f(0.0f, 0.0f, 0.0f);;
	for (int i = -1; i < model->size + 1; i++)
	{
		for (int j = -1; j < model->size + 1; j++)
		{
			if (i == -1 || j == -1 || i == model->size || j == model->size)
				ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	/*for (int i = 0; i < exits.size(); i++)
	{
		if (exits[i].first == 0)
			ogl_drawFilledRectXZ((exits[i].first - 1 - size / 2) * SCALER - OFFSET, (exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].second == 0)
			ogl_drawFilledRectXZ((exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].second - 1 - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].second == size - 1)
			ogl_drawFilledRectXZ((exits[i].first - size / 2) * SCALER - OFFSET, (exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, (exits[i].second + 2 - size / 2) * SCALER - OFFSET, 1);
		else if (exits[i].first == size - 1)
			ogl_drawFilledRectXZ((exits[i].first + 1 - size / 2) * SCALER - OFFSET, (exits[i].first + 2 - size / 2) * SCALER - OFFSET, (exits[i].second - size / 2) * SCALER - OFFSET, (exits[i].second + 1 - size / 2) * SCALER - OFFSET, 1);
	}*/
	for (unsigned int i = 0; i < mExit.size(); i++)
	{
		int x = mExit[i].position.first;
		int z = mExit[i].position.second;
		int dx = mExit[i].direction.first;
		int dz = mExit[i].direction.second;
		for (int j = 0; j < mExit[i].width; j++)
		{
			if (guiParameter.drawExitColor)
				pl_glColor3f(mExitColor[i]);
			int ex = x + dx*j;
			int ez = z + dz*j;
			if (ex == 0)
				ogl_drawFilledRectXZ((ex - 1 - size / 2) * SCALER - OFFSET, (ex - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, 1);
			else if (ez == 0)
				ogl_drawFilledRectXZ((ex - size / 2) * SCALER - OFFSET, (ex + 1 - size / 2) * SCALER - OFFSET, (ez - 1 - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, 1);
			else if (ez == size - 1)
				ogl_drawFilledRectXZ((ex - size / 2) * SCALER - OFFSET, (ex + 1 - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, (ez + 2 - size / 2) * SCALER - OFFSET, 1);
			else if (ex == size - 1)
				ogl_drawFilledRectXZ((ex + 1 - size / 2) * SCALER - OFFSET, (ex + 2 - size / 2) * SCALER - OFFSET, (ez - size / 2) * SCALER - OFFSET, (ez + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
	// draw obstacles
	//draw_obstacle();
}

void CS_CELLULAR_AUTOMATA::draw_colorTable(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 10.0f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0.0, 1.0));
	//travelLength.addEntry(Rv*1.0, vector3(0.0, 0.33, 1.0));
	//travelLength.addEntry(Rv*2.0, vector3(0.0, 0.66, 1.0));
	travelLength.addEntry(Rv*(mColorProportion[0]) * 10, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*4.0, vector3(0.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*5.0, vector3(0.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*(mColorProportion[0] + mColorProportion[1] + mColorProportion[2]) * 10, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*12.0, vector3(1.0, 0.0, 0.0));
	//travelLength.addEntry(Rv*12.0, vector3(0.5, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(cell[i][j].sFF);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_dFF_colorTable(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0.0, 1.0));
	//travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.5, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	//travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	//travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.5, vector3(1.0, 0.75, 0.0));
	//travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	//travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(cell[i][j].dFF);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_volunteer_colorTable(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.0, vector3(0.0, 1.0, 1.0));
	travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(volunteer_distance_map[model->colorTable_ID][i][j].sFF_color);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_special_colorTable(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.0, vector3(0.0, 1.0, 1.0));
	travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));
	//travelLength.addEntry(Rv*11.0, vector3(1.0, 0, 1.0));
	//travelLength.addEntry(Rv*12.0, vector3(1.0, 0, 0.75));
	//travelLength.addEntry(Rv*13.0, vector3(1.0, 0, 0.5));
	//travelLength.addEntry(Rv*14.0, vector3(1.0, 0, 0.25));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			//vector3 c = travelLength.getColor_Linear((guiParameter.specailColorTable_normalizeValue - cell[i][j].special_sFF[guiParameter.colorTable_special_ID]) / guiParameter.specailColorTable_normalizeValue);
			vector3 c = travelLength.getColor_Linear((cell_manager.max_sFF - cell[i][j].special_sFF[guiParameter.colorTable_special_ID]) / cell_manager.max_sFF);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_colorTable_withoutExit(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.0, vector3(0.0, 1.0, 1.0));
	travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(cell[i][j].specific_color_sFF[guiParameter.colorTable_withoutExit_ID]);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_AFF(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 1.0f;
	travelLength.addEntry(Rv*0.0, vector3(1.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*5.0, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	//travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(0.5, 1.0, 0.0));
	//travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*7.5, vector3(1.0, 0.75, 0.0));
	//travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	//travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*13.0, vector3(1.0, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(cell[i][j].aFF);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_totalFloorField(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 10.0f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0.0, 1.0));
	//travelLength.addEntry(Rv*1.0, vector3(0.0, 0.33, 1.0));
	//travelLength.addEntry(Rv*2.0, vector3(0.0, 0.66, 1.0));
	travelLength.addEntry(Rv*(mColorProportion[0]) * 10, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*4.0, vector3(0.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*5.0, vector3(0.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*(mColorProportion[0] + mColorProportion[1] + mColorProportion[2]) * 10, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));
	//travelLength.addEntry(Rv*12.0, vector3(0.5, 0.0, 0.0));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector3 c = travelLength.getColor_Linear(cell[i][j].aFF + cell[i][j].sFF);
			if (cell[i][j].obstacle)
				glColor3f(0.0, 0.0, 0.0);
			if (!cell[i][j].obstacle)
				pl_glColor3f(c);
			ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_AgentEvacuationTime(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.0, vector3(0.0, 1.0, 1.0));
	travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));
	for (int i = 0; i < model->agent_number; i++)
	{
		vector3 c = travelLength.getColor_Linear(agent[i].escapeTime / (model->maxEvacuationTime + 0.000001f));
		glColor3f(c.x, c.y, c.z);
		//glColor4f(0.0f, 0.0f, 1.0f, agent[i].anxiety);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_AgentEscapeExit(){

	int agentNumber = model->agent_number;
	int size = model->size;
	glColor3f(0, 0, 0);
	glLineWidth(1.0f);
	for (int i = 0; i < agentNumber; i++){
		if (agent[i].position.first == -1)
			continue;
		pl_glColor3f(mExitColor[agent[i].mEscapeExit]);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
	draw_agent_face_direction();
	// draw obstacles
	draw_obstacle();
}

void CS_CELLULAR_AUTOMATA::draw_agent_anxiety(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	/*float Rv = ANXIETY_THRESHOLD;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0.0, 0.0));
	travelLength.addEntry(Rv*0.1, vector3(0.0, 0.0, 0.1));
	travelLength.addEntry(Rv*0.2, vector3(0.0, 0.0, 0.2));
	travelLength.addEntry(Rv*0.3, vector3(0.0, 0.0, 0.3));
	travelLength.addEntry(Rv*0.4, vector3(0.0, 0.0, 0.4));
	travelLength.addEntry(Rv*0.5, vector3(0.0, 0.0, 0.5));
	travelLength.addEntry(Rv*0.6, vector3(0.0, 0.0, 0.6));
	travelLength.addEntry(Rv*0.7, vector3(0.0, 0.0, 0.7));
	travelLength.addEntry(Rv*0.8, vector3(0.0, 0.0, 0.8));
	travelLength.addEntry(Rv*0.9, vector3(0.0, 0.0, 0.9));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.0, 1.0));*/
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	//travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*0.33, vector3(0.0, 1.0, 1.0));
	//travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	//travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	//travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*0.67, vector3(1.0, 1.0, 0.0));
	//travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	//travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	//travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));
	for (int i = 0; i < model->agent_number; i++)
	{
		vector3 c = travelLength.getColor_Linear(agent[i].anxiety);
		glColor3f(c.x, c.y, c.z);
		//glColor4f(0.0f, 0.0f, 1.0f, agent[i].anxiety);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		if (agent[i].compressive_leader)
		{
			glLineWidth(2.0f);
			glColor3f(0.69f, 0.17f, 1.0f);
			ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_agent_willness(){

	int size = model->size;
	COLOR_TABLE travelLength;
	travelLength.enableAutoScale(true);
	float Rv = 0.1f;
	travelLength.addEntry(Rv*0.0, vector3(0.0, 0, 1.0));
	travelLength.addEntry(Rv*1.0, vector3(0.0, 0.5, 1.0));
	travelLength.addEntry(Rv*2.0, vector3(0.0, 1.0, 1.0));
	travelLength.addEntry(Rv*3.0, vector3(0.0, 1.0, 0.5));
	travelLength.addEntry(Rv*4.0, vector3(0.0, 0.85, 0.0));
	travelLength.addEntry(Rv*5.0, vector3(0.5, 1.0, 0.0));
	travelLength.addEntry(Rv*6.0, vector3(1.0, 1.0, 0.0));
	travelLength.addEntry(Rv*7.0, vector3(1.0, 0.75, 0.0));
	travelLength.addEntry(Rv*8.0, vector3(1.0, 0.5, 0.0));
	travelLength.addEntry(Rv*9.0, vector3(1.0, 0.25, 0.0));
	travelLength.addEntry(Rv*10.0, vector3(1.0, 0.0, 0.0));
	for (int i = 0; i < (int)model->agent_number; i++)
	{
		vector3 c = travelLength.getColor_Linear(agent[i].psychology.willness);
		glColor3f(c.x, c.y, c.z);
		//glColor4f(0.0f, 0.0f, 1.0f, agent[i].anxiety);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		if (agent[i].compressive_leader)
		{
			glLineWidth(1.5f);
			glColor3f(0.69f, 0.17f, 1.0f);
			ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_obstacle(){

	int size = model->size;

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (cell[i][j].obstacle)
			{
				glColor3f(0.5f, 0.5f, 0.5f);
				ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, -0.1f);
			}
			if (cell[i][j].occupied == 1)
			{
				//glColor3f(0.0f, 0.5f, 0.0f);
				//ogl_drawFilledRectXZ((i - size / 2) * SCALER - OFFSET, (i + 1 - size / 2) * SCALER - OFFSET, (j - size / 2) * SCALER - OFFSET, (j + 1 - size / 2) * SCALER - OFFSET, -0.1f);
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_currentExitBlockAgent(){

	int agentNumber = model->agent_number;
	int size = model->size;
	glColor3f(0, 0, 0);
	glLineWidth(1.0f);
	for (int i = 0; i < agentNumber; i++){
		if (agent[i].position.first == -1)
			continue;
		if (agent[i].currentVisibleBlockedExit == -1)
		{
			glColor3f(0, 0, 0);
			ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		}
		else
		{
			pl_glColor3f(mExitColor[agent[i].currentVisibleBlockedExit]);
			ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		}
	}
	draw_agent_face_direction();
	// draw obstacles
	draw_obstacle();
}

void CS_CELLULAR_AUTOMATA::show_node_intersectObstacle(){

	for (int i = 0; i < model->size; i++)
	{
		for (int j = 0; j < model->size; j++)
		{
			if (cell[i][j].intersect_obstacle)
				glColor3f(1, 0, 0);
			if (!cell[i][j].intersect_obstacle)
				glColor3f(0, 0, 1);
			ogl_drawFilledRectXZ((i - model->size / 2) * SCALER - OFFSET, (i + 1 - model->size / 2) * SCALER - OFFSET, (j - model->size / 2) * SCALER - OFFSET, (j + 1 - model->size / 2) * SCALER - OFFSET, 1);
		}
	}
}

void CS_CELLULAR_AUTOMATA::show_specific_group(){
	/*
	draw other agent
	*/
	int size = model->size;
	glLineWidth(1.0f);
	for (int i = 0; i < (int)agent_group.size(); i++)
	{
		glColor3f(0, 0, 0);
		for (int j = 0; j < (int)agent_group[i].member.size(); j++)
		{
			if (agent[agent_group[i].member[j]].position.first == -1)
				continue;
			ogl_drawSimpleCircleXZ((agent[agent_group[i].member[j]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[i].member[j]].position.second - size / 2) * SCALER, 5, 30);
		}
	}
	/*
	draw specific group's member
	ID: group_ID
	*/
	if (agent_group[model->group_ID].member.size() == 1)
		glColor3f(0, 0, 1);
	if (agent_group[model->group_ID].member.size() == 2)
		glColor3f(1, 0, 0);
	if (agent_group[model->group_ID].member.size() == 3)
		glColor3f(0, 1, 0);
	/*
	draw leader to pink
	*/
	glColor3f(1, 0, 1);
	if (agent[agent_group[model->group_ID].member[0]].position.first != -1)
		ogl_drawSimpleSolidCircleXZ((agent[agent_group[model->group_ID].member[0]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[model->group_ID].member[0]].position.second - size / 2) * SCALER, 5, 30);
	/*
	draw othe member to black
	*/
	for (int i = 0; i < (int)agent_group[model->group_ID].member.size(); i++)
	{
		if (agent[agent_group[model->group_ID].member[i]].position.first == -1)
			continue;
		/*
		draw leader to pink
		*/
		glColor3f(0, 0, 0);
		/*
		draw leader to pink
		*/
		if (agent[agent_group[model->group_ID].member[i]].leader)
			glColor3f(1, 0, 1);
		ogl_drawSimpleSolidCircleXZ((agent[agent_group[model->group_ID].member[i]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[model->group_ID].member[i]].position.second - size / 2) * SCALER, 5, 30);
	}
}

void CS_CELLULAR_AUTOMATA::draw_group_circle(){

	int size = model->size;

	for (int i = 0; i < (int)agent_group[agent[model->select_agent_ID].group_id].member.size(); i++)
	{
		if (agent[agent_group[agent[model->select_agent_ID].group_id].member[i]].position.first == -1)
			continue;
		if (agent[agent_group[agent[model->select_agent_ID].group_id].member[i]].leader)
		{
			glColor3f(1.0f, 1.0f, 0);
			ogl_drawSimpleSolidCircleXZ(float(agent[agent_group[agent[model->select_agent_ID].group_id].member[i]].position.first - size / 2) * SCALER, 0.1f, float(agent[agent_group[agent[model->select_agent_ID].group_id].member[i]].position.second - size / 2) * SCALER, 5 * model->group_radius * 2, 100);
			break;
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_guider(){

	int size = model->size;

	for (int i = 0; i < (int)model->guider_ID.size(); i++)
	{
		if (agent[model->guider_ID[i]].position.first == -1)
			continue;
		glColor3f(0.5f, 0.0f, 0);
		ogl_drawSimpleSolidCircleXZ(float(agent[model->guider_ID[i]].position.first - size / 2) * SCALER, 0.1f, float(agent[model->guider_ID[i]].position.second - size / 2) * SCALER, 5 * 2 * agent[model->guider_ID[i]].height / 10, 100);
		glColor3f(1.0f, 0.0f, 0);
		ogl_drawSimpleSolidCircleXZ(float(agent[model->guider_ID[i]].position.first - size / 2) * SCALER, 0.0f, float(agent[model->guider_ID[i]].position.second - size / 2) * SCALER, 5, 100);
	}
	/*for (int i = 0; i < model->agent_number; i++)
	{
	if (agent[i].position.first == -1)
	continue;
	if (agent[i].guider)
	ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5 * model->group_radius * 2, 100);
	}*/
}

void CS_CELLULAR_AUTOMATA::draw_agent_path(){

	int size = model->size;
	if (agent[model->select_agent_ID].path.size() == 0)
		return;
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	for (int i = 0; i < (int)agent[model->select_agent_ID].path.size() - 1; i++)
	{
		glVertex3f((agent[model->select_agent_ID].path[i].first - size / 2) * SCALER, 0.0f, (agent[model->select_agent_ID].path[i].second - size / 2) * SCALER);
		glVertex3f((agent[model->select_agent_ID].path[i + 1].first - size / 2) * SCALER, 0.0f, (agent[model->select_agent_ID].path[i + 1].second - size / 2) * SCALER);
	}

	glEnd();
}

void CS_CELLULAR_AUTOMATA::draw_clicked_agent(){

	int agentNumber = model->agent_number;
	int size = model->size;
	for (int i = 0; i < agentNumber; i++)
	{
		if (!agent[i].click)
			continue;
		if (agent[i].click)
		{
			glLineWidth(2.0f);
			//glColor3f(0, 1, 0);
			int group_size = agent_group[agent[i].group_id].member.size();
			glBegin(GL_LINES);
			for (int j = 0; j < group_size; j++)
			{
				switch (group_size)
				{
				case 1:
					glColor3f(0, 1, 0);
					break;
				case 2:
					glColor3f(1, 0, 1);
					break;
				case 3:
					glColor3f(0, 0, 1);
					break;
				}
				glVertex3f((agent[agent_group[agent[i].group_id].member[0]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[agent[i].group_id].member[0]].position.second - size / 2) * SCALER);
				glVertex3f((agent[agent_group[agent[i].group_id].member[j]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[agent[i].group_id].member[j]].position.second - size / 2) * SCALER);
				//ogl_drawSimpleCircleXZ((agent[agent_group[agent[i].group_id].member[j]].position.first - size / 2) * SCALER, 0.0f, (agent[agent_group[agent[i].group_id].member[j]].position.second - size / 2) * SCALER, 5, 100);
			}
			glEnd();
			for (int j = 0; j < group_size; j++)
			{
				//leader
				if (j == 0)
					ogl_drawSimpleSolidCircleXZ((agent[agent_group[agent[i].group_id].member[j]].position.first - size / 2) * SCALER, 0.1f, (agent[agent_group[agent[i].group_id].member[j]].position.second - size / 2) * SCALER, 5, 100);
				else
					ogl_drawSimpleCircleXZ((agent[agent_group[agent[i].group_id].member[j]].position.first - size / 2) * SCALER, 0.1f, (agent[agent_group[agent[i].group_id].member[j]].position.second - size / 2) * SCALER, 5, 100);
			}
		}
		//ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		switch (guiParameter.visibleState)
		{
		case person:
			draw_visible_agent(i);
			break;
		case area:
			draw_visible_area(i);
			break;
		}
		//draw_visible_agent(i);
		//draw_visible_area(i);
	}
}

void CS_CELLULAR_AUTOMATA::draw_clicked_cell(){

	int size = model->size;
	glLineWidth(1.5f);
	//cout << screen.cur_x << " " << screen.cur_z << endl;
	if (!isValid(screen.cur_x, screen.cur_z))
		return;
	if (cell[screen.cur_x][screen.cur_z].obstacle)
		glColor3f(0.0f, 1.0f, 0.0f);
	if (!cell[screen.cur_x][screen.cur_z].obstacle)
		glColor3f(0.0f, 0.0f, 0.0f);
	if (cell[screen.cur_x][screen.cur_z].click && !model->start)
	{
		int o_id = cell[screen.cur_x][screen.cur_z].obstacle_id;
		if (o_id == -1)
		{
			ogl_drawUnFilledRectXZ((screen.cur_x - size / 2) * SCALER - OFFSET, (screen.cur_x + 1 - size / 2) * SCALER - OFFSET, (screen.cur_z - size / 2) * SCALER - OFFSET, (screen.cur_z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
			return;
		}
		for (int i = 0; i < (int)obstacles[o_id].component.size(); i++)
		{
			if (screen.cur_x == obstacles[o_id].component[i].first && screen.cur_z == obstacles[o_id].component[i].second)
				obstacles[o_id].clicked_o_id = i;
			int x = obstacles[o_id].component[i].first;
			int z = obstacles[o_id].component[i].second;
			ogl_drawUnFilledRectXZ((x - size / 2) * SCALER - OFFSET, (x + 1 - size / 2) * SCALER - OFFSET, (z - size / 2) * SCALER - OFFSET, (z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_moved_obstacle(){

	int size = model->size;
	glLineWidth(1.5f);
	switch (agent_psychology.obstacle_moveType)
	{
	case CS_OBSTACLE_MANUAL_MOVE:
		draw_moved_obstacle_manual();
		break;
	case CS_OBSTACLE_AUTOMATIC_MOVE:
		draw_moved_obstacle_automatic();
		break;
	}
	/*for (int i = 0; i < removed_obstacles.size(); i++)
	{
	int x = removed_obstacles[i].position.first;
	int z = removed_obstacles[i].position.second;
	//cout << x << " " << z << endl;
	if (removed_obstacles[i].arrival_destination)
	continue;
	glColor3f(1.0f, 0.0f, 0.0f);
	ogl_drawFilledRectXZ((x - size / 2) * SCALER - OFFSET, (x + 1 - size / 2) * SCALER - OFFSET, (z - size / 2) * SCALER - OFFSET, (z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
	glColor3f(1.0f, 0.0f, 0.0f);
	int des_x = removed_obstacles[i].destination.first;
	int des_z = removed_obstacles[i].destination.second;
	ogl_drawUnFilledRectXZ((des_x - size / 2) * SCALER - OFFSET, (des_x + 1 - size / 2) * SCALER - OFFSET, (des_z - size / 2) * SCALER - OFFSET, (des_z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
	}*/
}

void CS_CELLULAR_AUTOMATA::draw_moved_obstacle_automatic(){

	return;
	int size = model->size;
	glLineWidth(1.5f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < (int)blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		if (!obstacles[o_id].enough_volunteers)
			continue;
		if (!check_volunteer_setup(o_id))
			continue;
		for (int j = 0; j < (int)obstacles[o_id].component.size(); j++)
		{
			int x = obstacles[o_id].component[j].first;
			int z = obstacles[o_id].component[j].second;
			ogl_drawFilledRectXZ((x - size / 2) * SCALER - OFFSET, (x + 1 - size / 2) * SCALER - OFFSET, (z - size / 2) * SCALER - OFFSET, (z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_moved_obstacle_manual(){

	int size = model->size;
	glLineWidth(1.5f);
	for (int i = 0; i < (int)blocked_obstacles.size(); i++)
	{
		int o_id = blocked_obstacles[i];
		//cout << "id:" << o_id << endl;
		if (obstacles[o_id].arrive_destination)
			continue;
		PAIR_INT des = obstacles[o_id].volunteer_destination;
		int offset_x = obstacles[o_id].component[obstacles[o_id].final_clicked_o_id].first - des.first;
		int offset_z = obstacles[o_id].component[obstacles[o_id].final_clicked_o_id].second - des.second;
		//cout << "size:" << obstacles[o_id].component.size() << endl;
		for (int j = 0; j < (int)obstacles[o_id].component.size(); j++)
		{
			int x = obstacles[o_id].component[j].first;
			int z = obstacles[o_id].component[j].second;
			//cout << x << " " << z << endl;
			glColor3f(0.5f, 0.5f, 0.5f);
			ogl_drawFilledRectXZ((x - size / 2) * SCALER - OFFSET, (x + 1 - size / 2) * SCALER - OFFSET, (z - size / 2) * SCALER - OFFSET, (z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
			if (des.first == -1)
				continue;
			int des_x = x - offset_x;
			int des_z = z - offset_z;
			glColor3f(1.0f, 0.0f, 0.0f);
			ogl_drawUnFilledRectXZ((des_x - size / 2) * SCALER - OFFSET, (des_x + 1 - size / 2) * SCALER - OFFSET, (des_z - size / 2) * SCALER - OFFSET, (des_z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
		}
	}
}

void CS_CELLULAR_AUTOMATA::draw_escape_direction(){

	int size = model->size;

	for (int i = 0; i < model->agent_number; i++)
	{
		if (!agent[i].be_leaded)
			continue;
		int exit_id = agent[i].guide_exit_id;
		vector3 direction = vector3(vector3((exits[exit_id].first - size / 2) * SCALER, -0.1f, (exits[exit_id].second - size / 2) * SCALER) - vector3((agent[i].position.first - size / 2) * SCALER, -0.1f, (agent[i].position.second - size / 2) * SCALER)).Normalized();
		vector3 agent_pos = vector3((agent[i].position.first - size / 2) * SCALER, -0.1f, (agent[i].position.second - size / 2) * SCALER);
		//glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(2.0f);
		ogl_drawSimpleLineSegment(agent_pos.x, agent_pos.y, agent_pos.z, agent_pos.x + direction.x * SCALER, direction.y, agent_pos.z + direction.z * SCALER);
	}
}

void CS_CELLULAR_AUTOMATA::draw_stress_area(){

	int size = model->size;
	//for (int i = 0; i < stress_area_position.size(); i++)
	//{
	//	glColor3f(0, 0.5f, 0);
	//	ogl_drawSimpleSolidCircleXZ((stress_area_position[i].first - size / 2) * SCALER, 0.1f, (stress_area_position[i].second - size / 2) * SCALER, stress_model.stress_radius * 2 * 5, 100);
	//}
	glColor3f(1.0f, 0, 0);
	for (auto e : leader_pool)
	{
		if (agent[e].arrival)
			continue;
		ogl_drawSimpleCircleXZ((agent[e].position.first - size / 2) * SCALER, 0.1f, (agent[e].position.second - size / 2) * SCALER, VISIBILITY_GRID_LENGTH * 2 * 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_volunteer(){

	if (model->start == false)
		return;
	int size = model->size;
	for (int i = 0; i < (int)removed_obstacles.size(); i++)
	{
		int id = removed_obstacles[i].volunteer_id;
		if (agent[id].arrival)
			continue;
		glColor3f(1.0f, 0.0f, 1.0f);
		ogl_drawSimpleSolidCircleXZ((agent[id].position.first - size / 2) * SCALER, 0.0f, (agent[id].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_compressive_leader(){

	int size = model->size;
	//cout << leader_pool.size();
	for (int i = 0; i < (int)leader_pool.size(); i++)
	{
		int id = leader_pool[i];
		glColor3f(1.0f, 0.0f, 0.0f);
		ogl_drawSimpleSolidCircleXZ((agent[id].position.first - size / 2) * SCALER, 0.0f, (agent[id].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_strength_agent(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].psychology.willness < guiParameter.willing_threshold /*WILLNESS_THRESHOLD*/)
			continue;
		//glColor3f(0.0f, 0.0f, 0.0f);
		//if (agent[i].volunteer)
		//	glColor3f(1.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (agent[i].volunteer)
		{
			if (agent[i].beside_obstacle)
				glColor3f(0.5f, 0, 0);
			ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
		}
		if (agent[i].mVolunteered)
			glColor3f(0.0f, 0.5f, 0.0f);
		ogl_drawSimpleCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_selfish_agent(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].personality != selfish && agent[i].personality != littleSelfish)
			continue;
		glColor3f(0.0f, 0.5f, 0.5f);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_neutral_agent(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].personality != neutral)
			continue;
		glColor3f(0.0f, 1.0f, 0.0f);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_brave_agent(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].personality != brave && agent[i].personality != littleBrave)
			continue;
		glColor3f(0.0f, 1.0f, 1.0f);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_idiot_agent(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (!agent[i].blockTheRoad)
			continue;
		glColor3f(1.0f, 0.38f, 0.0f);
		ogl_drawSimpleSolidCircleXZ((agent[i].position.first - size / 2) * SCALER, 0.0f, (agent[i].position.second - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_density_around_obstacle(){

	int size = model->size;
	glLineWidth(1.0f);
	/*for (int i = 0; i < obstacles.size(); i++)
	{
	for (int j = 0; j < obstacles[i].component.size(); j++)
	{
	int com_x = obstacles[i].component[j].first;
	int com_z = obstacles[i].component[j].second;
	for (int x = -VISIBILITY_DISTANCE; x < VISIBILITY_DISTANCE; x++)
	{
	for (int z = -VISIBILITY_DISTANCE; z < VISIBILITY_DISTANCE; z++)
	{
	if (!isValid(com_x + x, com_z + z))
	continue;
	if (cell[com_x + x][com_z + z].obstacle)
	continue;
	if (cell[com_x + x][com_z + z].sFF < cell[com_x][com_z].sFF)
	continue;
	glColor3f(1.0f, 0.0f, 0.0f);
	ogl_drawUnFilledRectXZ((x - model->size / 2) * SCALER - OFFSET, (x + 1 - model->size / 2) * SCALER - OFFSET, (z - model->size / 2) * SCALER - OFFSET, (z + 1 - model->size / 2) * SCALER - OFFSET, -0.11f);
	}
	}
	}
	}*/
	for (int x = 0; x < size; x++)
	{
		for (int z = 0; z < size; z++)
		{
			if (cell[x][z].check)
			{
				if (cell[x][z].occupied == 0)
					continue;
				glColor3f(1.0f, 0.0f, 0.0f);
				ogl_drawUnFilledRectXZ((x - model->size / 2) * SCALER - 4.0f, (x + 1 - model->size / 2) * SCALER - 6.0f, (z - model->size / 2) * SCALER - 4.0f, (z + 1 - model->size / 2) * SCALER - 6.0f, -0.11f);
			}
		}
	}
}

void CS_CELLULAR_AUTOMATA::show_cell_beOccupied(){

	glLineWidth(1.5f);
	for (int x = 0; x < model->size; x++)
	{
		for (int z = 0; z < model->size; z++)
		{
			glColor3f(0.45f, 0.29f, 0.07f);
			if (cell[x][z].occupied == 1)
				ogl_drawUnFilledRectXZ((x - model->size / 2) * SCALER - OFFSET, (x + 1 - model->size / 2) * SCALER - OFFSET, (z - model->size / 2) * SCALER - OFFSET, (z + 1 - model->size / 2) * SCALER - OFFSET, -0.11f);
		}
	}
	draw_obstacle();
}

void CS_CELLULAR_AUTOMATA::draw_agent_face_direction(){

	int size = model->size;
	for (int i = 0; i < model->agent_number; i++)
	{
		if (agent[i].arrival)
			continue;
		agent[i].draw_direction(size);
	}
}

void CS_CELLULAR_AUTOMATA::draw_visible_agent(int id){

	//cout << "draw visible agent." << endl;
	int size = model->size;
	glLineWidth(1.5f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < (int)agent[id].visible_agentID.size(); i++)
	{
		int x = agent[agent[id].visible_agentID[i]].position.first;
		int z = agent[agent[id].visible_agentID[i]].position.second;
		ogl_drawSimpleCircleXZ((x - size / 2) * SCALER, 0.0f, (z - size / 2) * SCALER, 5, 100);
	}
}

void CS_CELLULAR_AUTOMATA::draw_visible_area(int id){

	int size = model->size;
	glLineWidth(1.5f);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < (int)agent[id].visible_area.size(); i++)
	{
		//cout << "i" << endl;
		int x = agent[id].visible_area[i].first;
		int z = agent[id].visible_area[i].second;
		ogl_drawUnFilledRectXZ((x - size / 2) * SCALER - OFFSET, (x + 1 - size / 2) * SCALER - OFFSET, (z - size / 2) * SCALER - OFFSET, (z + 1 - size / 2) * SCALER - OFFSET, -0.11f);
	}
}

