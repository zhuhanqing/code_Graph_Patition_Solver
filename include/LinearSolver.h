/**
 * @file   LinearSolver.h
 * @method Conjugate Gradient Method
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#ifndef _LINEARSOLVER_H__
#define _LINEARSOLVER_H__

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include "CsrMatrix.h"
#include "operator.h"

#include <stdexcept>

namespace solver
{
    /// solve Ax = b 
    /// use Conjugate Gradient Method to get high convergence rate 
    /// no preconditioner now just CG algorithm
    template <typename TIndex, typename TValue>
    //void conjGradSolver(const CsrMatrix<TIndex, TValue>& mat, const Vector<TIndex, TValue>& vec, Vector<TIndex, TValue>& resVec,TValue tol)
    //void conjGradSolver(const CsrMatrix<TIndex, TValue>& mat, const Vector<TIndex, TValue>& vec, Vector<TIndex, TValue>& resVec,TValue tol)
    void conjGradSolver(const CsrMatrix<TIndex, TValue>& mat, const std::vector<TValue>& vec, std::vector<TValue>& resVec,TValue tol)
    {
        // mat = A, vec = b, resVec = x  To solve Ax = b
        
        //TValue tol = 1e-11;
        TIndex loopSize = vec.size();

        //define residual
        //Vector<TIndex, TValue> r(loopSize);
        //Vector<TIndex, TValue> tempVec(loopSize);
        std::vector<TValue> r(loopSize);
        std::vector<TValue> tempVec(loopSize);
        //compute r 
        project_operator::csrMatrixMulVec(mat, resVec, tempVec);
        project_operator::vecMinusVec(vec, tempVec, r);
        
        //Vector<TIndex, TValue> p(r);
        //Vector<TIndex, TValue> Ap(loopSize);
        std::vector<TValue> p(r);
        std::vector<TValue> Ap(loopSize);

        TValue rInnerProductOld, rInnerProductNew;
        TValue alpha, beta, tempValue;
        rInnerProductOld = project_operator::vecMulVec(r, r);

        std::cout<<"enter loop "<<"with a loopSize: "<<loopSize<<std::endl;
        // k loop to loopSize
        for (TIndex k = 0; k < loopSize; ++k)
        {
            project_operator::csrMatrixMulVec(mat, p, Ap);
            tempValue = project_operator::vecMulVec(p, Ap);
            alpha = rInnerProductOld / tempValue;

            //compute new x
            project_operator::vecMulNum(p, alpha, tempVec);
            project_operator::vecAddVec(resVec, tempVec, resVec);
            //compute new r
            project_operator::vecMulNum(Ap, alpha, tempVec);
            project_operator::vecMinusVec(r, tempVec, r);
            //compute r inner product
            rInnerProductNew = project_operator::vecMulVec(r, r);
            
            if (sqrt(rInnerProductNew) < tol or k == 150000)           
            {
                std::cout<<"check solver converge times: "<<k<<std::endl;
                break;
            } 
            
            beta = rInnerProductNew / rInnerProductOld;
            project_operator::vecMulNum(p, beta, tempVec);
            project_operator::vecAddVec(r, tempVec, p);
            rInnerProductOld = rInnerProductNew;
            
            if(k == 100 or k == 1000 or k == 10000 or k == 100000 or k == 200000 or k == 300000){
                std::cout<<"check repete times: "<<k<<" ";
            }
        }
    }
}
#endif
