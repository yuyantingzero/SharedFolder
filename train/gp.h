#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>

#define POPULATION 100
#define GENERATION 20
#define TOURNAMENT_SIZE 10
#define REPLACEMENT_RATIO 0.25
#define MUTATION_RATIO 0.5
#define BENCHMARK "fib_rec"
#define BENCHMARK_CYCLES 1518

using namespace std;

enum OP_TYPE {
    OP_NOOP = 0,
    OP_BINARY = 1,
    OP_CBINARY = 2,
    OP_UNARY = 3,
};

typedef double (*BinaryFunc)(double, double);
typedef double (*CBinaryFunc)(double, double, bool);
typedef double (*UnaryFunc)(double);

// binary functions
double add(double a, double b)
{
    return a + b;
}

double sub(double a, double b)
{
    return a - b;
}

double mul(double a, double b)
{
    return a * b;
}

double div(double a, double b)
{
    if (fabs(b) < 0.000001) {
        return 0;
    }
    return a / b;
}

// conditional binary functions
double cmul(double a, double b, bool c)
{
    if (c)
        return a * b;
    return b;
}

double tern(double a, double b, bool c)
{
    if (c)
        return a;
    return b;
}

// unary functions
double sqrtt(double a)
{
    if (a < 0)
        return 0;
    return sqrt(a);
}

double square(double a)
{
    return a * a;
}

double cube(double a)
{
    return a * a * a;
}

BinaryFunc binaryFuncs[] = {add, sub, mul, div};
string binaryFuncDiscs[] = {"add", "sub", "mul", "div"};

CBinaryFunc cBinaryFuncs[] = {cmul, tern};
string cBinaryFuncDiscs[] = {"cmul", "tern"};

UnaryFunc unaryFuncs[] = {sqrtt, square, cube};
string unaryFuncDiscs[] = {"sqrt", "sq", "cube"};

int numBinaryFuncs = 4;
int numCBinaryFuncs = 2;
int numUnaryFuncs = 3;

