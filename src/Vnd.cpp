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
            VNDswap(Guloso, InstanceReader->c, Guloso->getAlocacao());
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

// void Vnd::VNDswap(Guloso* guloso, vector<vector<int>>& custo, vector<vector<int>>& alocacao) {
//     int melhorDiferenca = 0; // Guarda a melhor diferença de custo encontrada
//     pair<int, int> melhorTrocaServidores; // Guarda os índices dos servidores da melhor troca
//     pair<int, int> melhorTrocaJobs; // Guarda os índices dos jobs da melhor troca

//     // Itera sobre todos os pares possíveis de servidores para a troca
//     for (int servidor1 = 0; servidor1 < alocacao.size() - 1; ++servidor1) {
//         for (int servidor2 = servidor1 + 1; servidor2 < alocacao.size(); ++servidor2) {
//             // Itera sobre todos os jobs alocados para esses servidores
//             for (int job1Index = 0; job1Index < alocacao[servidor1].size(); ++job1Index) {
//                 for (int job2Index = 0; job2Index < alocacao[servidor2].size(); ++job2Index) {
//                     int job1 = alocacao[servidor1][job1Index];
//                     int job2 = alocacao[servidor2][job2Index];

//                     // Calcula o custo antes e depois da troca proposta
//                     int custoAntes = custo[servidor1][job1] + custo[servidor2][job2];
//                     int custoDepois = custo[servidor2][job1] + custo[servidor1][job2];
//                     int diferencaCusto = custoDepois - custoAntes;

//                     // Se a troca resultar em uma redução maior do custo total, registra a troca
//                     if (diferencaCusto < melhorDiferenca) {
//                         melhorDiferenca = diferencaCusto;
//                         melhorTrocaServidores = {servidor1, servidor2};
//                         melhorTrocaJobs = {job1Index, job2Index};
//                     }
//                 }
//             }
//         }
//     }

//     // Se uma melhoria foi encontrada, realiza a melhor troca
//     if (melhorDiferenca < 0) {
//         int servidor1 = melhorTrocaServidores.first;
//         int servidor2 = melhorTrocaServidores.second;
//         int job1Index = melhorTrocaJobs.first;
//         int job2Index = melhorTrocaJobs.second;

//         // Realiza a troca
//         swap(alocacao[servidor1][job1Index], alocacao[servidor2][job2Index]);

//         // Atualiza o custo total no objeto Guloso
//         guloso->setCustoTotal(guloso->getCustoTotal() + melhorDiferenca);

//         cout << "Melhoria encontrada e troca realizada entre Job " 
//              << alocacao[servidor1][job1Index] << " no Servidor " << servidor1
//              << " e Job " << alocacao[servidor2][job2Index] << " no Servidor " << servidor2 
//              << " com uma redução de custo de " << -melhorDiferenca << ".\n";
//     } else {
//         cout << "Nenhuma melhoria encontrada através do método swap." << endl;
//     }
// }

void Vnd::VNDswap(Guloso* guloso, vector<vector<int>>& custo, vector<vector<int>>& alocacao) {
    int melhorDiferenca = INT_MAX; // Inicializa com o máximo para encontrar a menor diferença
    pair<int, int> melhorTrocaServidores; // Guarda os índices dos servidores da melhor troca
    pair<int, int> melhorTrocaJobs; // Guarda os índices dos jobs da melhor troca

    // Itera sobre todos os pares possíveis de servidores para a troca
    for (int servidor1 = 0; servidor1 < alocacao.size() - 1; ++servidor1) {
        for (int servidor2 = servidor1 + 1; servidor2 < alocacao.size(); ++servidor2) {
            // Itera sobre todos os jobs alocados para esses servidores
            for (int job1Index = 0; job1Index < alocacao[servidor1].size(); ++job1Index) {
                for (int job2Index = 0; job2Index < alocacao[servidor2].size(); ++job2Index) {
                    int job1 = alocacao[servidor1][job1Index];
                    int job2 = alocacao[servidor2][job2Index];

                    // Calcula o custo antes e depois da troca proposta
                    int custoAntes = custo[servidor1][job1] + custo[servidor2][job2];
                    int custoDepois = custo[servidor2][job1] + custo[servidor1][job2];
                    int diferencaCusto = custoDepois - custoAntes;

                    // Se a troca resultar em uma redução maior do custo total, registra a troca
                    if (diferencaCusto < melhorDiferenca) {
                        melhorDiferenca = diferencaCusto;
                        melhorTrocaServidores = {servidor1, servidor2};
                        melhorTrocaJobs = {job1Index, job2Index};
                    }
                }
            }
        }
    }

    // Se uma melhoria foi encontrada, realiza a melhor troca
    if (melhorDiferenca < 0) {
        int servidor1 = melhorTrocaServidores.first;
        int servidor2 = melhorTrocaServidores.second;
        int job1Index = melhorTrocaJobs.first;
        int job2Index = melhorTrocaJobs.second;

        // Realiza a troca
        swap(alocacao[servidor1][job1Index], alocacao[servidor2][job2Index]);

        // Atualiza o custo total no objeto Guloso
        guloso->setCustoTotal(guloso->getCustoTotal() + melhorDiferenca);

        cout << "Melhoria encontrada e troca realizada entre Job " 
             << alocacao[servidor1][job1Index] << " no Servidor " << servidor1
             << " e Job " << alocacao[servidor2][job2Index] << " no Servidor " << servidor2 
             << " com uma redução de custo de " << -melhorDiferenca << ".\n";
    } else {
        cout << "Nenhuma melhoria encontrada através do método swap." << endl;
    }
}