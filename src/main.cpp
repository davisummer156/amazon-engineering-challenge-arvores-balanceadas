#include "avl.hpp"
#include "rubro_negra.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

enum { ADICIONAR, EXCLUIR, CONSULTAR };

long long instante_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

struct EstruturaBusca {
    long long total_rotacoes = 0, total_recoloracoes = 0, duracoes[3] = {0, 0, 0}, chamadas[3] = {0, 0, 0};
    std::size_t quantidade = 0;
    bool usa_cores = false;

    virtual ~EstruturaBusca() {}
    virtual const char* identificacao() = 0;
    virtual const char* invariantes() = 0;
    virtual bool adicionar(int valor_procurado) = 0;
    virtual bool excluir(int valor_procurado) = 0;
    virtual bool consultar(int valor_procurado) = 0;
    virtual void exportar_ordenado(std::vector<int>& valores) = 0;
    virtual int nivel_total() = 0;
    virtual bool estrutura_valida() = 0;
    virtual void exibir() = 0;

    void registrar_insercao() {
        ++quantidade;
    }
    void registrar_remocao() { quantidade--; }

    bool executar_medido(int operacao, int valor) {
        const long long inicio = instante_ns();
        const bool resultado = executar(operacao, valor);
        duracoes[operacao] += instante_ns() - inicio;
        ++chamadas[operacao];
        return resultado;
    }

    bool executar(int operacao, int valor) {
        switch (operacao) {
            case ADICIONAR: return adicionar(valor);
            case EXCLUIR: return excluir(valor);
            default: return consultar(valor);
        }
    }

    bool adicionar_cronometrado(int valor) { return executar_medido(ADICIONAR, valor); }
    bool excluir_cronometrado(int valor) { return executar_medido(EXCLUIR, valor); }
    bool consultar_cronometrado(int valor) { return executar_medido(CONSULTAR, valor); }
    long long processar_lote(int escolha, const std::vector<int>& valores) {
        long long sucesso = 0, marco = instante_ns();
        for (int valor_procurado : valores) sucesso += executar(escolha, valor_procurado);
        duracoes[escolha] += instante_ns() - marco;
        chamadas[escolha] += valores.size();
        return sucesso;
    }

    bool verificar() {
        std::vector<int> valores;
        exportar_ordenado(valores);
        for (std::size_t i = 1; i < valores.size(); i++)
            if (valores[i - 1] >= valores[i]) return false;
        return valores.size() == quantidade && estrutura_valida();
    }

    void exibir_estatisticas() {
        const char* ops[3] = {"Insercao", "Remocao", "Busca"};
        printf("\n===== Relatorio: %s =====\n", identificacao());
        printf("Elementos: %lld | Altura (em nos): %d\n", (long long)quantidade, nivel_total());
        printf("Rotacoes: %lld", total_rotacoes);
        if (usa_cores) printf(" | Recoloracoes: %lld", total_recoloracoes);
        puts("\nTempo (ns):");
        for (int i = 0; i < 3; i++)
            printf("  %-9s operacoes=%-8lld total=%-12lld media=%.1f\n", ops[i], chamadas[i], duracoes[i],
                   chamadas[i] ? (double)duracoes[i] / chamadas[i] : 0.0);

    }
};

template <typename Implementacao>
class ArvoreMedida : public EstruturaBusca {
    Implementacao arvore;

    void atualizar_contadores() {
        total_rotacoes = arvore.total_rotacoes;
        total_recoloracoes = arvore.total_recoloracoes;
    }

public:
    explicit ArvoreMedida(bool colorida) { usa_cores = colorida; }
    const char* identificacao() override { return arvore.identificacao(); }
    const char* invariantes() override { return arvore.invariantes(); }
    bool adicionar(int valor) override {
        const bool inserido = arvore.adicionar(valor);
        if (inserido) registrar_insercao();
        atualizar_contadores();
        return inserido;
    }
    bool excluir(int valor) override {
        const bool removido = arvore.excluir(valor);
        if (removido) registrar_remocao();
        atualizar_contadores();
        return removido;
    }
    bool consultar(int valor) override { return arvore.consultar(valor); }
    void exportar_ordenado(std::vector<int>& valores) override { arvore.exportar_ordenado(valores); }
    int nivel_total() override { return arvore.nivel_total(); }
    bool estrutura_valida() override { return arvore.estrutura_valida(); }
    void exibir() override { arvore.exibir(); }
};

std::unique_ptr<EstruturaBusca> construir(int modelo) {
    if (modelo == 0) return std::unique_ptr<EstruturaBusca>(new ArvoreMedida<ArvoreAvl>(false));
    return std::unique_ptr<EstruturaBusca>(new ArvoreMedida<ArvoreRubroNegra>(true));
}

std::string ler_linha() {
    std::string s;
    if (!std::getline(std::cin, s)) exit(0);
    return s;
}

int ler_inteiro(const char* msg = "Opcao: ") {
    printf("%s", msg);
    fflush(stdout);
    std::istringstream is(ler_linha());
    int atual;
    return (is >> atual) ? atual : -1;
}

std::vector<int> ler_valores(const char* msg) {
    printf("%s", msg);
    fflush(stdout);
    std::istringstream is(ler_linha());
    std::vector<int> valores;
    int atual;
    while (is >> atual) valores.push_back(atual);
    if (!is.eof()) puts("Aviso: entrada invalida ignorada a partir do primeiro valor incorreto.");
    if (valores.empty()) puts("Nenhum valor valido informado.");
    return valores;
}

