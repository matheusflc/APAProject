#ifndef VNDRe_H
#define VNDRe_H

using namespace std;

#include "Guloso.h"
#include "InstanceReader.h"

class VndRe
{
private:
    /* data */
public:
    VndRe(/* args */);
    ~VndRe();

    void VNDReInsertion(Guloso* guloso, InstanceReader* reader);
};


#endif