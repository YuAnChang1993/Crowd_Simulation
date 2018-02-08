#ifndef __CROWD_MONITOR_MANAGER_H__
#define __CROWD_MONITOR_MANAGER_H__


#include "../headers.h"
#include "../primitives/vector3d.h"
#include "../cs_basic_structs.h"
#include "crowd_monitor.h"
#include "../primitives/provider.h"
#include "../manager_template/manager_template.h"

class CROWD_MONITOR_MANAGER : public MANAGER_TEMPLATE<CROWD_MONITOR> 
{
private:
	static CROWD_MONITOR_MANAGER *_instance;
protected:

	virtual void analyze( );
public:
	CROWD_MONITOR_MANAGER( int maxNumObjs );
	virtual CROWD_MONITOR *addObject( const CROWD_MONITOR &obj );
	virtual void resetHistory( );
	virtual void addHistoryRecord( const vector3 &p, float w );
	
	//
	virtual void printf_Data( ) const;
	virtual void update( float time_step );
	//
	virtual void read( const char *fileName );
	virtual void save( const string &fileName ) const;
	//
	virtual void draw( ) const;
	//
	static CROWD_MONITOR_MANAGER *instance( int maxNumObjs = 0 ) {
		if ( _instance == 0) {
			_instance = new CROWD_MONITOR_MANAGER( maxNumObjs );
		}
		return _instance;
	}
};

#endif