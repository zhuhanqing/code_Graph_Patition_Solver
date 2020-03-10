/**
 * @file   GraphParser.h
 * @author zhu hanqing
 * @date   Jul 2019
 */
#ifndef _GRAPHPARSER_H__
#define _GRAPHPARSER_H__


#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
//include the _splitpath
#include <stdlib.h>

//using namespace std;

class GraphParser{
    public:
        void loadGraphFromFile(const std::string &file_name);
        int num_nodes = 0;
        int num_edges = 0;
        std::vector<int>range_v;   
        std::vector<int>connect_v;
        std::vector<int>values_v;
        std::string filepath;
    private:
        int no_use = 0;
};

void GraphParser::loadGraphFromFile(const std::string &file_name)
{   
    //define the nodes and edges extracted from the 1st line  
    filepath = file_name;
    
    //  get input graph
    std::ifstream infile;
    infile.open(file_name);
    //std::cout << file_name<<std::endl;
    //infile.open("/home/local/eda03/hanqing/NewPartition/benchmark/simple.graph");

    //int start_node=0;
    //define    r_index     i+1 row
    //define    c_index     2*i connection
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
        values_v.resize(2 * num_edges);
        
    }
    //following part
    //int i = 0;
    int intToken;
    while (getline(infile,line)){
        r_index += 1;
        std::istringstream iss(line);
        //std::cout<<line<<std::std::endl;
        while ((iss) >> intToken)
        {
            connect_v[c_index]  = intToken - 1;
            //connect_v[c_index] -=1;
            c_index +=1;
        }

        range_v[r_index] = c_index;
    
    }
    
    infile.close();
    //test
    /*
    int nSize=range_v.size();
    for (int i=0;i<nSize;i++){
            std::cout<<range_v[i]<<" ";
        }
    std::cout<<std::endl;
    */
    /*
    std::cout<<"parser inner check: ";
    int mSize=connect_v.size();
    for (int i=0;i<100;++i){
            std::cout<<connect_v[i]<<" ";
        }
    std::cout<<std::endl;
    */
}

#endif
