#ifndef GULOSO_H
#define GULOSO_H

#include "InstanceReader.h"

using namespace std;



class Guloso{
    private:
        int custoTotal;
        

    public:
        Guloso();
        
        ~Guloso();

        void printar();

        void play(InstanceReader* reader);

        void setCustoTotal(int custoTotal);

        void setAlocacao(vector<vector<int>> alocacao);

        int getCustoTotal();

        vector<vector<int>> getAlocacao();

        vector<vector<int>> alocacao;




};

#endif