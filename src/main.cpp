#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <numeric>
#include <memory>
#include "globals.h"
#include "coluna.h"
#include "solucao.h"

uint8_t NUMERO_LINHAS;
uint16_t NUMERO_COLUNAS;
unsigned RANDOM_SEED;


//uint32_t TEMPO_MAX = 2000000;
double SEGUNDOS_ESPERANDO = 2.0;
size_t POPULACAO_SIZE = 100;
double TAXA_MIN_MUTACAO = 0.05;
int SELECAO_POR_TORNEIO = 5;
bool suppressInfo = false;

int readLinhas(std::string filename)
{
    std::string tmp;
    std::ifstream fin;
    fin.open(filename);
    if(fin.fail())
    {
        std::cerr << "Nao foi possivel abrir o arquivo " << filename << std::endl;
        return -1;
    }

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

double
taxa_de_mutacao(Solucao* menos_apto, Solucao* mais_apto, double tx_minima = TAXA_MIN_MUTACAO)
{

    double maxCusto = menos_apto->getCusto();
    double minCusto = mais_apto->getCusto();

    double deltaCusto = maxCusto - minCusto;

    return ( (tx_minima) / (1 - std::exp( -(deltaCusto/maxCusto) ) ) );
}

double rand_zero_um()
{
    unsigned seed = RANDOM_SEED;

    static std::default_random_engine gerador(seed);
    static std::uniform_real_distribution<double> dist_double(0.0, 1.0);

    return dist_double(gerador);
}

std::unique_ptr<Solucao>
busca_algoritmo_genetico()
{
    std::cout << "Gerando população inicial de " << POPULACAO_SIZE << " individuos..." << std::flush;
    Populacao pop = Solucao::gerarPopulacaoInicial(POPULACAO_SIZE);
    std::cout << "[pronto]" << std::endl;
    //uint32_t tempo = 0;


    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::microseconds ms;
    typedef std::chrono::duration<double> dsec;
    auto t = Clock::now();

    auto cmp_sol = [](Solucao* fst, Solucao* snd) -> bool{
                    return (fst->getCusto() < snd->getCusto());
                };

    do{
        Solucao* X = Solucao::selecaoPorTorneio(pop, SELECAO_POR_TORNEIO);
        Solucao* Y = Solucao::selecaoPorTorneio(pop, SELECAO_POR_TORNEIO);

        Solucao* S = new Solucao(X, Y);

        Solucao* menos_apto = *pop.rbegin();
        Solucao* mais_apto = *pop.begin();

        double randnum = rand_zero_um();
        double TxM = taxa_de_mutacao(menos_apto, mais_apto, TAXA_MIN_MUTACAO);
        if(randnum < TxM){
            S->mutarSolucao();
        }

        if(S->getCusto() < menos_apto->getCusto()){

            bool isCopy = false;
            auto range_custo = pop.equal_range(S);
            if(range_custo.first != pop.end())
            {
                for(auto it = range_custo.first; it != range_custo.second;
                ++it)
                {
                    Solucao* cmp = (*it);
                    if(cmp->getListaColunas() == S->getListaColunas())
                    {
                        isCopy = true;
                        break;
                    }
                }
            }

            if(!isCopy){
                Populacao::iterator it = pop.end();
                --it;
                delete *it;
                pop.erase(it);

                pop.insert(S);

                auto diff_ms = std::chrono::duration_cast<ms>((dsec)(Clock::now() - t));

                if(!suppressInfo)
                    std::cout << "Nova Solucao: " << S->getCusto() << " após " << diff_ms.count() << "μs" << std::endl;

                //tempo = 0;
                t = Clock::now();
            }
            else{
                delete S;
            }
        }
        else{
            /*tempo += 1;

            if(tempo % 100000 == 0)
            {
                std::cout << "tempo: " << tempo << " custo desta iter: " << S->getCusto() << std::endl;
            }*/


            delete S;
        }

        auto diff_secs = std::chrono::duration_cast<std::chrono::seconds>((dsec)(Clock::now() - t));
        if(diff_secs.count() > SEGUNDOS_ESPERANDO){
            break;
        }
    }while(true);


    auto min_elem = std::min_element(pop.begin(), pop.end(), cmp_sol);
    std::unique_ptr<Solucao> res(*min_elem);

    return res;
}

int main(int argv, char* argc[])
{
    std::cout << "Programa de Algoritmo Genetico" << std::endl;
    RANDOM_SEED = std::chrono::system_clock::now().time_since_epoch().count();

    std::string filename;
    if(argv < 2){
        //std::cout << "Digite o nome do arquivo" << std::endl;
        //return 0;
        filename = "casos/teste1.txt";
    }
    else{
        filename = argc[1];
    }

    if(argv >= 3){
        POPULACAO_SIZE = strtoul(argc[2], NULL, 10);
    }

    if(argv >= 4){
        TAXA_MIN_MUTACAO = strtod(argc[3], NULL);
    }

    if(argv >= 5){
        SEGUNDOS_ESPERANDO = strtod(argc[4], NULL);
    }

    if(argv >= 6){
        suppressInfo = true;
    }

    std::cout << "Lendo arquivo " << filename << std::endl;
    if(readLinhas(filename) < 0){
        return -1;
    }

    std::cout << "Numero de linhas: " << (int)NUMERO_LINHAS << std::endl;
    std::cout << "Numero de colunas: " << (int)NUMERO_COLUNAS << std::endl;


    std::unique_ptr<Solucao> res = busca_algoritmo_genetico();
    std::cout << "Tempo finalizado!" << std::endl
                << "Custo: " << res->getCusto() << std::endl
                << "\nColunas:" << std::flush;
    for(uint16_t c : res->getListaColunas() )
    {
        Coluna* col = Coluna::getColunas()[c];
        std::cout << (int)col->getId() << " => ";
        for(uint8_t linha : col->getLinhas())
        {
            std::cout << (int)linha+1 << " ";
        }
        std::cout << "(" << col->getCusto() << ")" << std::endl;
    }

    return 0;
}


