#include "crowd_monitor_manager.h"
#include "../cs_parameters.h"

CROWD_MONITOR_MANAGER *CROWD_MONITOR_MANAGER::_instance = 0;

CROWD_MONITOR_MANAGER::CROWD_MONITOR_MANAGER( int maxNumObjs ):MANAGER_TEMPLATE<CROWD_MONITOR>(maxNumObjs)
{
}




CROWD_MONITOR *CROWD_MONITOR_MANAGER::addObject( const CROWD_MONITOR &g )
{
	int objIndex = mObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return 0;
	CROWD_MONITOR *obj = mObjProvider.getEle( objIndex );
	
	obj->set( g );
	obj->resetHistory( );
	mObjProvider.autoSetUniqueObjID( obj );
	return obj;
}



void CROWD_MONITOR_MANAGER::printf_Data( ) const
{
}

void CROWD_MONITOR_MANAGER::read( const char *fileName )
{
}

void CROWD_MONITOR_MANAGER::save( const string &fileName ) const
{
}

void CROWD_MONITOR_MANAGER::resetHistory( )
{
	mObjProvider.resetIterator( );
	CROWD_MONITOR *g;
	while (	g = mObjProvider.getNext( ) ) {
		g->resetHistory( );
	}
}

void CROWD_MONITOR_MANAGER::addHistoryRecord( const vector3 &p, float w )
{
	bool flg_catch = false;
	
	mObjProvider.resetIterator( );
	CROWD_MONITOR *g;
	while ( g = mObjProvider.getNext( ) ) {
		vector3 g_p;
		float g_r;
		g->getInfo( g_p, g_r );
		if ( g_r*g_r >= g_p.distance2( p ) ) {
			g->addHistoryRecord( p, w );
			flg_catch = true;
			break;
		} else {
			
		}	
	}
	if ( flg_catch == false ) {
		CROWD_MONITOR tmp_g;
		tmp_g.set( p, csp_CrowdMonitorRadius );
		CROWD_MONITOR *g = addObject( tmp_g );
		if ( g ) {
			g->addHistoryRecord( p, w );
		}
	}
}

void CROWD_MONITOR_MANAGER::analyze( )
{
		//cout << "CROWD_MONITOR_MANAGER::analyze( )........." << endl;
		
		mObjProvider.resetIterator( );
		CROWD_MONITOR *g;
		g = mObjProvider.getNext( );
		if ( g ) {
			g->analyze( );
		}
}

void CROWD_MONITOR_MANAGER::update( float time_step )
{
	analyze( );
}