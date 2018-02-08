//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include "environment.h"
#include "./cs_parameters.h"

using namespace std;

CS_ENVIRONMENT *CS_ENVIRONMENT::_instance = 0;

namespace {
	bool _flg_init_envObjNameMap = false;
	map<int, string> _envObjNameMap;
};
//
void init_envObjNameMap( )
{
	if ( _flg_init_envObjNameMap ) {
		return;
	}
	_envObjNameMap[CS_ENV_OBJECT_TYPE_NONE] = "NONE";
	_envObjNameMap[CS_ENV_OBJECT_TYPE_RECTANGLE] = "RECTANGLE";
	_envObjNameMap[CS_ENV_OBJECT_TYPE_CIRCLE] = "CIRCLE";
	_envObjNameMap[CS_ENV_OBJECT_TYPE_POLYGON] = "POLYGON";
}

string getEnvironmentObjTypeName( int type )
{
	string name;
	if ( _flg_init_envObjNameMap == false ) {
		init_envObjNameMap( );
		_flg_init_envObjNameMap = true;
	}
	map<int, string>::iterator record = _envObjNameMap.find( type );
	if ( record != _envObjNameMap.end( ) ) {
		name = record->second;
	}
	return name;
}



CS_ENVIRONMENT::CS_ENVIRONMENT( )
{
	mBVHRoot = 0;
	//
	mCDEncodingSchemeType = 0;
	//mCDEncodingSchemeType = 1;
	mEncodedCDData = 0;
	mEncodedCD_GridNX = 100;
	mEncodedCD_GridNZ = 100;
	mCDGrid_WX = 1.0; // range of the space
	mCDGrid_WZ = 1.0;

	mCDGrid_DX = 1.0;
	mCDGrid_DZ = 1.0;


	//
	mMaxNumEnvObjects = csp_maxNum_EvironmentObjs;
	mEnvObjProvider.init( mMaxNumEnvObjects );
	mMapObj = new CS_ENV_OBJECT;
	mMapObj->addPoint(-300.0, 0.0, -300.0);
	mMapObj->addPoint(300.0, 0.0, 300.0);
	//
	m_bo_CurAction = CS_EDIT_ACTION_NONE;
	mCurPickedObj = 0;
	mCurHitObj = 0;
	mFlgNewlyPickedObj = false;
	mLockPickedObject = false;
}

void CS_ENVIRONMENT::setCollisionDetectionScheme( int schemeType )
{
	mCDEncodingSchemeType = schemeType;
	cout << "***CS_ENVIRONMENT::setCollisionDetectionScheme:" << mCDEncodingSchemeType << endl;
}

void CS_ENVIRONMENT::setCollisionDetectionGridDimension( int nx, int nz ) 
{
	mEncodedCD_GridNX = nx;
	mEncodedCD_GridNZ = nz;

	cout << "mEncodedCD_GridNX:" << mEncodedCD_GridNX << endl;
	cout << "mEncodedCD_GridNZ:" << mEncodedCD_GridNZ << endl;


}

PROVIDER<CS_ENV_OBJECT> *CS_ENVIRONMENT::getProvider( )
{
	return &mEnvObjProvider;
}

void CS_ENVIRONMENT::update( float time_step )
{
	mEnvObjProvider.resetIterator( );
	CS_ENV_OBJECT *envObj;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		envObj->update( time_step );
	}
}

void CS_ENVIRONMENT::setObjAction( int action )
{
	m_bo_CurAction = action;
}


void CS_ENVIRONMENT::addEnvObject( const CS_ENV_OBJECT &obj )
{
	if ( obj.ptArr.size( ) == 0 ) return;
	int objIndex = mEnvObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_ENV_OBJECT *envObj = mEnvObjProvider.getEle( objIndex );
	envObj->set( obj );
	envObj->setActivatedFlag( true );
	envObj->setUsedFlag( true );
	envObj->computeAABB( );
	//
	mEnvObjProvider.autoSetUniqueObjID( envObj );
}

void CS_ENVIRONMENT::setMapObj( const CS_ENV_OBJECT &obj )
{
	if ( mMapObj ) {
		mMapObj->set( obj );
	}
}


void CS_ENVIRONMENT::addEnvironmentObject_Rect( const vector3 &p0, const vector3 &p1 )
{
	int objIndex = mEnvObjProvider.getFreeElement( );
	if ( objIndex < 0 ) return;
	CS_ENV_OBJECT *envObj = mEnvObjProvider.getEle( objIndex );
	envObj->clear( );
	envObj->setObjType( CS_ENV_OBJECT_TYPE_RECTANGLE );
	envObj->addPoint( p0 );
	envObj->addPoint( p1 );
}




