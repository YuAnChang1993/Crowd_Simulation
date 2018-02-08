//
// Instructor: Yu-An Chang
// Email:	andy123654.cs04g@g2.nctu.edu.tw
//			j60508888@gmail.com
//
// Date: 2017/3/30
//

#ifndef _CS_CELLULAR_AUTOMATA_H_
#define _CS_CELLULAR_AUTOMATA_H_
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "../headers.h"
#include "../openGLbasic/opengl_stuff.h"
#include "../primitives/vector3d.h"
#include "../myDijkstra/myDijkstra.h"

#define GROUP_RADIUS 10.0f
#define ADDITION_FORCE 2.0f
#define M_PI 3.14159265358979323846
#define LEADER_INFLUENCE_RANGE 5
#define LEADER_PROPORTION 0.1f
#define LEADERSHIP_WEIGHT 0.5f
#define WILLNESS_THRESHOLD 0.3f
#define VISIBILITY_DISTANCE 5.0f
#define OBSTACLE_AVOID_DIS 2 
#define ANXIETY_THRESHOLD 1.0f
#define AGE_DIV 50
#define MALE_SCORE 1.0f
#define FEMALE_SCORE 0.5f
#define AGE_WEIGHT 0.2f
#define GENDER_WEIGHT 0.2f
#define CHARACTER_WEIGHT 0.5f
#define EDUCATION_WEIGHT 0.3f
#define TIME_STEP 0.033f
#define DENSITY_THRESHOLD 0.5f
#define CS_OBSTACLE_MANUAL_MOVE 0
#define CS_OBSTACLE_AUTOMATIC_MOVE 1
#define CROWD_SIMULATION_WITH_AGENT_PERSONALITY 0
#define CROWD_SIMULATION_WITHOUT_AGENT_PERSONALITY 1
#define VISIBILITY_GRID_LENGTH 10
#define GROUP_SIZE 4
#define DYNAMIC_VALUE 1.0f
#define LOWER_INTERVAL 0.9f
#define HIGHER_INTERVAL 0.99f

using namespace std;

typedef pair<int, int> PAIR_INT;

enum INERTIA{
	standStatic, 
	fromTop, 
	fromBottom, 
	fromLeft, 
	fromRight,
	fromLeftTop,
	fromRightTop,
	fromLeftBottom,
	fromRightBottom
};

enum PERSONALITY{
	selfish,
	littleSelfish,
	neutral,
	littleBrave,
	brave
};

enum Direction{
	_up,
	_right,
	_down,
	_left,
	local
};

enum VisibleState{
	person,
	area
};

class CS_DIJKSTRA;

class CellManager{
public:
	float max_sFF;
	vector<float> specific_max_sFF;
};

class CELL{
public:
	CELL(){
		probability = 0;
		dFF = 0;
		temp_dFF = 0;
		occupied = 0;
		inobstacle = false;
		cell_type = -1;
		intersect_obstacle = false;
		obstacle = false;
		normalize_dFF = 0;
		_sFF = 0;
		obstacle_ = 1;
		click = false;
		obstacle_id = -1;
		occupant_id = -1;
		mark = false;
		mark_ = false;
		check = false;
		max_sFF = 0.0f;
		exit_id = -1;
		aFF = 0;
		exit = false;
		m_OccupantID.resize(4,-1);
		occupied_number = 0;
	}
	int x, y; //grid's position
	int exit_id;
	float sFF;
	float dFF;
	float pre_sFF;
	float clean_sFF;
	float temp_dFF;
	float normalize_dFF;
	float probability;
	float sFF_color; //between 0~1
	float _sFF; //between 0~1
	float dFF_color; //between 0~1
	float max_sFF;
	float aFF;
	int occupied; //1: cell be occupied; 0: cell don't be occupied by pedestrian
	int cell_type; //0: agent; 1: exit; 2: obstacle
	int obstacle_; //0:obstacle
	int occupant_id; //record which agent occupied the cell
	int obstacle_id;
	int occupied_number;
	bool inobstacle;
	bool intersect_obstacle; //false: blue; true: red
	bool obstacle; //true: obstacle; false: not obstacle
	bool click;
	bool mark;
	bool mark_;
	bool check;
	bool exit;
	//vector<bool> intersect_obstacle_segment; //determine the cell intersect obstacle's edge whether or not
	vector<float> specific_sFF;
	vector<float> specific_normalize_sFF;
	vector<float> specific_color_sFF;
	vector<float> specific_max_sFF;
	vector<float> special_sFF;
	vector<float> special_normalize_sFF;
	vector<float> special_color_sFF;
	vector<float> special_max_sFF;
	vector<int> m_OccupantID;
	void create_space(int); //size
	void create_spaceForSpecialMap(int);
	void reset();
};

