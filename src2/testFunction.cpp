/**
 * @file   try.cpp
 * @author hanqing zhu
 * @date   Jul 2019
 */
#include <iostream>  //std::cout
#include <algorithm> //std::min_element, std::max_element

#include <vector>
#include <libgen.h> //basename & dirname

int main(){

    std::vector<double> testv(5);


    for (int i = 0;i < 5; i++)
    {
        std::cout<<testv[i]<<" ";
    }
    std::cout<<std::endl;
    
    for (int j = 0;j < 5;j++)
    {
        testv[j] =double(j);
    }
    
    for (int i = 0;i < 5; i++)
    {
        std::cout<<testv[i]<<" ";
    }
    std::cout<<std::endl; 


    return 0;
}
