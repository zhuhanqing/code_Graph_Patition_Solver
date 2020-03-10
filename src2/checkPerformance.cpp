/**
 * @file   checkPerformance.cpp
 * @author Zhu Hanqing
 * @date   Aug 2019
 */
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include "GraphParser.h"

//use to check cutsize
int main()
{
	
	std::ifstream infile;
    infile.open("/home/zhuhanqing/zhuhanqing/hanqing/NewPartition/benchmark/mdual.graph");

    std::vector<int> range_v;
    std::vector<int> connect_v;

    int num_edges, num_nodes;
    int r_index = 0, c_index = 0;
    std::string line;
    
    //first line
    if (getline(infile,line)){
        std:: istringstream iss(line);
        //std::cout<<"ok"<<line<<std::endl;
        if (! (iss >> num_nodes >> num_edges)){
            std::cerr << "Unexcept graph size format" << std::endl;
        }
        range_v.resize(num_nodes + 1);
        connect_v.resize(2 * num_edges);      
    }
    //following part
    //int i = 0;
    int intToken;
    while (getline(infile,line)){
        r_index += 1;
        std::istringstream iss(line);
        while ((iss) >> intToken)
        {
            connect_v[c_index]  = intToken - 1;
            c_index +=1;
        }

        range_v[r_index] = c_index;
    
    }

    infile.close();
    std::cout<<"finish parser part"<<std::endl;

	std::ifstream inResfile;
    inResfile.open("/home/zhuhanqing/zhuhanqing/hanqing/NewPartition/Result/result.txt");

    std::vector<int> partitionResult(num_nodes);

    std::string resline; 
    int partNum;

    while (getline(inResfile,resline)){
        std::istringstream iss(resline);
        while ((iss) >> intToken >> partNum)
        {
        	partitionResult[intToken] = partNum;
        }
    }

    inResfile.close();
    std::cout<<"finish partition result input"<<std::endl;

    //start to check cutsize

    std::cout<<"check cutsize here"<<std::endl;
    int cutsize = 0;
	
	int indexPartitionNum,indexConnectIndexPartitionNum;
	int connectSort, connectRange, connectRangeBegin;

	for(int indexCnt = 0;indexCnt < num_nodes; indexCnt++)
	{
		connectRangeBegin = range_v[indexCnt];
		connectRange = range_v[indexCnt+1] - range_v[indexCnt];
		
		indexPartitionNum = partitionResult[indexCnt];

		for (int connectCnt = 0; connectCnt < connectRange; ++connectCnt)
		{
			connectSort = connect_v[connectRangeBegin + connectCnt];
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
    cutsize = cutsize/2;
	std::cout<<"cutsize: " <<cutsize<<std::endl;

    return 0;

}