#include <iostream>
#include <vector>
#include <string>

using namespace std;


int main(){
    vector<int> a = {1,2,3,4,5};

    a[3] += 10;
    for(int i = 0; i < a.size(); i++){
        cout << a[i] << endl;
    }

    return 0;
}