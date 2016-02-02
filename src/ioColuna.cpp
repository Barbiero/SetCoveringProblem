#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "globals.h"
#include "coluna.h"

uint8_t NUMERO_LINHAS;
uint16_t NUMERO_COLUNAS;

int readLinhas(std::string filename)
{
    std::string tmp;
    std::ifstream fin;
    fin.open(filename);

    //ler numero de linhas
    int num_linhas;
    fin >> tmp >> num_linhas;

    NUMERO_LINHAS = (uint8_t)num_linhas;
    //numero de colunas
    int num_colunas;
    fin >> tmp >> num_colunas;


    NUMERO_COLUNAS = (uint16_t)num_colunas;
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


    readLinhas(argc[1]);
    std::cout << (int)NUMERO_LINHAS << " " << (int)NUMERO_COLUNAS << std::endl;

    return 0;
}

