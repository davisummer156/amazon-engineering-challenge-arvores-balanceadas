#ifndef ARVORES_AVL_HPP
#define ARVORES_AVL_HPP

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

struct CelulaAvl {
    int valor, nivel;
    CelulaAvl *esquerda, *direita;
};
class ArvoreAvl {
private:
    template <class Celula, class Formatador>
    void desenhar_ramo(Celula* atual, Celula* nulo, const std::string& prefixo, bool direita, Formatador formatar) {
        if (atual == nulo) return;
        desenhar_ramo(atual->direita, nulo, prefixo + (direita ? "    " : "|   "), true, formatar);
        printf("%s%s%s\n", prefixo.c_str(), direita ? "/-- " : "\\-- ", formatar(atual).c_str());
        desenhar_ramo(atual->esquerda, nulo, prefixo + (direita ? "|   " : "    "), false, formatar);
    }
    template <class Celula, class Formatador>
    void exibir_estrutura(Celula* inicio, Celula* nulo, Formatador formatar) {
        if (inicio == nulo) {
            puts("Arvore vazia");
            return;
        }
        desenhar_ramo(inicio->direita, nulo, "", true, formatar);
        printf("%s\n", formatar(inicio).c_str());
        desenhar_ramo(inicio->esquerda, nulo, "", false, formatar);
    }
    template <typename Node>
    void listar_ordenado(Node* inicio, Node* vazio, std::vector<int>& destino) {
        std::vector<Node*> pendentes;
        Node* atual = inicio;
        while (atual != vazio || !pendentes.empty()) {
            if (atual != vazio) {
                pendentes.push_back(atual);
                atual = atual->esquerda;
                continue;
            }
            atual = pendentes.back();
            pendentes.pop_back();
            destino.push_back(atual->valor);
            atual = atual->direita;
        }
    }
    template <typename Node>
    void destruir_ramos(Node* inicio, Node* vazio) {
        if (inicio == vazio) return;
        std::vector<Node*> pendentes(1, inicio);
        while (!pendentes.empty()) {
            Node* atual = pendentes.back();
            pendentes.pop_back();
            if (atual->esquerda != vazio) pendentes.push_back(atual->esquerda);
            if (atual->direita != vazio) pendentes.push_back(atual->direita);
            delete atual;
        }
    }
public:
    long long total_rotacoes = 0;
    long long total_recoloracoes = 0;
    void exibir() {
        exibir_estrutura<CelulaAvl>(topo, nullptr, [](CelulaAvl* atual) { return std::to_string(atual->valor) + " (fb=" + std::to_string(diferenca(atual)) + ")"; });
    }
    bool estrutura_valida() {
        bool sucesso = true;
        conferir_niveis(topo, sucesso);
        return sucesso;
    }
    int conferir_niveis(CelulaAvl* atual, bool& sucesso) {
        if (!atual) return 0;
        int primeiro = conferir_niveis(atual->esquerda, sucesso), segundo = conferir_niveis(atual->direita, sucesso);
        if (std::abs(primeiro - segundo) > 1 || atual->nivel != 1 + std::max(primeiro, segundo)) sucesso = false;
        return 1 + std::max(primeiro, segundo);
    }
    int nivel_total() { return nivel_de(topo); }
    void exportar_ordenado(std::vector<int>& valores) { percorrer(topo, valores); }
    void percorrer(CelulaAvl* atual, std::vector<int>& valores) { listar_ordenado<CelulaAvl>(atual, nullptr, valores); }
    bool consultar(int valor_procurado) {
        CelulaAvl* atual = topo;
        while (atual && atual->valor != valor_procurado) atual = valor_procurado < atual->valor ? atual->esquerda : atual->direita;
        return atual != nullptr;
    }
    bool excluir(int valor_procurado) {
        bool sucesso;
        topo = excluir_ramo(topo, valor_procurado, sucesso);
        return sucesso;
    }
    bool adicionar(int valor_procurado) {
        bool sucesso;
        topo = adicionar_ramo(topo, valor_procurado, sucesso);
        return sucesso;
    }
    CelulaAvl* excluir_ramo(CelulaAvl* atual, int valor_procurado, bool& sucesso) {
        if (!atual) {
            sucesso = false;
            return nullptr;
        }
        if (valor_procurado < atual->valor) atual->esquerda = excluir_ramo(atual->esquerda, valor_procurado, sucesso);
        else if (valor_procurado > atual->valor) atual->direita = excluir_ramo(atual->direita, valor_procurado, sucesso);
        else {
            sucesso = true;
            if (!atual->esquerda || !atual->direita) {
                CelulaAvl* filho = atual->esquerda ? atual->esquerda : atual->direita;
                delete atual;
                return filho;
            }
            CelulaAvl* s = atual->direita;
            while (s->esquerda) s = s->esquerda;
            atual->valor = s->valor;
            atual->direita = excluir_ramo(atual->direita, s->valor, sucesso);
        }
        return reequilibrar(atual);
    }
    CelulaAvl* adicionar_ramo(CelulaAvl* atual, int valor_procurado, bool& sucesso) {
        if (!atual) {
            sucesso = true;
            return new CelulaAvl{valor_procurado, 1, nullptr, nullptr};
        }
        if (valor_procurado < atual->valor) atual->esquerda = adicionar_ramo(atual->esquerda, valor_procurado, sucesso);
        else if (valor_procurado > atual->valor) atual->direita = adicionar_ramo(atual->direita, valor_procurado, sucesso);
        else {
            sucesso = false;
            return atual;
        }
        return reequilibrar(atual);
    }
    CelulaAvl* reequilibrar(CelulaAvl* atual) {
        atualizar_nivel(atual);
        int fator = diferenca(atual);
        if (fator > 1) {
            if (diferenca(atual->esquerda) < 0) atual->esquerda = girar_esquerda(atual->esquerda);
            return girar_direita(atual);
        }
        if (fator < -1) {
            if (diferenca(atual->direita) > 0) atual->direita = girar_direita(atual->direita);
            return girar_esquerda(atual);
        }
        return atual;
    }
    CelulaAvl* girar_esquerda(CelulaAvl* atual) {
        CelulaAvl* substituto = atual->direita;
        atual->direita = substituto->esquerda;
        substituto->esquerda = atual;
        atualizar_nivel(atual);
        atualizar_nivel(substituto);
        total_rotacoes++;
        return substituto;
    }
    CelulaAvl* girar_direita(CelulaAvl* substituto) {
        CelulaAvl* atual = substituto->esquerda;
        substituto->esquerda = atual->direita;
        atual->direita = substituto;
        atualizar_nivel(substituto);
        atualizar_nivel(atual);
        total_rotacoes++;
        return atual;
    }
    void destruir(CelulaAvl* atual) { destruir_ramos<CelulaAvl>(atual, nullptr); }
    static void atualizar_nivel(CelulaAvl* atual) { atual->nivel = 1 + std::max(nivel_de(atual->esquerda), nivel_de(atual->direita)); }
    static int diferenca(CelulaAvl* atual) { return nivel_de(atual->esquerda) - nivel_de(atual->direita); }
    static int nivel_de(CelulaAvl* atual) { return atual ? atual->nivel : 0; }
    const char* invariantes() { return "ordem BST, alturas corretas e fator de balanceamento em {-1, 0, 1}"; }
    const char* identificacao() { return "AVL"; }
    ~ArvoreAvl() { destruir(topo); }
    CelulaAvl* topo = nullptr;
};

#endif
