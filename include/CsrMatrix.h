/**
 * @file   sparseMatrix.h
 * @author Zhu Hanqing
 * @date   Jul 2019
 */
#ifndef _CSRMATRIX_H__
#define _CSRMATRIX_H__

#include <algorithm>
#include <iostream>

template <typename TIndex, typename TValue>
class CsrMatrix {
private:
    //statement
    TIndex* _rowOffset = nullptr;
    TIndex* _columnIndices = nullptr;
    TValue* _values = nullptr;

    TIndex _sizeMat = 0;
    TIndex _numNonZero = 0;

public:
    explicit CsrMatrix()  = default ;
    
    explicit CsrMatrix(TIndex size, TIndex sizeNonZeroMem)
        : _sizeMat(size), _numNonZero(sizeNonZeroMem)
    {
        _rowOffset = new TIndex [size + 1];
        _columnIndices = new TIndex [_numNonZero];
        _values = new TValue [_numNonZero];
        //std::cout << "Construct a matrix successfully \n";
    }
    
    explicit CsrMatrix(const CsrMatrix &other)
    {
        _sizeMat = other.size();
        _numNonZero = other.sizeNonZeroMem();
        
        _rowOffset = new TIndex [other._sizeMat];
        _columnIndices = new TIndex [other._numNonZero];
        _values = new TValue [other._numNonZero];
       
        std::copy(other._rowOffset, other._rowOffset + (_sizeMat+1), _rowOffset);
        std::copy(other._columnIndices, other._columnIndices + _numNonZero, _columnIndices); 
        std::copy(other._values, other._values + _numNonZero, _values);
        //std::cout << "Copy a matrix successfully \n";
    }

    void csrMatrixSizeSet(TIndex newSize)
    {    
        delete [] _columnIndices;
        _columnIndices = nullptr;
        delete [] _values;
        _values = nullptr;
        _numNonZero = 0;

        _columnIndices = new TIndex [newSize];
        _values        = new TValue [newSize];

        _numNonZero = newSize;
    }

    ~CsrMatrix()
    {
        delete [] _rowOffset;
        _rowOffset = nullptr;
        delete [] _columnIndices;
        _columnIndices = nullptr;
        delete [] _values;
        _values = nullptr;

        _sizeMat = 0;
        _numNonZero = 0;           
    }
    TValue* getValuePointer()
    {
        return _values;
    }
    
    // return pointer
    TIndex& rowOffsetVal(TIndex i) 
    {
        return _rowOffset[i];
    }

    const TIndex& rowOffsetVal(TIndex i) const
    {
        return _rowOffset[i];
    }

    TIndex& columnIndexVal(TIndex i) 
    {
        return _columnIndices[i];
    }
    const TIndex& columnIndexVal(TIndex i) const
    {
        return _columnIndices[i];
    }

    TValue& valuesVal(TIndex i) 
    {
        return _values[i];
    }
    const TValue& valuesVal(TIndex i) const
    {
        return _values[i];
    }

    TIndex  size()         const       { return _sizeMat;        } 
    TIndex  sizeNonZeroMem() const     { return _numNonZero; }

    void printMatrix()
    {
         //print Matrix
        TIndex tempNum;
        bool valueExistFlag = 0;
        for (TIndex i = 0; i < _sizeMat; ++i)
        {
            if (_rowOffset[i+1] - _rowOffset[i] != 0)
            {   
                tempNum = _rowOffset[i+1] - _rowOffset[i];
                //TIndex iii = 0;
                for (TIndex ii = 0; ii < _sizeMat; ++ii)
                {
                    TIndex flag = 0;
                    for (TIndex iii = 0; iii < tempNum; ++iii)
                    {
                        if(ii == _columnIndices[_rowOffset[i] + iii])
                        {
                            std::cout<<_values[_rowOffset[i] + iii]<<" ";
                            flag = 1;
                        }
                    }
                    if(flag == 0)
                    {
                        std:: cout << 0 << " ";
                    }
                }
            }
            else 
            {
                for (int ii = 0; ii<_sizeMat; ++ii){
                    std::cout << 0 <<" ";
                }
            }
            std::cout<< std::endl;                
        } 
        std::cout<<"finish one matrix print"<<std::endl;
    }
};
#endif

