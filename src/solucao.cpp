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
#include <iterator>
#include <random>
#include "solucao.h"

Solucao::Solucao() :
    isValid(false), coberturaLinhas(std::vector<uint8_t>(NUMERO_LINHAS)),
    colunas(ColunaSet()), custoTotal(0.0)
{}

/**
    * Cria uma Solucao a partir de outras duas solucoes
  */
Solucao::Solucao(Solucao* a, Solucao* b) :
    isValid(false), coberturaLinhas(std::vector<uint8_t>(NUMERO_LINHAS)),
    colunas(ColunaSet()), custoTotal(0.0)
{

    for(auto coluna : a->getListaColunas())
    {
        colunas.insert(coluna);
    }

    for(auto coluna : b->getListaColunas())
    {
        colunas.insert(coluna);
    }

    if(checkValidade(true)){
        eliminarRedundancia();
    }
}

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
        calcularCusto();
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

        auto iter = T.end(); --iter;
        uint16_t id = *(iter);

        Coluna* j = Coluna::getColunas()[id];
        T.erase(id);

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
    calcularCusto();
}

double
Solucao::calcularCusto()
{
    double custo = 0.0;
    std::for_each(colunas.begin(), colunas.end(),
        [&custo](uint16_t colid)
        {
            Coluna* c = Coluna::getColunas()[colid];
            custo += c->getCusto();
        }
    );

    custoTotal = custo;
    return custoTotal;
}

/**
  * Muta uma solucao aleatoriamente
  */
void
Solucao::mutarSolucao()
{
    unsigned seed = RANDOM_SEED;

    static std::default_random_engine gerador(seed);
    static std::uniform_real_distribution<double> dist_double(0.0, 1.0);

    double lambda = dist_double(gerador);

    int itermax = (int)(lambda * colunas.size());
    for(int k = 0; k < itermax; k++)
    {
        Coluna* j = Coluna::selecionarColunaAleatoria();
        colunas.insert(j->getId());
    }
    eliminarRedundancia();
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

    //funcao bem lenta quando NUMERO_COLUNAS é grande

    Solucao* s = new Solucao();
    std::default_random_engine eng(seed);
    std::uniform_int_distribution<uint16_t> dist(1, NUMERO_COLUNAS);
    while(!s->checkValidade(true)){
        auto id = dist(eng);

        s->colunas.insert(id);
    }
    s->eliminarRedundancia();

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
Populacao
Solucao::gerarPopulacaoInicial(size_t n)
{
    Populacao sol_iniciais;
    unsigned seed = RANDOM_SEED;
    for(size_t i = 0; i < n; i++)
    {
        Solucao* s = gerarSolucaoAleatoria(seed);
        seed++;

        sol_iniciais.insert(s);
        std::cout << i << " " << std::flush;
    }

    return sol_iniciais;
}

/**
    * Seleciona o melhor elemento entre k elementos aleatorios
    * @param populacao: A populacao de elementos a ser pesquisada
    * @param k: numero de elementos aleatorios a serem testados
    * @return Solucao*
  */
Solucao*
Solucao::selecaoPorTorneio(Populacao& populacao, int k)
{
    unsigned seed = RANDOM_SEED;

    //gerador de numero aleatorio
    static std::default_random_engine gerador(seed);
    static std::uniform_int_distribution<int> distribution(0, populacao.size()-1);

    Solucao* vencedor = nullptr;

    for(; k > 0; k--)
    {
        Populacao::iterator elem_it = populacao.begin();
        int delta_index = distribution(gerador);
        std::advance(elem_it, delta_index);

        Solucao* sol_selecionada = (*elem_it);

        if(vencedor == nullptr){
            vencedor = sol_selecionada;
        }
        else{
            double c1 = sol_selecionada->getCusto();
            double c2 = vencedor->getCusto();
            if(c1 < c2){
                vencedor = sol_selecionada;
            }
        }
    }

    return vencedor;
}

