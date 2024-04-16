#include <Guloso.h>
#include <InstanceReader.h>

using namespace std;


int main(){
    Guloso guloso;
    InstanceReader reader;
    guloso.printar();

    if(reader.readInstance("instancias_projeto/n60m10A.txt")){
        reader.printInstance();
    }


    return 0;
}