vector3 CS_ENVIRONMENT::computeSeparationVector_Circle_NormalScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	vector3 out_s;
	vector3 s;
	flg_hit = false;
	feasible_direction.zero( );
	mEnvObjProvider.resetIterator( );
	CS_ENV_OBJECT *envObj;
	float contact_time = -1.0;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		bool flg = envObj->isContained( q0, q1, r );
		if ( flg == false ) continue;
		float ct;
		vector3 fdir;
		s = envObj->computeSeparationVector_Circle( q0, q1, r, flg, ct, fdir );
		if ( flg == false ) continue;
		out_s = s;
		if ( contact_time < 0.0 || contact_time > ct ) {
			contact_time = ct;
			feasible_direction = fdir;
		}
		flg_hit = true;
	}
	return out_s;
}

vector3 CS_ENVIRONMENT::computeSeparationVector_Circle_BVHScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	vector3 out_s;
	vector3 s;
	flg_hit = false;
	feasible_direction.zero( );
	vector<CS_ENV_OBJECT *> objArr;
	
	traverseBVH( objArr, q0, q1, r );
	float contact_time = -1.0;
	for (int i = 0; i < objArr.size( ); ++i ) {
		CS_ENV_OBJECT *envObj = objArr[i];
		//bool flg = envObj->isContained( q0, q1, r );
		//if ( flg == false ) continue;
		bool flg = false;
		float ct;
		vector3 fdir;
		s = envObj->computeSeparationVector_Circle( q0, q1, r, flg, ct, fdir );
		if ( flg == false ) continue;
		out_s = s;
		if ( contact_time < 0.0 || contact_time > ct ) {
			contact_time = ct;
			feasible_direction = fdir;
		}
		flg_hit = true;
	}
	return out_s;
}

vector3 CS_ENVIRONMENT::computeNormalDirection_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &prefer_vec ) const
{
	vector3 n;
	flg_hit = false;
	bool tmp_flg_hit;
	vector3 pv;
	n.zero( );
	pv.zero( );
	mEnvObjProvider.resetIterator( );
	CS_ENV_OBJECT *envObj;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		n += envObj->computeNormalDirection_Circle( q0, q1, r, tmp_flg_hit, pv );
		if ( tmp_flg_hit ) {
			prefer_vec += pv;
		}
		flg_hit |= tmp_flg_hit;
		if ( n.x != n.x ) {
			cout << "computeNormalDirection_Circle" << endl;
			cout << "NAN" << endl;
		}
		//if ( flg_hit ) break;
	}
	if ( flg_hit ) {
		if ( n.Length( ) == 0.0 ) {
			//cout << "n.Length( ):" << 0 << endl;
			flg_hit = false;
		} else {
			n.Normalize( );
			prefer_vec.Normalize( );
		}
	}
	return n;
}

bool CS_ENVIRONMENT::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void CS_ENVIRONMENT::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}

void CS_ENVIRONMENT::bo_setNewPosition_XZ( float x, float z )
{
}
void CS_ENVIRONMENT::bo_drawWithHighLight( ) const
{
}

void CS_ENVIRONMENT::bo_drawWithHighLight( float r, float g, float b ) const
{
}

void CS_ENVIRONMENT::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	mEnvObjProvider.resetIterator( );

	CS_ENV_OBJECT *envObj;

	bool flgFirst = true;
	vector3 aabbMin, aabbMax;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		envObj->computeAABB( aabbMin, aabbMax );
		if ( flgFirst ) { 
			AABBmin = aabbMin;
			AABBmax = aabbMax;
			flgFirst = false;
		} else {
			mergeAABB( AABBmin, AABBmax, aabbMin, aabbMax );
		}
	}

}

