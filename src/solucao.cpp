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

#include <algorithm>
#include <random>
#include "solucao.h"

unsigned RANDOM_SEED = 28;

Solucao::Solucao() :
    isValid(false), coberturaLinhas(std::vector<uint8_t>(NUMERO_LINHAS)),
    colunas(ColunaSet())
{}

/**
 * verifica a validade de uma solucao e retorna true se ela for valida
 * @param forceCheck: Se true, verifica a validade por suas colunas
 */
bool
Solucao::checkValidade(bool forceCheck = false)
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


/**
  * Elimina redundancias da solucao selecionando a partir da coluna
  * mais pesada
 **/
void
Solucao::eliminarRedundancia()
{
    ColunaSet T(colunas);
    while(!T.empty()){

        uint16_t id = *(T.begin());

        Coluna* j = Coluna::getColunas()[id];
        T.erase(T.begin());

        bool isRedundante = true;
        for(uint8_t linha : j->getLinhas())
        {
            if(coberturaLinhas[linha] < 2){
                isRedundante = false;
                break;
            }
        }

        if(isRedundante){
            for(uint8_t linha : j->getLinhas())
            {
                coberturaLinhas[linha]--;
            }

            colunas.erase(id);
        }

    }
}

double
Solucao::calcularCusto()
{
    double result = 0.0;
    for_each(colunas.begin(), colunas.end(),
        [&result](uint16_t elem)
        {
            Coluna* i = Coluna::getColunas()[elem];
            result += i->getCusto();
        });

    return result;
}

/**
  * @param seed: semente de geracao aleatoria para criar a solução
  * @return Solucao*: uma Solucao aleatoria gerada a partir das colunas existentes
  * @throw string: se uma solucao nao for possivel de ser gerada
 **/

Solucao*
Solucao::gerarSolucaoAleatoria(unsigned seed)
{
    //Pegar as colunas mais pesadas e gerar uma solução a partir dessas
    //Depois eliminar redundancias que existam

    //Nao eh muito eficiente mas essa funcao deveria ser chamada apenas 1x

    Solucao* s = new Solucao();
    std::vector<uint16_t> coluna_copy;
    for(auto it : Coluna::getColunas())
    {
        coluna_copy.push_back(it.first);
    }

    std::shuffle(coluna_copy.begin(), coluna_copy.end(), std::default_random_engine(seed));

    for(auto it : coluna_copy)
    {
        s->colunas.insert(it);
        if(s->checkValidade(true)){
            s->eliminarRedundancia();
            break;
        }
    }

    if(!s->valida()){
        delete s;
        throw "Não foi possível gerar uma solução válida das colunas existentes";
        return nullptr;
    }

    return s;
}

/**
  * @param n: numero de solucoes a serem criadas
  * @return std::set<Solucao*>: conjunto de solucoes geradas aleatoriamente
**/
std::unordered_set<Solucao*>
Solucao::gerarSolucoesIniciais(uint8_t n)
{
    std::unordered_set<Solucao*> sol_iniciais;
    unsigned seed = RANDOM_SEED;
    for(int i = 0; i < n; i++)
    {
        Solucao* s = gerarSolucaoAleatoria(seed);
        seed++;

        sol_iniciais.insert(s);
    }

    return sol_iniciais;
}
