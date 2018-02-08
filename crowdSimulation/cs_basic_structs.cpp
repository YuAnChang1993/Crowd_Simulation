//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#include "cs_basic_structs.h"
#include "../primitives/basic_tools.h"
#include "cs_parameters.h"


bool CS_PATH_POINT::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	float d2 = p.distance2( vector3(x, 0.0, z) );
	if ( r*r >= d2 ) return true;
	return flg;
}

void CS_PATH_POINT::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
	p.x += x;
	p.z += z;
}

void CS_PATH_POINT::bo_setNewPosition_XZ( float x, float z )
{
	p.x = x;
	p.z = z;
}

CS_PATH::CS_PATH( )
{
	mFlgUsed = false;
	mFlgActivated = false;
	numPoints = 0;
	maxPoints = 32;
	ptArr = new CS_PATH_POINT[maxPoints];
	mEdgeLoadIndex = 0;
	mEdgeLoadArr[ 0 ] = new float[maxPoints];
	mEdgeLoadArr[ 1 ] = new float[maxPoints];
	mEdgeLoadDiffArr = new float[maxPoints];
	//
	mWorsePositionArr = new vector3[ maxPoints ];
	mWorseValueArr = new float[ maxPoints ];
	mExpectedPathWidth = new float[ maxPoints ];
	maxProjectedDistance = new float[ maxPoints ];
	//
	trendOfProjectedDistance = new float[ maxPoints ];
	for ( int i = 0; i < 5; ++i ) {
		projectedDistanceRecords[i] = new float[ maxPoints ];
	}
	//
	for ( int i = 0; i < maxPoints; ++i ) {
		for ( int j = 0; j < 5; ++j ) {
			projectedDistanceRecords[j][i] = 0.0;
		}
		trendOfProjectedDistance[ i ] = 0.0;
	}
	mRecordIndexOfProjectedDistance = 0;
	//
	for ( int i = 0; i < 4; ++i ) {
		affectingSelfPointArr[i] = new vector3[ maxPoints ];
		affectingPointArr[i] = new vector3[ maxPoints ];
		affectingPointD2[i] = new float[ maxPoints ];
	}

	for ( int i = 0; i < numPoints; ++i ) {
		mEdgeLoadArr[mEdgeLoadIndex][i] = 0.0;
		mEdgeLoadDiffArr[i] = 0.0;
		mWorseValueArr[i] = 0.0;
		mExpectedPathWidth[i] = csp_agent_radius;
		maxProjectedDistance[ i ] = 0.0;
		
	}
	
	for ( int j = 0; j < 4; ++j ) {
		for ( int i = 0; i < numPoints; ++i ) {
			affectingPointD2[j][i] = -1.0; // not valid 
		}
	}

	

	pathColor = vector3( 0.0, 0.0, 0.0 );
	//
}

CS_PATH::~CS_PATH( )
{
}

void CS_PATH::resetWorsePosition( int node_index, float time_step  )
{
	if ( mWorseValueArr[ node_index ] == 0.0 ) return;
	mWorseValueArr[ node_index ] -= time_step;
	if ( mWorseValueArr[ node_index ] <= 0.0 ) {
		mWorseValueArr[ node_index ] = 0.0;
	}
	maxProjectedDistance[ node_index ] = 0.0;
}

bool CS_PATH::setWorseValue( int node_index, float v, const vector3 &p )
{
	bool flgWorse = false;
	if ( v > mWorseValueArr[ node_index ] ) {
		mWorseValueArr[ node_index ] = v;
		mWorsePositionArr[ node_index ] = p;
		flgWorse = true;
	}
	return flgWorse;
}

vector3 CS_PATH::getWorsePosition( int node_index, float &worseValue ) const
{
	worseValue = mWorseValueArr[ node_index ];
	return mWorsePositionArr[ node_index ];
}

