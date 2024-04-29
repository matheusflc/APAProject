#include <Guloso.h>
#include <InstanceReader.h>
#include <Vnd.h>

using namespace std;

#define ILSPLAY 1


int main(){
    Guloso guloso;
    InstanceReader reader;
    Vnd vnd;
    guloso.printar();
    srand(static_cast<unsigned int>(time(NULL)));
    string nomearquivo = "n60m10A";

    if(reader.readInstance("instancias_projeto/" + nomearquivo + ".txt")){
        reader.printInstance();
    }
    
    guloso.play(&reader);
    vnd.Run(&guloso, &reader);
    if(ILSPLAY){
        vnd.playILS(&guloso, &reader);
        nomearquivo = "ILS_" + nomearquivo;
    }
    
<<<<<<< HEAD
    guloso.escreverResultadoEmArquivo(&guloso, "resultados_instancias/" + nomearquivo + "_Result.txt");

    
    cout << "Capacidade dos servidores após a execução:" << endl;
    for(size_t i = 0; i < guloso.getCapSv().size(); ++i) {
        cout << "Servidor " << i << ": " << guloso.getCapSv()[i] << endl;
    }
    
    
=======
    guloso.escreverResultadoEmArquivo(&guloso, "resultados/" + nomearquivo + "_Result.txt");
>>>>>>> 6743d78a9ef0a237280cf43c867eacc6502b1da9


    return 0;

}