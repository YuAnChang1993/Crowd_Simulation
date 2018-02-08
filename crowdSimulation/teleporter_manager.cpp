
#include "teleporter_manager.h"
#include "./cs_parameters.h"

using namespace std;

TELEPORTER_MANAGER::TELEPORTER_MANAGER( int maxNumObjs ):MANAGER_TEMPLATE<TELEPORTER>(maxNumObjs)
{
	
}

TELEPORTER *TELEPORTER_MANAGER::addObject( const TELEPORTER &obj )
{
	TELEPORTER *g = 0;
	return g;
}


 void TELEPORTER_MANAGER::update( float time_step )
 {
 }
	 void TELEPORTER_MANAGER::read( const char *fileName )
 {
 }
	 void TELEPORTER_MANAGER::save( const string &fileName ) const
 {
 }
	 void TELEPORTER_MANAGER::printf_Data( ) const
 {
 }
