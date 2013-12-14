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
    cin >> numVariables;
    for (int i = 0; i < numVariables; i++)
        cin >> variables[i];
    cin >> numConditions;
    for (int i = 0; i < numConditions; i++)
        cin >> conditions[i];
    ExpTree *tree = ExpTree::readFrom("/home/thewbp/SharedFolder/train/tree.txt");
    tree->print();
    cout << endl;
    double result = tree->eval();
    if (result < 0)
        result = 0;
    
    ofstream ofile;
    ofile.open("/home/thewbp/SharedFolder/train/eval.txt");
    ofile << result;
    ofile.close();
    return 0;
}
