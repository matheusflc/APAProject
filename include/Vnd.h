#ifndef VND_H
#define VND_H

#include "Guloso.h"
#include "InstanceReader.h"

class Vnd
{
    private:
        void VNDswap(Guloso* guloso, vector<vector<int>>& custo, vector<vector<int>>& alocacao);
        
        
    public:
        Vnd();
        ~Vnd();

        void Run(Guloso* Guloso, InstanceReader* InstanceReader);



};

#endif
