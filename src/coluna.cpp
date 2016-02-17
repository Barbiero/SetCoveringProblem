///////////////////
// Aquivo de definir coluna
//////////////////

#include "coluna.h"

typedef std::map<uint16_t, Coluna*> ColunaArray;

Coluna::Coluna(uint16_t _id, double _custo, LinhaSet _linhas) :
id(_id), custo(_custo), linhas(_linhas)
{}

uint16_t
Coluna::getId()
{
    return id;
}

double
Coluna::getCusto()
{
    return custo;
}

const LinhaSet&
Coluna::getLinhas()
{
    return linhas;
}

bool
Coluna::cobreLinha(uint8_t linha)
{
    return linhas.find(linha) != linhas.end();
}

void
Coluna::globalizar()
{
    Coluna::setColuna(this);
    for(auto linha : getLinhas())
    {
        getCoberturaLinhas().insert(
            std::pair<uint8_t, uint16_t>
            (linha, getId())
        );
    }
}

void
Coluna::setColuna(Coluna* coluna)
{
    getColunas().insert(std::pair<uint16_t, Coluna*>(coluna->getId(), coluna));
}

ColunaArray&
Coluna::getColunas()
{
    static ColunaArray colunas;
    return colunas;
}

CoberturaLinhas&
Coluna::getCoberturaLinhas()
{
    static CoberturaLinhas linhas;
    return linhas;
}

std::set<Coluna*>
Coluna::getColunasCobrindoLinha(uint8_t linha)
{
    std::set<Coluna*> conj_cols();
    std::pair<CoberturaLinhas::iterator, CoberturaLinhas::iterator>
        cobertura_range = getCoberturaLinhas().equal_range(linha);

    for(CoberturaLinhas::iterator it = cobertura_range.first;
            it != cobertura_range.second; ++it)
    {
        uint16_t id_coluna = (*it).second;
        conj_cols.insert(getColunas()[id_coluna]);
    }

    return conj_cols;
}git
