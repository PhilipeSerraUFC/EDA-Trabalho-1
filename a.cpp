#include<iostream>

using namespace std;

const string data_path = "in.txt";

int main(){
    int x = 3;
    do{
        cout<<--x<<endl;
        if(x == 2) break; }
    while(x > 0);

    return 0;

}