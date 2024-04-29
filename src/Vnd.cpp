#include "Vnd.h"


Vnd::Vnd(){
}

Vnd::~Vnd(){
}


void Vnd::Run(Guloso* Guloso, InstanceReader* InstanceReader){
    //auto t1 = chrono::high_resolution_clock::now();
    int k = 1;
    int custotal = Guloso->getCustoTotal();

    while (k <= 3)
    {
        if (k == 1){ 
            /*chamar swap*/
            VDNSwap->VNDswap(Guloso, InstanceReader); 
        }else if (k == 2){ 
            /*chamar 2-pot*/
            VDN2opt->VND2opt(Guloso, InstanceReader);
        }else{
            /*chamar re-insertion*/
            VDNRe->VNDReInsertion(Guloso, InstanceReader);
        }
        if(Guloso->getCustoTotal() < custotal) {
            custotal = Guloso->getCustoTotal();
            k = 1;
        }else{
            k += 1;
        }
    }
    //auto t2 = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::nanoseconds>( t2 - t1 ).count();

    //cout << duration << endl;
}

void Vnd::playILS(Guloso* guloso, InstanceReader* reader){
    int iteracoes = 15;
    Guloso melhorGuloso = *guloso;
    cout << "rodouils" << endl;

     while(iteracoes--){
        
        // Cria uma nova solução perturbada a partir da solução atual
        vector<vector<int>> solucaoPerturbada = ILSPerturbation(guloso, reader);
        
        // Cria uma cópia temporária do objeto guloso para aplicar a solução perturbada
        Guloso gulosoTemp = *guloso;
        gulosoTemp.setAlocacao(solucaoPerturbada);
        
        // Aplica o VND na solução perturbada
        Run(&gulosoTemp, reader);

        // Compara a solução melhorada com a melhor solução até agora
        if (gulosoTemp.getCustoTotal() < melhorGuloso.getCustoTotal()) {
            melhorGuloso = gulosoTemp; // Atualiza a melhor solução encontrada
        }
    }

    // Após encontrar a melhor solução dentre as iterações, atualiza a solução original
    *guloso = melhorGuloso;

    if(guloso->getCustoTotal() == melhorGuloso.getCustoTotal()){
        cout << "nenhuma melhora encontrada no ILS" << endl;
    }

    // Saída dos resultados da melhor solução
    cout << guloso->getCustoTotal() << endl;
    cout << guloso->getCustoAlocacao() << endl;
    cout << guloso->getCustoLocal() << endl;

    for (int s = 0; s <= reader->getM(); ++s) {
        //cout << "< lista de jobs alocados no servidor " << s + 1 << ">" << endl;
        for (int jobId : guloso->alocacao[s]) {
            cout << jobId << " "; // +1 para ajustar a indexação base-0 para uma apresentação mais intuitiva
        }
        cout << endl;
    }
}

