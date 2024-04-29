#include "Guloso.h"

using namespace std;
Guloso::Guloso(){
    custoAlocacao = 0;
    custoLocal = 0;
}

Guloso::~Guloso(){
}

void Guloso::printar(){
    cout << "Guloso" << endl;
}

void Guloso::setCustoTotal(int& custoTotal){
    this->custoTotal = custoTotal;
}

int Guloso::getCustoTotal(){
    return this->custoTotal;
}

void Guloso::play(InstanceReader *reader){
   // Atributos extraídos do leitor de instâncias
    vector<int> capacidades = reader->b; // Capacidade de cada servidor

    // Estrutura para armazenar a alocação de jobs aos servidores
    vector<vector<int>> alocacao(reader->getM()+1);
    CapSv.resize(reader->getM());

    // Estrutura para armazenar os jobs e seus respectivos servidores com menor tempo
    vector<pair<int, pair<int, int>>> jobs; // <tempoMinimo, <servidorId, jobId>>
    for (int j = 0; j < reader->getN(); ++j) {
        int tempoMinimo = INT_MAX;
        int servidorId = -1;
        for (int s = 0; s < reader->getM(); ++s) {
            if (reader->getT()[s][j] < tempoMinimo) {
                tempoMinimo = reader->getT()[s][j];
                servidorId = s;
            }
        }
        //cout << servidorId << " = "<< j << endl;
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
            custoAlocacao += reader->getC()[servidorId][jobId];
        } else {
            // Job executado localmente
            custoLocal += reader->getP();
            alocacao[reader->getM()].push_back(jobId);
        }
    }

    custoTotal = custoAlocacao + custoLocal;

    for(int s = 0; s <= reader->getM(); s++){
        this->alocacao.push_back(alocacao[s]);
    }
}


void Guloso::escreverResultadoEmArquivo(Guloso* guloso, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    // Escreve os custos no arquivo
    arquivo << guloso->custoTotal << endl;
    arquivo << guloso->custoAlocacao << endl;
    arquivo << guloso->custoLocal << endl;
    arquivo << endl; // Linha em branco conforme o formato especificado

    // Escreve a lista de jobs alocados em cada servidor
    for (size_t i = 0; i < guloso->alocacao.size() - 1; ++i) { // Exclui a última lista, que é para jobs locais
        for (int job : guloso->alocacao[i]) {
            arquivo  << job << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}


vector<vector<int>> Guloso::getAlocacao(){
    return this->alocacao;
}

void Guloso::setAlocacao(vector<vector<int>> alocacao){
    this->alocacao = alocacao;
}

void Guloso::setCustoAlocacao(int& custoAlocacao){
    this->custoAlocacao = custoAlocacao;
}

int Guloso::getCustoAlocacao(){
    return this->custoAlocacao;
}

void Guloso::setCustoLocal(int& custoLocal){
    this->custoLocal = custoLocal;
}

int Guloso::getCustoLocal(){
    return this->custoLocal;
}

void Guloso::setCapSv(int var, int& server, int& valor){
    if (var == 1){
        this->CapSv[server] += valor;
    }else{
        this->CapSv[server] = valor;
    }
}

vector<int> Guloso::getCapSv(){
    return this->CapSv;
}

void Guloso::setCapSvVector(vector<int>& CapSv){
    this->CapSv = CapSv;
}


