/**
 * @file   testCsrMatrix.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#include <iostream>
#include <vector>
#include "operator.h"
#include "Vector.h"
#include "CsrMatrix.h"
using std::cout;
using std::endl;

//using namespace std;
int main(){
    int n = 5;
    int e = 7;

    int a[6] = {0,3,6,8,11,14};
    int b[14] = {2,3,4,1,4,5,1,5,1,2,5,2,3,4};
    std::vector <int> r (a,a+6);
    std::vector <int> c (b,b+14);
    
    std::cout<<"Try constructing CsrMat \n";
    CsrMatrix <int, double> test (n);
    CsrMatrix <int, double> test2 (n);
    CsrMatrix <int, double> test3 (n);
    test.setMatrixValue(n,e,r,c,0);
    test2.setMatrixValue(n,e,r,c,1);
    test3.setMatrixValue(n,e,r,c,2);
    // test multiplication
    Vector <int, double> _testVector(5);
    
    //set value for vector
    for(int i = 0; i < 5; ++i)
    {
        _testVector[i] = 0.5;
    }
    
    //test mul vec
    Vector <int, double> _testResVector(5);
    //_testResVector = project_operator::csrMatrixMulVec(test,_testVector);
    project_operator::csrMatrixMulVec(test,_testVector,_testResVector);

    for (int i = 0; i<5;++i)
    {
        cout<< _testResVector [i]<<" ";
    }
    cout<<endl;
#if 0
    /*
    Vector<int, int> ttt;
    ttt = test.getRowOffsetVec();
    for (int i = 0; i<5; ++i)
    {
        cout<<ttt[i]<<" ";
    }
    */
    //test mul num
    double tt = 0.5;
    CsrMatrix<int,double> testCopy;
    testCopy.CsrMatrixCopy(test);
    
    project_operator::csrMatrixMulNum(test,tt,testCopy);
    //testCopy.printMatrix();
    test.printMatrix();
    test2.printMatrix();
    test3.printMatrix();
#endif
    return 0;
}

