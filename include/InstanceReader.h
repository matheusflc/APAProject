#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H

#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>

using namespace std;

class InstanceReader {
private:
    
    int n; // número de jobs
    int m; // número de servidores
    int p; // custo fixo para job não alocado

    vector<int> b; // array b
    vector<vector<int>> t; // matriz t (servidores x jobs)
    vector<vector<int>> c; // matriz c (servidores x jobs)
    

public:
    InstanceReader();
    bool readInstance(const string& filename);
    void printInstance();

    
    vector<int> getB();
    vector<vector<int>> getT();
    vector<vector<int>> getC();
    int getN();
    int getM();
    int getP();
};

#endif /* INSTANCEREADER_H */
