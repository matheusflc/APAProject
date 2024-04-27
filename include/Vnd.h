#ifndef VND_H
#define VND_H

#include "Guloso.h"
#include "InstanceReader.h"

class Vnd
{
    private:
        void VNDswap(Guloso* guloso, InstanceReader* redaer);
        
        
    public:
        Vnd();
        ~Vnd();

        void Run(Guloso* Guloso, InstanceReader* InstanceReader);



};

#endif
