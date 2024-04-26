#include <Guloso.h>
#include <InstanceReader.h>
#include <Vnd.h>

using namespace std;


int main(){
    Guloso guloso;
    InstanceReader reader;
    Vnd vnd;
    guloso.printar();

    if(reader.readInstance("instancias_projeto/teste.txt")){
        reader.printInstance();
    }

    guloso.play(&reader);

    vnd.Run(&guloso, &reader);
    


    return 0;
}