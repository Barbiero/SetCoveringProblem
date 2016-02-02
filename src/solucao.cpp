////////////////////////
//solucao.cpp
////////////////////////
//Determina o conceito de solucao
////////////////////////

/**
 * solucao = conjunto de colunas tal que todas as linhas do sistema
 * estao coberta
 *
 * solucao otima = solucao tal que o custo de suas colunas é minimo
 *
 *
 * Solucao X é melhor do que a Solucao Y quando o custo das colunas
 * de X é meno do que o custo das colunas de Y
 *
 * uma Solucao contem redundancia se for possivel remover uma coluna dela e
 * ela continuar sendo uma solucao
 *
 *
 */

#include <vector>
#include <unordered_set>
#include "globals.h"
#include "coluna.h"

class Solucao
{
public:

    Solucao();

    bool checkValidade(bool forceCheck);

    bool valida() { return isValid; }

private:

    /**
     * marca quantas colunas estao cobrindo uma certa linha
     */
    std::vector<uint8_t> coberturaLinhas;

    /**
     * marca quais colunas fazem parte da solucao
     * marca apenas o ID das colunas
     */
    std::unordered_set<uint8_t> colunas;

    /**
     * marca se uma solucao é valida
     */
    bool isValid;
};

Solucao::Solucao()
{
    isValid = false;

    coberturaLinhas = std::vector<uint8_t>(NUMERO_LINHAS);
    colunas = std::unordered_set<uint8_t>();
}

/**
 * verifica a validade de uma solucao e retorna true se ela for valida
 * @param forceCheck: Se true, verifica a validade por suas colunas
 */
bool Solucao::checkValidade(bool forceCheck = false)
{
    if(forceCheck)
    {
        //checagem total
        //resetar vetor de cobertura
        std::fill(coberturaLinhas.begin(), coberturaLinhas.end(), 0);

        for(auto idColuna : colunas)
        {
            Coluna* coluna = Coluna::getColunas()[idColuna];
            for(auto linha : coluna->getLinhas())
            {
                coberturaLinhas[linha]++;
            }
        }
    }

    for(auto linha : coberturaLinhas)
    {
        if(linha == 0)
        {
            isValid = false;
            return false;
        }
    }

    isValid = true;
    return true;
}


