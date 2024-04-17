#ifndef GULOSO_H
#define GULOSO_H

#include "InstanceReader.h"

using namespace std;



class Guloso{
    private:

    public:
        Guloso();
        
        ~Guloso();

        void printar();

        void play(InstanceReader* reader);


};

#endif