void CS_PATH::getPathPoints( int edgeIndex, vector3 &p0, vector3 &p1 ) const
{
	int vIndex = edgeIndex;
	p0 = ptArr[ vIndex ].p;
	p1 = ptArr[ vIndex + 1].p;
}

void CS_PATH::resetLoad( )
{
	int prev_index = mEdgeLoadIndex;
	mEdgeLoadIndex = ( mEdgeLoadIndex + 1 )%2;
	for ( int i = 0; i < numPoints; ++i ) {
		maxProjectedDistance[ i ] = 0.0;
		mEdgeLoadDiffArr[i] = mEdgeLoadArr[prev_index][i] - mEdgeLoadArr[mEdgeLoadIndex][i];
		mEdgeLoadArr[mEdgeLoadIndex][i] = 0.0;
		mExpectedPathWidth[ i ] = csp_agent_radius*2.0;
	}

/*
	mRecordIndexOfProjectedDistance = 0;

	for ( int i = 0; i < numPoints; ++i ) {
		for ( int j = 0; j < 5; ++j ) {
			projectedDistanceRecords[j][i] = 0.0;
		}
		trendOfProjectedDistance[ i ] = 0.0;
	}
*/
	mRecordIndexOfProjectedDistance = (mRecordIndexOfProjectedDistance+1)%5;
	
}

void CS_PATH::addMaxProjectedDistance( int edgeIndex, float d_to_next_pathPoint, float proj_d )
{
	//cout << "d_to_next_pathPoint:" << d_to_next_pathPoint << endl;
	//cout << "proj_d:" << proj_d << endl;
	if ( d_to_next_pathPoint > 2.0*ptArr[ edgeIndex + 1].r ) {
		return;
	}
	if ( proj_d >= 0.0 && proj_d > maxProjectedDistance[ edgeIndex ] ) {

		maxProjectedDistance[ edgeIndex ] = proj_d;
	}

}

void CS_PATH::addLoadRecord( int edgeIndex, float w )
{
	mEdgeLoadArr[mEdgeLoadIndex][ edgeIndex ] += w;
}


float CS_PATH::computeExpectedPathWidth( int edgeIndex )
{
	int vIndex = edgeIndex;
	vector3 p0 = ptArr[ vIndex ].p;
	vector3 p1 = ptArr[ vIndex + 1].p;
	float L = p0.distance( p1 );
	mExpectedPathWidth[ edgeIndex ]  = mEdgeLoadArr[mEdgeLoadIndex][ edgeIndex ]/L;
	if ( mExpectedPathWidth[ edgeIndex ] <= csp_agent_radius*2.0 ) {
		mExpectedPathWidth[ edgeIndex ] = csp_agent_radius*2.0;
	}
	return mExpectedPathWidth[ edgeIndex ];
}

void CS_PATH::computeExpectedPathWidth( )
{
	for ( int i = 0; i < numPoints - 1; ++i ) {
		computeExpectedPathWidth( i );
		if ( mExpectedPathWidth[ i ] <= csp_agent_radius*2.0 ) {
			mExpectedPathWidth[ i ] = csp_agent_radius*2.0;
		}
	}
	if ( numPoints - 2 >= 0 ) {
		mExpectedPathWidth[ numPoints - 1 ] = mExpectedPathWidth[ numPoints - 2 ];
	}
}

void CS_PATH::resetNearestAffectingPointsForEdge( )
{
	for ( int j = 0; j < 4; ++j ) {
		for ( int i = 0; i < numPoints; ++i ) {
			affectingPointD2[ j ][ i ] = -1.0; // not valid 
		}
	}
}

void CS_PATH::setNearestAffectingPointsForEdge( int node_index, vector3 *near_p, vector3 *near_q, float *d2 )
{
	for ( int i = 0; i < 4; ++i ) {
		if ( d2[i] < 0.0 ) continue;
		if ( 
			(affectingPointD2[i][node_index] >= 0.0 
			&& d2[i] > affectingPointD2[i][node_index]) ) continue;
		affectingSelfPointArr[i][node_index] = near_p[i]; // affecting points

		affectingPointArr[i][node_index] = near_q[i]; // affecting points
		affectingPointD2[i][node_index] = d2[i];
	}

}

