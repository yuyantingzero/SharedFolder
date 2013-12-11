//
//  main.cpp
//  583
//
//  Created by Jun Chen on 12/10/13.
//  Copyright (c) 2013 Jun Chen. All rights reserved.
//

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, const char * argv[])
{
    
    std::string line;
    std::ifstream infile("/Users/junchen/course/583/final/PD_STATS");
    
    bool flag = false;
    
    while (std::getline(infile, line))
    {
       // std::istringstream iss(line);
        
        if (line == "Totals {") {
            flag = true;
            continue ;
        }
        
        if(flag){
            //std::cout << line <<"\n";
            
            if (line.substr (0,13) != "\ttotal_cycles") {
                std::cout << line.substr (0,13) <<"\n";
                return -1;
            }
           
            for(int i=13; i<line.size(); i++){
                if(line[i] == '.'){
                    continue;
                }
                
                string num = line.substr(i);
                int value = atoi(num.c_str()); //value = 45
                 std::cout << value <<"\n";
                
                return value;
            }
            return -1;
        }
    }
    // insert code here...
    std::cout << "no total cycles";
    return -1;
}

