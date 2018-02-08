//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/10/09
//
//------------------------------

#ifndef __PROVIDER_H_
#define __PROVIDER_H_

/*
Assign each element an ID from 0 to mMaxNumOfElements-1.
*/
template <class T> class PROVIDER {
	protected:
		T *mEleArr;
		int mMaxNumOfElements;
		int *mFreeEleIndexArr;
		int mCurNumOfFreeElements;
		int *mUsedEleIndexArr[2];
		int mCurNumOfUsedElements[2];
		int mUsedEleIndexArrIndex;
		//
		bool *mFlgForIDArr; // false: not used yet. true: used.
		// use the array index as the ID
		int mCurNextFreeIDIndex; // auto updated in autoSetUniqueObjID
		//
		mutable int _mCurIndexNumber_Iteration;
public:
	PROVIDER( ) {
		mEleArr = 0;
		mMaxNumOfElements = 0;
		mCurNumOfFreeElements = 0;
		mFreeEleIndexArr = 0;
		mUsedEleIndexArr[0] = 0;
		mUsedEleIndexArr[1] = 0;
		mCurNumOfUsedElements[0] = 0;
		mCurNumOfUsedElements[1] = 0;
		mUsedEleIndexArrIndex = 0;

		mFlgForIDArr = 0;
		mCurNextFreeIDIndex = 0;

	}

	void autoSetUniqueObjID( T *obj ) {
		int ID = obj->getObjID( );
		if ( ID >= 0 && ID < mMaxNumOfElements ) {
		if ( mFlgForIDArr[ ID ] == false ) {
				mFlgForIDArr[ ID ] = true;
				if ( mCurNextFreeIDIndex > ID ) {
					mCurNextFreeIDIndex = ID + 1;
				}
				return;
		}
		}
		for ( int i = mCurNextFreeIDIndex; i < mMaxNumOfElements; ++i ) {
			if ( mFlgForIDArr[ i ] == false ) {
				mFlgForIDArr[ i ] = true;
				obj->setObjID( i );
				mCurNextFreeIDIndex = i + 1;
				break;
			}
		}
	}

	int getNumEle( ) const {
		return mCurNumOfUsedElements[mUsedEleIndexArrIndex];
	}

	T *getObjArr( ) const
	{
		return mEleArr;
	}

	T* init( int num ) {
		mMaxNumOfElements = num;
		mEleArr = new T[mMaxNumOfElements];
		mFreeEleIndexArr = new int[mMaxNumOfElements];
		mUsedEleIndexArr[0] = new int[mMaxNumOfElements];
		mUsedEleIndexArr[1] = new int[mMaxNumOfElements];
		mFlgForIDArr = new bool[mMaxNumOfElements];
		mCurNumOfFreeElements = mMaxNumOfElements;
		reset( );
		
		return mEleArr;
	}

	void reset( )
	{
		mCurNextFreeIDIndex = 0;
		mCurNumOfFreeElements = mMaxNumOfElements;
		for (int i = 0; i < mMaxNumOfElements; ++i ) {
			mEleArr[i]._index = i;
			mFreeEleIndexArr[i] = i;
			mFlgForIDArr[ i ] = false;
		}
		mCurNumOfUsedElements[0] = 0;
		mCurNumOfUsedElements[1] = 0;
		mUsedEleIndexArrIndex = 0;
	}

	// -1: invalid
	int getFreeElement( )
	{
		if ( mCurNumOfFreeElements <= 0 ) {
			return -1;
		}
		--mCurNumOfFreeElements;
		int index = mCurNumOfFreeElements;
		
		mUsedEleIndexArr[mUsedEleIndexArrIndex][ mCurNumOfUsedElements[mUsedEleIndexArrIndex] ] = mFreeEleIndexArr[index];
		++mCurNumOfUsedElements[mUsedEleIndexArrIndex];
		return mFreeEleIndexArr[index];
	}

	T *getEleBasedOnObjID( int objID ) const
	{
		resetIterator( );
		T *obj;
		while ( obj = getNext( ) ) {
			if ( obj->getObjID( ) == objID ) break;
		}
		return obj;
	}

	T *getEle( int objIndex ) const {
		return &mEleArr[ objIndex ];
	}

	int *getUsedEleIndexArr( int &num ) const {
		num = mCurNumOfUsedElements[mUsedEleIndexArrIndex];
		return mUsedEleIndexArr[mUsedEleIndexArrIndex];
	}

	void begin_update( )
	{
		int arrIndex = (mUsedEleIndexArrIndex+1)%2; //cyclic 0->1->0-> ...etc
		mCurNumOfUsedElements[ arrIndex ] = 0;
	}

	void updateUsedElement( T *obj ) 
	{
		updateUsedElement( obj->_index );
	}

	void updateUsedElement( int index ) 
	{
		int arrIndex = (mUsedEleIndexArrIndex+1)%2;
		if ( mEleArr[index].mFlgUsed == false ) {
			mFreeEleIndexArr[mCurNumOfFreeElements] = index;
			++mCurNumOfFreeElements;
			int ID = mEleArr[ index ].getObjID( );
			if ( ID >= 0 && ID < mMaxNumOfElements ) {
			mFlgForIDArr[ ID ] = false;
			if ( mCurNextFreeIDIndex > ID ) {
				mCurNextFreeIDIndex = ID;
			}
			}
		} else {
			mUsedEleIndexArr[arrIndex][ mCurNumOfUsedElements[arrIndex] ] = index;
			++mCurNumOfUsedElements[arrIndex];
		}
	}

	void end_update( )
	{
		mUsedEleIndexArrIndex = (mUsedEleIndexArrIndex+1)%2;
	}

	void resetIterator( )  const
	{
		_mCurIndexNumber_Iteration = 0;
	}

	T *getNext( ) const {
		if ( _mCurIndexNumber_Iteration >= mCurNumOfUsedElements[mUsedEleIndexArrIndex] )  return 0;
		int objIndex = mUsedEleIndexArr[mUsedEleIndexArrIndex][_mCurIndexNumber_Iteration];
		++_mCurIndexNumber_Iteration;
		return &mEleArr[ objIndex ];
	}

	void autoUpdate( ) {
		begin_update( );
		resetIterator( );
		T *g;
		while ( g = getNext( ) ) {
			updateUsedElement( g );
		}
		end_update( );
	}
};

#endif