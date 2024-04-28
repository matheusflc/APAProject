#include "VndSwap.h"

VndSwap::VndSwap(/* args */)
{

}

VndSwap::~VndSwap()
{

}

void VndSwap::VNDswap(Guloso* guloso, InstanceReader* reader){
    int melhorDiferenca = guloso->getCustoTotal(); // Inicializa com o máximo para encontrar a menor diferença
    pair<int, int> melhorTrocaServidores; // Guarda os índices dos servidores da melhor troca
    pair<int, int> melhorTrocaJobs; // Guarda os índices dos jobs da melhor troca
    pair<int, int> melhorJob;  // Guarda os melhores Job
    int diferencaCusto, custoAntes, custoDepois, custoAlocacao;
    
    
    // Itera sobre todos os pares possíveis de servidores para a troca
    for (int servidor1 = 0; servidor1 < guloso->alocacao.size() - 1; ++servidor1) {
        for (int servidor2 = servidor1 + 1; servidor2 < guloso->alocacao.size(); ++servidor2) {
            // Itera sobre todos os jobs alocados para esses servidores
            for (int job1Index = 0; job1Index < guloso->alocacao[servidor1].size(); ++job1Index) {
                for (int job2Index = 0; job2Index < guloso->alocacao[servidor2].size(); ++job2Index) {
                    int job1 = guloso->alocacao[servidor1][job1Index];
                    int job2 = guloso->alocacao[servidor2][job2Index];
                    
                    if(servidor2 + 1 != guloso->alocacao.size()){
                        if((guloso->getCapSv()[servidor1] - reader->getT()[servidor1][job1]) + reader->getT()[servidor1][job2] > reader->getB()[servidor1] || 
                            (guloso->getCapSv()[servidor2] - reader->getT()[servidor2][job2]) + reader->getT()[servidor2][job1] > reader->getB()[servidor2]){
                            continue;
                        }
                    }else{
                        if((guloso->getCapSv()[servidor1] - reader->getT()[servidor1][job1]) + reader->getT()[servidor1][job2] > reader->getB()[servidor1]){ 
                            continue;
                        }
                    }

                    if(servidor2 + 1 == guloso->alocacao.size()){
                        custoAntes = reader->getC()[servidor1][job1];

                        custoDepois = reader->getC()[servidor1][job2];
                    }else{
                        custoAntes = reader->getC()[servidor1][job1] + reader->getC()[servidor2][job2];
                    
                        custoDepois = reader->getC()[servidor2][job1] + reader->getC()[servidor1][job2];
                    }

                    // Calcula o custo antes e depois da troca proposta
            
                    diferencaCusto = guloso->getCustoTotal() + custoDepois - custoAntes;

                    // Se a troca resultar em uma redução maior do custo total, registra a troca
                    if (diferencaCusto < melhorDiferenca) {
                        custoAlocacao =  diferencaCusto - guloso->getCustoLocal();
                        melhorDiferenca = diferencaCusto;
                        melhorTrocaServidores = {servidor1, servidor2};
                        melhorTrocaJobs = {job1Index, job2Index};
                        melhorJob = {job1, job2};
                    }
                }
            }
        }
    }

    // Se uma melhoria foi encontrada, realiza a melhor troca
    if (melhorDiferenca < guloso->getCustoTotal()) {

        //atualiza o custo de cada servidor 
        int newCap_1 = guloso->getCapSv()[melhorTrocaServidores.first] - 
                        reader->getT()[melhorTrocaServidores.first][melhorJob.first] 
                        + reader->getT()[melhorTrocaServidores.first][melhorJob.second];
        
        guloso->setCapSv(2, melhorTrocaServidores.first, newCap_1);

        if (melhorTrocaServidores.second + 1 != guloso->getAlocacao().size()){
            int newCap_2 = guloso->getCapSv()[melhorTrocaServidores.second] - 
                            reader->getT()[melhorTrocaServidores.second][melhorJob.second] 
                            + reader->getT()[melhorTrocaServidores.second][melhorJob.first];
        
            guloso->setCapSv(2, melhorTrocaServidores.second, newCap_2);
        }

        // Realiza a troca
        swap(guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.first], 
                guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.second]);

        // Atualiza o custo total no objeto Guloso
        guloso->setCustoTotal(melhorDiferenca);

        guloso->setCustoAlocacao(custoAlocacao);

    }
}