class CELL_BFS{
public:
	CELL_BFS(){

	}
	float _sFF; //0~1
	float sFF;
	float sFF_color;
	float distance;
	bool visisted;
	int x, y;
};

class FFMODEL{
public:
	FFMODEL(){
		group_ID = 0;
		size2_num = 0;
		size3_num = 0;
		start = false;
		select_agent_ID = 0;
		pre_select_agent_ID = 0;
		pause_through_time = 0;
		total_pause_through_time = 0;
		pause_start_time = clock();
		pause_end_time = clock();
		group_count = 0;
		group_radius = 5;
		density_thresold = 0.1f;
		colorTable_ID = 0;
		travel_time_step = 0;
		last_time_step = clock();
		out = false;
		remain_agent = -1;
		mNewSeed = 1;
		out_anxiety = false;
		mShowAgentInfo = 0;
		mParallelUpdate = 0;
		mFriction = 0.3f;
		mExperiment = 0;
		mMoveObstacleTime = 0;
		mMoveDistance = 5;
		mTimes = 0;
		mRunTimes = 0;
		mRecordVolunteerEvacuationTime = false;
		mDisToExit = 5;
		mVisableType = 0;
		mLeaderDistrubution = 0;
		mSameSeed = false;
		mAnxietyExperiment = 0;
	}
	float decay;
	float diffusion;
	float ks;
	float kd;
	float ki;
	float kw;
	float meu;
	float d_max;
	float alpha;
	float density;
	float pw;
	float ks_member;
	float kd_member;
	float waiting; //agent waiting probability
	float density_thresold;
	float travel_time_step;
	float mFriction;
	int size;
	int agent_number;
	int obstacle_size;
	int group_ID;
	int group_count;
	int size2_num;
	int size3_num;
	int remain_agent; //record the residual agent in the scene
	int select_agent_ID; //agent be selected by mouse
	int pre_select_agent_ID;
	int group_radius; //control the group area
	int colorTable_ID;
	int mType; //0: not consider exit width 1: consider exit width
	int mNewSeed;
	int maxEvacuationTime;
	int mShowAgentInfo;
	int mParallelUpdate;
	int mExperiment;
	int mMoveObstacleTime;
	int mMoveDistance;
	int mTimes;
	int mRunTimes;
	int mDisToExit;
	int mVisableType;
	int mLeaderDistrubution;
	int mAnxietyExperiment;
	bool mRecordVolunteerEvacuationTime;
	bool start; //control when the simulation start
	bool out;
	bool out_anxiety;
	bool mSameSeed;
	double pause_through_time;
	double total_pause_through_time;
	string agent_info; //agent information
	clock_t pause_start_time;
	clock_t pause_end_time;
	clock_t last_time_step;
	vector<int> guider_ID; //record guider ID
	unsigned seed;
	void reset();
};

class SCREEN{
public:
	SCREEN(){
		left_up_corner_x = -95;
		left_up_corner_y = 70;
		camera_translation_x = 0.0f;
		camera_translation_y = 0.0f;
		cur_x = 0;
		cur_z = 0;
	}
	string agent_info;
	int left_up_corner_x;
	int left_up_corner_y;
	float camera_translation_x;
	float camera_translation_y;
	int cur_x;
	int cur_z;
};

class STRESS_MODEL{
public:
	STRESS_MODEL(){

	}
	float stress_accumulation_rate; //arfa
	float max_stress; //beta
	float k; //power law coefficient
	float n; //power law exponent
	float tranlateStress(float stress){
		return k * pow(stress, n); //a generic model of the effect of the stressors.
	}
	float area_stress_intensity;
	float standard_deviation;
	int stress_radius;
};

class STRESSOR{
public:
	STRESSOR(){
		cur_neighbor_number = 0;
		pref_neighbor_number = 0;
	}
	// Time stressor
	float time_stress;
	// Area stressor
	float area_stress;
	// Positional stressor
	float position_stress;
	// Interpersonal stressor
	float interpersonal_stress;
	int cur_neighbor_number;
	int pref_neighbor_number;
	float compute_interpersonal_stress(){
		return (float)max(cur_neighbor_number - pref_neighbor_number, 0);
	};
};

