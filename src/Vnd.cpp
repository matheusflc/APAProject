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
            VNDswap(Guloso, InstanceReader);
            /*chamar swap*/
        }else if (k == 2){
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
                            cout << "DEU O BREAK" << endl;
                            break;
                        }
                    }else{
                        if(guloso->getCapSv()[servidor1] - reader->t[servidor1][job1] + reader->t[servidor1][job2] > reader->b[servidor1]){
                            cout << "DEU O BREAK" << endl;
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
             << guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.first] << " no Servidor " << melhorTrocaServidores.first
             << " e Job " << guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.second] << " no Servidor " << melhorTrocaServidores.second 
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