void CS_PATH::set( const CS_PATH &path )
{
	this->mFlgUsed = true;
	this->setActivatedFlag( true );
	this->setObjID( path.getObjID( ) );
	this->numPoints = 0;

	for (int i = 0; i < path.numPoints; ++i ) {
		addPoint( path.ptArr[i].p, path.ptArr[i].r );
	}
}

bool CS_PATH::bo_isPicked_XZPlane( float x, float z )
{
	bool flg = false;
	return flg;
}

void CS_PATH::bo_setNewPosition_XZ( float x, float z )
{
}

void CS_PATH::bo_translatePosition_XZ( float x, float z, float global_posx, float global_posz )
{
}



void CS_PATH::addPoint( float x, float y, float z )
{
	addPoint( vector3(x, y, z), r );
}

void CS_PATH::setRadius( float r )
{
	this->r = r;
	if ( numPoints > 0 ) this->ptArr[ numPoints - 1 ].r = r;
}

void CS_PATH::setPathNodePosition( const vector3 &p, int node_index )
{
	if ( numPoints == 0) return;
	ptArr[node_index].p = p;
}

vector3 CS_PATH::getEdgeDirectionBasedOnPathNode( int node_index ) const
{
	if ( node_index >= numPoints - 1 ) {
		return vector3( 0.0, 0.0, 0.0);
	}
	return ptArr[ node_index + 1 ].p - ptArr[ node_index ].p;
}

vector3 CS_PATH::getPathNodePosition( int node_index ) const
{
	if ( numPoints == 0) return vector3( 0.0, 0.0, 0.0 );
	return ptArr[node_index].p;
}

vector3 CS_PATH::getFirstPointPosition( ) const
{
	if ( numPoints == 0) return vector3( 0.0, 0.0, 0.0 );
	return ptArr[0].p;

}

//
// edge one : vert[node_index-1], vert[node_index]
// edge two : vert[node_index], vert[node_index+1]
//
void CS_PATH::computeClosestPointsOnTwoEdges( const vector3 &p, int node_index, vector3 &q0, vector3 &q1 )
{
	int n0_index = node_index - 1;
	int n1_index = node_index;
	vector3 p0, p1;
	p0 = ptArr[ n0_index ].p;
	p1 = ptArr[ n0_index + 1].p;

	q0 = bt_computeNearestPoint_LineSegment( p, p0, p1 );
	//
	p0 = ptArr[ n1_index ].p;
	p1 = ptArr[ n1_index + 1].p;
	q1 = bt_computeNearestPoint_LineSegment( p, p0, p1 );
}

void CS_PATH::clear( )
{
	numPoints = 0;
}

void CS_PATH::addPoint( const vector3 &p, float r )
{
	if ( numPoints >= maxPoints ) return;
	this->ptArr[numPoints].p = p;
	this->ptArr[numPoints].r = r;
	numPoints++;
}

void CS_PATH::setPickedPathNodeRadius( float r )
{
	if ( m_bo_CurPickedVertexIndex >= 0 ) {
		ptArr[m_bo_CurPickedVertexIndex].r = r;
	}
}

float CS_PATH::getPickedPathNodeRadius( ) const
{
	float r = 0.0;
	if ( m_bo_CurPickedVertexIndex >= 0 ) {
		r = ptArr[m_bo_CurPickedVertexIndex].r;
	}
	return r;
}


BASE_OBJ *CS_PATH::pickPathPoint_XZPlane( float x, float z ) const
{
	BASE_OBJ* obj = 0;
	for ( int i = 0; i < numPoints; ++i ) {
		bool flg;
		flg = ptArr[i].bo_isPicked_XZPlane( x, z );
		if ( flg ) {
			obj = &ptArr[i];
			break;
		}
	}
	return obj;
}