class PSYCHOLOGY{
public:
	PSYCHOLOGY()
	{
		leadership = 0;
		strength = 0;
		potential = 0;
		resistance = 0;
		willness = 0;
		panic = 0;
	}
	float leadership;//[0,1]
	float strength;//F
	float potential;//leadership growth rate
	float resistance;//constant, growth through time
	float willness;
	float panic;//[-1,1]
};

class Personality{
public:
	Personality()
	{
		openness = 0.0f;
		conscientiousness = 0.0f;
		extraversion = 0.0f;
		agreeableness = 0.0f;
		neuroticism = 0.0f;
	}
	float openness;
	float conscientiousness;
	float extraversion;
	float agreeableness;
	float neuroticism;
};

class Emotion{
public:
	Emotion()
	{
		empathetic = false;
		susceptible = false;
	}
	bool empathetic;
	bool susceptible;
	float susceptible_threshold;
};

class PersonalityParameter{
public:
	PersonalityParameter(){
		W_E_L = 0.5f;
		W_N_L = 0.5f;
		W_N_P = 0.5f;
		W_C_P = 0.5f;
	}
	float W_E_L; //Weight_E_Leadership
	float W_N_L; //Weight_N_Leadership
	float W_N_P; //Weight_N_Panic
	float W_C_P; //Weight_C_Panic
};

class AGENT{
public:
	AGENT(){
		arrival = false;
		inertia = standStatic;
		assigned = false;
		leader = true;
		waiting = false;
		leader_distance = 0.0f;
		start_time = clock();
		escape_time = clock();
		added = false;
		click = false;
		cur_click = false;
		guider = false;
		height = 160;
		stop_guide = false;
		top = 1.0f;
		down = 1.0f;
		left = 1.0f;
		right = 1.0f;
		be_leaded = false;
		volunteer = false;
		beside_obstacle = false;
		finish_task = false;
		compressive_leader = false;
		strength = false;
		obstacle_component_id = -1;
		push_obstacle_id = -1;
		int rnd = rand() % 5;
		switch (rnd)
		{
		case 0:
			personality = selfish;
			break;
		case 1:
			personality = littleSelfish;
			break;
		case 2:
			personality = neutral;
			break;
		case 3:
			personality = littleBrave;
			break;
		case 4:
			personality = brave;
			break;
		}
		blockTheRoad = false;
		close_obstalce = false;
		//anxiety = (double)rand()/RAND_MAX;
		//anxiety = 0.0f;
		//anxiety = (double)rand() / RAND_MAX - 0.5f;
		if (anxiety < 0)
			anxiety = 0;
		anxiety = (double)rand() / RAND_MAX;
		while (anxiety < 0.4f || anxiety > 0.6f)
			anxiety = (double)rand() / RAND_MAX;
		//anxiety = 0;
		anxiety_delta = 0.0f;
		age = rand() % AGE_DIV;
		if (rand() % 2 == 0)
			gender = MALE_SCORE;
		else
			gender = FEMALE_SCORE;
		character = (double)rand() / RAND_MAX;
		education = (double)rand() / RAND_MAX;
		agent_personality = float(EDUCATION_WEIGHT * education + GENDER_WEIGHT * gender + CHARACTER_WEIGHT * character);
		map_size = 0;
		comity = false;
		direction = local;
		blocked_obstacle_id = -1;
		empathy = (double)rand() / RAND_MAX;
		//empathy = 0.8f;
		pre_sFF = 0.0f;
		stay_counter = 0;
		add_sFF = false;
		bystander = false;
		escapeTime = 0.0f;
		visible_bystander = 0;
		standerEffect_base = LOWER_INTERVAL + mPersonality.conscientiousness * (HIGHER_INTERVAL - LOWER_INTERVAL);
		currentVisibleBlockedExit = -1;
		w = 1;
		mEscapeExit = -1;
		mStayInPlace = false;
		mPushTimeCounter = 0;
		agent_id = -1;
		leader_id = -1;
		group_id = -1;
		group_size = -1;
		guide_exit_id = -1;
		close_obstacle_id = -1;
		close_obstacle_com_id = -1;
		pool_index = -1;
		guide_area_density = 0;
		mVolunteered = false;
		mTendency = 0.1f;
		mBias = 0.1f;
	}
	int agent_id;
	int leader_id;
	int group_id;
	int group_size;
	int height;
	int guide_exit_id;
	int push_obstacle_id;
	int obstacle_component_id;
	int close_obstacle_id;
	int close_obstacle_com_id;
	int pool_index;
	int map_size;
	int blocked_obstacle_id;
	int stay_counter;
	int visible_bystander;
	int *blockByExit;
	int currentVisibleBlockedExit;
	int mEscapeExit;
	int mPushTimeCounter;
	bool arrival;
	bool assigned; //false: haven't assign a group to agent
	bool leader; //true: is leader; false: not leader
	bool waiting;
	bool added;
	bool click;
	bool cur_click;
	bool guider;
	bool stop_guide; //guide area's density is lower than thresold
	bool be_leaded; //agent be leaded by guider
	bool volunteer;
	bool beside_obstacle;
	bool add_sFF;
	bool finish_task;
	bool compressive_leader;
	bool strength;
	bool blockTheRoad;
	bool close_obstalce;
	bool comity;
	bool bystander;
	bool mStayInPlace;
	bool mVolunteered;
	float leader_distance;
	float guide_area_density;
	float top, down, left, right;
	double anxiety;
	float anxiety_delta;
	float agent_personality;
	int age;
	double education;
	float gender;
	double character;
	double empathy;
	float pre_sFF;
	float escapeTime;
	float standerEffect_base;//0.9~0.99
	float w;
	float mTendency;
	float mBias;
	PAIR_INT position;
	PAIR_INT escape_position;
	PAIR_INT pre_pos;
	INERTIA inertia;
	clock_t start_time;
	clock_t escape_time;
	vector<PAIR_INT> path;
	vector<PAIR_INT> visible_area;
	vector<PAIR_INT> last_visible_area;
	vector<int> visible_agentID;
	vector<float> visible_area_advancesFF;
	vector<int> memberID;
	vector<float> anxiety_variation;
	vector<float> remain_anxiety;
	vector<int> mNeighboringAgent;
	STRESSOR stressor;
	PSYCHOLOGY psychology;
	PERSONALITY personality;
	Direction direction;
	Personality mPersonality;
	void reset();
	void draw_direction(int);
	void computeWillness();
	void receiveWillingFromCrowd();
	void receiveEmotionFromVisibleAgent();
	void insertAnxiety(float);
	float computePanicConscientiousnessFunction();
	float averageResidualAnxiety();
	float getDistanceWeight(PAIR_INT);
};

