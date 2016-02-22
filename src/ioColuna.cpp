#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "globals.h"
#include "coluna.h"
#include "solucao.h"

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
            linhas.insert(linha_id-1);
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

    try{
        std::unordered_set<Solucao*> sol_ini = Solucao::gerarSolucoesIniciais(100);

        auto min_max =
            std::minmax_element(sol_ini.begin(), sol_ini.end(),
                [](Solucao* fst, Solucao* snd) -> bool{
                    return (fst->calcularCusto() < snd->calcularCusto());
                }
            );
        std::cout << "Menor peso: " << (*(min_max.first))->calcularCusto()
                  << " Maior peso: " << (*(min_max.second))->calcularCusto() << std::endl;


    }
    catch(const char* e){
        std::cerr << e << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