bool CS_PATH::canBePicked
( 
	const vector3 &p, 
	float hitDistance,
	float &shortest_distance2 
	)
{
	if ( numPoints <= 0 ) return false;
	m_bo_SelectedPoint = p;
	m_bo_FlgPicked = false;
	bool flg = false;
	m_bo_CurPickedVertexIndex = -1;
	m_bo_CurPickedEdgeIndex = -1;
	float hitDistance2 = hitDistance*hitDistance;
	for ( int i = 0; i < numPoints; ++i ) {
		float d2 = p.distance2( ptArr[i].p );
		if ( d2 > ptArr[i].r*ptArr[i].r ) {
			if ( hitDistance2 > 0.0 && d2 > hitDistance2 ) continue;
		}
		if ( shortest_distance2 < 0 || shortest_distance2 > d2 ) {
			shortest_distance2 = d2;
			m_bo_CurPickedVertexIndex = i;
			flg = true;
		}
	}

	if ( m_bo_CurPickedVertexIndex >= 0 ) {
		float r = ptArr[m_bo_CurPickedVertexIndex].r;
		float r2 = r*r;
		if ( r2 >= shortest_distance2 ) {
			shortest_distance2 = 0.0; // inside the circle; distance = 0
		}
	}
	float vertex_distance_threshold = 0.2;

	/*
	if ( shortest_distance2 >= 0.0 
		&& 
		vertex_distance_threshold > shortest_distance2 ) return flg;
		*/
	//

	float d2 = -1.0;

	int eIndex = bt_computeClosestPathEdgeIndex<CS_PATH_POINT>( p, ptArr, numPoints, d2 );

	//cout << "hitDistance2:" << hitDistance2 << endl;
	//cout << "d2:" << d2 << endl;

	if ( (shortest_distance2 < 0.0 || shortest_distance2 > d2) 
		&& ( hitDistance2 <= 0.0 || hitDistance2 > d2 )
		) {
		shortest_distance2 = d2;
		m_bo_CurPickedEdgeIndex = eIndex;
		m_bo_CurPickedVertexIndex = -1;
		flg = true;
	}

	cout << "CS_PATH::canBePicked" << endl;
	cout << "m_bo_CurPickedEdgeIndex:" << m_bo_CurPickedEdgeIndex << endl;
	cout << "m_bo_CurPickedVertexIndex:" << m_bo_CurPickedVertexIndex << endl;

	return flg;
}

void CS_PATH::moveSelectedVertexPosition_XZ( const vector3 &p )
{
	if ( m_bo_CurPickedVertexIndex < 0 ) return;
	if ( m_bo_CurPickedVertexIndex >= numPoints ) return;
	vector3 translated_vector = p - m_bo_SelectedPoint;

	//cout << "setSelectedVertexPosition_XZ, mCurPickedVertexIndex:" << mCurPickedVertexIndex << endl;
	ptArr[ m_bo_CurPickedVertexIndex ].p += translated_vector;
	m_bo_SelectedPoint = p;
	computeAABB( );
}

void CS_PATH::moveObjPosition_XZ( const vector3 &p )
{
	vector3 translated_vector = p - m_bo_SelectedPoint;
	translateAABB( translated_vector );
	for ( int i = 0; i < numPoints; ++i ) {
		ptArr[i].p += translated_vector;

	}
	m_bo_SelectedPoint = p;
}

void CS_PATH::computeAABB( vector3 &AABBmin, vector3 &AABBmax )
{
	computeAABB( );
	AABBmin = mAABBmin;
	AABBmax = mAABBmax;
}