class AGENT_PSYCHOLOGY{
public:
	AGENT_PSYCHOLOGY(){
		anxiety_weight = 10.0f;
		bias = 0.0f;
		tendency = 0.1f;
		timeEffect = 25;
	}
	int actionType;
	int obstacle_moveType;
	float anxiety_weight;
	float bias;
	float tendency;
	float timeEffect;
};

class AGENT_GROUP{
public:
	AGENT_GROUP(){
		erase = false;
	}
	vector<int> member; //member's ID
	bool erase;
};

class Obstacle_manager{
public:
	
};

class OBSTACLE{
public:
	OBSTACLE(){
		block = false;
		block_exit = false;
		arrive_destination = false;
		volunteer_destination = pair<int, int>(-1, -1);
		clicked_o_id = 0;
		enough_volunteers = false;
		direction = pair<int, int>(0, 0);
		finish_task = false;
		weight = 6.0f;
		timer = 0.0f;
		compute = false;
		hasLeader_guider = false;
		density = 0.0f;
		up_count = 0;
		right_count = 0;
		down_count = 0;
		left_count = 0;
		ready = false;
		type = 0;
		block_direction = -1;
		movable = false;
		blockExitID = -1;
		moveAlready = false;
		mMoveStep = 0;
		up = false;
		right = false;
		down = false;
		left = false;
		mHaveDes = false;
		stuck_time = 0;
	}
	float minx, maxx, minz, maxz;
	vector<vector3> nodes;
	//
	int id;
	int size;
	vector<pair<int, int>> component;
	// candidate volunteer id
	vector<vector<int>> candidate_id;
	//vector<vector<vector<bool>>> density_map;
	//
	bool movable;
	bool block;
	bool block_exit;
	vector<int> volunteer_id;
	vector<bool> component_removable;
	int blockExitID;
	//
	bool arrive_destination;
	pair<int, int> volunteer_destination;
	int clicked_o_id;
	int final_clicked_o_id;
	float offset_x;
	float offset_z;
	//
	bool enough_volunteers;
	PAIR_INT pre_direction;
	pair<int, int> direction; //(1,0):right,(-1,0):left,(0,1):up,(0,-1):down
	bool finish_task;
	//
	float weight;
	float timer;
	//
	bool compute;
	//
	bool hasLeader_guider;
	int pool_index;
	float density;
	//
	//
	int up_count, right_count, down_count, left_count;
	bool up, right, down, left;
	bool ready;
	int highest_x, highest_z, lowest_x, lowest_z;
	//
	vector<vector<vector<CELL_BFS>>> distance_map;
	//
	int type; //0: vertical, 1: horizontal
	//
	int block_direction; //0:up, 1:right, 2:down, 3:left, -1: didn't block exit
	//
	int mMoveStep;
	bool moveAlready;
	//
	void reset();
	//
	vector<PAIR_INT> moveDestination;
	bool mHaveDes;
	int stuck_time;
};

