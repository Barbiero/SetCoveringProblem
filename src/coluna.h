#pragma once

#include <cstdint>
#include <unordered_set>
#include <map>

#include <iostream>

typedef std::unordered_set<uint8_t> LinhaSet;



class Coluna
{

public:
    typedef std::map<uint16_t, Coluna*> ColunaArray;

    Coluna(uint16_t _id, double _custo, LinhaSet _linhas) :
        id(_id), custo(_custo), linhas(_linhas) {};
    
    uint16_t getId() { return id; }
    double getCusto() { return custo; }

    bool cobreLinha(uint8_t linha) {
        return (linhas.find(linha) != linhas.cend());
    }

    void globalizar() {
        Coluna::setColuna(this);
    }

    static void setColuna(Coluna* coluna){
        getColunas().insert(
            std::pair<uint16_t, Coluna*>(coluna->getId(), coluna));
    }

    static ColunaArray& getColunas() { 
        static Coluna::ColunaArray colunas;
        return colunas;
    }
    
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

    /**
     * Lista persistente de colunas
     */
    //static ColunaArray colunas;
};

