//
//  main.cpp
//  GPAlgorithm
//
//  Created by Buping Wang on 12/8/13.
//  Copyright (c) 2013 Buping Wang. All rights reserved.
//

#include "gp.h"
   
int main(int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));
    Population pop(POPULATION);
    for (int i = 0; i < GENERATION; i++) {
        
        //pop.print();
        pop.mate(i);
        //cout << endl << endl << endl;
        //pop.print();
    }
    /*
    ExpTree *tree = ExpTree::readFrom("/home/thewbp/SharedFolder/train/base_line.txt");
    tree->print();
    cout << endl;
    cout << endl;*/
    //    ExpTree *tree = new ExpTree();
    //    tree->print();
    //    cout << endl;
    //    cout << endl;
    //    cout << endl;
    //    tree->writeTo("/Users/thewbp/Desktop/1.txt");
    //    delete tree;
    //    tree = ExpTree::readFrom("/Users/thewbp/Desktop/1.txt");
    //    tree->print();
    //    cout << endl;
    //    cout << endl;
    return 0;
}
