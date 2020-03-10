#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdexcept>
#include <algorithm>
#include "define.h"

template <typename TIndex, typename TValue>
class Vector
{
public:
    explicit Vector() 
    {
        _mem = nullptr;
        _size = 0;
        //std::cout<<"go through default constructor \n";
    }

    explicit Vector(TIndex size)
    {
        _mem = new TValue [size];
        _size = size;
        //std::cout<<"go through size constructor " << _size <<" \n" ;
    }

    explicit Vector(const Vector &other)
    {
        _mem = new TValue [other._size];
        _size = other._size;
        std::copy(other._mem, other._mem + _size, _mem);
        //std::cout<<"go through here \n";
    }
    
    void resize(TIndex size)
    {
        delete [] _mem;
        _mem = nullptr;
        _size = 0;
        _mem = new TValue [size];
        _size = size;
    }

    ~Vector()                                  
    { 
        //std::cout<<"\nCall deletor" <<std::endl;
        if (_size == 0)
        {
            return;
        }
        delete [] _mem; 
        _mem = nullptr; 
        _size = 0;
    }

#ifdef VECTOR_RANGE_CHECK
    TValue &        operator[](TIndex i)       { if  (i >= _size) { throw std::out_of_range("out of range"); } return _mem[i]; }
    const TValue &  operator[](TIndex i) const { if  (i >= _size) { throw std::out_of_range("out of range"); } return _mem[i];}
#else
    TValue &        operator[](TIndex i)       { return _mem[i]; }
    const TValue &  operator[](TIndex i) const { return _mem[i]; }
#endif
    TIndex          size() const               { return _size; }

private:
    TValue * _mem  = nullptr;
    TIndex   _size = 0;
};

#endif // __VECTOR_H__    
