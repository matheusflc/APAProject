#include <Guloso.h>
#include <InstanceReader.h>
#include <Vnd.h>

using namespace std;


int main(){
    Guloso guloso;
    InstanceReader reader;
    Vnd vnd;
    guloso.printar();
    srand(static_cast<unsigned int>(time(NULL)));

    if(reader.readInstance("instancias_projeto/n25m5A.txt")){
        reader.printInstance();
    }
    
    guloso.play(&reader);
    //cout << "lalalal" << endl;
    vnd.Run(&guloso, &reader);
    //cout << "lalalal" << endl;
    //vnd.playILS(&guloso, &reader);
    
    


    return 0;
}