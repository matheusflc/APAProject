#ifndef VND_H
#define VND_H

#include "Guloso.h"
#include "InstanceReader.h"
#include "VndSwap.h"
#include "Vnd2opt.h"
#include "VndRe.h"

using namespace std;

class Vnd
{
    private:
        void VNDswap(Guloso* guloso, InstanceReader* redaer);

        void VND2opt(Guloso* guloso, InstanceReader* redaer);

        bool canSwap(Guloso* guloso, InstanceReader* redaer, int& sv1, int& sv2);

        int calculateImprovement(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2,
                                    int& totalCostAfterSwap_1, int& totalCostAfterSwap_2);
                                    

        VndSwap* VDNSwap;
        Vnd2opt* VDN2opt;
        VndRe* VDNRe;
        
        
    public:
        Vnd();
        ~Vnd();

        void Run(Guloso* Guloso, InstanceReader* InstanceReader);

        vector<vector<int>> ILSPerturbation(Guloso* guloso, InstanceReader* reader);

        void playILS(Guloso* guloso, InstanceReader* instanceReader);

        void VNDReInsertion(Guloso* guloso, InstanceReader* reader);



};

#endif
