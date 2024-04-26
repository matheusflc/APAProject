#include "Guloso.h"

using namespace std;
Guloso::Guloso(){
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
    vector<vector<int>> alocacao(m);
    
    
    int custoAlocacao = 0;
    int custoLocal = 0;

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
            custoAlocacao += custo[servidorId][jobId];
        } else {
            // Job executado localmente
            custoLocal += reader->p;
        }
    }

   

    custoTotal = custoAlocacao + custoLocal;

    // Saída dos resultados
    cout << custoTotal << endl;
    cout << custoAlocacao << endl;
    cout << custoLocal << endl;

    for (int s = 0; s < m; ++s) {
        //cout << "< lista de jobs alocados no servidor " << s + 1 << ">" << endl;
        for (int jobId : alocacao[s]) {
            cout << jobId + 1 << " "; // +1 para ajustar a indexação base-0 para uma apresentação mais intuitiva
        }
        cout << endl;
    }
    setAlocacao(alocacao);
}


vector<vector<int>> Guloso::getAlocacao(){
    return this->alocacao;
}

void Guloso::setAlocacao(vector<vector<int>> alocacao){
    this->alocacao = alocacao;
}