vector<vector<int>> Vnd::ILSPerturbation(Guloso* guloso, InstanceReader* reader) {
    usleep(500);
    
    int m = reader->getM(); // Número de servidores
    vector<vector<int>> novaAlocacao = guloso->getAlocacao();
    bool local = false;
    
    
    // Escolhe dois servidores aleatoriamente para o swap
    int servidor1 = rand() % m;
    int servidor2 = rand() % m;

   
    int novaCapacidadeSv1;
    int novaCapacidadeSv2;

    if(servidor1 == m || servidor2 == m)
        local = true;



    while (servidor1 == servidor2) { // Garante que os dois servidores sejam diferentes
        servidor2 = rand() % m;
    }

    cout << "servidor 1: " << servidor1 << endl;
    cout << "servidor 2: " << servidor2 << endl;

    
    
    

    // Verifica se ambos os servidores têm pelo menos um job alocado para realizar o swap
    if (!novaAlocacao[servidor1].empty() && !novaAlocacao[servidor2].empty()) {
        // Escolhe aleatoriamente um job em cada servidor para o swap
        int jobIndex1 = rand() % novaAlocacao[servidor1].size();
        int jobIndex2 = rand() % novaAlocacao[servidor2].size();

        int job1 = novaAlocacao[servidor1][jobIndex1];
        int job2 = novaAlocacao[servidor2][jobIndex2];

        cout << "job1: " << job1 << endl;
        cout << "job2: " << job2 << endl;

        cout << "M: "<< m << endl;
        // Calcula o impacto da troca na capacidade dos servidores
        if(servidor1 != m){
            novaCapacidadeSv1 = guloso->getCapSv()[servidor1] - reader->getT()[servidor1][job1] + reader->getT()[servidor1][job2];
            cout << "capacidadesv1: " << guloso->getCapSv()[servidor1] << endl;
            cout << "novaCapacidadesv1: " << novaCapacidadeSv1 << endl;
        }

        if(servidor2 != m){
            novaCapacidadeSv2 = guloso->getCapSv()[servidor2] - reader->getT()[servidor2][job2] + reader->getT()[servidor2][job1];
            cout << "capacidadesv2: " << guloso->getCapSv()[servidor2] << endl;
            cout << "novaCapacidadesv1: " << novaCapacidadeSv2 << endl;
        }

        

        // Verifica se a troca respeita a capacidade dos servidores
        if (servidor2 == m && novaCapacidadeSv1 <= reader->getB()[servidor1]) {
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor1, novaCapacidadeSv1);

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->getC()[servidor1][job1];
            custoAlocacao -= reader->getC()[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->getC()[servidor1][job2];
            custoAlocacao += reader->getC()[servidor2][job1];
            guloso->setCustoAlocacao(custoAlocacao);

            // Não é necessário atualizar custoLocal, pois a perturbação não afeta jobs processados localmente

            // Recalcula o custo total após a perturbação
            custoTotal = guloso->getCustoAlocacao() + guloso->getCustoLocal();
            guloso->setCustoTotal(custoTotal);
        }else if(servidor1 == m && novaCapacidadeSv2 <= reader->getB()[servidor2]){
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);
            

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor2, novaCapacidadeSv2);

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->getC()[servidor1][job1];
            custoAlocacao -= reader->getC()[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->getC()[servidor1][job2];
            custoAlocacao += reader->getC()[servidor2][job1];
            guloso->setCustoAlocacao(custoAlocacao);

            // Não é necessário atualizar custoLocal, pois a perturbação não afeta jobs processados localmente

            // Recalcula o custo total após a perturbação
            custoTotal = guloso->getCustoAlocacao() + guloso->getCustoLocal();
            guloso->setCustoTotal(custoTotal);
        }else if(!local && novaCapacidadeSv2 <= reader->getB()[servidor2] && novaCapacidadeSv1 <= reader->getB()[servidor1]){
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor2, novaCapacidadeSv2);
            guloso->setCapSv(2, servidor1, novaCapacidadeSv1);
            

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->getC()[servidor1][job1];
            custoAlocacao -= reader->getC()[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->getC()[servidor1][job2];
            custoAlocacao += reader->getC()[servidor2][job1];
            guloso->setCustoAlocacao(custoAlocacao);

            // Não é necessário atualizar custoLocal, pois a perturbação não afeta jobs processados localmente

            // Recalcula o custo total após a perturbação
            custoTotal = guloso->getCustoAlocacao() + guloso->getCustoLocal();
            guloso->setCustoTotal(custoTotal);

        }
    }
// Saída dos resultados
    cout << "Pertubacao feita" << endl;
    cout << guloso->getCustoTotal() << endl;
    cout << guloso->getCustoAlocacao() << endl;
    cout << guloso->getCustoLocal() << endl;
    

    for (int s = 0; s <= novaAlocacao.size(); ++s) {
        //cout << "< lista de jobs alocados no servidor " << s + 1 << ">" << endl;
        for (int jobId : novaAlocacao[s]) {
            cout << jobId << " "; // +1 para ajustar a indexação base-0 para uma apresentação mais intuitiva
        }
        cout << endl;
    }
    cout << "aaaaaaaaaaa" << endl;
    return novaAlocacao;

}