vector3 CS_ENVIRONMENT::computeSeparationVector_Circle_EncodedScheme_Type_01( unsigned int cdCode, const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	vector3 out_s;
	vector3 s;

	unsigned int objID = cdCode;
	objID = objID >> 23;
	objID = objID & 0x7f;

	unsigned int numEdges = cdCode & 0x07;
	//
	flg_hit = false;
	unsigned int edgeCode = cdCode >> 3;
	CS_ENV_OBJECT *envObj = mEnvObjProvider.getEle( objID );
	//
	float contact_time = -1.0;
	/*
	{
		bool flg = envObj->isContained( q0, q1, r );
		if ( flg ) {
		float ct;
		vector3 fdir;
		s = envObj->computeSeparationVector_Circle( q0, q1, r, flg, ct, fdir );
		if ( flg ) {
			out_s = s;
			if ( contact_time < 0.0 || contact_time > ct ) {
				contact_time = ct;
				feasible_direction = fdir;
			}
			flg_hit = true;
		}
		}
	}
	return out_s;
	*/
	//------------------------------------------
	
	for ( int i = 0; i < numEdges; ++i ) {
		bool flg;
		float ct;
		vector3 fdir;
		unsigned int edgeID = edgeCode & 0xf;
		edgeCode = edgeCode >> 4;
		s = envObj->computeSeparationVector_Circle_Edge( q0, q1, r, flg, ct, fdir, edgeID );
		if ( flg == false ) continue;
		out_s = s;
		if ( contact_time < 0.0 || contact_time > ct ) {
			contact_time = ct;
			feasible_direction = fdir;
		}
		flg_hit = true;
	}
	return out_s;
}

vector3 CS_ENVIRONMENT::computeSeparationVector_Circle_EncodedScheme_Type_02( unsigned int cdCode, const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	vector3 out_s;
	vector3 s;

	unsigned int objID00 = cdCode;
	objID00 = objID00 >> 23;
	objID00 = objID00 & 0x7f;

	//
	flg_hit = false;
	CS_ENV_OBJECT *envObj = mEnvObjProvider.getEle( objID00 );
	//
	float contact_time = -1.0;
	unsigned int edgeCode = cdCode >> (7+8);
	unsigned int prev_edgeID = 9999999; //invalid edge ID
	int numEdges = 2;
	for ( int i = 0; i < numEdges; ++i ) {
		bool flg;
		float ct;
		vector3 fdir;
		unsigned int edgeID = edgeCode & 0xf;
		if ( prev_edgeID == edgeID ) break;
		prev_edgeID = edgeID;
		edgeCode = edgeCode >> 4;
		s = envObj->computeSeparationVector_Circle_Edge( q0, q1, r, flg, ct, fdir, edgeID );
		if ( flg == false ) continue;
		out_s = s;
		if ( contact_time < 0.0 || contact_time > ct ) {
			contact_time = ct;
			feasible_direction = fdir;
		}
		flg_hit = true;
	}
	//
	objID00 = cdCode;
	objID00 = objID00 >> 8;
	objID00 = objID00 & 0x7f;
	edgeCode = cdCode;
	prev_edgeID = 9999999; //invalid edge ID
	envObj = mEnvObjProvider.getEle( objID00 );
	for ( int i = 0; i < numEdges; ++i ) {
		bool flg;
		float ct;
		vector3 fdir;
		unsigned int edgeID = edgeCode & 0xf;
		if ( prev_edgeID == edgeID ) break;
		prev_edgeID = edgeID;
		edgeCode = edgeCode >> 4;
		s = envObj->computeSeparationVector_Circle_Edge( q0, q1, r, flg, ct, fdir, edgeID );
		if ( flg == false ) continue;
		out_s = s;
		if ( contact_time < 0.0 || contact_time > ct ) {
			contact_time = ct;
			feasible_direction = fdir;
		}
		flg_hit = true;
	}
	//
	return out_s;
}

vector3 CS_ENVIRONMENT::computeSeparationVector_Circle_EncodedScheme( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	vector3 out_s;
	vector3 s;
	flg_hit = false;
	feasible_direction.zero( );
	int gridIndex = computeGridIndex_EncodedScheme( q0, q1 );

	unsigned int cdCode = mEncodedCDData[ gridIndex ];
	if ( cdCode == 0 ) return out_s;

	unsigned int type = cdCode & 0xc0000000;
	if ( type == 0x40000000 ) {
		return computeSeparationVector_Circle_EncodedScheme_Type_01( 
			cdCode, q0, q1, r, flg_hit, feasible_direction);
	}

	if ( type == 0x80000000 ) {
		return computeSeparationVector_Circle_EncodedScheme_Type_02( 
			cdCode, q0, q1, r, flg_hit, feasible_direction);
	}


	return out_s;
}

