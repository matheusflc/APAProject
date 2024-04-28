#include <Guloso.h>
#include <InstanceReader.h>
#include <Vnd.h>

using namespace std;

#define ILSPLAY 0


int main(){
    Guloso guloso;
    InstanceReader reader;
    Vnd vnd;
    guloso.printar();
    srand(static_cast<unsigned int>(time(NULL)));
    string nomearquivo = "n25m5A";

    if(reader.readInstance("instancias_projeto/" + nomearquivo + ".txt")){
        reader.printInstance();
    }
    
    guloso.play(&reader);
    //cout << "lalalal" << endl;
    vnd.Run(&guloso, &reader);
    //cout << "lalalal" << endl;
    if(ILSPLAY){
        vnd.playILS(&guloso, &reader);
        nomearquivo = "ILS_" + nomearquivo;
    }
    
    guloso.escreverResultadoEmArquivo(&guloso, "instancias_projeto/" + nomearquivo + "_Result.txt");

    
    cout << "Capacidade dos servidores após a execução:" << endl;
    for(size_t i = 0; i < guloso.getCapSv().size(); ++i) {
        cout << "Servidor " << i << ": " << guloso.getCapSv()[i] << endl;
    }
    
    


    return 0;
}