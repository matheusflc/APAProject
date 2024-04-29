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
    string nomearquivo = "n5m15A";

    if(reader.readInstance("instancias_projeto/" + nomearquivo + ".txt")){
        reader.printInstance();
    }
    
    guloso.play(&reader);
    vnd.Run(&guloso, &reader);
    if(ILSPLAY){
        vnd.playILS(&guloso, &reader);
        nomearquivo = "ILS_" + nomearquivo;
    }
    
    guloso.escreverResultadoEmArquivo(&guloso, "resultados/" + nomearquivo + "_Result.txt");


    return 0;

}