class Exit{
public:
	Exit(){
		mEscapeTime = 0;
		active = true;
	}
	int width;
	PAIR_INT position;
	PAIR_INT direction;
	int mEscapeTime;
	bool active;
};

class REMOVED_OBSTACLE{
public:
	REMOVED_OBSTACLE(){
		radius = 10;
		has_volunteer = false;
		arrival_destination = false;
		volunteer_id = -1;
	}
	int radius;
	int id;
	int volunteer_id;
	bool has_volunteer;
	bool arrival_destination;
	pair<int, int> position;
	pair<int, int> destination;
};

class REMOVED_LARGE_OBSTACLE{
public:
	REMOVED_LARGE_OBSTACLE(){

	}

};

class CELL_SEGMENT{
public:
	CELL_SEGMENT(){
		x = 0;
		z = 0;
	}
	int x; 
	int z;
};

class GuiParameter{
public:
	GuiParameter(){
		visible_count = 0;
		visibleState = person;
		colorTable_withoutExit_ID = 0;
		colorTable_special_ID = 0;
		pos_weight = 0.05f;
		stayInPlace_weight = 0.02f;
		visibleAnxiety_weight = 0.03f;
		sFF_weight = 0.05f;
		emotion_weight = 0.3f;
		anxiety_weight = 0.6f;
		leader_proportion = 0.1f;
		group_size = 4;
		willing_threshold = 0.3f;
		specailColorTable_normalizeValue = 50.0f;
		drawExitColor = false;
		mEditingMode = 0;
	}
	int visible_count;
	int colorTable_withoutExit_ID;
	int colorTable_special_ID;
	float specailColorTable_normalizeValue;
	VisibleState visibleState;
	// emotion parameter total equal to one
	float pos_weight;
	float stayInPlace_weight;
	float visibleAnxiety_weight;
	// anxiety parameter
	float sFF_weight;
	float emotion_weight;
	// willing parameter
	float anxiety_weight;
	// leader proportion
	float leader_proportion;
	//
	int group_size;
	// control small group number
	int groupSizeTwo;
	int groupSizeThree;
	// control threshold
	float willing_threshold;
	//
	bool drawExitColor;
	// 
	int mEditingMode; //0: normal, 1: exit, 2: obstacle, 3:agent
};

class EditingTools{
public:
	EditingTools(){
		mNeedChooseAnotherCell = false;
	}
	bool mNeedChooseAnotherCell;
	Exit mTempExit;
	OBSTACLE mTempObstacle;
};

class ExperimentData
{
public:
	ExperimentData()
	{

	}
	vector<vector<float>> mAverageEvacuationTime;
	vector<vector<float>> mAverageLastAgentEvacuationTime;
	vector<vector<float>> mEscapeAgentNumber;
	vector<float> mStandardDeviation0;
	vector<float> mStandardDeviation1;
	vector<float> mStamderdDeviation2;
	float* mTotalAverageEvaciationTime;
	float* mTotalAverageLastAgentEvacuationTime;
	//vector<float> mTotalAverageEvaciationTime;
	//vector<float> mTotalAverageLastAgentEvacuationTime;
};

