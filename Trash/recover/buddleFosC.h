/**
 * @file   buddleFosC.cpp
 * @author Hanqing Zhu
 * @date   Jul 2019
 */
#ifndef _BUDDLEFOSC_H__
#define _BUDDLEFOSC_H__

//To complete framework 
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>

#include "GraphParser.h"
#include "CsrMatrix.h"
#include "operator.h"
#include "LinearSolver.h"

///@ given indices 
///@ first:   graph:                       maybe graph in crs format
///@ second:  graph partition result:      vector size:V = graph node num
///@ third:   num of iteration:            control outer loop
///@ fourth:  num of consolidation        control inner loop to refine result
///@ add num of partition

namespace buddleFosCSolver
{
//define a struct to know the load distribution belong to which seed or partition 
template<typename TIndex, typename TValue> 
struct loadDistribution
{
    std::vector<TValue> loadDistributionVec;
    std::vector<TValue> drainDistributionVec;
    /*
    Vector<TIndex, TValue> loadDistributionVec;
    Vector<TIndex, TValue> drainDistributionVec;
    */
};

// key: solve Lwp = d, where L is the property of graph whereas wp and d are determined by seed location 
template<typename TIndex, typename TValue, typename TData, typename TFlag>
//void loadDiffusion(GraphParser& gp, const CsrMatrix<TIndex, TValue>& graph, Vector<TIndex, TData>& partResPie,TIndex numOfPartition, TIndex numOfIteration, TIndex numOfConsolidation, TValue cgTolerance)
void loadDiffusion(GraphParser& gp, const CsrMatrix<TIndex, TValue>& graph, std::vector<TData>& partResPie,TIndex numOfPartition, TIndex numOfIteration, TIndex numOfConsolidation, TValue cgTolerance, TFlag seedGenFlag, TValue maxImbalance)
{
    //set sparse matrix

    // partResPie is equal to assignment
    TIndex numOfIndex = graph.size();

    TValue loadPerSeed = 2; //decide by yourself; no effect on algorithm itself
    TValue loadDrainNotSeed = 0.2; //a small param determined by yourself delta = 0.6 = 0.1 load
    TIndex bestCutSize = 0;   
    // seedList to store p seed sort
    std::vector<int> seedList(numOfPartition);
    std::vector<int> cutsizeList(numOfIteration);
    /*
    Vector<TIndex, TValue> seedList(numOfPartition);
    Vector<TIndex, TValue> cutsizeList(numOfIteration);
    */
    for(TIndex iteration = 0;iteration < numOfIteration; ++iteration)
    {
        //step 1 : get seedList 
        // if not define pie need self-select seed
        // so how to judge whether have a pie: extend the partResPie to V + 1: last as flag to show whether the partition vector has already been modified
        if (partResPie[numOfIndex] == 0)
        {            
        	if (seedGenFlag == 0)
            {
                //randomly generate seed
                //set random_number_generator using uniform distribution
                //define random_number_generator member static to keep only copy of this member
                static std::random_device seed;
                static std::mt19937 random_number_generator(seed());
                std::uniform_int_distribution<TIndex> indices(0, numOfIndex - 1);
                
                //get p = numOfPartition seed
                //only need store seed sort
                std::cout<<"random generate seed Index: ";
                for (TIndex seedCnt = 0;seedCnt < numOfPartition; ++seedCnt)
                {
                    seedList[seedCnt] = indices(random_number_generator);
                    std::cout<<seedList[seedCnt]<<" ";
                }
                std::cout<<std::endl;
            }
            else if(seedGenFlag == 1)
            {   
                //BFS search to get ideal seed list
                //still need a random seed generator to get the first seed
                TIndex numOfSeed = 0;

                static std::random_device seed;
                static std::mt19937 random_number_generator(seed());
                std::uniform_int_distribution<TIndex> indices(0, numOfIndex - 1);

                seedList[0] = indices(random_number_generator);
                numOfSeed += 1;
                //seedList[0] = 10066;

                //? set a vec to store n seed generate n * numOfIndex load distribution res
                std::vector<TValue> chooseSeedLoadStoreVec(numOfIndex);
                //Vector<TIndex, TValue> chooseSeedLoadStoreVec(numOfIndex);
                for (TIndex fuzhiCnt = 0; fuzhiCnt < numOfIndex; ++fuzhiCnt)
                {
                    chooseSeedLoadStoreVec[fuzhiCnt] = 0;
                }

                for (TIndex seedGenCnt = 1; seedGenCnt < numOfPartition; ++seedGenCnt)
                {
                    loadDistribution<TIndex, TValue> chooseSeedLoad;  

                    chooseSeedLoad.loadDistributionVec.resize(numOfIndex);
                    chooseSeedLoad.drainDistributionVec.resize(numOfIndex);

                    //assign loadDistributionVec
                    //place load on seed 
                    //O(N * number < numOfPartition)
                    for (TIndex loadAssignCnt = 0;loadAssignCnt < numOfIndex; ++loadAssignCnt)
                    {
                        if (loadAssignCnt == seedList[seedGenCnt - 1])
                        {
                            chooseSeedLoad.loadDistributionVec[loadAssignCnt] = loadPerSeed;
                        }
                        else
                        {
                            chooseSeedLoad.loadDistributionVec[loadAssignCnt] = 0;
                        }
                    }

                    //assign drainDistributionVec
                    std::cout<<"check seed num: " << numOfSeed << std::endl;
                    TValue drainPerSource = TValue(numOfIndex) * loadDrainNotSeed - loadDrainNotSeed;
                    std::cout<<"check drain on source: " << drainPerSource << std::endl;
                    
                    for (TIndex drainAssignCnt = 0;drainAssignCnt < numOfIndex; ++drainAssignCnt)
                    {
                        if (drainAssignCnt == seedList[seedGenCnt - 1])
                        {
                            chooseSeedLoad.drainDistributionVec[drainAssignCnt] = drainPerSource;
                        }
                        else
                        {
                            chooseSeedLoad.drainDistributionVec[drainAssignCnt] = -loadDrainNotSeed;
                        }
                    }

                    std::cout<<"start BFS search seed "<<std::endl;
                    
                    solver::conjGradSolver(graph, chooseSeedLoad.drainDistributionVec, chooseSeedLoad.loadDistributionVec,cgTolerance);
                    std::cout<<"end   BFS search seed "<<std::endl;

                    for (TIndex fuzhiCnt = 0; fuzhiCnt < numOfIndex; ++fuzhiCnt)
                    {
                        chooseSeedLoadStoreVec[fuzhiCnt] += chooseSeedLoad.loadDistributionVec[fuzhiCnt];
                    }
                    //get minimum not maximum
                    TValue min = chooseSeedLoadStoreVec[0];
                    TIndex minSort = 0;

                    for(TIndex compareCnt = 0; compareCnt < numOfIndex; ++compareCnt)
                    {
                        if (chooseSeedLoadStoreVec[compareCnt] < min)
                        {
                            min = chooseSeedLoadStoreVec[compareCnt];
                            minSort = compareCnt;
                        }
                    }                   
                    seedList[seedGenCnt] = minSort;
                    numOfSeed += 1;
                }

                std::cout<<"BFS get seed Index: ";
                for (TIndex seedCnt = 0;seedCnt < numOfPartition; ++seedCnt)
                {
                    std::cout<<seedList[seedCnt]<<" ";
                }
                std::cout<<std::endl;
            }
            else
            {
                std::cout<<"error: no seed generate!!!"<<std::endl;
            }
                  
        }
        // have a previous partition
        // contract seedList
        else if (partResPie[numOfIndex] != 0)
        {
            //assign drainDistributionVec each partition use same d
            //??? right?

            //p parallel for each partition
            //p * V load result
            //std::string loadDistributionName;               
            for (TIndex partParallelCnt = 0; partParallelCnt < numOfPartition; ++partParallelCnt)
            {
                loadDistribution<TIndex, TValue> loadDistributionSeed;  

                loadDistributionSeed.loadDistributionVec.resize(numOfIndex);
                loadDistributionSeed.drainDistributionVec.resize(numOfIndex);

                int perPartitionNumCnt = 0;
                for (TIndex loadAssignCnt = 0;loadAssignCnt < numOfIndex; ++loadAssignCnt)
                {
                    if (partResPie[loadAssignCnt] == partParallelCnt)
                    {
                        loadDistributionSeed.loadDistributionVec[loadAssignCnt] = loadPerSeed;
                        perPartitionNumCnt += 1;
                    }
                    else
                    {
                        loadDistributionSeed.loadDistributionVec[loadAssignCnt] = 0;
                    }
                }

                TValue drainPerSource = TValue(numOfIndex)/perPartitionNumCnt * loadDrainNotSeed - loadDrainNotSeed;
                
                for (TIndex drainAssignCnt = 0;drainAssignCnt < numOfIndex; ++drainAssignCnt)
                {
                    if (partResPie[drainAssignCnt] == partParallelCnt)
                    {
                        loadDistributionSeed.drainDistributionVec[drainAssignCnt] = drainPerSource;
                    }
                    else
                    {
                        loadDistributionSeed.drainDistributionVec[drainAssignCnt] = -loadDrainNotSeed;
                    }
                } 
	    		std::cout<<"begin contraction here"<<std::endl;
                solver::conjGradSolver(graph, loadDistributionSeed.drainDistributionVec, loadDistributionSeed.loadDistributionVec,cgTolerance);
                std::cout<<"end        contraction"<<std::endl;

                TValue max = loadDistributionSeed.loadDistributionVec[0];
                TIndex maxSort = 0;

                for(TIndex compareCnt = 0; compareCnt < numOfIndex; ++compareCnt)
                {
                    if (loadDistributionSeed.loadDistributionVec[compareCnt] > max)
                    {
                        max = loadDistributionSeed.loadDistributionVec[compareCnt];
                        maxSort = compareCnt;
                    }
                }
                
                seedList[partParallelCnt] = maxSort;
            }

            std::cout<<"Contraction get seed Index: ";
            for (TIndex seedCnt = 0;seedCnt < numOfPartition; ++seedCnt)
            {
                std::cout<<seedList[seedCnt]<<" ";
            }
            std::cout<<std::endl;
        }
    
        //step2: compute partitioning
        //create a partitionNum * indexNum distributionVec
        //Vector<TIndex, TValue> loadDistributionVecStore(numOfPartition * numOfIndex);
        std::vector<TValue> loadDistributionVecStore(numOfPartition * numOfIndex);
        //std::string loadDistributionName;
        TIndex cnt = 0;
       
        for (TIndex partParallelCnt = 0; partParallelCnt < numOfPartition; ++partParallelCnt)
        {                

            // construct partition load distribution and drain wp dp

            //std::string a = std::to_string(cnt);
           
            //loadDistributionName = "loadDistribution" + a;
            loadDistribution<TIndex, TValue> loadDistributionName;  
            
            //get seed sort
            TIndex seedSort = seedList[partParallelCnt];
             
            loadDistributionName.loadDistributionVec.resize(numOfIndex);
            loadDistributionName.drainDistributionVec.resize(numOfIndex);
            //seedRank and seedInfo is default for we don not need

            //loadDistributionName.seedRank = partParallelCnt;//which partition
            //loadDistributionName.seedInfo = seedSort;//which seed

            //assign loadDistributionVec
            //place load on seed
            for (TIndex loadAssignCnt = 0;loadAssignCnt < numOfIndex; ++loadAssignCnt)
            {
                if (loadAssignCnt == seedSort)
                {
                    loadDistributionName.loadDistributionVec[seedSort] = loadPerSeed;
                }
                else
                {
                    loadDistributionName.loadDistributionVec[loadAssignCnt] = 0;
                }
            }
            std::cout<< "load distribution ok" <<std::endl; 

            //assign drainDistributionVec
            TValue drainPerSource = TValue(numOfIndex) * loadDrainNotSeed - loadDrainNotSeed;
            std::cout<<"check drain on source" << drainPerSource << std::endl;

            for (TIndex drainAssignCnt = 0;drainAssignCnt < numOfIndex; ++drainAssignCnt)
            {
                if (drainAssignCnt == seedSort)
                {
                    loadDistributionName.drainDistributionVec[drainAssignCnt] = drainPerSource;
                }
                else
                {
                    loadDistributionName.drainDistributionVec[drainAssignCnt] = -loadDrainNotSeed;
                }
            } 
            std::cout << "drain distribution ok" << std::endl;
            /*
            for (TIndex loadAssignCnt = 0;loadAssignCnt < numOfIndex; ++loadAssignCnt)
            {
                loadDistributionName.loadDistributionVec[loadAssignCnt] = 0;
            }
            loadDistributionName.loadDistributionVec[seedSort] = loadPerSeed;
            */
            /*
            std::cout << loadDistributionName.drainDistributionVec.size() << " " << loadDistributionName.loadDistributionVec.size()<< ' ' << graph.size()<<std::endl;
            int ttt = graph.rowOffsetVal(24);
            */
            std::cout<<"start 2nd step solver "<<std::endl;
            solver::conjGradSolver(graph, loadDistributionName.drainDistributionVec, loadDistributionName.loadDistributionVec,cgTolerance);

            /*
            TValue max = 0;
            TIndex maxSort = -1;

            for(TIndex compareCnt = 0; compareCnt < numOfIndex; ++compareCnt)
            {
                if (loadDistributionName.loadDistributionVec[compareCnt] > max)
                {
                    max = loadDistributionName.loadDistributionVec[compareCnt];
                    maxSort = compareCnt;
                }
            }
            
            seedList[numOfPartition] = maxSort;
            */
            for (TIndex fuzhiCnt = 0; fuzhiCnt < numOfIndex; ++fuzhiCnt)
            {
                loadDistributionVecStore[cnt*numOfIndex + fuzhiCnt] = loadDistributionName.loadDistributionVec[fuzhiCnt];
            }
            ++cnt;
        }
        
        TValue max = 0;
    	TIndex indexPartNum;
        for (TIndex indexSort = 0;indexSort < numOfIndex; ++indexSort)
        {
            indexPartNum = 0;
			max = loadDistributionVecStore[indexSort];
            for (TIndex partCnt = 0;partCnt < numOfPartition; ++partCnt)
            {

                if (loadDistributionVecStore[indexSort + partCnt * numOfIndex] > max)
                {
                    max = loadDistributionVecStore[indexSort + partCnt * numOfIndex];
                    indexPartNum = partCnt;
                }
            }
            partResPie[indexSort] = indexPartNum;
        }

        partResPie[numOfIndex] = 1;
        
        //step3 optional consolidation
        TIndex maxPartNum, minPartNum; 
        std::vector<TValue> loadDistributionVecStoreConsolid(numOfPartition * numOfIndex);
    	//Vector<TIndex, TValue> loadDistributionVecStoreConsolid(numOfPartition * numOfIndex);
        for (TIndex consolidationCnt = 0; consolidationCnt < numOfConsolidation; ++consolidationCnt)
        {   
            //Vector<TIndex, TValue> loadDistributionVecStoreConsolid(numOfPartition * numOfIndex);
            for (TIndex partParallelCnt = 0; partParallelCnt < numOfPartition; ++partParallelCnt)
            {
                // construct partition load distribution and drain wp dp

                //AT!! we have distribution here so we need normalize wp to partition
                
                //std::string a = std::to_string(cnt);                   
                //loadDistributionNameForCons = "loadDistribution" + a;
                
                loadDistribution<TIndex, TValue> loadDistributionNameForCons;  
                
                loadDistributionNameForCons.loadDistributionVec.resize(numOfIndex);
                loadDistributionNameForCons.drainDistributionVec.resize(numOfIndex);
                
                //assign loadDistributionVec
                //normalize load distribution vec
                int perPartitionNumCnt = 0;
                for (TIndex loadAssignCnt = 0;loadAssignCnt < numOfIndex; ++loadAssignCnt)
                {
                    if (partResPie[loadAssignCnt] == partParallelCnt)
                    {
                        loadDistributionNameForCons.loadDistributionVec[loadAssignCnt] = loadPerSeed;
                        perPartitionNumCnt += 1;
                    }
                    else
                    {
                        loadDistributionNameForCons.loadDistributionVec[loadAssignCnt] = 0;
                    }
                }

                if(perPartitionNumCnt == 0)
                {
                    break;
                }

                if(partParallelCnt == 0)
                {
                    maxPartNum = perPartitionNumCnt;
                    minPartNum = perPartitionNumCnt;
                }
                else
                {
                    if (maxPartNum < perPartitionNumCnt)
                    {
                        maxPartNum = perPartitionNumCnt;     
                    }
                    if (minPartNum > perPartitionNumCnt)
                    {
                        minPartNum = perPartitionNumCnt;     
                    }
                }

	    		std::cout<<"check partition: "<<partParallelCnt<<" check perPartitionNumCnt: "<<perPartitionNumCnt<<std::endl;
                TValue drainPerSource = TValue(numOfIndex)/perPartitionNumCnt * loadDrainNotSeed - loadDrainNotSeed;
                std::cout<<"check drain on source" << drainPerSource << std::endl;
                for (TIndex drainAssignCnt = 0;drainAssignCnt < numOfIndex; ++drainAssignCnt)
                {
                    if (partResPie[drainAssignCnt] == partParallelCnt)
                    {
                        loadDistributionNameForCons.drainDistributionVec[drainAssignCnt] = drainPerSource;
                    }
                    else
                    {
                        loadDistributionNameForCons.drainDistributionVec[drainAssignCnt] = - loadDrainNotSeed;
                    }
                } 
                
                std::cout<<"start 3rd step solver: "<< std::endl;
                solver::conjGradSolver(graph, loadDistributionNameForCons.drainDistributionVec, loadDistributionNameForCons.loadDistributionVec,cgTolerance);
                std::cout<<"finish 3rd step solver" << std::endl;

                for (TIndex fuzhiCnt = 0; fuzhiCnt < numOfIndex; ++fuzhiCnt)
                {
                    loadDistributionVecStoreConsolid[partParallelCnt*numOfIndex + fuzhiCnt] = loadDistributionNameForCons.loadDistributionVec[fuzhiCnt];
                }
            }

            TValue max = 0;
            TIndex indexPartNum = 0;
            for (TIndex indexSort = 0;indexSort < numOfIndex; ++indexSort)
            {
                indexPartNum = 0;
	    		max = loadDistributionVecStoreConsolid[indexSort];
                for (TIndex partCnt = 0;partCnt < numOfPartition; ++partCnt)
                {			
                    if (loadDistributionVecStoreConsolid[indexSort + partCnt * numOfIndex] > max)
                    {
                        max = loadDistributionVecStoreConsolid[indexSort + partCnt * numOfIndex];
                        indexPartNum = partCnt;
                    }
                    //m++;
                }
                /*
                if(consolidationCnt == 0 and indexSort > 256600)
                {
                	std::cout<< "check the partition num: " << indexPartNum << "check max value" << max;
                }
                */
                partResPie[indexSort] = indexPartNum;
            }
            TValue ratioMaxMean = TValue(maxPartNum)/numOfIndex * numOfPartition;
            TValue ratioMinMean = TValue(minPartNum)/numOfIndex * numOfPartition + 1;

            std::cout<<"check scale balance: " << "MaxPartRatio: "<<ratioMaxMean<<"MinPartRatio: "<<ratioMinMean<<std::endl;

            if ((ratioMaxMean > maxImbalance) or (ratioMinMean > maxImbalance))
            {
                std::cout<<"--------------------Not fit your imbalance requirement in "<<consolidationCnt<<" times consolidation"<<std::endl;
            }
            else
            {
                std::cout<<"--------------------Fit your imbalance requirement----------------------"<<std::endl;
            }
        }
        //check the cutsize converge?
        std::cout<<iteration<<" times check cutsize here"<<std::endl;
        TIndex cutsize = 0;
		TIndex indexNum = partResPie.size();
		TIndex indexPartitionNum,indexConnectIndexPartitionNum;
		TIndex connectSort, connectRange, connectRangeBegin;

		for(TIndex indexCnt = 0;indexCnt < indexNum; indexCnt++)
		{
			connectRangeBegin = gp.range_v[indexCnt];
			connectRange = gp.range_v[indexCnt+1] - gp.range_v[indexCnt];
			
			indexPartitionNum = partResPie[indexCnt];

			for (TIndex connectCnt = 0; connectCnt < connectRange; ++connectCnt)
			{
				connectSort = gp.connect_v[connectRangeBegin + connectCnt];
				indexConnectIndexPartitionNum = partResPie[connectSort];
				if (indexPartitionNum != indexConnectIndexPartitionNum)
				{
					cutsize += 1;
				}
				else
				{
					//std::cout<<"no cut here"；
				}
			}
		}
		cutsize= cutsize/2;
		cutsizeList[iteration] = cutsize;
		std::cout<<"cutsize: " <<cutsize<<std::endl;
    }
    std::cout << "The past " << numOfIteration <<"times cutsize Info: ";
    bestCutSize = cutsizeList[0];
    for (TIndex cnt = 0;cnt < numOfIteration; ++cnt)
    {
    	std::cout<<cutsizeList[cnt]<<" ";
    	if(bestCutSize > cutsizeList[cnt])
    	{
    		bestCutSize = cutsizeList[cnt];
    	}
    }
    std::cout<<std::endl;
    std::cout<<"record best cutsize result: " <<bestCutSize<<std::endl;
}

//print partition result into given file
template <typename TValue>
//void printPartitionResult(Vector<TIndex, TValue> &partitionResult,std::string& fileName)
void printPartitionResult(std::vector<TValue> &partitionResult,std::string& fileName)
{
    std::string pathName;
	//metis
	//pathName= "/home/zhuhanqing/zhuhanqing/hanqing/metis-5.1.0/graphs/" + fileName;
	//more graph
	pathName= "/home/zhuhanqing/zhuhanqing/hanqing/graphs/" + fileName;

    std::ofstream outfile;
    outfile.open(pathName, std::ios::out);
    //further extend to a solid output operator to have specific file name
    //input graph name + part + partition number
    //like 4elt.graph.part.12
    int printSize = partitionResult.size();

    for (int i = 0;i < printSize; ++i)
    {
        outfile << i << " " << partitionResult[i] <<"\n";
    }

    outfile.close();
    std::cout<<"write file into path: "<<pathName<<std::endl;
}

//test cutsize
template <typename TValue>
//void checkCutSize(GraphParser& gp, Vector<TIndex, TValue>&partitionResult)
void checkCutSize(GraphParser& gp, std::vector<TValue>&partitionResult)
{
	//graphparser inner index
	/*
	int num_nodes = 0;
    int num_edges = 0;
    std::vector<int>range_v;   
    std::vector<int>connect_v;
    std::vector<int>values_v;
    */
    std::cout<<"check cutsize here"<<std::endl;
    int cutsize = 0;
	int indexNum = partitionResult.size();
	int indexPartitionNum,indexConnectIndexPartitionNum;
	int connectSort, connectRange, connectRangeBegin;

	for(int indexCnt = 0;indexCnt < indexNum; indexCnt++)
	{
		connectRangeBegin = gp.range_v[indexCnt];
		connectRange = gp.range_v[indexCnt+1] - gp.range_v[indexCnt];
		
		indexPartitionNum = partitionResult[indexCnt];

		for (int connectCnt = 0; connectCnt < connectRange; ++connectCnt)
		{
			connectSort = gp.connect_v[connectRangeBegin + connectCnt];
			indexConnectIndexPartitionNum = partitionResult[connectSort];
			if (indexPartitionNum != indexConnectIndexPartitionNum)
			{
				cutsize += 1;
			}
			else
			{
				//std::cout<<"no cut here"；
			}
		}
	}
	cutsize= cutsize/2;
	std::cout<<"cutsize: " <<cutsize<<std::endl;
	//write cutsize to file

}

}

#endif
