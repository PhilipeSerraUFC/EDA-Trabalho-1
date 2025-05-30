#include <bits/stdc++.h>
#include "binary_search_tree.hpp"

/*Funções para leitura do arquivo de texto de entrada e escrita do arquivo de saída*/

using namespace std;

const string data_path = "in.txt";

enum command {
    /*Enumeração das possíveis operações na BST*/
    INC, REM, SUC, IMP
};

//Alias para facilitar a leitura do codigo
using operation = tuple<command,int,int>;



/*Funções principais*/

vector<operation> parser_text(string data_path){
    /*Função que lê o arquivo de entrada e retorna um vetor de triplas (Comando, Chave, Versão)
    
    No caso de Inclusao e remocao, onde não há versão, o campo de versão será igual a -1
    
    No caso da imprimir, onde não há chave, o campo de chave será igual a -1 */

    ifstream text(data_path);
    vector<operation> result;

    if(!text.is_open()){
        cerr << "Erro ao abrir o arquivo.\n";
        exit(1);
        return result;
    }

    string line;

    int line_index = 0;

    while (getline(text, line)){

        command comm;
        int key;
        int version;

        line_index ++;
        vector<string> operation_string = split_string(line, ' ');

        if (operation_string[0] == "INC"){
            comm = INC;
            key = stoi(operation_string[1]);
            version = -1;
        }

        else if (operation_string[0] == "REM"){
            comm = REM;
            key = stoi(operation_string[1]);
            version = -1;
        }
        
        else if (operation_string[0] == "SUC"){
            comm = SUC;
            key = stoi(operation_string[1]); 
            version = stoi(operation_string[2]);
        }

        else if (operation_string[0] == "IMP"){
            comm = IMP;
            key = -1; 
            version = stoi(operation_string[1]);
        }

        else {
            cerr << "Operacao Invalida na linha " << line_index << endl;
            exit(1);
        }

        operation op = operation(comm, key, version);
        result.push_back(op);
    }

    text.close();

    return result;
}

void operate_tree();



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






