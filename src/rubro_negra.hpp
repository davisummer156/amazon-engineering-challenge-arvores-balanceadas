#ifndef ARVORES_RUBRO_NEGRA_HPP
#define ARVORES_RUBRO_NEGRA_HPP

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

struct CelulaColorida {
    int valor;
    bool rubro;
    CelulaColorida *esquerda, *direita, *ancestral;
};
class ArvoreRubroNegra {
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
        exibir_estrutura<CelulaColorida>(topo, sentinela, [](CelulaColorida* atual) { return std::to_string(atual->valor) + (atual->rubro ? " [VERMELHO]" : " [PRETO]"); });
    }
    bool estrutura_valida() {
        bool sucesso = !topo->rubro && topo->ancestral == sentinela;
        conferir_altura_negra(topo, sucesso);
        return sucesso;
    }
    int conferir_altura_negra(CelulaColorida* atual, bool& sucesso) {
        if (atual == sentinela) return 1;
        if (atual->rubro && (atual->esquerda->rubro || atual->direita->rubro)) sucesso = false;
        if ((atual->esquerda != sentinela && atual->esquerda->ancestral != atual) || (atual->direita != sentinela && atual->direita->ancestral != atual)) sucesso = false;
        int primeiro = conferir_altura_negra(atual->esquerda, sucesso), segundo = conferir_altura_negra(atual->direita, sucesso);
        if (primeiro != segundo) sucesso = false;
        return primeiro + (atual->rubro ? 0 : 1);
    }
    int nivel_total() { return nivel_de(topo); }
    int nivel_de(CelulaColorida* atual) { return atual == sentinela ? 0 : 1 + std::max(nivel_de(atual->esquerda), nivel_de(atual->direita)); }
    void exportar_ordenado(std::vector<int>& valores) { percorrer(topo, valores); }
    void percorrer(CelulaColorida* atual, std::vector<int>& valores) { listar_ordenado<CelulaColorida>(atual, sentinela, valores); }
    bool consultar(int valor_procurado) { return localizar(valor_procurado) != sentinela; }
    bool excluir(int valor_procurado) {
        CelulaColorida* alvo = localizar(valor_procurado);
        if (alvo == sentinela) return false;
        CelulaColorida* substituto = alvo;
        CelulaColorida* atual;
        bool era_rubro = substituto->rubro;
        if (alvo->esquerda == sentinela) {
            atual = alvo->direita;
            substituir_subarvore(alvo, alvo->direita);
        } else if (alvo->direita == sentinela) {
            atual = alvo->esquerda;
            substituir_subarvore(alvo, alvo->esquerda);
        } else {
            substituto = alvo->direita;
            while (substituto->esquerda != sentinela) substituto = substituto->esquerda;
            era_rubro = substituto->rubro;
            atual = substituto->direita;
            if (substituto->ancestral == alvo) {
                atual->ancestral = substituto;
            } else {
                substituir_subarvore(substituto, substituto->direita);
                substituto->direita = alvo->direita;
                substituto->direita->ancestral = substituto;
            }
            substituir_subarvore(alvo, substituto);
            substituto->esquerda = alvo->esquerda;
            substituto->esquerda->ancestral = substituto;
            substituto->rubro = alvo->rubro;
        }
        delete alvo;
        if (!era_rubro) ajustar_exclusao(atual);
        sentinela->ancestral = sentinela;
        return true;
    }
    bool adicionar(int valor_procurado) {
        CelulaColorida* ancestral = sentinela;
        CelulaColorida* atual = topo;
        while (atual != sentinela) {
            if (valor_procurado == atual->valor) return false;
            ancestral = atual;
            atual = valor_procurado < atual->valor ? atual->esquerda : atual->direita;
        }
        CelulaColorida* alvo = new CelulaColorida{valor_procurado, true, sentinela, sentinela, ancestral};
        if (ancestral == sentinela) topo = alvo;
        else if (valor_procurado < ancestral->valor) ancestral->esquerda = alvo;
        else ancestral->direita = alvo;
        ajustar_adicao(alvo);
        return true;
    }
    CelulaColorida* localizar(int valor_procurado) {
        CelulaColorida* atual = topo;
        while (atual != sentinela && atual->valor != valor_procurado) atual = valor_procurado < atual->valor ? atual->esquerda : atual->direita;
        return atual;
    }
    void ajustar_exclusao(CelulaColorida* atual) {
        while (atual != topo && !atual->rubro) {
            bool atual_a_esquerda = atual == atual->ancestral->esquerda;
            CelulaColorida* irmao = atual_a_esquerda ? atual->ancestral->direita : atual->ancestral->esquerda;
            if (irmao->rubro) {
                definir_cor(irmao, false);
                definir_cor(atual->ancestral, true);
                atual_a_esquerda ? girar_esquerda(atual->ancestral) : girar_direita(atual->ancestral);
                irmao = atual_a_esquerda ? atual->ancestral->direita : atual->ancestral->esquerda;
            }
            CelulaColorida* perto = atual_a_esquerda ? irmao->esquerda : irmao->direita;
            CelulaColorida* longe = atual_a_esquerda ? irmao->direita : irmao->esquerda;
            if (!perto->rubro && !longe->rubro) {
                definir_cor(irmao, true);
                atual = atual->ancestral;
            } else {
                if (!longe->rubro) {
                    definir_cor(perto, false);
                    definir_cor(irmao, true);
                    atual_a_esquerda ? girar_direita(irmao) : girar_esquerda(irmao);
                    irmao = atual_a_esquerda ? atual->ancestral->direita : atual->ancestral->esquerda;
                }
                definir_cor(irmao, atual->ancestral->rubro);
                definir_cor(atual->ancestral, false);
                definir_cor(atual_a_esquerda ? irmao->direita : irmao->esquerda, false);
                atual_a_esquerda ? girar_esquerda(atual->ancestral) : girar_direita(atual->ancestral);
                atual = topo;
            }
        }
        definir_cor(atual, false);
    }
    void ajustar_adicao(CelulaColorida* alvo) {
        while (alvo->ancestral->rubro) {
            CelulaColorida* avo = alvo->ancestral->ancestral;
            bool ancestral_a_esquerda = alvo->ancestral == avo->esquerda;
            CelulaColorida* tio = ancestral_a_esquerda ? avo->direita : avo->esquerda;
            if (tio->rubro) {
                definir_cor(alvo->ancestral, false);
                definir_cor(tio, false);
                definir_cor(avo, true);
                alvo = avo;
            } else {
                if (ancestral_a_esquerda ? alvo == alvo->ancestral->direita : alvo == alvo->ancestral->esquerda) {
                    alvo = alvo->ancestral;
                    ancestral_a_esquerda ? girar_esquerda(alvo) : girar_direita(alvo);
                }
                definir_cor(alvo->ancestral, false);
                definir_cor(avo, true);
                ancestral_a_esquerda ? girar_direita(avo) : girar_esquerda(avo);
            }
        }
        definir_cor(topo, false);
    }
    void substituir_subarvore(CelulaColorida* u, CelulaColorida* valores) {
        if (u->ancestral == sentinela) topo = valores;
        else if (u == u->ancestral->esquerda) u->ancestral->esquerda = valores;
        else u->ancestral->direita = valores;
        valores->ancestral = u->ancestral;
    }
    void girar_direita(CelulaColorida* base) { girar(base, false); }
    void girar_esquerda(CelulaColorida* base) { girar(base, true); }
    void girar(CelulaColorida* base, bool para_esquerda) {
        CelulaColorida* promovido = para_esquerda ? base->direita : base->esquerda;
        CelulaColorida* transferido = para_esquerda ? promovido->esquerda : promovido->direita;
        if (para_esquerda) base->direita = transferido;
        else base->esquerda = transferido;
        if (transferido != sentinela) transferido->ancestral = base;
        promovido->ancestral = base->ancestral;
        if (base->ancestral == sentinela) topo = promovido;
        else if (base->ancestral->esquerda == base) base->ancestral->esquerda = promovido;
        else base->ancestral->direita = promovido;
        if (para_esquerda) promovido->esquerda = base;
        else promovido->direita = base;
        base->ancestral = promovido;
        ++total_rotacoes;
    }
    void definir_cor(CelulaColorida* atual, bool rubro) {
        if (atual->rubro != rubro) {
            atual->rubro = rubro;
            total_recoloracoes++;
        }
    }
    void destruir(CelulaColorida* atual) { destruir_ramos<CelulaColorida>(atual, sentinela); }
    const char* invariantes() {
        return "ordem BST, raiz preta, sem vermelhos consecutivos e mesma altura negra em todos os caminhos";
    }
    const char* identificacao() { return "Rubro-Negra"; }
    ~ArvoreRubroNegra() {
        destruir(topo);
        delete sentinela;
    }
    ArvoreRubroNegra() {
        sentinela = new CelulaColorida{0, false, nullptr, nullptr, nullptr};
        sentinela->esquerda = sentinela->direita = sentinela->ancestral = sentinela;
        topo = sentinela;
    }
    CelulaColorida* topo;
    CelulaColorida* sentinela;
};

#endif
