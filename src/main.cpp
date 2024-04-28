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
    vnd.Run(&guloso, &reader);
    if(ILSPLAY){
        vnd.playILS(&guloso, &reader);
        nomearquivo = "ILS_" + nomearquivo;
    }
    
    guloso.escreverResultadoEmArquivo(&guloso, "resultados/" + nomearquivo + "_Result.txt");


    return 0;

    //auto t1 = chrono::high_resolution_clock::now();
    //auto t2 = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::nanoseconds>( t2 - t1 ).count();
}