vector3 CS_ENVIRONMENT::computeSeparationVector_Circle( const vector3 & q0, const vector3 & q1, float r, bool &flg_hit, vector3 &feasible_direction ) const
{
	switch( mCDEncodingSchemeType ) {
		case 0:
			return computeSeparationVector_Circle_NormalScheme( q0, q1, r, flg_hit, feasible_direction );
			break;
		case 1:
			return computeSeparationVector_Circle_EncodedScheme( q0, q1, r, flg_hit, feasible_direction );
			break;
		case 2:
			return computeSeparationVector_Circle_BVHScheme( q0, q1, r, flg_hit, feasible_direction );
			break;
		default:
			return computeSeparationVector_Circle_NormalScheme( q0, q1, r, flg_hit, feasible_direction );
			break;
	}
	
	return vector3(0.0, 0.0, 0.0);
}

void decode_CDCode( unsigned int cdCode ) {
	cout << "decode_CDCode:" << cdCode << endl;
	unsigned int type;
	unsigned int objID;
	unsigned int numEdges;
	type = cdCode >> 30;
	cout << "type:" << type << endl;

	if ( type == 1 ) {
	objID = cdCode >> 23;
	objID &= 0x7f;
	cout << "objID:" << objID << endl;
	numEdges = cdCode;
	numEdges &= 0x7;
	cout << "numEdges:" << numEdges << endl;
	unsigned int edgeCode = cdCode;
	edgeCode = edgeCode >> 3;
	
	for ( int i = 0; i < numEdges; ++i ) {
		unsigned int edgeID = edgeCode;	
		edgeID &= 0xf;
		cout << "edgeID:" << edgeID << endl;
		edgeCode = edgeCode >> 4;
		//if ( edgeID == 5 ) system("pause");

	}
	}

	if ( type == 2 ) {
		objID = cdCode >> 23;
		objID &= 0x7f;
		cout << "objID 0:" << objID << endl;
		unsigned int edgeCode; 
		unsigned int edgeID;
		//
		edgeCode = cdCode >> 7 + 8;
		edgeCode = edgeCode &0xff;
		edgeID = edgeCode;
		edgeID &= 0xf;
		cout << "edgeID:" << edgeID << endl;
		//
		edgeID = edgeCode >> 4;
		edgeID &= 0xf;
		cout << "edgeID:" << edgeID << endl;
		//
		//
		objID = cdCode >> 8;
		objID &= 0x7f;
		cout << "objID 1:" << objID << endl;
		//
		edgeCode = cdCode;
		edgeCode = edgeCode &0xff;
		edgeID = edgeCode;
		edgeID &= 0xf;
		cout << "edgeID:" << edgeID << endl;
		//
		edgeID = edgeCode >> 4;
		edgeID &= 0xf;
		cout << "edgeID:" << edgeID << endl;
	}
}

