#ifndef VND_H
#define VND_H

#include "Guloso.h"
#include "InstanceReader.h"

class Vnd
{
    private:
        void VNDswap(Guloso* guloso, InstanceReader* redaer);

        void VND2opt(Guloso* guloso, InstanceReader* redaer);

        bool canSwap(Guloso* guloso, InstanceReader* redaer, int& sv1, int& sv2);

        int calculateImprovement(Guloso* guloso, InstanceReader* instanceReader, int& sv1, int& sv2);
        
        
    public:
        Vnd();
        ~Vnd();

        void Run(Guloso* Guloso, InstanceReader* InstanceReader);



};

#endif
