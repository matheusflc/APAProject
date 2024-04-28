#include "Guloso.h"

using namespace std;
Guloso::Guloso(){
    //local = vector<int>();
}

Guloso::~Guloso(){
}

void Guloso::printar(){
    cout << "Guloso" << endl;
}

void Guloso::setCustoTotal(int custoTotal){
    this->custoTotal = custoTotal;
}

int Guloso::getCustoTotal(){
    return this->custoTotal;
}

void Guloso::play(InstanceReader *reader){
   // Atributos extraídos do leitor de instâncias
    int n = reader->n; // Número de jobs
    int m = reader->m; // Número de servidores
    vector<int> capacidades = reader->b; // Capacidade de cada servidor
    vector<vector<int>> tempo = reader->t; // Matriz de tempo (servidor x job)
    vector<vector<int>> custo = reader->c; // Matriz de custo (servidor x job)

    // Estrutura para armazenar a alocação de jobs aos servidores
    vector<vector<int>> alocacao(m+1);
    CapSv.resize(m);
    
    
    custoAlocacao = 0;
    custoLocal = 0;

    // Estrutura para armazenar os jobs e seus respectivos servidores com menor tempo
    vector<pair<int, pair<int, int>>> jobs; // <tempoMinimo, <servidorId, jobId>>
    for (int j = 0; j < n; ++j) {
        int tempoMinimo = INT_MAX;
        int servidorId = -1;
        for (int s = 0; s < m; ++s) {
            if (tempo[s][j] < tempoMinimo) {
                tempoMinimo = tempo[s][j];
                servidorId = s;
            }
        }
        jobs.push_back({tempoMinimo, {servidorId, j}});
    }

    // Ordena os jobs pelo menor tempo de processamento
    sort(jobs.begin(), jobs.end());
    
    // Alocação dos jobs com base no menor tempo de processamento
    for (auto& job : jobs) {
        int tempoMinimo = job.first;
        int servidorId = job.second.first;
        int jobId = job.second.second;

        // Verifica se a capacidade do servidor permite a alocação do job
        if (capacidades[servidorId] >= tempoMinimo) {
            alocacao[servidorId].push_back(jobId);
            capacidades[servidorId] -= tempoMinimo;
            CapSv[servidorId] += tempoMinimo;
            custoAlocacao += custo[servidorId][jobId];
        } else {
            // Job executado localmente
            custoLocal += reader->p;
            alocacao[m].push_back(jobId);
        }
    }

   

    custoTotal = custoAlocacao + custoLocal;

    // Saída dos resultados
    cout << custoTotal << endl;
    cout << custoAlocacao << endl;
    cout << custoLocal << endl;

    for (int s = 0; s <= m; ++s) {
        //cout << "< lista de jobs alocados no servidor " << s + 1 << ">" << endl;
        for (int jobId : alocacao[s]) {
            cout << jobId << " "; // +1 para ajustar a indexação base-0 para uma apresentação mais intuitiva
        }
        cout << endl;
    }
    for(int s = 0; s <= m; s++){
        this->alocacao.push_back(alocacao[s]);
    }
}



void Guloso::recalculateCost(InstanceReader* instanceReader) {
    // Reinicia o custo total, custo de alocação e custo local
    this->custoTotal = 0;
    this->custoAlocacao = 0;
    this->custoLocal = 0;

    // Itera por todos os servidores para calcular o custo de alocação
    for (int s = 0; s < instanceReader->m; ++s) {
        for (int jobIndex : this->alocacao[s]) {
            this->custoAlocacao += instanceReader->c[s][jobIndex];
        }
    }

    // Assume que o último "servidor" no vetor de alocação representa os jobs processados localmente
    for (int jobIndex : this->alocacao[instanceReader->m]) {
        this->custoLocal += instanceReader->p; // Adiciona o custo fixo para cada job processado localmente
    }

    // Atualiza o custo total
    this->custoTotal = this->custoAlocacao + this->custoLocal;
}



vector<vector<int>> Guloso::getAlocacao(){
    return this->alocacao;
}

void Guloso::setAlocacao(vector<vector<int>> alocacao){
    this->alocacao = alocacao;
}

void Guloso::setCustoAlocacao(int custoAlocacao){
    this->custoAlocacao = custoAlocacao;
}

int Guloso::getCustoAlocacao(){
    return this->custoAlocacao;
}

void Guloso::setCustoLocal(int custoLocal){
    this->custoLocal = custoLocal;
}

int Guloso::getCustoLocal(){
    return this->custoLocal;
}

void Guloso::setCapSv(int var, int server, int valor){
    if (var == 1){
        this->CapSv[server] += valor;
    }else{
        this->CapSv[server] = valor;
    }
}

vector<int> Guloso::getCapSv(){
    return this->CapSv;
}


