#include "Vnd2opt.h"

Vnd2opt::Vnd2opt(/* args */)
{
}

Vnd2opt::~Vnd2opt()
{
}

void Vnd2opt::VND2opt(Guloso* guloso, InstanceReader* reader){
    int bestImprovement = guloso->getCustoTotal();
    int bestImprovementLocacao = 0;
    int totalCostAfterSwap_1 = 0, totalCostAfterSwap_2 = 0;
    pair<int, int> bestSwap;
    int newCap_1 , newCap_2;
    int newCost = 0;
    
    // Avalia todas as possíveis trocas
    for (int i = 0; i < guloso->getAlocacao().size() - 1; ++i) {
        for (int j = i + 1; j < guloso->getAlocacao().size(); ++j) {
            // Verifica se a troca é possível respeitando a capacidade dos servidores
            if (canSwap(guloso, reader, i, j)) {
                int improvement = calculateImprovement(guloso, reader, i, j, 
                                                totalCostAfterSwap_1, totalCostAfterSwap_2);
                if (improvement < bestImprovement) {
                    bestImprovement = improvement;
                    bestSwap = {i, j};
                }
            }
        }
    }
    
    // Realiza a melhor troca, se encontrada
    if (bestImprovement < guloso->getCustoTotal()) {
        swap(guloso->alocacao[bestSwap.first], guloso->alocacao[bestSwap.second]);
        
        // Atualiza o custo total após a troca
        int calculeLocal = guloso->getAlocacao().back().size() * 1000;

        int alocacao = bestImprovement - calculeLocal;
        guloso->setCustoTotal(bestImprovement);
        guloso->setCustoAlocacao(alocacao);
        guloso->setCustoLocal(calculeLocal);

        if (bestSwap.second + 1 != guloso->getAlocacao().size()){
            newCap_1 = guloso->getCapSv()[bestSwap.second];
            newCap_2 = guloso->getCapSv()[bestSwap.first];
            guloso->setCapSv(2,bestSwap.first, newCap_1);
            guloso->setCapSv(2,bestSwap.second, newCap_2);
        }else{
            for (int Index : guloso->alocacao[bestSwap.first]) {
                newCost += reader->getT()[bestSwap.first][Index];
            }
            guloso->setCapSv(2,bestSwap.first, newCost);
        }
    }
}

bool Vnd2opt::canSwap(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2){
    if (guloso->getAlocacao()[sv1].size() == 1 && guloso->getAlocacao()[sv2].size() == 1){
        return false;
    }

    int timeSv1 = 0;
    int timeSv2 = 0;
    bool server1CanAccommodate;
    bool server2CanAccommodate;

    if(sv2 + 1 == guloso->getAlocacao().size()){
        // verifica se a quantidade de job no servidor 1 é maior que localmente
        if(guloso->getAlocacao()[sv1].size() > guloso->getAlocacao()[sv2].size()){
            return false;
        }
    }else{
        // Calcula o tempo total de processamento dos jobs do servidor 1 se fossem alocados no servidor 2
        for (int jobIndex : guloso->alocacao[sv1]) {
            timeSv2 += reader->getT()[sv2][jobIndex];
        }
    }
    
    // Calcula o tempo total de processamento dos jobs do servidor 2 se fossem alocados no servidor 1
    for (int jobIndex : guloso->alocacao[sv2]) {
        timeSv1 += reader->getT()[sv1][jobIndex];
    }

    // Verifica se a soma dos tempos de processamento dos jobs realocados não excede a capacidade dos servidores
    server1CanAccommodate = timeSv1 <= reader->getB()[sv1];

    if(sv2 + 1 == guloso->getAlocacao().size()){
        server2CanAccommodate = true;
    }else{
        server2CanAccommodate = timeSv2 <= reader->getB()[sv2];
    }

    return server1CanAccommodate && server2CanAccommodate;
}

int Vnd2opt::calculateImprovement(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2,
                            int& totalCostAfterSwap_1, int& totalCostAfterSwap_2){
    int localCost_1 = 0;
    int localCost_2 = 0;
    int improvement = 0;
    int totalCostAfter1 = 0;
    int totalCostAfter2 = 0;

    
    // Calcula o custo total se os jobs dos servidores server1 e server2 fossem trocados
    // Primeiro, adiciona o custo dos jobs de server1 se fossem processados em server2
    if(sv2 + 1 != guloso->getAlocacao().size()){
        // calcula o tempo antes no servidor 2
        for (int jobIndex : guloso->alocacao[sv2]) {
            totalCostAfter1 += reader->getC()[sv2][jobIndex];
        }
        // Em seguida, adiciona o custo dos jobs de server1 se fossem processados em server2
        for (int jobIndex : guloso->alocacao[sv1]) {
            totalCostAfterSwap_1 += reader->getC()[sv2][jobIndex];
        }
    }else{
        // calcula caso a troca seja com os jobs alocados localmente
        localCost_1 = guloso->getAlocacao()[sv1].size() * 1000;
        localCost_2 = guloso->getAlocacao()[sv2].size() * 1000;
    }
   
    // Em seguida, adiciona o custo dos jobs de server2 se fossem processados em server1
    for (int jobIndex : guloso->alocacao[sv2]) {
        totalCostAfterSwap_2 += reader->getC()[sv1][jobIndex];
    }
    // calcula o tempo antes no servidor 1
    for (int jobIndex : guloso->alocacao[sv1]) {
        totalCostAfter1 += reader->getC()[sv1][jobIndex];
    }
  

    if(sv2 + 1 != guloso->getAlocacao().size()){
        improvement = guloso->getCustoTotal() - totalCostAfter1 - totalCostAfter2
                        + totalCostAfterSwap_1 + totalCostAfterSwap_2;
    }else{
        improvement = guloso->getCustoTotal() - localCost_2 + totalCostAfterSwap_2
                        + localCost_1;
    }
    
    return improvement;
}