unsigned int CS_ENVIRONMENT::computeCDCode( const vector3 &p, float r )
{
	const int maxNumObjs = 4;
	int numEdges[maxNumObjs];
	int edgeArr[maxNumObjs][32];
	int objIDArr[maxNumObjs];
	unsigned int cdCode = 0;

	mEnvObjProvider.resetIterator( );
	CS_ENV_OBJECT *envObj;
	float contact_time = -1.0;
	int numObjs = 0;
	int objID = 0;
	while ( envObj = mEnvObjProvider.getNext( ) ) {
		bool flg = envObj->isContained( p, p, r );
		if ( flg == false ) {
			++objID;
			continue;
		}
		//objIDArr[ numObjs ] = objID;
		objIDArr[ numObjs ] = envObj->getImplicitIndex( );
		
		envObj->detectProximity_Edges( p, r, edgeArr[numObjs], numEdges[ numObjs ] );
		//cout << "numEdges:" << numEdges[ numObjs ] << endl;
		
		++objID;
		if ( numEdges[ numObjs ] > 0 ) {
			++numObjs;
		}
		if ( numObjs >= maxNumObjs ) {
			break;
		}
	}

	cdCode = 0;

	if ( numObjs >= 1 ) {
		if ( numObjs == 1 ) {
			//cout << "numObjs:" << numObjs << endl;
			cdCode = 0x40000000;
			unsigned int objCode = objIDArr[ 0 ] << 23;
			cdCode += objCode;
			cdCode |= numEdges[ 0 ];
			unsigned int edgeCode = 0;
			for ( int i = 0; i < numEdges[ 0 ]; ++i ) {
				edgeCode |= edgeArr[ 0 ][i];
				if ( i < numEdges[ 0 ] - 1 ) {
					edgeCode = edgeCode << 4;
				}
			}
			edgeCode = edgeCode << 3;
			cdCode |= edgeCode;
//BEGIN OUTPUT**************************************************************8
			/*
			cout << "objIDArr[ 0 ]:" << objIDArr[ 0 ] << endl;
			cout << "numEdges[ 0 ]:" << numEdges[ 0 ] << endl;
			for ( int i = 0; i < numEdges[ 0 ]; ++i ) {
				cout << "Edge ID:" << edgeArr[ 0 ][i] << endl;
			}
			decode_CDCode( cdCode );
			cout << "-----" << endl;
			*/
//END OUTPUT**************************************************************8

			
		} 
		if ( numObjs == 2 ) {

			
			cdCode = 0x80000000;
			unsigned int objCode;
			objCode = objIDArr[ 0 ] << 23;
			cdCode += objCode;

			objCode = objIDArr[ 1 ] << 8;
			cdCode += objCode;

			unsigned int edgeCode;
			if ( numEdges[ 0 ] > 2 ||
				numEdges[ 1 ] > 2 ) {
					cout << "***error" << endl;
					cout << "numEdges[ 0 ]:" << numEdges[ 0 ] << endl;
					cout << "numEdges[ 1 ]:" << numEdges[ 1 ] << endl;
			}
			if ( numEdges[ 0 ] == 1 ) {
				edgeArr[ 0 ][ 1 ] = edgeArr[ 0 ][ 0 ];
				
			}
			numEdges[ 0 ] = 2;
			edgeCode = 0;
			for ( int i = 0; i < numEdges[ 0 ]; ++i ) {
				edgeCode |= edgeArr[ 0 ][i];
				if ( i < numEdges[ 0 ] - 1 ) {
					edgeCode = edgeCode << 4;
				}
			}
			cdCode += edgeCode << (7+8);
			//
			if ( numEdges[ 1 ] == 1 ) {
				edgeArr[ 1 ][ 1 ] = edgeArr[ 1 ][ 0 ];
				
			}
			numEdges[ 1 ] = 2;
			edgeCode = 0;
			for ( int i = 0; i < numEdges[ 1 ]; ++i ) {
				edgeCode |= edgeArr[ 1 ][i];
				if ( i < numEdges[ 1 ] - 1 ) {
					edgeCode = edgeCode << 4;
				}
			}
			cdCode += edgeCode;
			/////////////////////////////
		//BEGIN OUTPUT**************************************************************8
			//cout << "===" << endl;
			/*
			for (int j = 0; j < 2; ++j ) {
				cout << "objIDArr[ j ]:" << objIDArr[ j ] << endl;
				cout << "numEdges[ j ]:" << numEdges[ j ] << endl;
				for ( int i = 0; i < numEdges[ j ]; ++i ) {
					cout << "Edge ID:" << edgeArr[ j ][i] << endl;
				}
				
			}
			decode_CDCode( cdCode );
			cout << "-----" << endl;
			*/
//END OUTPUT**************************************************************8	
		}

		
		
		if ( numObjs >= 3 ) {
			cout << "error ******numObjs:" << numObjs << endl;
			system("pause");
		}
	}


	return cdCode;
}

int CS_ENVIRONMENT::computeGridIndex_EncodedScheme( const vector3 & q0, const vector3 & q1 ) const
{
	int index;
	float x = q0.x - mMinAABB_CD.x;
	float z = q0.z - mMinAABB_CD.z;

	int ix = x/mCDGrid_DX;
	int iz = z/mCDGrid_DZ;

	if ( ix >= mEncodedCD_GridNX ) ix = mEncodedCD_GridNX - 1;
	if ( iz >= mEncodedCD_GridNZ ) iz = mEncodedCD_GridNZ - 1;

	index = iz*mEncodedCD_GridNX + ix;
	return index;
}

