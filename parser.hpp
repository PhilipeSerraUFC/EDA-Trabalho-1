#include <bits/stdc++.h>
#include "binary_search_tree.hpp"

/*Funções para leitura do arquivo de texto de entrada e escrita do arquivo de saída*/

using namespace std;

const string out_path = "out.txt";

enum Command {
    /*Enumeração das possíveis operações na BST*/
    INC, REM, SUC, IMP
};

//Alias para facilitar a leitura do codigo
using Operation = tuple<Command,int,int>;

/*Funções auxiliares*/

vector<string> split_string(string& str, char delimiter){
    
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;

}

/*Funções principais*/

vector<Operation> parser_text(string data_path){
    /*Função que lê o arquivo de entrada e retorna um vetor de triplas (Comando, Chave, Versão)
    
    No caso de Inclusao e remocao, onde não há versão, o campo de versão será igual a -1
    
    No caso da imprimir, onde não há chave, o campo de chave será igual a -1 */

    ifstream text(data_path);
    vector<Operation> result;

    if(!text.is_open()){
        cerr << "Erro ao abrir o arquivo.\n";
        exit(1);
        return result;
    }

    string line;

    int line_index = 0;

    while (getline(text, line)){

        Command command;
        int key;
        int version;

        line_index ++;
        vector<string> operation_string = split_string(line, ' ');

        if (operation_string[0] == "INC"){
            command = INC;
            key = stoi(operation_string[1]);
            version = -1;
        }

        else if (operation_string[0] == "REM"){
            command = REM;
            key = stoi(operation_string[1]);
            version = -1;
        }
        
        else if (operation_string[0] == "SUC"){
            command = SUC;
            key = stoi(operation_string[1]); 
            version = stoi(operation_string[2]);
        }

        else if (operation_string[0] == "IMP"){
            command = IMP;
            key = -1; 
            version = stoi(operation_string[1]);
        }

        else {
            cerr << "Operacao Invalida na linha " << line_index << endl;
            exit(1);
        }

        Operation op = Operation(command, key, version);
        result.push_back(op);
    }

    text.close();

    return result;
}

void WriteOut(string text){
    cout<<text<<endl;
    std::ofstream file(out_path, std::ios::app); 

    if (file.is_open()) {
        file << text << "\n"; 
        
        file.close();

    } else {
        std::cerr << "Não foi possivel abrir o arquivo.\n";
    }

}

string VectorPairsToString(vector<pair<int,int>>& vec){
    string result;
    for(pair<int,int> ints : vec){
        result.append(to_string(ints.first));
        result.append(",");
        result.append(to_string(ints.second));
        result.append(" ");
    }

    return result;
}

void OperateTree(BinarySearchTree* tree, vector<Operation> operations){

    for(Operation operation : operations){
        if(get<Command>(operation) == INC) Insert(tree, get<1>(operation));
        if(get<Command>(operation) == REM) Remove(tree, get<1>(operation));
        if(get<Command>(operation) == IMP){
            vector<pair<int,int>> x = DFS(tree, get<2>(operation));
            WriteOut(VectorPairsToString(x));
        } 
    }


};