void CS_PATH::computeAABB( )
{
	if ( numPoints == 0 ) {
		mAABBmin = mAABBmax = vector3(0.0, 0.0, 0.0);
		return;
	}
	float r = ptArr[0].r;
	mAABBmin = ptArr[0].p - vector3( r, 0.0, r );
	mAABBmax = ptArr[0].p + vector3( r, 0.0, r );

	for ( int i = 0; i < numPoints; ++i ) {
		vector3 p = ptArr[i].p;
		r = ptArr[i].r;
		if ( p.x - r < mAABBmin.x ) {
			mAABBmin.x = p.x - r;
		} else if ( p.x + r > mAABBmax.x ) {
			mAABBmax.x = p.x + r;
		} 

		if ( p.y - r < mAABBmin.y ) {
			mAABBmin.y = p.y - r;
		} else if ( p.y + r > mAABBmax.y ) {
			mAABBmax.y = p.y + r;
		} 

		if ( p.z -r < mAABBmin.z ) {
			mAABBmin.z = p.z - r;
		} else if ( p.z + r > mAABBmax.z ) {
			mAABBmax.z = p.z + r;
		} 
	}
}

CS_PATH_POINT *CS_PATH::getVertArr( ) const
{
	return ptArr;
}

int CS_PATH::getNumPoints( ) const
{
	return numPoints;
}

bool CS_PATH::isPickedObj( ) const
{
	return m_bo_CurPickedEdgeIndex >= 0;
}


bool CS_PATH::isPickedVertex( ) const
{
	return m_bo_CurPickedVertexIndex >= 0;
}

void CS_PATH::deletePickedVertex( )
{
	bool flg = false;
	if ( m_bo_CurPickedVertexIndex < 0 ) return;
	int j = 0;
	for ( int i = 0; i < numPoints; ++i ) {
		if ( m_bo_CurPickedVertexIndex == i ) {
			flg = true;
			continue;
		}
		ptArr[j].p = ptArr[i].p;
		ptArr[j].r = ptArr[i].r;
		++j;
	}

	if ( flg ) --numPoints;
}


void CS_PATH::printf_Data( ) const
{
}

float *CS_PATH::getExpectedPathWidth( ) const
{
	//mEdgeLoadArr[mEdgeLoadIndex]

	return mExpectedPathWidth;
}

void CS_PATH::getAffectingInfo( CS_PATH_AFFECT_DATA &g ) const
{
	for ( int i = 0; i < 4; ++i ) {
		g.affectingSelfPointArr[ i ] = affectingSelfPointArr[ i ];
		g.affectingPointArr[ i ] = affectingPointArr[ i ];
		g.affectingPointD2[ i ] = affectingPointD2[ i ];
		
	}
	g.maxProjectedDistance = maxProjectedDistance;
	//
	for ( int i = 0; i < numPoints; ++i ) {
		projectedDistanceRecords[ mRecordIndexOfProjectedDistance ][ i ] = maxProjectedDistance[ i ];
	}

	for ( int i = 0; i < numPoints; ++i ) {
		float total = 0.0;
		for ( int j = 0; j < 5; ++j ) {
			total += projectedDistanceRecords[ j ][i];
		}
		float cur_v = projectedDistanceRecords[ mRecordIndexOfProjectedDistance ][ i ];
		total -= cur_v;
		float avg = total/4;
		float dv = cur_v - avg;
		trendOfProjectedDistance[ i ] = dv;
	}
	//
	g.trendOfProjectedDistance = trendOfProjectedDistance;


}

void CS_PATH::showInfo( ) const
{
	cout << "Path information----" << endl;
	cout << "mObjID:" << mObjID << endl;
	cout << "numPoints:" << numPoints << endl;
	for ( int i = 0; i < numPoints; ++i ) {
		cout << "Path Width:" << i << "\t" << mExpectedPathWidth[ i ] << endl;
		cout << "proj_d:" << maxProjectedDistance[ i ] << endl;
		cout << "trendOfProjectedDistance:" << trendOfProjectedDistance[ i ] << endl;

		for ( int j = 0; j < 4; ++j ) {
			float d2 = affectingPointD2[ j ][ i ];
			if ( d2 >= 0.0 ) {
				float d = sqrt( d2 );
				cout << "j:" << j << "\tD:" << d << endl;
			}
		}
	}
}

