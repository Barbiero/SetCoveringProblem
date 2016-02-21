#pragma once

#include <cstdint>
#include <unordered_set>
#include <map>
#include <set>

#include <iostream>

typedef std::unordered_set<uint8_t> LinhaSet;
typedef std::multimap<uint8_t, uint16_t> CoberturaLinhas;

class Coluna
{

public:
    typedef std::map<uint16_t, Coluna*> ColunaArray;

    Coluna(uint16_t _id, double _custo, LinhaSet _linhas);
    uint16_t getId();
    double getCusto();

    bool cobreLinha(uint8_t linha);

    const LinhaSet& getLinhas();

    void globalizar();

    static void setColuna(Coluna* coluna);

    static ColunaArray& getColunas();

    static CoberturaLinhas& getCoberturaLinhas();

    static std::set<Coluna*> getColunasCobrindoLinha(uint8_t linha);

private:

    /**
     * Identificador da coluna
     */
    uint16_t id;

    /**
     * Custo(ou peso) da coluna, menor = melhor
     */
    double custo;

    /**
     * Conjunto de linhas que a coluna cobre
     */
    LinhaSet linhas;
};

