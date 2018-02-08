//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// Date: 2014/12/10
//
#ifndef __MAPPER_H_
#define __MAPPER_H_

template < class DA, class DB> class MAPPER_ARRAY_TO_SPARSE_DATA {
protected:
	int mMaxNumDataElements;
	int mCurNumDataElements;
	const DA *mDataArr;
	DB **mDataSparseArr;
public:
	MAPPER_ARRAY_TO_SPARSE_DATA( ) {
		mMaxNumDataElements = 256;
		mCurNumDataElements = 0;
		mDataSparseArr = new DB*[mMaxNumDataElements];
	}
	void setNumOfElements( int num ) {
		mCurNumDataElements = num;
	}
	void registerArr( const DA *arr ) { mDataArr = arr; }
	void registerSparseData( int index, DB *ptr_Element ) { 
		mDataSparseArr[index] = ptr_Element;
	}
	void update( ) {
		for ( int i = 0; i  < mCurNumDataElements; ++i ) 
		{
			DB *ptr;
			ptr = mDataSparseArr[i];
			*ptr = mDataArr[i];
		}
	}
};

#endif