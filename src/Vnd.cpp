#include "Vnd.h"


Vnd::Vnd(){
}

Vnd::~Vnd(){
}


void Vnd::Run(Guloso* Guloso, InstanceReader* InstanceReader){
    int k = 1;
    int custotal = Guloso->getCustoTotal();

    while (k <= 3)
    {
        if (k == 1){
            cout << "Entrou no Swap" << endl;
            VNDswap(Guloso, InstanceReader);
            /*chamar swap*/
        }else if (k == 2){
            cout << "Entrou no 2opt" << endl;
            VND2opt(Guloso, InstanceReader);
            /*chamar 2-pot*/
        }else{
            /*chamar re-insertion*/
        }
        if(Guloso->getCustoTotal() < custotal) {
            custotal = Guloso->getCustoTotal();
            k = 1;
        }else{
            k += 1;
        }
    }
    
}

void Vnd::VNDswap(Guloso* guloso, InstanceReader* reader) {
    int melhorDiferenca = guloso->getCustoTotal(); // Inicializa com o máximo para encontrar a menor diferença
    pair<int, int> melhorTrocaServidores; // Guarda os índices dos servidores da melhor troca
    pair<int, int> melhorTrocaJobs; // Guarda os índices dos jobs da melhor troca
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
                        if(guloso->getCapSv()[servidor1] - reader->t[servidor1][job1] + reader->t[servidor2][job2] > reader->b[servidor1] && 
                        guloso->getCapSv()[servidor2] - reader->t[servidor2][job2] + reader->t[servidor1][job1] > reader->b[servidor2]){
                            break;
                        }
                    }else{
                        if(guloso->getCapSv()[servidor1] - reader->t[servidor1][job1] + reader->t[servidor1][job2] > reader->b[servidor1]){ 
                            break;
                        }
                    }

                    if(servidor2 + 1 == guloso->alocacao.size()){
                        custoAntes = reader->c[servidor1][job1];

                        custoDepois = reader->c[servidor1][job2];
                    }else{
                        custoAntes = reader->c[servidor1][job1] + reader->c[servidor2][job2];
                    
                        custoDepois = reader->c[servidor2][job1] + reader->c[servidor1][job2];
                    }

                    // Calcula o custo antes e depois da troca proposta
            
                    diferencaCusto = guloso->getCustoTotal() + custoDepois - custoAntes;

                    // Se a troca resultar em uma redução maior do custo total, registra a troca
                    if (diferencaCusto < melhorDiferenca) {
                        //cout << job1 << endl;
                        //cout << job2 << endl;
                        custoAlocacao =  diferencaCusto - guloso->getCustoLocal();
                        melhorDiferenca = diferencaCusto;
                        melhorTrocaServidores = {servidor1, servidor2};
                        melhorTrocaJobs = {job1Index, job2Index};
                    }
                }
            }
        }
    }

    // Se uma melhoria foi encontrada, realiza a melhor troca
    if (melhorDiferenca < guloso->getCustoTotal()) {

        // Realiza a troca
        swap(guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.first], 
                guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.second]);

        // Atualiza o custo total no objeto Guloso
        guloso->setCustoTotal(melhorDiferenca);

        cout << "Melhoria encontrada e troca realizada entre Job " 
             << guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.second] << " no Servidor " << melhorTrocaServidores.first
             << " e Job " << guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.first] << " no Servidor " << melhorTrocaServidores.second 
             << " com custo agora de " << melhorDiferenca << ".\n";
        

        guloso->setCustoAlocacao(custoAlocacao);
        
        cout << guloso->getCustoTotal() << endl;
        cout << guloso->getCustoAlocacao() << endl;
        cout << guloso->getCustoLocal() << endl;

        for(int s = 0; s < guloso->alocacao.size(); s++){
            for(int j = 0; j < guloso->alocacao[s].size(); j++){
                cout << guloso->alocacao[s][j] << " ";
            }
            cout << endl;
        }

        
    } else {
        cout << "Nenhuma melhoria encontrada através do método swap." << endl;
    }

}

