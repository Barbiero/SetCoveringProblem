#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include "globals.h"
#include "coluna.h"

class Solucao;
struct solucomp;

typedef std::multiset<Solucao*, solucomp> Populacao;


/**
  * Comparador de colunas
  * Se duas colunas possuem custo igual,
  * compara o ID das colunas
  * isto porque std::set nao admite dois elementos equivalentes
  */
struct coluna_compare {
    bool operator() (const uint16_t& lhs, const uint16_t& rhs)
    {
        assert(lhs <= NUMERO_COLUNAS);
        assert(rhs <= NUMERO_COLUNAS);

        Coluna* left = Coluna::getColunas()[lhs];
        Coluna* right = Coluna::getColunas()[rhs];

        return (left->getCustoPorLinhas() < right->getCustoPorLinhas())
         || (left->getCustoPorLinhas() == right->getCustoPorLinhas()
            && lhs < rhs);
    }
};

typedef std::set<uint16_t, coluna_compare> ColunaSet;

class Solucao
{
public:

    Solucao();

    Solucao(Solucao* a, Solucao* b);

    bool checkValidade(bool forceCheck);

    bool valida() { return isValid; }

    void eliminarRedundancia();

    std::pair<ColunaSet::iterator,bool> insertColuna(uint16_t col) {
        return colunas.insert(col); }

    const ColunaSet& getListaColunas() { return colunas; }

    double calcularCusto();

    double getCusto() { return custoTotal; }

    double getCusto() const { return custoTotal; }

    void mutarSolucao();

    static Solucao* gerarSolucaoAleatoria(unsigned seed);

    static Populacao gerarPopulacaoInicial(size_t n);

    static Solucao* selecaoPorTorneio(Populacao& populacao, int k);

private:

    /**
     * marca se uma solucao é valida
     */
    bool isValid;

    /**
     * marca quantas colunas estao cobrindo uma certa linha
     */
    std::vector<uint8_t> coberturaLinhas;

    /**
     * marca quais colunas fazem parte da solucao
     * marca apenas o ID das colunas
     */
    ColunaSet colunas;


    /**
     * Soma do custo de todas as colunas
     */
    double custoTotal;

};

struct solucomp {
    bool operator()(const Solucao* lhs, const Solucao* rhs){
        return lhs->getCusto() < rhs->getCusto();
    }
};
