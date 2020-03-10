/**
 * @file   testLinearSolver.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#include <iostream>
#include "Vector.h"
#include "CsrMatrix.h"
#include "LinearSolver.h"

int main()
{
    Vector<int, double> b(2);
    Vector<int, double> x(2);

    CsrMatrix<int, double> A(2,4);
    
    //assign data
    b[0] = 1;
    b[1] = 2;

    x[0] = 2;
    x[1] = 1;
    
    A.rowOffsetVal(0) = 0;
    A.rowOffsetVal(1) = 2;
    A.rowOffsetVal(2) = 4;
    A.columnIndexVal(0) = 0;
    A.valuesVal(0)      = 4;
    A.columnIndexVal(1) = 1;
    A.valuesVal(1)      = 1;
    A.columnIndexVal(2) = 0;
    A.valuesVal(2)      = 1;
    A.columnIndexVal(3) = 1;
    A.valuesVal(3)      = 3;

    A.printMatrix();
    
    solver::conjGradSolver(A, b, x);
    
    std::cout << x[0] <<" " << x[1];
    return 0;
}
