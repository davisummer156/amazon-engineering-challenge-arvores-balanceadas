#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
// Reutiliza a instrumentacao e a fabrica do programa, sem executar seu menu.
#define main main_interativo
#include "../src/main.cpp"
#undef main

int main() {
    std::cout << std::unitbuf;
    try {
        std::cout << "INICIO comparador revisao 3\n";
        {
        const int n = 10000;
        std::ofstream saida("results/atual/comparison.csv");
        if (!saida) throw std::runtime_error("Falha ao abrir comparison.csv");
        saida << "cenario,arvore,fase,n,altura,rotacoes_acumuladas,recoloracoes_acumuladas,insercao_ms,busca_ms,remocao_ms,valida\n";
        saida << std::fixed << std::setprecision(6);
        for (int modo = 0; modo < 3; ++modo) {
            std::vector<int> dados(n), ausentes(n);
            std::iota(dados.begin(), dados.end(), 1);
            std::iota(ausentes.begin(), ausentes.end(), n + 1);
            const char* cenario = modo == 0 ? "crescente" : modo == 1 ? "decrescente" : "aleatorio";
            if (modo == 1) std::reverse(dados.begin(), dados.end());
            if (modo == 2) { std::mt19937 gerador(42); std::shuffle(dados.begin(), dados.end(), gerador); }
            std::ofstream entrada(std::string("results/atual/input_") + cenario + ".txt");
            if (!entrada) throw std::runtime_error("Falha ao salvar entrada");
            for (int k : dados) entrada << k << '\n';
            entrada.close();
            if (entrada.fail()) throw std::runtime_error("Falha ao fechar arquivo de entrada");
            for (int tipo = 0; tipo < 2; ++tipo) {
                std::cout << "CASO " << cenario << " tipo=" << tipo << '\n';
                auto arvore = construir(tipo);
                auto registrar = [&](const char* fase) {
                    const bool valida = arvore->verificar();
                    saida << cenario << ',' << arvore->identificacao() << ',' << fase << ','
                          << arvore->quantidade << ',' << arvore->nivel_total() << ','
                          << arvore->total_rotacoes << ',' << arvore->total_recoloracoes << ','
                          << arvore->duracoes[ADICIONAR] / 1e6 << ',' << arvore->duracoes[CONSULTAR] / 1e6 << ','
                          << arvore->duracoes[EXCLUIR] / 1e6 << ',' << valida << '\n';
                    if (!valida) throw std::runtime_error("Propriedades violadas");
                };
                std::cout << "ETAPA insercao\n";
                if (arvore->processar_lote(ADICIONAR, dados) != n) throw std::runtime_error("Falha na insercao");
                registrar("insercao");
                std::cout << "ETAPA buscas\n";
                if (arvore->processar_lote(CONSULTAR, dados) != n ||
                    arvore->processar_lote(CONSULTAR, ausentes) != 0) throw std::runtime_error("Falha na busca");
                const std::vector<int> remocoes(dados.begin(), dados.begin() + n / 2);
                std::cout << "ETAPA remocao\n";
                if (arvore->processar_lote(EXCLUIR, remocoes) != n / 2) throw std::runtime_error("Falha na remocao");
                registrar("remocao_metade");
                std::cout << "PASS " << cenario << ' ' << arvore->identificacao()
                          << " insercoes=10000 buscas=20000 remocoes=5000 altura_final="
                          << arvore->nivel_total() << '\n';
                std::cout << "ETAPA liberando arvore\n";
                arvore.reset();
                std::cout << "ETAPA arvore liberada\n";
            }
        }
        std::cout << "ENCERRAMENTO fechando comparison.csv\n";
        saida.close();
        if (saida.fail()) throw std::runtime_error("Falha ao fechar comparison.csv");
        }
        std::cout << "Comparacao concluida: results/atual/comparison.csv\n";
        std::cout << "FIM comparador revisao 3\n";
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "FAIL " << erro.what() << '\n';
        return 1;
    }
}
