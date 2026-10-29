#ifndef EDB_TEST_SUPPORT_HPP
#define EDB_TEST_SUPPORT_HPP
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include "../src/avl.hpp"
#include "../src/rubro_negra.hpp"

// Adapta somente os testes; os algoritmos em src permanecem inalterados.
template<class Implementacao>
class SuporteTeste : public Implementacao {
    std::size_t elementos = 0;
public:
    bool insert(int k) {
        const bool ok = this->adicionar(k);
        if (ok) ++elementos;
        return ok;
    }
    bool erase(int k) {
        const bool ok = this->excluir(k);
        if (ok) --elementos;
        return ok;
    }
    bool contains(int k) { return this->consultar(k); }
    std::vector<int> inorder() {
        std::vector<int> valores;
        this->exportar_ordenado(valores);
        return valores;
    }
    std::size_t size() const { return elementos; }
    int height() { return this->nivel_total(); }
    bool valid() {
        const auto valores = inorder();
        return valores.size() == elementos &&
               std::adjacent_find(valores.begin(), valores.end(),
                   [](int a, int b) { return a >= b; }) == valores.end() &&
               this->estrutura_valida();
    }
    int rootKey() {
        if (!elementos) throw std::runtime_error("Arvore vazia");
        return this->topo->valor;
    }
    int children(int k) {
        auto atual = this->topo;
        auto vazio = this->topo;
        if constexpr (std::is_same<Implementacao, ArvoreAvl>::value) vazio = nullptr;
        else vazio = this->sentinela;
        while (atual != vazio && atual->valor != k)
            atual = k < atual->valor ? atual->esquerda : atual->direita;
        if (atual == vazio) return -1;
        return int(atual->esquerda != vazio) + int(atual->direita != vazio);
    }
};
using AVL = SuporteTeste<ArvoreAvl>;
using RB = SuporteTeste<ArvoreRubroNegra>;
#endif