void CS_ENVIRONMENT::precomputeCollisionDetectionAcceleratedStructure_EncodingScheme( )
{
	
	cout << "BEGIN CS_ENVIRONMENT::precomputeCollisionDetectionAcceleratedStructure_EncodingScheme( )" << endl;

	//mEncodedCD_GridNX = 100;
	//mEncodedCD_GridNZ = 100;

	if ( mEncodedCDData == 0 ) {
		int cdDataSize = mEncodedCD_GridNX*mEncodedCD_GridNZ;
		mEncodedCDData = new unsigned int[cdDataSize];
	}

	computeAABB( mMinAABB_CD, mMaxAABB_CD );
	mCDGrid_WX = mMaxAABB_CD.x - mMinAABB_CD.x;
	mCDGrid_WZ = mMaxAABB_CD.z - mMinAABB_CD.z;

	mCDGrid_DX = mCDGrid_WX/mEncodedCD_GridNX;
	mCDGrid_DZ = mCDGrid_WZ/mEncodedCD_GridNZ;


	cout << "mCDGrid_WX:" << mCDGrid_WX << endl;
	cout << "mCDGrid_WZ:" << mCDGrid_WZ << endl;
	cout << "mCDGrid_DX:" << mCDGrid_DX << endl;
	cout << "mCDGrid_DZ:" << mCDGrid_DZ << endl;

	float agent_speed = 1.5;
	float time_step = 0.333333;
	float f = 2.0;
	float r = agent_speed*time_step*f;
	float max_d = mCDGrid_DX;
	if ( max_d < mCDGrid_DZ ) max_d = mCDGrid_DZ;
	r += max_d;
	r += csp_Generator_AgentRadiusMax;
	//
	int numZero = 0;
	int numOne = 0;
	int numTwo = 0;
	int numThree = 0;


	int index = 0;
	for ( int j = 0; j < mEncodedCD_GridNZ; ++j ) {
		float z = mMinAABB_CD.z + mCDGrid_DZ*0.5 + mCDGrid_DZ*j;
		for ( int i = 0; i < mEncodedCD_GridNX; ++i, ++index ) {
			float x = mMinAABB_CD.x + mCDGrid_DX*0.5 + mCDGrid_DX*i;
			
			unsigned int cdCode = computeCDCode( vector3(x, 0, z), r ); 

			unsigned int type = cdCode >> 30;
			if ( type == 0 ) {
				++numZero;
			}
			if ( type == 1 ) {
				++numOne;
			}
			if ( type == 2 ) {
				++numTwo;
			}
			if ( type == 3 ) {
				++numThree;
			}

			
			mEncodedCDData[ index ] = cdCode;

		}
	}

	cout << "numZero:" << numZero << endl;
	cout << "numOne:" << numOne << endl;
	cout << "numTwo:" << numTwo << endl;
	cout << "numThree:" << numThree << endl;
	//
	cout << "END CS_ENVIRONMENT::precomputeCollisionDetectionAcceleratedStructure_EncodingScheme( )" << endl;
}

//void CS_ENVIRONMENT::setDijkstraPoints(CS_DIJKSTRA *mDijkstra, float mMapMinX, float mMapMaxX, float mMapMinZ, float mMapMaxZ){
//
//	int edgeStartIterator = 0;
//	int edgeEndIterator = -1;
//
//	mEnvObjProvider.resetIterator();
//	CS_ENV_OBJECT *envObj;
//	CS_LINE_SEGMENT tmpEdge;
//	cout << "setDijkstra" << endl;
//	while (envObj = mEnvObjProvider.getNext()) {
//		cout << "setDijkstra" << endl;
//		for (int i = 0; i < (int)envObj->ptArr_extendedObj.size() - 1; i++)
//		{
//			tmpEdge.n = envObj->ptArr_extendedObj[i].n;
//			tmpEdge.p0 = envObj->ptArr[i];
//			tmpEdge.p1 = envObj->ptArr[i + 1];
//			mDijkstra->addObstacleEdge(tmpEdge);
//			edgeEndIterator++;
//		}
//		tmpEdge.n = envObj->ptArr_extendedObj.back().n;
//		tmpEdge.p0 = envObj->ptArr.back();
//		tmpEdge.p1 = envObj->ptArr.front();
//		mDijkstra->addObstacleEdge(tmpEdge);
//		edgeEndIterator++;
//
//
//		mDijkstra->addNode(envObj->ptArr.front(), edgeStartIterator, edgeEndIterator);		//add the first point
//		for (int i = 1; i < (int)envObj->ptArr.size() - 1; i++)
//		{
//			mDijkstra->addNode(envObj->ptArr[i], edgeStartIterator + i - 1, edgeStartIterator + i);
//		}
//		mDijkstra->addNode(envObj->ptArr.back(), edgeEndIterator - 1, edgeEndIterator);
//
//		edgeStartIterator = edgeEndIterator + 1;													//add the last point
//	}
//
//	mDijkstra->initStarts(mMapMinX, mMapMaxX, mMapMinZ, mMapMaxZ);
//}