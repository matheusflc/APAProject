#include "VndRe.h"

VndRe::VndRe(/* args */)
{
}

VndRe::~VndRe()
{
}

void VndRe::VNDReInsertion(Guloso* guloso, InstanceReader* reader){
    int aux = 0;
    int job, improvement;
    int bestImprovement = guloso->getCustoTotal();
    pair<int, int> bestServidor;
    pair<int, int> bestJob;
    int newCap_1, newCap_2;

    while (aux < guloso->getAlocacao().size()){
        for (int job1Index = 0; job1Index < guloso->getAlocacao()[aux].size(); ++job1Index)
        {
            job = guloso->getAlocacao()[aux][job1Index];
            for (int servidorIndex = 0; servidorIndex < guloso->getAlocacao().size() - 1; ++servidorIndex){
                if (servidorIndex == aux ){
                    continue;
                }else{
                    if (guloso->getCapSv()[servidorIndex] + reader->getT()[servidorIndex][job] > reader->getB()[servidorIndex] ){
                        continue;
                    }

                    if (aux + 1 == guloso->getAlocacao().size()){
                        improvement = (guloso->getCustoTotal() - 1000) + reader->getC()[servidorIndex][job];
                    }else{
                        improvement = (guloso->getCustoTotal() - reader->getC()[aux][job]) + reader->getC()[servidorIndex][job];
                    }
                    
                    if (improvement < bestImprovement){
                        bestImprovement = improvement;
                        bestServidor = {aux, servidorIndex};
                        bestJob = {job1Index, job};
                    }

                }
            }   
        }   

        aux++;
    }

    if(bestImprovement < guloso->getCustoTotal()){
        guloso->alocacao[bestServidor.first].erase(guloso->alocacao
                [bestServidor.first].begin() + bestJob.first);

        guloso->alocacao[bestServidor.second].push_back(bestJob.second);
        
        int calculeLocal = guloso->getAlocacao().back().size() * 1000;
        int alocacao = bestImprovement - calculeLocal;
        guloso->setCustoTotal(bestImprovement);
        guloso->setCustoAlocacao(alocacao);
        guloso->setCustoLocal(calculeLocal);

        newCap_1 = guloso->getCapSv()[bestServidor.second] + reader->getT()[bestServidor.second][bestJob.second];

        if(bestServidor.first + 1 != guloso->getAlocacao().size()){
            newCap_2 = guloso->getCapSv()[bestServidor.first] - reader->getT()[bestServidor.first][bestJob.second];
            guloso->setCapSv(2,bestServidor.first, newCap_2);
        }

        guloso->setCapSv(2,bestServidor.second, newCap_1);
    }
}