void executar_menu(std::unique_ptr<EstruturaBusca>& primeiro, int modelo) {
    while (true) {
        printf("\n--- Arvore %s ---\n", primeiro->identificacao());
        puts("1) Inserir");
        puts("2) Remover");
        puts("3) Buscar");
        puts("4) Percurso em ordem");
        puts("5) Imprimir arvore");
        puts("6) Verificar propriedades");
        puts("7) Relatorio (tempo, rotacoes)");
        puts("8) Esvaziar arvore e zerar contadores");
        puts("0) Voltar");
        int escolha = ler_inteiro();
        if (escolha == 0) return;
        if (escolha == 1) {
            for (int valor_procurado : ler_valores("Valores a inserir (separados por espaco): "))
                printf("%d: %s\n", valor_procurado, primeiro->adicionar_cronometrado(valor_procurado) ? "inserido" : "ja existe, insercao ignorada");
        } else if (escolha == 2) {
            for (int valor_procurado : ler_valores("Valores a remover (separados por espaco): "))
                printf("%d: %s\n", valor_procurado, primeiro->excluir_cronometrado(valor_procurado) ? "removido" : "nao encontrado, nada removido");
        } else if (escolha == 3) {
            for (int valor_procurado : ler_valores("Valores a buscar (separados por espaco): "))
                printf("%d: %s\n", valor_procurado, primeiro->consultar_cronometrado(valor_procurado) ? "encontrado" : "nao encontrado");
        } else if (escolha == 4) {
            std::vector<int> valores;
            primeiro->exportar_ordenado(valores);
            if (valores.empty()) {
                puts("Arvore vazia");
            } else {
                printf("Em ordem:");
                for (int valor_procurado : valores) printf(" %d", valor_procurado);
                puts("");
            }
        } else if (escolha == 5) {
            primeiro->exibir();
        } else if (escolha == 6) {
            printf("Propriedades (%s): %s\n", primeiro->invariantes(), primeiro->verificar() ? "OK" : "VIOLADAS");
        } else if (escolha == 7) {
            primeiro->exibir_estatisticas();
        } else if (escolha == 8) {
            primeiro = construir(modelo);
            puts("Arvore esvaziada.");
        } else {
            puts("Opcao invalida.");
        }
    }
}

void executar_comparacao() {
    int tamanho = ler_inteiro("Quantidade de elementos N (2 a 2000000): ");
    if (tamanho < 2 || tamanho > 2000000) {
        puts("Valor invalido.");
        return;
    }
    std::vector<int> ascendente(tamanho), descendente(tamanho), embaralhado(tamanho), ausentes(tamanho);
    std::iota(ascendente.begin(), ascendente.end(), 1);
    std::iota(ausentes.begin(), ausentes.end(), tamanho + 1);
    std::reverse_copy(ascendente.begin(), ascendente.end(), descendente.begin());
    embaralhado = ascendente;
    std::mt19937 gen(42);
    std::shuffle(embaralhado.begin(), embaralhado.end(), gen);
    const char* nomes[3] = {"crescente", "decrescente", "aleatorio"};
    std::vector<int>* amostras[3] = {&ascendente, &descendente, &embaralhado};
    int limite_altura = 0;
    while ((1LL << limite_altura) < tamanho + 1LL) limite_altura++;
    printf("\nN = %d | altura minima possivel = %d (alturas em nos)\n", tamanho, limite_altura);
    puts("Sequencia: inserir N | buscar N existentes + N inexistentes | remover os N/2 primeiros do conjunto");
    printf("%-11s %-11s %7s %7s %9s %9s %8s %8s %8s  %s\n", "Conjunto", "Arvore", "Alt.ins", "Alt.fin",
           "Rotacoes", "Recolor.", "Ins(ms)", "Bus(ms)", "Rem(ms)", "valida");
    for (int c = 0; c < 3; c++)
        for (int modelo = 0; modelo < 2; modelo++) {
            std::unique_ptr<EstruturaBusca> marco = construir(modelo);
            long long ni = marco->processar_lote(ADICIONAR, *amostras[c]);
            int h1 = marco->nivel_total();
            bool sucesso = marco->verificar();
            long long nb1 = marco->processar_lote(CONSULTAR, *amostras[c]), nb2 = marco->processar_lote(CONSULTAR, ausentes);
            long long nr = marco->processar_lote(EXCLUIR, std::vector<int>(amostras[c]->begin(), amostras[c]->begin() + tamanho / 2));
            sucesso = sucesso && marco->verificar() && ni == tamanho && nb1 == tamanho && nb2 == 0 && nr == tamanho / 2;
            std::string rc = marco->usa_cores ? std::to_string(marco->total_recoloracoes) : "-";
            printf("%-11s %-11s %7d %7d %9lld %9s %8.3f %8.3f %8.3f  %s\n", nomes[c], marco->identificacao(), h1,
                   marco->nivel_total(), marco->total_rotacoes, rc.c_str(), marco->duracoes[ADICIONAR] / 1e6, marco->duracoes[CONSULTAR] / 1e6, marco->duracoes[EXCLUIR] / 1e6,
                   sucesso ? "OK" : "ERRO");
        }
}

int main() {
    std::unique_ptr<EstruturaBusca> arv[2] = {construir(0), construir(1)};
    while (true) {
        puts("\n=== Arvores Balanceadas ===");
        puts("1) Arvore AVL");
        puts("2) Arvore Rubro-Negra");
        puts("3) Comparar as duas arvores");
        puts("0) Sair");
        int escolha = ler_inteiro();
        switch (escolha) {
            case 0: return 0;
            case 1:
            case 2: executar_menu(arv[escolha - 1], escolha - 1); break;
            case 3: executar_comparacao(); break;
            default: puts("Opcao invalida."); break;
        }
    }
}
