#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include "globals.h"
#include "coluna.h"

struct coluna_compare {
    bool operator() (const uint16_t& lhs, const uint16_t& rhs)
    {
        assert(lhs <= NUMERO_COLUNAS);
        assert(rhs <= NUMERO_COLUNAS);

        Coluna* left = Coluna::getColunas()[lhs];
        Coluna* right = Coluna::getColunas()[rhs];

        return (right->getCusto() - left->getCusto());
    }
};

typedef std::set<uint16_t, coluna_compare> ColunaSet;

class Solucao
{
public:

    Solucao();

    bool checkValidade(bool forceCheck);

    bool valida() { return isValid; }

    void eliminarRedundancia();

    std::pair<ColunaSet::iterator,bool> insertColuna(uint16_t col) {
        return colunas.insert(col); }

    const ColunaSet& getListaColunas() { return colunas; }

    double calcularCusto();

    static Solucao* gerarSolucaoAleatoria(unsigned seed);

    static std::unordered_set<Solucao*> gerarSolucoesIniciais(uint8_t n);



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

};

