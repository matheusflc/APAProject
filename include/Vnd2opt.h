#ifndef VND2opt_H
#define VND2opt_H

using namespace std;

#include "Guloso.h"
#include "InstanceReader.h"

class Vnd2opt
{
private:
    
public:
    Vnd2opt();
    ~Vnd2opt();

    void VND2opt(Guloso* guloso, InstanceReader* reader);

    bool canSwap(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2);

    int calculateImprovement(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2,
                            int& totalCostAfterSwap_1, int& totalCostAfterSwap_2);
                                    

};


#endif