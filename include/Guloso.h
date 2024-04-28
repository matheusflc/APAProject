#ifndef GULOSO_H
#define GULOSO_H

#include "InstanceReader.h"

using namespace std;



class Guloso{
    private:
        int custoTotal;
        int custoAlocacao;
        int custoLocal;
        vector<int> CapSv;
        

    public:
        vector<vector<int>> alocacao;
        
        Guloso();
        
        ~Guloso();

        void printar();

        void play(InstanceReader* reader);

        void recalculateCost(InstanceReader* instanceReader);

        void escreverResultadoEmArquivo(Guloso* guloso, const string& nomeArquivo);

        

        

        //Getters
        int getCustoTotal();
        vector<vector<int>> getAlocacao();
        int getCustoLocal();
        int getCustoAlocacao();
        vector<int> getCapSv();

        //Setters
        void setAlocacao(vector<vector<int>> alocacao);
        void setCustoTotal(int custoTotal);
        void setCustoLocal(int custoLocal);
        void setCapSv(int var, int server, int valor);
        void setCustoAlocacao(int custoAlocacao);

        

        

        






};

#endif