class CS_CELLULAR_AUTOMATA{
public:
	CS_CELLULAR_AUTOMATA();
	void copy_dynamicFF();
	void copy_preUpdate_time();
	void update_dynamicFF();
	void update_agent_position();
	void update_virtual_agent_position();
	void update_agent_psychology();
	void update_distanceToLeader();
	void update_pause_time();
	void update_agent_stress();
	void update_leader_influence();
	void update_volunteer_towardToObstacle_action();
	void update_blocked_obstacle_position();
	void update_obstacle_position();
	void update_agent_closeToObstalce();
	void update_strength_agent();
	void update_moveable_obstacle();
	void updateVisibleAgent();
	void updateTimeStep();
	void updateDensityAroundObstacle();
	void updateScene();
	void collectAverageAnxietyAroundObserveAgent();
	void load_simulation();
	void resolve_conflicts();
	void reset();
	void start();
	void pause();
	void setMoveObstacleTime();
	void setMoveObstacleDistance();
	void setObstacleDestination();
	void addStatisticalArea();
	void draw_segmentsBetweenCellAndExits();
	void draw_agent();
	void draw_group();
	void draw_group_circle();
	void draw_specific_group();
	void draw_lattice();
	void draw_border();
	void draw_colorTable();
	void draw_dFF_colorTable();
	void draw_obstacle();
	void draw_guider();
	void draw_agent_path();
	void draw_clicked_agent();
	void draw_clicked_cell();
	void draw_escape_direction();
	void draw_stress_area();
	void draw_volunteer();
	void draw_moved_obstacle();
	void draw_moved_obstacle_automatic();
	void draw_moved_obstacle_manual();
	void draw_compressive_leader();
	void draw_strength_agent();
	void draw_selfish_agent();
	void draw_neutral_agent();
	void draw_brave_agent();
	void draw_idiot_agent();
	void draw_volunteer_colorTable();
	void draw_special_colorTable();
	void draw_colorTable_withoutExit();
	void draw_agent_anxiety();
	void draw_AFF();
	void draw_totalFloorField();
	void draw_AgentEvacuationTime();
	void draw_AgentEscapeExit();
	void draw_agent_willness();
	void draw_density_around_obstacle();
	void draw_agent_face_direction();
	void draw_visible_agent(int); // a_id
	void draw_visible_area(int); // a_id
	void draw_currentExitBlockAgent();
	void show_node_intersectObstacle();
	void show_specific_group();
	void show_agent_info();
	void set_scene();
	void set_cell_exit_segment(int, int);
	void set_specific_group(int);
	void set_numOfgroup_size(int, int);
	void set_group_radius(int);
	void set_corner_coordinate(int, int);
	void set_group_leader();
	void set_guider(int);
	void set_cur_mouse_position(int, int);
	void set_obstacleMustRemove();
	void set_ColorTable_ID(int);
	void set_ColorTable_witoutExit_ID(int);
	void set_WillingThreshold(float);
	void set_SpecialID(int);
	void set_colorTableNormalizeValue(float);
	void set_SameSeed();
	void set_Bias();
	void set_ObserveAgent();
	void change_visible_state();
	void get_agent_info(int, int);
	void add_obstacles(float, float, float, float); //minx, maxx, minz, maxz
	void add_agent_to_group();
	void add_new_group();
	void add_stress_source();
	void add_dynamic_stress_source();
	void remove_agent_from_group();
	void choose_obstacle_destination();
	void choose_large_obstacle_detination();
	void edit_obstacle();
	void enabled_edit_obstacle();
	void enabled_edit_exit();
	void show_cell_beOccupied();
	void reset_agent_parameters();
	void blocked_obstacle_findVolunteer();
	void find_obstacleMustMove();
	void check_cell_occupied();
	void agent_detect_obstalce();
	void setFlgExitColor(bool);
	void detectEditingMode();
	void recordingEvacuationTime();
	void saveImage();
	void outputSFFExperiment();
	void outputMoveObstacleTimeExperiment();
	void outputMoveObstacleDistanceExperiment();
	void outputObstacleLocatedExperiment();
	void outputStatisticsOnAnxiety_LeaderAndMember();
	void generateRandomAgentOrder();
	void assignColorProportion();
	string get_remain_agent();
	string get_through_timeStep();
	string get_through_time();
	int getExperimentType();
	int getRunTimes();
	int getTimeCounter();
protected:
	void initialize_variables();
	void initialize_agent_position();
	void initialize_group_member_position(int); // group ID
	void initialize_gaussianDistribution_parameter();
	void initialize_agent_anxiety();
	void calculateCellProbality(int, int);
	void compute_staticFF();// static floor field
	void compute_clean_sFF();
	void compute_volunteer_staticFF(int, int); // o_id, com_id
	void compute_guide_influence(int);
	void computeAgentLeadership();
	void computeAgentPanic();
	void computeNewMapWithoutExit(int[]);
	void computeNewMapWithoutExit(int); //o_id
	void read_data(); // read data from the config. file to initialize
	void reset_addition_force();
	void reset_OccupiedAgentID();
	void reset_AgentStayInPlaceFlg();
	void reset_area_stress();
	void output_data();
	void output_data(int); //o_id
	void output_colorTableWithoutExit(int); //o_id
	void output_specialColorTable(int[]);
	void outputInformation();
	void outputAnxietyVariation();
	void outputStatisticInfo();
	void outputMaxDFF();
	void outputSeed();
	void outputVolunteerEvacuation();
	void outputAgentPersonality();
	void readAgentPersonality();
	void check_arrival();
	void check_task_finish();
	void set_obstacle();
	void set_wall();
	void set_group();
	void scale_dFF();
	void scale_distanceToLeader();
	void get_individual_agent_info(int, int);
	void check_sFF();
	void check_volunteer_besideObstacle();
	void check_obstacleMovement_blocked(int, Direction&); //obsalce_id
	void check_obstacleMovement_blocked_auto(int, Direction&);
	void check_obstacleMovement_blocked_manual(int);
	void check_volunteer_finish_task(int); //o_id
	void check_agent_willness(int); //o_id
	void check_volunteer_willness(int); //o_id
	void check_volunteer_beside_obstacle();
	void check_volunteer_position();
	void check_obstacle_arrive(int); //o_id
	void update_agent_info();
	void update_guider_density(int);
	void update_neighbors_count();
	void update_area_stress();
	void update_interpersonal_stress(int);
	void update_positional_stress();
	void update_time_stress();
	void update_sFF();
	void update_agent_anxiety();
	void update_agent_willness();
	void update_obstalce_type();
	void update_agent_blockedByObstacle();
	void update_ExitID_BlockedByObstacle();
	void update_AgentPositionParallel();
	void update_AgentPositionDisordered();
	void update_Cell_AgentWantOccpied();
	void find_nearestExit(int);
	void obstacle_findNearestAgent();
	void find_direction_towardToObstacleDestination(int,Direction&); //o_id
	void find_obstacle_best_direction(int); //o_id
	void find_guider(int); //o_id
	void find_volunteers(int); //o_id
	void automatic_findVolunteers();
	void manual_findVolunteers();
	void translate_perceived_stress();
	void push_obstacle(int);
	void test_large_obstacle1();
	void move_obstalce();
	void move_volunteersAndObstacles(int); //o_id
	void pushAgent_createSpace(int); //o_id
	void set_virtual_obstacle(int); //o_id
	void obstacle_timer_accumulation(int); //o_id
	void reset_cellMark();
	void reset_obstacle_counter(int); //o_id
	void reset_advanced_sFF(int); //a_id
	void automatic_update_blocked_obstacle_position();
	void manual_update_blocked_obstacle_position();
	void crowd_simulation_actionType();
	void add_volunteer(int, int, int); //o_id, volunteer_id, agent_id
	void delete_volunteer(int, int, int); //o_id, volunteer_id, agent_id
	void obstacle_volunteer_movement(int, PAIR_INT, Direction); //o_id, direction
	void print_illegal_agent_information(string);
	void disperse_crowd(int, PAIR_INT); //o_id
	void agent_comity_move(int, PAIR_INT);
	void gaussian_generate_personality();
	void add_blocked_obstacle(int); //o_id
	void add_normal_obstacle(int); //o_id
	void decideToBeVolunteer(int); //a_id
	void decideToBeBystander(int); //a_id
	void decideBehavior(int);
	void clearCandidateVolunteer(int, int); //o_id, com_id
	void addsFF_resolveCounterFlow(int); //a_id
	void collect_visible_bystander(int); //a_id
	void follow_behavior(int); //member_id
	void randomGenerateExitColor();
	void loadTexture();
	void editExit();
	void editObstacle();
	void editAgent();
	void collectExit();
	void collectObstacle();
	void regenerate_ExitID();
	void regenerate_ObstacleID();
	void constructAFF(int);
	void findNeighboringAgent();
	void push_observedVolunteer(int); //a_id
	void findOptimalDestination(int); //o_id
	void switchVolunteer(int, int, int); //o_id, com0_id, com1_id
	float findNearestDistanceToExit(vector<PAIR_INT>);
	float findNearestDistanceToExit(int); //o_id
	float distanceToLeader(int, int, int); //x, y, agent_ID
	float get_normalize(int, int); // get the total probability nearby position(x,y)
	double get_probability(int, int, int);
	float get_probability_volunteer(int, int, int);
	float get_probability_obstacle(int, int, int);
	float get_probability_volunteer(int, int, int, int, int); //x,z,agent_id,component_id,obstacle_id
	float get_inertia(int, int, int);
	float get_leader_inertia(int, int, int);
	float get_position_stress(PAIR_INT, PAIR_INT);
	float pair_int_distance(PAIR_INT, PAIR_INT);
	float get_density_threshold(float); //willness
	float get_obstacle_density(int); //o_id
	float gaussianFunc(float, float, float); //dis, mean, variance
	float get_angle(pair<float, float>, pair<float, float>);
	float decodingFloorFieldID(int[]);
	float getImpactFromOtherAgents(int); //a_id
	float getContagionStrengthFromContactAgents(int); //a_id
	float getContagionStrength(int, int); //receiverID, senderID
	float getInfluenceStrengthFromContactAgents(int); //a_id
	float getLeaderEffect(int); //a_id
	float getTimeInfluence(float);
	float combinationFunction(int); //a_id
	float testCombinationFunction(int, float, float); //a_id, tendency, bias
	bool check_intersect(vector3, vector3, int, int); //node1, node2, edgeID1, edgeID2
	bool check_member_arrival(int); // leader ID
	bool check_agentDistanceNearestThanObstacle(int);
	bool check_occupiedAgent_sameTask(int, int, int, int); //x,z,o_id,a_id
	bool check_obstacle_weight(int); //o_id
	bool check_volunteer_setup(int); //o_id
	bool check_obstacleID_exist(int); //o_id
	bool check_obstacle_movement_block(int); //o_id
	bool check_volunteer_movement_block(int); //a_id
	bool check_moveToLowSFF(int o_id);
	bool determine_leader_waiting(bool, bool);//parameter: is leader or not; second: other member leaver or not
	bool determine_leader_waiting(bool, int);
	bool opposite_direction(int, float);
	bool opposite_direction_toObstacle(int, float);
	bool opposite_direction_toDestination(int, float);
	bool opposite_direction_toOb(int, float, int, int); //agent_id, distanceToOb, com_id, o_id
	bool isValid(int, int); //true: valid cell; false: no valid
	bool isBlock(int, int); //true: exit be blocked
	bool isBlock(int, int, int); //
	bool isBesideWall(int, int);
	bool isFarEnough(int, int);
	bool isObstacle(int, int);
	bool isBelongSameObstacle(int, int, int);
	bool isBeoccupied(int);
	bool isExit(int, int);
	PAIR_INT choose_direction(int, int, int); // choose the direction pedestrian want to go : up, down, left, right
	PAIR_INT choose_direction_HighValueBased(int, int, int);
	PAIR_INT choose_direction_HighValueBased_fourDirection(int, int, int);
	PAIR_INT choose_direction_volunteer(int, int, int); //x,z,agent_id
	PAIR_INT choose_direction_volunteer(int); //agent_id
	PAIR_INT choose_direction_obstacle(int, int); //obstacle_id,component_id
	PAIR_INT agent_avoid_obstacle(int); //a_id
	Direction obstalce_move_direction(int);
private:
	FFMODEL* model;
	CELL_SEGMENT segment;
	SCREEN screen;
	CellManager cell_manager;
	CELL** cell;
	AGENT* agent;
	STRESS_MODEL stress_model;
	AGENT_PSYCHOLOGY agent_psychology;
	GuiParameter guiParameter;
	PersonalityParameter mParameter;
	EditingTools mEditingTools;
	ExperimentData mExperimentData;
	//
	vector<pair<int, int>> exits; // exit's position
	vector<OBSTACLE> obstacles;
	vector<Exit> mExit;
	vector<REMOVED_OBSTACLE> removed_obstacles;
	vector<CELL_BFS**> distance_map;
	vector<vector<vector<CELL_BFS>>> distance_map_;
	vector<vector<vector<CELL_BFS>>> volunteer_distance_map;
	vector<vector<vector<CELL_BFS>>> clean_distance_map;
	vector<vector<vector<CELL_BFS>>> special_distance_map;
	vector<vector<CELL_BFS>> cell_distance_;
	vector<vector<CELL_BFS>> distance_buffer;
	vector<AGENT_GROUP> agent_group;
	AGENT_GROUP temp_group;
	vector<pair<int, int>> stress_area_position;
	vector<pair<int, int>> dynamic_stress_area_position;
	vector<PAIR_INT> wall;
	vector<int> leader_pool;
	vector<int> strength_agent;
	vector<int> weakness_agent;
	vector<int> normal_obstacle;
	vector<int> blocked_obstacles;
	vector<int> volunteer_id;
	vector<float> mDynamicFloorField;
	vector<vector3> mExitColor;
	vector<int> mRandomAgentOrder;
	vector<float> mColorProportion;
	vector<int> mVolunteer;
	vector<int> mObserverAgent;
	vector<vector<float>> mAverageAnxeityAroundObserveAgent;
	vector<vector<float>> mObserveAgentAnxiety;
	//
	ifstream file;
	ofstream outputFile;
	bool editing_mode;
	bool change;
	//
	float mTimeStep;
	int mTimeCounter;
	int* eb;
	//
};

#endif