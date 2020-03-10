/**
 * @file   main.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#include <iostream>
#include <vector>
#include <string>
#include "GraphParser.h"
#include "CsrMatrix.h"
#include "operator.h"
#include "LinearSolver.h"
#include "buddleFosC.h"
#include <stdexcept>

int main ()
{   
    //parser
    GraphParser gp;
    gp.loadGraphFromFile("/home/zhuhanqing/zhuhanqing/hanqing/graphs/wing_nodal.graph");
    
    //gp.loadGraphFromFile("/home/zhuhanqing/zhuhanqing/hanqing/graphs/bcsstk32.graph");
    std::string outFileName;

    int numVertices = gp.num_nodes;
    int numConnect  = gp.num_edges * 2;
   
    CsrMatrix<int, double>G(numVertices, numConnect);
    project_operator::csrMatrixSetFormat(gp, 3, G);
     
    //Vector<int, int> partResVec(numVertices + 1);
    std::vector<int> partResVec(numVertices + 1);

    //param set area
    //int partitionNum = 3;
    int partitionNum = 12;

    int iterationNum = 5;
    
    int consolidarationNum = 10;

    double cgtorlence = 1e-11;

    double imbalance = 1.3;

    // define parallel computing threads number
    int threadsNum = 6;

    //seedGenFlag to control my seed generator 
    //0: defualt random select
    //1: BFS search
    int seedGenFlag = 0;

    outFileName = "wing_nodal_" + std::to_string(partitionNum) + "part_" + std::to_string(cgtorlence) +"_" + std::to_string(iterationNum ) +"iter_" + std::to_string(consolidarationNum) + "cons_search_src2";
    //outFileName = "144_12part_1e-11_3iter_10cons_search_src2";
    //outFileName ="mdual_6part_1e-11_10iter_5cons_search";
    
    std::cout << "test size and connect here:" << numVertices << " " <<  numConnect << std::endl;
   
    std::cout<<"start diffusion here: "<<std::endl;
    buddleFosCSolver::loadDiffusion(gp, G, partResVec, partitionNum, iterationNum, consolidarationNum,cgtorlence,seedGenFlag, imbalance, threadsNum);
    
    //store the partition result

    buddleFosCSolver::printPartitionResult(partResVec,outFileName);

    /*
    //check cutsize
    buddleFosCSolver::checkCutSize(gp,partResVec);
    */

    std::cout <<"<-------------------finish------------------------->"<<std::endl;
    return 0;
}
