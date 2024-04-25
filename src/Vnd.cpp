#include "Vnd.h"

Vnd::Vnd(){
}

Vnd::~Vnd(){
}


void Vnd::Run(Guloso* Guloso){
    int k = 1;
    int custotal = Guloso->getCustoTotal();

    while (k <= 3)
    {
        if (k == 1){
            /*chamar swap*/
        }else if (k == 2){
            /*chamar 2-pot*/
        }else{
            /*chamar re-insertion*/
        }
        if(Guloso->getCustoTotal() < custotal) {
            custotal = Guloso->getCustoTotal();
            k = 1;
        }else{
            k += 1;
        }
    }
    
}

void Vnd::swap(){

    for (int i = 0 ; i< )

}