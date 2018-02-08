#include "ogl_basic_tools.h"

COLOR_TABLE::COLOR_TABLE( )
{
	mMaxValue = 0.0;
	mFlgAutoUpdate = true;
	mFlgAutoScale = true;
}

void COLOR_TABLE::resetMaxValue( float v )
{
	mMaxValue = v;
}

void COLOR_TABLE::enableAutoScale( bool flg )
{
	mFlgAutoScale = flg;
}

void COLOR_TABLE::clear( )
{
		mColorValueArr.clear( );
	mColorMappedValue.clear( );
}
void COLOR_TABLE::updateMaxValue( float v ) const
{
	if ( v > mMaxValue ) mMaxValue = v;
}

void COLOR_TABLE::autoScale( )
{
	if ( mFlgAutoScale == false ) return;
	float cur_max = mColorMappedValue.back( );
	int num = mColorMappedValue.size( );
	for ( int i = 0; i < num; ++i ) {
		mColorMappedValue[i] = (i/(float)(num - 1))*mMaxValue;
	}
}


void COLOR_TABLE::addEntry(float v, const vector3 &c ) //incremental
{
mColorValueArr.push_back( c );
mColorMappedValue.push_back( v );
}

vector3 COLOR_TABLE::getColor_Linear( float v ) const
{
	if ( mFlgAutoUpdate ) {
		updateMaxValue( v );
	}
	vector3 c;

	if ( mColorMappedValue.size( ) > 1 ) {
		if ( v <= mColorMappedValue[0] ) {
			c = mColorValueArr[0];
		} else if ( v >= mColorMappedValue.back( ) ) {
			c = mColorValueArr.back( );
		} else {
			int num = mColorMappedValue.size( ); 
			for ( int i = 1; i < num ; ++i ) {
				int j = num - i - 1;
				if ( v >= mColorMappedValue[j] ) {
					float f;
					f =  ( v - mColorMappedValue[ j ] )/( mColorMappedValue[ j + 1 ] - mColorMappedValue[ j ]);
					c = mColorValueArr[ j ]*(1.0 - f) + mColorValueArr[ j + 1 ]*f;
					break;
				} else {
				}
			}
		}
	} else {
		//only one color
		c = mColorValueArr[0];
	}
	return c;
}