void Vnd::VND2opt(Guloso* guloso, InstanceReader* reader) {
    int bestImprovement = 0;
    std::pair<int, int> bestSwap;
    
    // Avalia todas as possíveis trocas
    for (int i = 0; i < reader->m - 1; ++i) {
        for (int j = i + 1; j < reader->m; ++j) {
            // Verifica se a troca é possível respeitando a capacidade dos servidores
            if (canSwap(guloso, reader, i, j)) {
                //cout << "saiu do canswap" << endl;
                int improvement = calculateImprovement(guloso, reader, i, j);
                if (improvement > bestImprovement) {
                    
                    cout << "Uma melhoria foi encontrada através do método 2opt" << endl;
                    bestImprovement = improvement;
                    bestSwap = {i, j};
                }
            }
        }
    }
    
    // Realiza a melhor troca, se encontrada
    if (bestImprovement > 0) {
        std::swap(guloso->alocacao[bestSwap.first], guloso->alocacao[bestSwap.second]);
        
        // Atualiza o custo total após a troca
        guloso->recalculateCost(reader);

        cout << guloso->getCustoTotal() << endl;
        cout << guloso->getCustoAlocacao() << endl;
        cout << guloso->getCustoLocal() << endl;

        for(int s = 0; s < guloso->alocacao.size(); s++){
            for(int j = 0; j < guloso->alocacao[s].size(); j++){
                cout << guloso->alocacao[s][j] << " ";
            }
            cout << endl;
        }

    }else{
        cout << "Nenhuma melhoria encontrada através do método 2opt." << endl;
    }

}

bool Vnd::canSwap(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2){
    // Calcula a soma dos tempos de processamento dos jobs atualmente alocados em cada servidor
    int timeSv1 = 0;
    int timeSv2 = 0;

    //cout << sv1 << " " << sv2 << endl;

    // Calcula o tempo total de processamento dos jobs do servidor 1 se fossem alocados no servidor 2
    for (int jobIndex : guloso->alocacao[sv1]) {
        timeSv2 += reader->t[sv2][jobIndex];
    }

    // Calcula o tempo total de processamento dos jobs do servidor 2 se fossem alocados no servidor 1
    for (int jobIndex : guloso->alocacao[sv2]) {
        timeSv1 += reader->t[sv1][jobIndex];
    }

    //cout << "TimeSv1: " << timeSv1 << endl;
    //cout << "TimeSv2: " << timeSv2 << endl;

    //cout << "Capacidade Sv1: " << reader->b[sv1] << endl;
    //cout << "Capacidade Sv2: " << reader->b[sv2] << endl;

    // Verifica se a soma dos tempos de processamento dos jobs realocados não excede a capacidade dos servidores
    bool server1CanAccommodate = timeSv1 <= reader->b[sv1];
    bool server2CanAccommodate = timeSv2 <= reader->b[sv2];

    return server1CanAccommodate && server2CanAccommodate;

}


int Vnd::calculateImprovement(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2) {
    // Custo total antes da troca
    int totalCostBeforeSwap = 0;
    // Custo total após a troca proposta
    int totalCostAfterSwap = 0;

    // Calcula o custo total atual considerando todos os servidores
    for (int s = 0; s < reader->m; ++s) {
        for (int jobIndex : guloso->alocacao[s]) {
            totalCostBeforeSwap += reader->c[s][jobIndex];
        }
    }
    
    // Calcula o custo total se os jobs dos servidores server1 e server2 fossem trocados
    // Primeiro, adiciona o custo dos jobs de server1 se fossem processados em server2
    for (int jobIndex : guloso->alocacao[sv1]) {
        totalCostAfterSwap += reader->c[sv2][jobIndex];
    }
    // Em seguida, adiciona o custo dos jobs de server2 se fossem processados em server1
    for (int jobIndex : guloso->alocacao[sv2]) {
        totalCostAfterSwap += reader->c[sv1][jobIndex];
    }
    // Para os demais servidores, o custo permanece o mesmo
    
    for (int s = 0; s < reader->m; ++s) {
        if (s != sv1 && s != sv2) {
            for (int jobIndex : guloso->alocacao[s]) {
                totalCostAfterSwap += reader->c[s][jobIndex];
            }
        }
    }
    cout << "Custo antes do opt: " << totalCostBeforeSwap << endl;
    cout << "Custo depois do opt: " << totalCostAfterSwap << endl;


    // A melhoria é a diferença entre o custo total antes e após a troca
    int improvement = totalCostBeforeSwap - totalCostAfterSwap;
    return improvement;
}