double variables[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
string variableDiscs[] = {"dep_height", "num_ops", "exec_ratio", "num_branches",
    "num_blocks", "max_dep_height", "total_ops", "total_paths", "total_zpaths",
    "avg_exec_ratio", "avg_dep_height", "max_num_branches", "max_num_ops",
    "predict_avg", "predict_prod"};

double conditions[] = {false, false, false};
string conditionDiscs[] = {"unsafe_JSR", "safe_JSR", "mem_hazard"};

int numVariables = 13;
int numConditions = 3;

class ExpTree
{
private:
    OP_TYPE op;
    int vid;
    int cid;
    int bfid;
    int cbfid;
    int ufid;
    
    double value;
    bool condition;
    BinaryFunc bf;
    CBinaryFunc cbf;
    UnaryFunc uf;
    
    vector<ExpTree *> getLevel(int level)
    {
        vector<ExpTree *> result;
        int cur = 1;
        queue<ExpTree *> currentLevel, nextLevel;
        currentLevel.push(this);
        while (!currentLevel.empty()) {
            ExpTree *curNode = currentLevel.front();
            currentLevel.pop();
            if (curNode) {
                nextLevel.push(curNode->left);
                nextLevel.push(curNode->right);
                if (cur == level) {
                    result.push_back(curNode);
                }
            }
            if (currentLevel.empty()) {
                if (cur == level) {
                    break;
                }
                cur++;
                swap(currentLevel, nextLevel);
            }
        }
        return result;
    }
    
public:
    ExpTree *parent;
    ExpTree *left;
    ExpTree *right;
    
    ExpTree()
    {
        // random tree
        vid = -1;
        cid = -1;
        bfid = -1;
        cbfid = -1;
        ufid = -1;
        
        bf = NULL;
        cbf = NULL;
        uf = NULL;
        parent = NULL;
        left = NULL;
        right = NULL;
        
        int r = rand() % 12;
        switch (r) {
            case 0:
            case 1:
                op = OP_NOOP;
                value = (double)rand() / RAND_MAX * 2;
                break;
            case 2:
            case 3:
            case 4:
                op = OP_BINARY;
                bfid = rand() % numBinaryFuncs;
                bf = binaryFuncs[bfid];
                left = new ExpTree();
                right = new ExpTree();
                break;
            case 5:
            case 6:
                op = OP_CBINARY;
                cid = rand() % numConditions;
                condition = conditions[cid];
                cbfid = rand() % numCBinaryFuncs;
                cbf = cBinaryFuncs[cbfid];
                left = new ExpTree();
                right = new ExpTree();
                break;
            case 7:
                op = OP_UNARY;
                ufid = rand() % numUnaryFuncs;
                uf = unaryFuncs[ufid];
                left = new ExpTree();
                break;
            default:
                op = OP_NOOP;
                vid = rand() % numVariables;
                value = variables[vid];
                break;
        }
    }
    
    ExpTree(OP_TYPE o, int v, int c, int b, int cb, int u, double v2,
            ExpTree *l, ExpTree *r) {
        op = o;
        vid = v;
        cid = c;
        bfid = b;
        cbfid = cb;
        ufid = u;
        
        if (vid == -1)
            value = v2;
        else
            value = variables[vid];
        if (cid != -1)
            condition = conditions[cid];
        if (bfid != -1)
            bf = binaryFuncs[bfid];
        if (cbfid != -1)
            cbf = cBinaryFuncs[cbfid];
        if (ufid != -1)
            uf = unaryFuncs[ufid];
        
        parent = NULL;
        left = l;
        right = r;
    }
    
    ~ExpTree()
    {
        delete left;
        delete right;
    }
    
    ExpTree *copy()
    {
        ExpTree *l = NULL;
        ExpTree *r = NULL;
        if (left != NULL) {
            l = left->copy();
        }
        if (right != NULL) {
            r = right->copy();
        }
        return new ExpTree(op, vid, cid, bfid, cbfid, ufid, value, l, r);
    }
    
    void validate()
    {
        if (left) {
            left->parent = this;
            left->validate();
        }
        if (right) {
            right->parent = this;
            right->validate();
        }
    }
    
    int getHeight()
    {
        int leftHeight = left == NULL ? 0 : left->getHeight();
        int rightHeight = right == NULL ? 0 : right->getHeight();
        return 1 + max(leftHeight, rightHeight);
    }
    
    int getSize()
    {
        int leftSize = left == NULL ? 0 : left->getSize();
        int rightSize = right == NULL ? 0 : right->getSize();
        return 1 + leftSize + rightSize;
    }
    
    ExpTree *random()
    {
        // randomly select level
        int h = getHeight();
        int level = rand() % h + 1;
        
        // randomly select a node on that level
        vector<ExpTree *> nodes = getLevel(level);
        return nodes[rand() % nodes.size()];
    }
    
    void mutate()
    {
        // select a random node
        ExpTree *sub = random();
        validate();
        ExpTree *par = sub->parent;
        if (par) {
            if (par->left == sub)
                par->left = new ExpTree();
            else
                par->right = new ExpTree();
        }
    }
    
    double eval()
    {
        double result = 0.0;
        switch (op) {
            case OP_NOOP:
                result = value;
                break;
            case OP_BINARY:
                result = bf(left->eval(), right->eval());
                break;
            case OP_CBINARY:
                result = cbf(left->eval(), right->eval(), condition);
                break;
            case OP_UNARY:
                if (left)
                    result = uf(left->eval());
                else
                    result = uf(right->eval());
                break;
            default:
                break;
        }
        return result;
    }
    
    void print()
    {
        printTo(cout);
    }
    
    void printTo(string filename, double fitness)
    {
        ofstream ofile;
        ofile.open(filename.c_str());
        printTo(ofile);
        ofile << endl << endl;
        ofile << fitness;
        ofile.close();
    }
            
    void printTo(ostream& os)
    {
        switch (op) {
        case OP_NOOP:
            if (vid == -1)
                os << value;
            else
                os << variableDiscs[vid];
            break;
        case OP_BINARY:
            os << binaryFuncDiscs[bfid] << "(";
            left->printTo(os);
            os << ", ";
            right->printTo(os);
            os << ")";
            break;
        case OP_CBINARY:
            os << cBinaryFuncDiscs[cbfid] << "(";
            left->printTo(os);
            os << ", ";
            right->printTo(os);
            os << ", " << conditionDiscs[cid] << ")";
            break;
        case OP_UNARY:
            os << unaryFuncDiscs[ufid] << "(";
            if (left)
                left->printTo(os);
            else
                right->printTo(os);
            os << ")";
            break;
        default:
            break;
        }
    }
    
    void writeTo(string filename)
    {
        ofstream ofile;
        ofile.open(filename.c_str());
        writeTo(ofile);
        ofile.close();
    }
    
    void writeTo(ofstream& ofile)
    {
        ofile << op << " " << vid << " " << cid << " " << bfid << " "
        << cbfid << " " << ufid << " " << value;
        ofile << endl;
        if (left)
            left->writeTo(ofile);
        else
            ofile << endl;
        if (right)
            right->writeTo(ofile);
        else
            ofile << endl;
        
    }
    
    static ExpTree *readFrom(string filename)
    {
        ifstream ifile;
        ifile.open(filename.c_str());
        ExpTree *tree = readFrom(ifile);
        ifile.close();
        return tree;
    }
    
    static ExpTree *readFrom(ifstream& ifile)
    {
        if (!ifile)
            return NULL;
        string s;
        if (!getline(ifile, s)) {
            return NULL;
        }
        
        istringstream ss(s);
        int op;
        int vid;
        int cid;
        int bfid;
        int cbfid;
        int ufid;
        double value;
        
        if (!(ss >> op))
            return NULL;
        if (!(ss >> vid))
            return NULL;
        if (!(ss >> cid))
            return NULL;
        if (!(ss >> bfid))
            return NULL;
        if (!(ss >> cbfid))
            return NULL;
        if (!(ss >> ufid))
            return NULL;
        if (!(ss >> value))
            return NULL;
        
        return new ExpTree((OP_TYPE)op, vid, cid, bfid, cbfid, ufid, value,
                           readFrom(ifile), readFrom(ifile));
    }
};

class Population
{
private:
    vector<ExpTree *> population;
    vector<ExpTree *> offsprings;
    map<ExpTree *, double> fitnesses;
    
    void crossover(ExpTree *tree1, ExpTree *tree2)
    {
        // copy first
        tree1 = tree1->copy();
        tree2 = tree2->copy();
        
        // validate
        tree1->validate();
        tree2->validate();
        
        // random nodes
        ExpTree *sub1 = tree1->random();
        ExpTree *sub2 = tree2->random();
        
        // parent nodes
        ExpTree *par1 = sub1->parent;
        ExpTree *par2 = sub2->parent;
        
        if (par1) {
            if (par1->left == sub1) {
                par1->left = sub2;
            } else {
                par1->right = sub2;
            }
            tree1->mutate();
            offsprings.push_back(tree1);
        }
        if (par2) {
            if (par2->left == sub2) {
                par2->left = sub1;
            } else {
                par2->right = sub1;
            }
            tree2->mutate();
            offsprings.push_back(tree2);
        }
    }
    
public:
    Population(int size)
    {
        population.reserve(size);
        for (int i = 0; i < size - 1; i++) {
            population.push_back(new ExpTree);
        }
        ExpTree *baseTree = ExpTree::readFrom("/home/thewbp/SharedFolder/train/base_line.txt");
        population.push_back(baseTree);
    }
    
    void print()
    {
        for (int i = 0; i < population.size(); i++) {
            population[i]->print();
            cout << endl << " = " << population[i]->eval();
            cout << endl;
            cout << endl;
        }
    }
    
    void mate(int gen = 0)
    {
        // tournament, do it in parallel
        for (int i = 0; i < population.size() * REPLACEMENT_RATIO; i += 2) {
            double maxFit = 0.0;
            int p1 = 0, p2 = 0;
            for (int j = 0; j < TOURNAMENT_SIZE; j++) {
                int r = rand() % population.size();
                double fit = computeFitness(population[r]);
                if (fit >= maxFit) {
                    // parsimony
                    if (fabs(fit - maxFit) < 0.005) {
                        if (population[r]->getSize() >= population[p1]->getSize())
                            continue;
                    }
                    maxFit = fit;
                    p1 = r;
                }
            }
            for (int j = 0; j < TOURNAMENT_SIZE; j++) {
                int r = rand() % population.size();
                double fit = computeFitness(population[r]);
                if (fit >= maxFit) {
                    // parsimony
                    if (fabs(fit - maxFit) < 0.005) {
                        if (population[r]->getSize() >= population[p2]->getSize())
                            continue;
                    }
                    maxFit = fit;
                    p2 = r;
                }
            }
            // crossover
            crossover(population[p1], population[p2]);
        }
        
        // replace older generation
        // should not be a pain
        ExpTree *maxTree = NULL;
        double maxFit = 0.0;
        for (map<ExpTree *, double>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
            if (it->second >= maxFit) {
                // enforce parsimony
                if (fabs(it->second - maxFit) < 0.005) {
                    if (it->first->getSize() >= maxTree->getSize())
                        continue;
                }
                maxTree = it->first;
                maxFit = it->second;
            }
        }
        string filename("/home/thewbp/SharedFolder/train/gen_");
        filename += to_string(gen);
        filename += ".txt";
        maxTree->printTo(filename, maxFit);
        maxTree->writeTo("/home/thewbp/SharedFolder/train/best.txt");
        for (int i = 0; i < offsprings.size() * REPLACEMENT_RATIO; i++) {
            int r = rand() % population.size();
            while (population[r] == maxTree) {
                r = rand() % population.size();
            }
            fitnesses.erase(population[r]);
            delete population[r];
            population[r] = offsprings[i];
        }
        
        // clear offsprings
        offsprings.clear();
    }
    
    double computeFitness(ExpTree *tree)
    {
        if (fitnesses.count(tree) == 0) {
            // save tree
            tree->writeTo("/home/thewbp/SharedFolder/train/tree.txt");
            // most time-consuming
            // recompile & run
            string script = string("cd ~/SharedFolder/test && ./run.sh ") + BENCHMARK;
            system(script.c_str());
            long cycles = grabStat();
            if (cycles == 0) {
                fitnesses[tree] = 0.0;
            } else {
                fitnesses[tree] = (double)BENCHMARK_CYCLES / grabStat();
            }
        }
        
        return fitnesses[tree];
    }
    
    long grabStat()
    {
        string filename = "/home/thewbp/SharedFolder/test/";
        filename += BENCHMARK;
        filename += "_HS/simu_intermediate/PD_STATS";
        cout << filename << endl;
        
        ifstream infile;
        infile.open(filename.c_str());
        
        std::string line;
        
        bool flag = false;
        while (std::getline(infile, line))
        {
            if (line == "Totals {") {
                flag = true;
                continue;
            }
            
            if(flag){
                if (line.substr (0,13) != "\ttotal_cycles") {
                    std::cout << line.substr(0, 13) << "\n";
                    return 0;
                }
                
                for(int i=13; i<line.size(); i++){
                    if(line[i] == '.'){
                        continue;
                    }
                    
                    string num = line.substr(i);
                    int value = atoi(num.c_str());
                    return value;
                }
                return 0;
            }
        }
        return 0;
    }
};
