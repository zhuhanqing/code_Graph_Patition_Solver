/**
 * @file   operator.h
 * @author Hanqing Zhu
 * @date   Jul 2019
 */

#ifndef _OPERATOR_H__
#define _OPERATOR_H__

#include <iostream>
#include <vector>
#include "CsrMatrix.h"
#include "Vector.h"
#include "GraphParser.h"

namespace project_operator
{
template <typename TIndex, typename TValue>
void csrMatrixSetFormat (const GraphParser &gp,const TIndex typeId, CsrMatrix<TIndex, TValue>& mat)
{
    /// @ define typeId
    //  0  default
    //  1  Adjacency Matrix
    //  2  Diagonal  Matrix
    //  3  Laplacian Matrix need a alpha
    //data assign
    if (typeId == 1)
    {
        TIndex matSize = gp.num_nodes;
        TIndex nonZeroMemNum = gp.num_edges * 2;
        TValue m = 1;
        matSize += 1;
        //first implement Adjacency to test
        for (TIndex i = 0; i < matSize; ++i){
            mat.rowOffsetVal(i) = gp.range_v[i];
        }
        for (TIndex i = 0; i < nonZeroMemNum; ++i){
            mat.columnIndexVal(i) = gp.connect_v[i];
            mat.valuesVal(i) = m;
            //mat.valuesVal(i) = gp.values(i)
        }
    }
    else if (typeId == 2)
    {
        //data assign
        TIndex matSize;
        matSize = mat.size();
        mat.csrMatrixSizeSet(matSize);
        
        for (TIndex i = 0; i < matSize; ++i)
        {
            mat.valuesVal(i) = mat.rowOffsetVal(i + 1) - mat.rowOffsetVal(i);
        }
        
        for (TIndex i = 0;i < matSize; ++i)
        {
            mat.rowOffsetVal(i)   = i;
            mat.columnIndexVal(i) = i;
        }
        mat.rowOffsetVal(matSize) = matSize;
    }
    else if (typeId == 3)
    {
        //check error
        TIndex matValuesNum, matSize, cntOffset;
        TValue m = -1;
        TValue alpha;

        matValuesNum = gp.num_nodes + gp.num_edges * 2;
        matSize      = gp.num_nodes;
        mat.csrMatrixSizeSet(matValuesNum);

        //set rowOffset
        for (TIndex i = 0;i < matSize + 1; ++i)
        {
            mat.rowOffsetVal(i) = gp.range_v[i] + i;
        }
        //get alpha to contribute to convergence
        TValue maxDegOfMatrix = 0;
        TValue eachVertexDegree = 0;
        for (TIndex seekCnt = 0;seekCnt < matSize + 1; ++seekCnt)
        {
            eachVertexDegree = gp.range_v[seekCnt + 1] - gp.range_v[seekCnt];
            if (eachVertexDegree > maxDegOfMatrix)
            {
                maxDegOfMatrix = eachVertexDegree;
            }
        }
        alpha = TValue(1)/(maxDegOfMatrix + 1);
        //set matrix value
        for (TIndex rowPointer = 0;rowPointer < matSize; ++rowPointer)
        {
            cntOffset = gp.range_v[rowPointer + 1] - gp.range_v[rowPointer];
            for (TIndex j = 0; j < (gp.range_v[rowPointer + 1] - gp.range_v[rowPointer]); ++j)
            {
                mat.columnIndexVal(rowPointer + gp.range_v[rowPointer] + j) = gp.connect_v[gp.range_v[rowPointer] + j];
                mat.valuesVal(rowPointer + gp.range_v[rowPointer] + j)      = m * alpha;
            }
            mat.columnIndexVal(rowPointer + gp.range_v[rowPointer] + cntOffset)     = rowPointer;
            mat.valuesVal(rowPointer + gp.range_v[rowPointer] + cntOffset)          = cntOffset * alpha; 
        }

    }
    else 
    {
        std::cerr << "not define typeId ";
    }
}
template <typename TIndex, typename TValue>
void csrMatrixMulVec (CsrMatrix<TIndex, TValue> &mat, const std::vector<TValue> &vec, std::vector<TValue> &resVec)
{
    TIndex n = vec.size();
    for (TIndex i = 0; i < n; ++i)
    {
        resVec[i] = 0;
    }
    //std::fill(resVec.begin(), resVec.end(), 0);
    for (TIndex row = 0; row < n; ++row)
    {
        TIndex beg = mat.rowOffsetVal(row);
        TIndex end = mat.rowOffsetVal(row + 1);

        for (TIndex idx = beg; idx < end; ++idx)
        {
            TIndex col = mat.columnIndexVal(idx);
            resVec[row] += mat.valuesVal(idx) * vec[col];
        }
    }
}
template <typename TIndex, typename TValue>
void csrMatrixMulVec (const CsrMatrix<TIndex, TValue> &mat, const std::vector<TValue> &vec, std::vector<TValue> &resVec)
{
    TIndex n = vec.size();
    for (TIndex i = 0; i < n; ++i)
    {
        resVec[i] = 0;
    }
    //std::fill(resVec.begin(), resVec.end(), 0);
    for (TIndex row = 0; row < n; ++row)
    {
        TIndex beg = mat.rowOffsetVal(row);
        TIndex end = mat.rowOffsetVal(row + 1);

        for (TIndex idx = beg; idx < end; ++idx)
        {
            TIndex col = mat.columnIndexVal(idx);
            resVec[row] += mat.valuesVal(idx) * vec[col];
        }
    }
}
/*
//mat * vec
template <typename TIndex, typename TValue>
void csrMatrixMulVec (CsrMatrix<TIndex, TValue> &mat, const Vector<TIndex, TValue> &vec, Vector<TIndex, TValue> &resVec)
{
    TIndex n = vec.size();
    for (TIndex i = 0; i < n; ++i)
    {
        resVec[i] = 0;
    }
    //std::fill(resVec.begin(), resVec.end(), 0);
    for (TIndex row = 0; row < n; ++row)
    {
        TIndex beg = mat.rowOffsetVal(row);
        TIndex end = mat.rowOffsetVal(row + 1);

        for (TIndex idx = beg; idx < end; ++idx)
        {
            TIndex col = mat.columnIndexVal(idx);
            
            resVec[row] += mat.valuesVal(idx) * vec[col];
        }
    }
}

//matrix * vec overloading
template <typename TIndex, typename TValue>
void csrMatrixMulVec (const CsrMatrix<TIndex, TValue> &mat, const Vector<TIndex, TValue> &vec, Vector<TIndex, TValue> &resVec)
{
    int ttt = mat.rowOffsetVal(24);
;
    TIndex n = vec.size();
    for (TIndex i = 0; i < n; ++i)
    {
        resVec[i] = 0;
    }
;
    for (TIndex row = 0; row < n; ++row)
    {
        TIndex beg = mat.rowOffsetVal(row);
        TIndex end = mat.rowOffsetVal(row + 1);

        for (TIndex idx = beg; idx < end; ++idx)
        {
            TIndex col = mat.columnIndexVal(idx);
            TIndex col2 = mat.columnIndexVal(idx+1);
            TIndex col3 = mat.columnIndexVal(idx+2);
            resVec[row] += mat.valuesVal(idx) * vec[col];
;
        }
    }
}
*/
/// define function for vec
// vec * vec 
template <typename TValue>
TValue vecMulVec (const std::vector<TValue> &vec1, const std::vector<TValue> &vec2)
{
    TValue res  = 0;
    int vecSize = vec1.size();
    for (int cnt = 0; cnt < vecSize; ++cnt)
    {
        res += vec1[cnt] * vec2[cnt];
        //std::cout<<res<<" ";
    }
    //std::cout<<std::endl;
    return res;
}
/*
template <typename TIndex, typename TValue>
TValue vecMulVec (const Vector<TIndex, TValue> &vec1, const Vector<TIndex, TValue> &vec2)
{
    TValue res;
    for (TIndex cnt = 0; cnt < vec1.size(); ++cnt)
    {
        res += vec1[cnt] * vec2[cnt];
        //std::cout<<res<<" ";
    }
    //std::cout<<std::endl;
    return res;
}
*/
// vec * num 
template <typename TValue>
void vecMulNum (const std::vector<TValue> &vec1, const TValue num, std::vector<TValue> &resVec)
{
    int n = vec1.size();
    
    for (int cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] * num;
    }

}
/*
template <typename TIndex, typename TValue>
void vecMulNum (const Vector<TIndex, TValue> &vec1, const TValue num, Vector<TIndex, TValue> &resVec)
{
    TIndex n = vec1.size();
    
    for (TIndex cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] * num;
    }

}
*/
// vec + vec
template <typename TValue, typename TData>
void vecAddVec (const std::vector< TValue> &vec1, const std::vector<TData> &vec2, std::vector<TData> &resVec)
{
    int n = vec1.size();
    
    for (int cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] + vec2[cnt];
        //std::cout<< vec1[cnt] << " " << vec2[cnt] << " "<< resVec[cnt] << std::endl;
    }
}
/*
template <typename TIndex, typename TValue, typename TData>
void vecAddVec (const Vector<TIndex, TValue> &vec1, const Vector<TIndex, TData> &vec2, Vector<TIndex, TData> &resVec)
{
    TIndex n = vec1.size();
    
    for (TIndex cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] + vec2[cnt];
        //std::cout<< vec1[cnt] << " " << vec2[cnt] << " "<< resVec[cnt] << std::endl;
    }
}
*/
// vec - vec
template <typename TValue, typename TData>
void vecMinusVec (const std::vector<TValue> &vec1, const std::vector<TData> &vec2, std::vector<TData> &resVec)
{
    int n = vec1.size();
    
    for (int cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] - vec2[cnt];
    }
}
/*
template <typename TIndex, typename TValue, typename TData>
void vecMinusVec (const Vector<TIndex, TValue> &vec1, const Vector<TIndex, TData> &vec2, Vector<TIndex, TData> &resVec)
{
    TIndex n = vec1.size();
    
    for (TIndex cnt = 0; cnt < n; ++cnt)
    {
        resVec[cnt] = vec1[cnt] - vec2[cnt];
    }
}
*/
// matrix * num 
template<typename TIndex, typename TData, typename TValue>
void csrMatrixMulNum(CsrMatrix <TIndex, TData>& mat1, const TValue mulNum, CsrMatrix <TIndex, TData>& tempMulNumRes)
{
    
    TIndex n = mat1.sizeNonZeroMem();

    for (TIndex idx = 0; idx < n; ++idx)
    {
        tempMulNumRes.valuesVal(idx) = mat1.valuesVal(idx) * mulNum;
    }
}     
/*
template<typename TIndex, typename TData, typename TValue>
void csrMatrixMulNum(CsrMatrix <TIndex, TData>& mat1, const TValue mulNum, CsrMatrix <TIndex, TData>& tempMulNumRes)
{
    
    TIndex n = mat1.sizeNonZeroMem();

    for (TIndex idx = 0; idx < n; ++idx)
    {
        tempMulNumRes.valuesVal(idx) = mat1.valuesVal(idx) * mulNum;
    }
}
*/
}
#endif
