#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//

class EVENT_HANDLER
{
public:
	EVENT_HANDLER( ) { }
	virtual bool mouseMotionFunc( int mouse_x, int mouse_y, float space_x = 0.0, float space_y = 0.0, float space_z = 0.0 ) { }
	virtual bool mouseFunc( int button, int state, int mouse_x, int mouse_y, float hitDistance, float space_x = 0.0, float space_y = 0.0, float space_z = 0.0  ) { }
	virtual bool passiveMouseFunc( int mouse_x, int mouse_y, float hitDistance, float space_x = 0.0, float space_y = 0.0, float space_z = 0.0  ) { }
};
#endif