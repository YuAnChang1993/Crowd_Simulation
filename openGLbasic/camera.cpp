#include "../headers.h"

#include "camera.h"

using namespace std;

CAMERA::CAMERA( )
{
	mWindowWidth = 512;
	mWindowHeight = 512;
	mZnear = 0.1;
	mZfar = 200;
	//
	mOrthoMarginX = 100.0;
	mOrthoMarginZ = 100.0;

	mFlgDragging = false;
	mCurX = 0;
	mCurY = 0;
	
	_mx = 24;
	_my = 30;
	left_up_corner_x = -95;
	left_up_corner_y = 70;

	//mType = CAMERA_TYPE_PERSPECTIVE;
	mType = CAMERA_TYPE_ORTHOGONAL;

	mZoomFactor = -600;
	mZoomChangeUnit = 1.0;

	mTranslationXYZ = vec3( 0.0, 0.0, 0.0 );
	if ( mType == CAMERA_TYPE_PERSPECTIVE ) {
		//mTranslationXYZ = vec3(0, 0, -10.0);
		mTranslationXYZ = vec3(-5.0, 5.0, -15.0);
	//mTranslationXYZ = vec3(-5.0, -5.0, -15.0);
	} else if ( mType ==  CAMERA_TYPE_ORTHOGONAL ) {
		mTranslationXYZ = vec3( 0.0, 0.0, 0.0 );
	}
}

void CAMERA::setViewport( int w, int h )
{
	mWindowWidth = w;
	mWindowHeight = h;

}

void CAMERA::init( )
{
	updateCamera( );

}

void CAMERA::translate(const vec3 &v)
{
	//cout << "translate" << endl;
	switch( mType ) {
		case CAMERA_TYPE_ORTHOGONAL:
		mZoomFactor += v.z*0.25;
		break;
		case CAMERA_TYPE_PERSPECTIVE:
		mTranslationXYZ += v;
		break;
	}
	updateCamera( );
}

void CAMERA::dragging( int x, int y )
{
	if ( mFlgDragging == false ) return;
	float dx = x - mCurX;
	float dy = y - mCurY;
	float sensitivity_factor = 0.005;
	float sx, sy;
	switch( mType ) {
		case CAMERA_TYPE_ORTHOGONAL:
			//mTranslationXYZ += sensitivity_factor*vec3(dx, -dy, 0)*5.0;
			//mOrthoMinX = xmin;
			//mOrthoMaxX = xmax;
			//mOrthoMinZ = ymin;
			//mOrthoMaxZ = ymax;

			sx = (mOrthoMaxX - mOrthoMinX)/mWindowWidth;
			sy = (mOrthoMaxZ - mOrthoMinZ)/mWindowHeight;

			mTranslationXYZ += vec3(sx*dx, -sy*dy, 0);
			//cout << "ORTHOGONAL!" << endl;
		break;
		case CAMERA_TYPE_PERSPECTIVE:
		mTranslationXYZ += sensitivity_factor*vec3(dx, -dy, 0);
		//cout << "PERSPECTIVE!" << endl;
		break;
	}
	updateCamera( );
	mCurX = x;
	mCurY = y;
}

void CAMERA::setDragging( int x, int y )
{
	mCurX = x;
	mCurY = y;

	mFlgDragging = true;

}

void CAMERA::endDragging( int x, int y )
{
	updateCamera( );
	mFlgDragging = false;

}

float CAMERA::getAbsoluteDistanceForPixels( int numPixels )
{
	float v = 0.0;
	float x0, y0, z0;
	float x1, y1, z1;

	getCoordinates( x0, y0, z0, 0, 0 );
	getCoordinates( x1, y1, z1, numPixels, 0 );

	v = x0 - x1;
	if ( v < 0.0 ) v = -v;
	return v;
}

void CAMERA::getCoordinates(float &x, float &y, float &z, float mx, float my)
{
	if ( mType != CAMERA_TYPE_ORTHOGONAL) {
		cout << "CAMERA::getCoordinates: wrong type" << endl;
		return;
	}
	// mx, my : mouse x, mouse y
	x = y = z = 0.0;

	float aspect_ratio;

	aspect_ratio = mWindowWidth/(GLfloat) mWindowHeight;

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float xw, zw;
	//xw = 10.0;
	//zw = 10.0;
	
	xw = mOrthoMarginX; // margin
	zw = mOrthoMarginZ;
	
	xmin = 	(- xw + mZoomFactor*mZoomChangeUnit);
	xmax = 	(+ xw - mZoomFactor*mZoomChangeUnit);
	float xWidth = xmax - xmin;
	zmin = -10.0;
	zmax = 10.0;

	ymin = 	(- zw + mZoomFactor*mZoomChangeUnit)/aspect_ratio;
	ymax = 	(+ zw - mZoomFactor*mZoomChangeUnit)/aspect_ratio;

	float yWidth = ymax - ymin;
	
	//mTranslationXYZ[0], mTranslationXYZ[1], mTranslationXYZ[2]

	float ix = mx / (float) mWindowWidth;
	//cout << mx << " " << my << endl;

	float iy = ( mWindowHeight - my ) / (float)mWindowHeight;

	ix = ix*xWidth-xWidth*0.5;
	iy = iy*yWidth-yWidth*0.5;

	//ix = (ix - xmin)/(xmax - xmin)*xWidth;
	//iy = (iy - ymin)/(ymax - ymin)*yWidth;

	ix -= mTranslationXYZ[0];
	iy -= mTranslationXYZ[1];

	x = ix;
	z = iy;
}

