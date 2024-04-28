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
    pair<int, int> melhorJob;  
    int diferencaCusto, custoAntes, custoDepois, custoAlocacao;
    int cont = 0;
    
    
    // Itera sobre todos os pares possíveis de servidores para a troca
    for (int servidor1 = 0; servidor1 < guloso->alocacao.size() - 1; ++servidor1) {
        for (int servidor2 = servidor1 + 1; servidor2 < guloso->alocacao.size(); ++servidor2) {
            // Itera sobre todos os jobs alocados para esses servidores
            for (int job1Index = 0; job1Index < guloso->alocacao[servidor1].size(); ++job1Index) {
                for (int job2Index = 0; job2Index < guloso->alocacao[servidor2].size(); ++job2Index) {
                    int job1 = guloso->alocacao[servidor1][job1Index];
                    int job2 = guloso->alocacao[servidor2][job2Index];
                    
                    if(servidor2 + 1 != guloso->alocacao.size()){
                        if((guloso->getCapSv()[servidor1] - reader->t[servidor1][job1]) + reader->t[servidor1][job2] > reader->b[servidor1] || 
                            (guloso->getCapSv()[servidor2] - reader->t[servidor2][job2]) + reader->t[servidor2][job1] > reader->b[servidor2]){
                            continue;
                        }
                    }else{
                        if((guloso->getCapSv()[servidor1] - reader->t[servidor1][job1]) + reader->t[servidor1][job2] > reader->b[servidor1]){ 
                            continue;
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
                        reader->t[melhorTrocaServidores.first][melhorJob.first] 
                        + reader->t[melhorTrocaServidores.first][melhorJob.second];
        
        guloso->setCapSv(2, melhorTrocaServidores.first, newCap_1);

        if (melhorTrocaServidores.second + 1 != guloso->getAlocacao().size()){
            int newCap_2 = guloso->getCapSv()[melhorTrocaServidores.second] - 
                            reader->t[melhorTrocaServidores.second][melhorJob.second] 
                            + reader->t[melhorTrocaServidores.second][melhorJob.first];
        
            guloso->setCapSv(2, melhorTrocaServidores.second, newCap_2);
        }

        // Realiza a troca
        swap(guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.first], 
                guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.second]);

        // Atualiza o custo total no objeto Guloso
        guloso->setCustoTotal(melhorDiferenca);

        guloso->setCustoAlocacao(custoAlocacao);

        cout << "Melhoria encontrada e troca realizada entre Job " 
             << guloso->alocacao[melhorTrocaServidores.second][melhorTrocaJobs.second] << " no Servidor " << melhorTrocaServidores.first
             << " e Job " << guloso->alocacao[melhorTrocaServidores.first][melhorTrocaJobs.first] << " no Servidor " << melhorTrocaServidores.second 
             << " com custo agora de " << melhorDiferenca << ".\n";

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
    int bestImprovement = guloso->getCustoTotal();
    int bestImprovementLocacao = 0;
    int totalCostAfterSwap_1 = 0, totalCostAfterSwap_2 = 0;
    pair<int, int> bestSwap;
    
    // Avalia todas as possíveis trocas
    for (int i = 0; i < guloso->getAlocacao().size() - 1; ++i) {
        for (int j = i + 1; j < guloso->getAlocacao().size(); ++j) {
            // Verifica se a troca é possível respeitando a capacidade dos servidores
            if (canSwap(guloso, reader, i, j)) {
                //cout << "saiu do canswap" << endl;
                int improvement = calculateImprovement(guloso, reader, i, j, 
                                                totalCostAfterSwap_1, totalCostAfterSwap_2);
                if (improvement < bestImprovement) {
                    bestImprovement = improvement;
                    bestSwap = {i, j};
                    bestImprovementLocacao = improvement - (guloso->getAlocacao()[i].size() * 1000);
                }
            }
        }
    }
    
    // Realiza a melhor troca, se encontrada
    if (bestImprovement < guloso->getCustoTotal()) {
        swap(guloso->alocacao[bestSwap.first], guloso->alocacao[bestSwap.second]);
        
        // Atualiza o custo total após a troca
        //guloso->recalculateCost(reader);

        guloso->setCustoTotal(bestImprovement);
        guloso->setCustoAlocacao(bestImprovementLocacao);
        guloso->setCapSv(2,bestSwap.first,totalCostAfterSwap_2);

        if (bestSwap.second + 1 == guloso->getAlocacao().size()){
            int local = guloso->getAlocacao()[bestSwap.second].size() * 1000;
            guloso->setCustoLocal(local);
        }else{
            guloso->setCapSv(2,bestSwap.second,totalCostAfterSwap_1);
        }

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

    if (guloso->getAlocacao()[sv1].size() == 1 && guloso->getAlocacao()[sv2].size() == 1){
        return false;
    }

    // Calcula a soma dos tempos de processamento dos jobs atualmente alocados em cada servidor
    int timeSv1 = 0;
    int timeSv2 = 0;
    bool server1CanAccommodate;
    bool server2CanAccommodate;

    if(sv2 + 1 == guloso->getAlocacao().size()){
        if(guloso->getAlocacao()[sv1].size() > 1){
            return false;
        }
    }else{
        // Calcula o tempo total de processamento dos jobs do servidor 1 se fossem alocados no servidor 2
        for (int jobIndex : guloso->alocacao[sv1]) {
            timeSv2 += reader->t[sv2][jobIndex];
        }
    }

    //cout << sv1 << " " << sv2 << endl;
    
    // Calcula o tempo total de processamento dos jobs do servidor 2 se fossem alocados no servidor 1
    for (int jobIndex : guloso->alocacao[sv2]) {
        timeSv1 += reader->t[sv1][jobIndex];
    }

    //cout << "TimeSv1: " << timeSv1 << endl;
    //cout << "TimeSv2: " << timeSv2 << endl;

    //cout << "Capacidade Sv1: " << reader->b[sv1] << endl;
    //cout << "Capacidade Sv2: " << reader->b[sv2] << endl;

    // Verifica se a soma dos tempos de processamento dos jobs realocados não excede a capacidade dos servidores
    server1CanAccommodate = timeSv1 <= reader->b[sv1];

    if(sv2 + 1 == guloso->getAlocacao().size()){
        server2CanAccommodate = true;
    }else{
        server2CanAccommodate = timeSv2 <= reader->b[sv2];
    }

    //cout << "tempo_1 " << timeSv1 << endl;
    //cout << "tempo_2 " << timeSv2 << endl;
    

    return server1CanAccommodate && server2CanAccommodate;

}


int Vnd::calculateImprovement(Guloso* guloso, InstanceReader* reader, int& sv1, int& sv2,
                              int& totalCostAfterSwap_1, int& totalCostAfterSwap_2) {
    // Custo total antes da troca
    //int totalCostBeforeSwap = 0;
    // Custo total após a troca proposta
    int localCost_1 = 0;
    int localCost_2 = 0;
    int improvement = 0;
    //totalCostAfterSwap_1 = 0;
    //totalCostAfterSwap_2 = 0;

    // Calcula o custo total atual considerando todos os servidores
    //for (int s = 0; s < reader->m; ++s) {
      //  for (int jobIndex : guloso->alocacao[s]) {
        //    totalCostBeforeSwap += reader->c[s][jobIndex];
        //}
    //}
    
    // Calcula o custo total se os jobs dos servidores server1 e server2 fossem trocados
    // Primeiro, adiciona o custo dos jobs de server1 se fossem processados em server2
    if(sv2 + 1 != guloso->getAlocacao().size()){
        for (int jobIndex : guloso->alocacao[sv1]) {
            totalCostAfterSwap_1 += reader->c[sv2][jobIndex];
        }
    }else{
        localCost_1 = guloso->getAlocacao()[sv1].size() * 1000;
        localCost_2 = guloso->getAlocacao()[sv2].size() * 1000;
    }
   
    // Em seguida, adiciona o custo dos jobs de server2 se fossem processados em server1
    for (int jobIndex : guloso->alocacao[sv2]) {
        totalCostAfterSwap_2 += reader->c[sv1][jobIndex];
    }
    // Para os demais servidores, o custo permanece o mesmo
    
    //for (int s = 0; s < reader->m; ++s) {
      //  if (s != sv1 && s != sv2) {
        //    for (int jobIndex : guloso->alocacao[s]) {
          //      totalCostAfterSwap += reader->c[s][jobIndex];
            //}
        //}
    //}

    //cout << "Custo antes do opt: " << totalCostBeforeSwap << endl;
    //cout << "Custo depois do opt: " << totalCostAfterSwap << endl;
    if(sv2 + 1 != guloso->getAlocacao().size()){
        improvement = guloso->getCustoTotal() - guloso->getCapSv()[sv1] - guloso->getCapSv()[sv2]
                        + totalCostAfterSwap_1 + totalCostAfterSwap_2;
    }else{
        //cout <<"total " << guloso->getCustoTotal() << endl;
        //cout <<"capSv " << guloso->getCapSv()[sv1] << endl;
        //cout <<"localcost_2 " << localCost_2 << endl;
        //cout <<"after " << totalCostAfterSwap_2 << endl;
        //cout <<"localCost_1 " << localCost_1 << endl;
        improvement = guloso->getCustoTotal() - guloso->getCapSv()[sv1] - localCost_2 + totalCostAfterSwap_2
                        + localCost_1;
    }
    

    // A melhoria é a diferença entre o custo total antes e após a troca
    //int improvement = totalCostBeforeSwap - totalCostAfterSwap;
    return improvement;
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

    for (int s = 0; s <= reader->m; ++s) {
        //cout << "< lista de jobs alocados no servidor " << s + 1 << ">" << endl;
        for (int jobId : guloso->alocacao[s]) {
            cout << jobId << " "; // +1 para ajustar a indexação base-0 para uma apresentação mais intuitiva
        }
        cout << endl;
    }
}

vector<vector<int>> Vnd::ILSPerturbation(Guloso* guloso, InstanceReader* reader) {
    usleep(500);
    
    int m = reader->m; // Número de servidores
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
            novaCapacidadeSv1 = guloso->getCapSv()[servidor1] - reader->t[servidor1][job1] + reader->t[servidor1][job2];
            cout << "capacidadesv1: " << guloso->getCapSv()[servidor1] << endl;
            cout << "novaCapacidadesv1: " << novaCapacidadeSv1 << endl;
        }

        if(servidor2 != m){
            novaCapacidadeSv2 = guloso->getCapSv()[servidor2] - reader->t[servidor2][job2] + reader->t[servidor2][job1];
            cout << "capacidadesv2: " << guloso->getCapSv()[servidor2] << endl;
            cout << "novaCapacidadesv1: " << novaCapacidadeSv2 << endl;
        }

        

        // Verifica se a troca respeita a capacidade dos servidores
        if (servidor2 == m && novaCapacidadeSv1 <= reader->b[servidor1]) {
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor1, novaCapacidadeSv1);

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->c[servidor1][job1];
            custoAlocacao -= reader->c[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->c[servidor1][job2];
            custoAlocacao += reader->c[servidor2][job1];
            guloso->setCustoAlocacao(custoAlocacao);

            // Não é necessário atualizar custoLocal, pois a perturbação não afeta jobs processados localmente

            // Recalcula o custo total após a perturbação
            custoTotal = guloso->getCustoAlocacao() + guloso->getCustoLocal();
            guloso->setCustoTotal(custoTotal);
        }else if(servidor1 == m && novaCapacidadeSv2 <= reader->b[servidor2]){
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);
            

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor2, novaCapacidadeSv2);

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->c[servidor1][job1];
            custoAlocacao -= reader->c[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->c[servidor1][job2];
            custoAlocacao += reader->c[servidor2][job1];
            guloso->setCustoAlocacao(custoAlocacao);

            // Não é necessário atualizar custoLocal, pois a perturbação não afeta jobs processados localmente

            // Recalcula o custo total após a perturbação
            custoTotal = guloso->getCustoAlocacao() + guloso->getCustoLocal();
            guloso->setCustoTotal(custoTotal);
        }else if(!local && novaCapacidadeSv2 <= reader->b[servidor2] && novaCapacidadeSv1 <= reader->b[servidor1]){
            
            // Realiza o swap
            swap(novaAlocacao[servidor1][jobIndex1], novaAlocacao[servidor2][jobIndex2]);

            // Atualiza as capacidades dos servidores após o swap
            guloso->setCapSv(2, servidor2, novaCapacidadeSv2);
            guloso->setCapSv(2, servidor1, novaCapacidadeSv1);
            

            // Atualiza os custos de alocação após o swap
            // Remove o custo dos jobs nas suas posições originais
            int custoAlocacao = guloso->getCustoAlocacao();
            int custoTotal = guloso->getCustoTotal();

            custoAlocacao -= reader->c[servidor1][job1];
            custoAlocacao -= reader->c[servidor2][job2];
            // Adiciona o custo dos jobs nas novas posições
            custoAlocacao += reader->c[servidor1][job2];
            custoAlocacao += reader->c[servidor2][job1];
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
