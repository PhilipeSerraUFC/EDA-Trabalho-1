//Avisos:

//1. Comecei fazendo o trabalho com Theo e Galvão, por isso alguns trechos de códigos estarão iguais
//Como eles já tinham implementado muita coisa no momento que pude começar o trabalho, resolvi implementar sozinho
//Para também não deixar de aprender.

//2. Uso o Header <bits/stdc++.h>, que inclui a várias outras bibliotecas. Acredito que no Linux ele já esteja inserido.

#include "parser.hpp"

using namespace std;

int main(){

    string data_path;

    cout << "Insira o caminho do arquivo de entrada: ";
    cin >> data_path;

    vector<Operation> operations = ParserText(data_path);
    BinarySearchTree* tree = new BinarySearchTree();

    OperateTree(tree, operations);

    cout<<"Operacoes na arvore realizada, veja o arquivo out.txt";
    return 0;

}