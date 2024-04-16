#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H

#include <bits/stdc++.h>

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
};

#endif /* INSTANCEREADER_H */
