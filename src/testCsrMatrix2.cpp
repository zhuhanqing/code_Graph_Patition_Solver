/**
 * @file   testCsrMatrix.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#include <iostream>
#include <vector>
#include <cmath>
#include "operator.h"
#include "Vector.h"
#include "CsrMatrix.h"
#include "GraphParser.h"
using std::cout;
using std::endl;

//using namespace std;
int main(){
    // initialize
    int n = 5;
    int e = 14;
    double m = 1;

    int a[6] = {0,3,6,8,11,14};
    int b[14] = {1,2,3,0,3,4,0,4,0,1,4,1,2,3};
    std::vector <int> r (a,a+6);
    std::vector <int> c (b,b+14);
    
    //parser
    GraphParser gp;
    gp.loadGraphFromFile("/home/local/eda03/hanqing/NewPartition/benchmark/simple.graph");
    
    std::cout<<"Try constructing CsrMat \n";
    CsrMatrix <int, double> test(n, e);
    CsrMatrix <int, double> test2(n ,e);
    CsrMatrix <int, double> test3(n, e);
    
    for (int i = 0; i < 6; ++i)
    {
        test.rowOffsetVal(i) = r[i];
        test3.rowOffsetVal(i) = r[i];
    }
    for (int i = 0; i < 14; ++i)
    {
        test.columnIndexVal(i) = c[i];
        test.valuesVal(i) = 1;

        test3.columnIndexVal(i) = c[i];
        test3.valuesVal(i) = 1;
    }
    
    CsrMatrix <int, double> test4(test);
    test4.printMatrix();
    CsrMatrix <int, double> test1(test);
    project_operator::csrMatrixSetFormat(gp, 1, test2);
    project_operator::csrMatrixSetFormat(gp, 2, test3);
    project_operator::csrMatrixMulNum(test, 0.4, test4);
    project_operator::csrMatrixSetFormat(gp, 4, test1);
    std::cout<<" test lip matrix" <<std::endl;
    test1.printMatrix();
    
    double *pp;
    pp=test4.getValuePointer();
    std::cout<<*(pp+2)<<std::endl;
    std::cout << "Matrix Test part finished"<<std::endl;
    
    *(pp+2) = 0.89;
    test.printMatrix();
    test2.printMatrix();
    test3.printMatrix();
    test4.printMatrix();
    
 
    Vector <int, double> testVector(5);    
    
    for(int i = 0; i < 5; ++i)
    {
        testVector[i] = 1;
    }

    Vector <int, double> testVector2(testVector);
    Vector <int, double> testResVector(5);
    Vector <int, double> testResVector2(5);
    Vector <int, double> testResVector3(5);
    project_operator::csrMatrixMulVec(test,testVector,testResVector);
    project_operator::vecMulNum (testVector, 0.5, testResVector2);    
    project_operator::vecMinusVec (testResVector, testResVector2,testResVector3);
    double mulRes = 0;
    mulRes = project_operator::vecMulVec (testResVector, testResVector2);
    
    for (int i = 0; i<5;++i)
    {
        cout<< testResVector[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i<5;++i)
    {
        cout<< testResVector2[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i<5;++i)
    {
        cout<< testResVector3[i]<<" ";
    }
    cout<<endl;
    project_operator::vecAddVec(testResVector,testResVector2,testResVector);
    for (int i = 0;i < 5; ++i)
    {
        cout<< testResVector[i] <<" ";
    }
    cout<<endl;
    
    cout << mulRes << " " << sqrt(mulRes) << "\nVector Test Part finished \n";
    return 0;
}
