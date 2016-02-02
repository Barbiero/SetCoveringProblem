#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "coluna.h"

int readLinhas(std::string filename)
{
    std::string tmp;
    std::ifstream fin;
    fin.open(filename);



    //ler numero de linhas
    int num_linhas;
    fin >> tmp >> num_linhas;

    //numero de colunas
    int num_colunas;
    fin >> tmp >> num_colunas;

    //pula
    fin >> tmp;

    //colunas
    std::string linhaAtual;
    while(getline(fin, linhaAtual)){
        int id_coluna = 0;
        double custo = 0.0;
        LinhaSet linhas;

        std::stringstream linhaStream(linhaAtual);

        linhaStream >> id_coluna >> custo;
        if(id_coluna <= 0 || id_coluna > num_colunas) continue;
        int linha_id;
        while(linhaStream >> linha_id){
            linhas.insert(linha_id);
        }

        Coluna* c = new Coluna(id_coluna, custo, linhas);
        c->globalizar();
    }

    return Coluna::getColunas().size();
}

int main(int argv, char* argc[])
{
    if(argv < 2){
        std::cout << "Digite o nome do arquivo" << std::endl;
        return 0;
    }

    std::cout << readLinhas(argc[1]) << std::endl;

    return 0;
}

