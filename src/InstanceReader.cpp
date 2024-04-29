#include "InstanceReader.h"

InstanceReader::InstanceReader() : n(0), m(0), p(0) {}

bool InstanceReader::readInstance(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        return false;
    }

    file >> n >> m >> p;

    b.resize(m);
    t.resize(m, vector<int>(n));
    c.resize(m, vector<int>(n));

    for (int i = 0; i < m; ++i) {
        file >> b[i];
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> t[i][j];
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> c[i][j];
        }
    }

    file.close();
    return true;
}

void InstanceReader::printInstance() {
    cout << "n: " << n << endl;
    cout << "m: " << m << endl;
    cout << "p: " << p << endl;

    cout << "array b:" << endl;
    for (int i = 0; i < m; ++i) {
        cout << b[i] << " ";
    }
    cout << endl;

    cout << "matriz t:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << t[i][j] << " ";
        }
        cout << endl;
    }

    cout << "matriz c:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
}

vector<int> InstanceReader::getB(){
    return this->b;
}

vector<vector<int>> InstanceReader::getT(){
    return this->t;
}

vector<vector<int>> InstanceReader::getC(){
    return this->c;
}

int InstanceReader::getN(){
    return this->n;
}

int InstanceReader::getM(){
    return this->m;
}

int InstanceReader::getP(){
    return this->p;
}