///////////////////
// Arquivo de definir coluna
//////////////////

#include <random>
#include "coluna.h"
#include "globals.h"

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

double
Coluna::getCustoPorLinhas()
{
    return (custo / linhas.size());
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
    std::set<Coluna*> conj_cols;
    std::pair<CoberturaLinhas::iterator, CoberturaLinhas::iterator>
        cobertura_range = getCoberturaLinhas().equal_range(linha);

    for(CoberturaLinhas::iterator it = cobertura_range.first;
            it != cobertura_range.second; ++it)
    {
        uint16_t id_coluna = (*it).second;
        conj_cols.insert(getColunas()[id_coluna]);
    }

    return conj_cols;
}

Coluna*
Coluna::selecionarColunaAleatoria()
{
    unsigned seed = RANDOM_SEED;

    static std::default_random_engine gerador(seed);
    static std::uniform_int_distribution<uint16_t> dis(1, NUMERO_COLUNAS);

    uint16_t index = dis(gerador);
    return getColunas()[index];
}
