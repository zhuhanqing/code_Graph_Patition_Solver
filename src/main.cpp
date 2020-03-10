/**
 * @file   main.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#include <iostream>
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
    //metis test
    gp.loadGraphFromFile("/home/local/eda03/zixuan/NewPar/benchmark/fe_4elt2.graph");
    
    //gp.loadGraphFromFile("../benchmark/bcsstk32.graph");
    std::string outFileName;

    int numVertices = gp.num_nodes;
    int numConnect  = gp.num_edges * 2;
    
   
    CsrMatrix<int, double>G(numVertices, numConnect);
    project_operator::csrMatrixSetFormat(gp, 3, G);
    //printMatrix();
     
    Vector<int, int> partResVec(numVertices + 1);

    //int partitionNum = 3;
    int partitionNum = 4;

    int iterationNum = 10;
    
    int consolidarationNum = 20;

    double cgtorlence = 1e-11;

    outFileName ="fe_4elt2_2part_1e-11_10iter_15cons_search_O3_scale";
    //outFileName ="4elt_6part_1e-11_10iter_5cons_search";
    //test consolidation result using one consolidation loop
    //int consolidarationNum = 1;
    
    std::cout << "test size and connect here:" << numVertices << " " <<  numConnect << std::endl;
   
    std::cout<<"start diffusion here: "<<std::endl;
    buddleFosCSolver::loadDiffusion(gp, G, partResVec, partitionNum, iterationNum, consolidarationNum, cgtorlence);
    
    //store the partition result

    buddleFosCSolver::printPartitionResult(partResVec,outFileName);

    //check cutsize
    buddleFosCSolver::checkCutSize(gp,partResVec);

    // now the CsrMatrix store adjacancy matrix
    // if need convert to laplacian
    // use following instead
    
    //test
    //cout << gp.num_edges <<endl;
    std::cout <<"<-------------------finish------------------------->"<<std::endl;
    return 0;
}