void CAMERA::getCoordinates(int &x, int &y) //2017/08/04
{
	left_up_corner_x = left_up_corner_y = 0;
	float aspect_ratio;

	aspect_ratio = mWindowWidth / (GLfloat)mWindowHeight;

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float xw, zw;
	//xw = 10.0;
	//zw = 10.0;

	xw = mOrthoMarginX; // margin
	zw = mOrthoMarginZ;

	xmin = (-xw + mZoomFactor*mZoomChangeUnit);
	xmax = (+xw - mZoomFactor*mZoomChangeUnit);
	float xWidth = xmax - xmin;
	zmin = -10.0;
	zmax = 10.0;

	ymin = (-zw + mZoomFactor*mZoomChangeUnit) / aspect_ratio;
	ymax = (+zw - mZoomFactor*mZoomChangeUnit) / aspect_ratio;

	float yWidth = ymax - ymin;

	//mTranslationXYZ[0], mTranslationXYZ[1], mTranslationXYZ[2]

	float ix = _mx / (float)mWindowWidth;
	//cout << mx << " " << my << endl;

	float iy = (mWindowHeight - _my) / (float)mWindowHeight;

	ix = ix*xWidth - xWidth*0.5;
	iy = iy*yWidth - yWidth*0.5;

	//ix = (ix - xmin)/(xmax - xmin)*xWidth;
	//iy = (iy - ymin)/(ymax - ymin)*yWidth;

	ix -= mTranslationXYZ[0];
	iy -= mTranslationXYZ[1];

	x = ix;
	y = iy;
}

void CAMERA::getOrthoActualMargins( float &xmin, float &xmax, float &zmin, float &zmax )
{
	xmin = mOrthoMinX-mTranslationXYZ[0];
	xmax = mOrthoMaxX-mTranslationXYZ[0];
	zmin = mOrthoMinZ-mTranslationXYZ[1];
	zmax = mOrthoMaxZ-mTranslationXYZ[1];
}

void CAMERA::updateCamera( )
{

	float aspect_ratio;

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float xw, zw;
	xw = mOrthoMarginX; // margin
	zw = mOrthoMarginZ;
	switch( mType ) {
		case CAMERA_TYPE_ORTHOGONAL:
			aspect_ratio = mWindowWidth/(GLfloat) mWindowHeight; // BUG FREE
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity( );
			xmin = 	(- xw + mZoomFactor*mZoomChangeUnit);
			xmax = 	(+ xw - mZoomFactor*mZoomChangeUnit);
			zmin = -10.0;
			zmax = 10.0;
			ymin = 	(- zw + mZoomFactor*mZoomChangeUnit)/aspect_ratio;
			ymax = 	(+ zw - mZoomFactor*mZoomChangeUnit)/aspect_ratio;
			glOrtho( xmin, xmax, ymin, ymax, zmin, zmax );
			mOrthoMinX = xmin;
			mOrthoMaxX = xmax;
			mOrthoMinZ = ymin;
			mOrthoMaxZ = ymax;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity( );
			glTranslatef(mTranslationXYZ[0], mTranslationXYZ[1], mTranslationXYZ[2]);
			glRotatef(-90.0, 1.0, 0, 0); // rotate 90 degrees about the x-axis
			break;
		case CAMERA_TYPE_PERSPECTIVE:
			aspect_ratio = mWindowWidth/(GLfloat) mWindowHeight; // BUG FREE
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity( );
			gluPerspective(45.0, aspect_ratio, mZnear,mZfar);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity( );
			glTranslatef(mTranslationXYZ[0], mTranslationXYZ[1], mTranslationXYZ[2]);
			//static float angle = 0.0;
			//angle += 0.1;
			//glRotatef(-90, 1, 0, 0);
			glRotatef(90, 1.0, 0, 0);
			//
			break;
	}
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX, mProjectMatrix);
}


void CAMERA::getProjectionMatrix( mat4 &m ) const
{
	m = mProjectMatrix;
}

void CAMERA::getModelViewMatrix( mat4 &m ) const
{
	m = mModelViewMatrix;
}