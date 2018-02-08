//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __TELEPORTER_MANAGER_H_
#define __TELEPORTER_MANAGER_H_

#include "../headers.h"
#include "../primitives/vector3d.h"
#include "cs_basic_structs.h"
#include "teleporter.h"
#include "../primitives/provider.h"
#include "./manager_template/manager_template.h"
//
class TELEPORTER_MANAGER : public MANAGER_TEMPLATE<TELEPORTER> 
{
private:
	static TELEPORTER_MANAGER *_instance; // singleton pattern

protected:
public:
	TELEPORTER_MANAGER( int maxNumObjs );
	//
	virtual TELEPORTER *addObject( const TELEPORTER &obj );
	virtual void draw( ) const;
	virtual void update( float time_step );
	virtual void read( const char *fileName );
	virtual void save( const string &fileName ) const;
	virtual void printf_Data( ) const;
};

#endif