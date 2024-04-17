#include <Guloso.h>
#include <InstanceReader.h>

using namespace std;


int main(){
    Guloso guloso;
    InstanceReader reader;
    guloso.printar();

    if(reader.readInstance("instancias_projeto/n5m15A.txt")){
        reader.printInstance();
    }

    guloso.play(&reader);


    return 0;
}