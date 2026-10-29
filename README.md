# Amazon Engineering Challenge — Árvores Balanceadas

Projeto acadêmico em C++ que implementa e compara árvores de busca balanceadas AVL e rubro-negra. Desenvolvido para a atividade **Amazon Engineering Challenge — Árvores Balanceadas**, utiliza chaves inteiras para explorar inserções, buscas e remoções no contexto proposto de gerenciamento de encomendas.

O programa permite operar as duas estruturas pelo terminal, visualizar suas árvores, verificar propriedades e comparar alturas, rotações, recolorações e tempos em sequências de entrada iguais. Não implementa um sistema completo de logística nem armazena os dados entre execuções.

## Identificação

| Campo | Informação |
|---|---|
| Autor | Davi Summer Silva Costa |
| Matrícula | 20250033590 |
| Instituição | Universidade Federal do Rio Grande do Norte |
| Curso | Bacharelado em Tecnologia da Informação |
| Disciplina | Estruturas de Dados Básicas II |
| Professora | Ligia Francielle Borges |

## Funcionalidades

- Inserção, busca e remoção de chaves inteiras nas duas árvores.
- Inserção, busca e remoção de vários valores separados por espaços.
- Percurso em ordem crescente e impressão da estrutura no terminal.
- Exibição do fator de balanceamento na AVL e das cores na rubro-negra.
- Verificação das propriedades de ordenação e balanceamento.
- Relatório de quantidade de elementos, altura, rotações e tempos; recolorações na rubro-negra.
- Limpeza da árvore com reinicialização dos contadores.
- Comparação automática com entradas crescentes, decrescentes e aleatórias.

As chaves são únicas: inserir um valor repetido informa que ele já existe e não altera o conjunto. Buscar um valor ausente informa que ele não foi encontrado; tentar removê-lo mantém a árvore inalterada. As duas árvores do menu são independentes.

## Organização do projeto

| Caminho | Responsabilidade |
|---|---|
| `src/avl.hpp` | Implementação da árvore AVL |
| `src/rubro_negra.hpp` | Implementação da árvore rubro-negra |
| `src/main.cpp` | Menu, instrumentação e comparação interativa |
| `tests/tests.cpp` | Testes de corretude e propriedades |
| `tests/test_support.hpp` | Adaptação das interfaces para os testes |
| `tests/compare.cpp` | Comparação reproduzível e geração de CSV |
| `tests/demo.cpp` | Demonstração das operações nas duas árvores |
| `tests/menu_test.py` | Teste automatizado da interface de terminal |
| `bin/` | Executáveis do programa e dos testes |
| `results/atual/` | Entradas e resultados da validação atual |
| `Makefile` | Compilação e execução dos testes com GNU Make |
| `testar.sh` | Compilação e testes C++ sem depender de Make |
| `INSTRUCOES_TESTES.md` | Guia complementar de testes |
| `docs/Amazon_Engineering_Challenge.pdf` | Apresentação técnica com 19 slides |

Os algoritmos de balanceamento são implementados no projeto. O `std::set` utilizado na suíte serve como referência independente para conferir os resultados; não substitui as árvores implementadas.

Esta pasta contém os executáveis e as evidências atuais. Arquivos de diagnóstico e versões antigas não integram esta cópia organizada.

## Requisitos

- Compilador C++ com suporte a C++17.
- Bash para executar `testar.sh`; no Windows, os comandos abaixo utilizam Git Bash.
- Python 3, sem pacotes adicionais, para o teste automatizado do menu.
- GNU Make, opcional, para os alvos do `Makefile`.

A validação no Windows utilizou GCC 15.2.0 do MSYS2 UCRT64 e compilação com `-O2`. Os executáveis dessa plataforma não devem ser utilizados como executáveis Linux.

## Preparar o ambiente no Windows

Abra o Git Bash na raiz do projeto, onde estão `Makefile`, `src/` e `tests/`.

Na instalação utilizada na validação, o compilador está em `C:\msys64\ucrt64\bin`. Priorize esse ambiente antes de compilar ou executar:

```bash
export PATH="/c/msys64/ucrt64/bin:$PATH"
hash -r
g++ --version
```

Se o MSYS2 estiver em outro local, ajuste o caminho. O comando `export` vale apenas para a sessão atual: repita-o ao abrir um novo terminal.

Esse preparo é importante porque foi identificado um conflito entre o compilador UCRT64 e a biblioteca `libstdc++-6.dll` carregada da instalação do Git. A falha de execução deixou de ocorrer após priorizar o ambiente UCRT64, mantendo `-O2`.

Os executáveis atuais dependem de bibliotecas do ambiente de compilação. Sua execução foi validada no ambiente descrito; a distribuição para computadores sem esse ambiente ainda precisa ser preparada e verificada.

## Compilar e executar

Para compilar somente o programa principal:

```bash
mkdir -p bin
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic src/main.cpp -o bin/encomendas.exe
./bin/encomendas.exe
```

O nome do executável permanece `encomendas.exe`, conforme a organização atual do projeto.

O menu principal oferece:

```text
1) Arvore AVL
2) Arvore Rubro-Negra
3) Comparar as duas arvores
0) Sair
```

Dentro de cada árvore, as opções são inserir, remover, buscar, listar em ordem, imprimir, verificar propriedades, consultar o relatório e esvaziar a estrutura.

### Exemplo de uso

1. Escolha `1` no menu principal para abrir a AVL.
2. Escolha `1` no submenu e informe `30 20 10 20`.
3. O segundo `20` será ignorado por ser repetido.
4. Escolha `4`: o percurso esperado é `10 20 30`.
5. Escolha `3` e informe `20 99`: `20` será encontrado e `99` não.
6. Escolha `2` e informe `20`; depois, escolha `6` para conferir as propriedades.
7. Escolha `0` para voltar ao menu principal.

O mesmo procedimento está disponível para a rubro-negra, escolhendo `2` no menu principal. A opção `3` do menu principal executa uma comparação com quantidade de elementos entre 2 e 2.000.000.

## Executar os testes

Na raiz do projeto, após preparar o ambiente:

```bash
bash testar.sh
echo "Codigo de saida: $?"
```

O script compila os quatro executáveis e executa a suíte C++, o comparador e a demonstração. Ele interrompe a execução se algum comando falhar. O código esperado ao final é `0`.

Execute também o teste do menu, que é separado do script:

```bash
python tests/menu_test.py
echo "Codigo de saida: $?"
```

O resultado deve começar com `PASS menu` e terminar com código `0`. Se sua instalação usar outro comando para Python 3, substitua `python` pelo comando correspondente.

### Alternativa com GNU Make

Se GNU Make estiver disponível, o ambiente já estiver preparado e Python 3 responder ao comando `python`:

```bash
make all
make verify
```

| Alvo | Ação |
|---|---|
| `make all` | Compila o programa e os três executáveis C++ de apoio |
| `make test` | Executa a suíte de corretude |
| `make compare` | Executa a comparação e gera os dados |
| `make demo` | Salva a demonstração das árvores |
| `make menu-test` | Executa o teste do menu com Python |
| `make verify` | Executa os quatro alvos de validação |
| `make clean` | Remove os quatro executáveis atuais, preservando código e resultados |

Para selecionar outro comando Python, por exemplo: `make verify PYTHON=python3`. Não é necessário instalar Make para usar `testar.sh`.

## Cobertura e resultados da validação

A suíte verifica árvores vazias e com um elemento, duplicatas, valores ausentes, entradas crescentes, decrescentes e aleatórias, remoção de nós com zero, um e dois filhos, remoções sucessivas da raiz, extremos inteiros e operações intercaladas. Também exercita os quatro casos de rotação da AVL. Os resultados são confrontados com um conjunto de referência e as propriedades das árvores são verificadas nos testes.

Na validação de 28/10/2026 no Windows:

| Evidência | Resultado |
|---|---|
| `tests.log` | `TOTAL checks=27202 FALHAS=0` |
| `operations.csv` | 27.100 registros, todos com `pass=1` |
| `comparison.csv` | 12 registros, todos com `valida=1` |
| `comparison.log` | Seis casos aprovados |
| `demo.log` | Demonstração das duas árvores com propriedades válidas |
| `menu_test.log` | Teste do menu aprovado |

O total de verificações é maior que o número de linhas em `operations.csv`, pois a suíte também executa verificações adicionais não representadas como operações nesse arquivo. O script C++ e o teste de menu terminaram com código de saída `0` no ambiente validado.

O teste do menu cobre entradas, duplicatas, buscas, remoções, percurso, cores, propriedades, limpeza, comparação e fim de entrada (EOF). Os testes não constituem uma prova de ausência de todos os defeitos nem uma certificação de ausência de vazamentos de memória.

## Metodologia de comparação

Para cada ordem de entrada — crescente, decrescente e aleatória — o comparador executa a mesma sequência nas duas árvores:

1. Insere 10.000 chaves distintas de 1 a 10.000.
2. Registra as métricas e verifica as propriedades após a inserção.
3. Busca as 10.000 chaves existentes e 10.000 chaves ausentes.
4. Remove as primeiras 5.000 chaves da sequência de entrada.
5. Registra novamente as métricas e verifica as propriedades.

A entrada aleatória é gerada com `std::mt19937`, semente `42`, e `std::shuffle`. Os arquivos `input_crescente.txt`, `input_decrescente.txt` e `input_aleatorio.txt` registram as sequências efetivamente utilizadas. Para comparar ambientes com implementações diferentes da biblioteca padrão, considere esses arquivos como referência das entradas.

As alturas são medidas em nós: árvore vazia tem altura 0 e um único nó tem altura 1. Rotações e recolorações são acumuladas; para obter somente os ajustes da remoção, subtraia o registro da inserção do registro final do mesmo caso. Os tempos do CSV são totais dos lotes, em milissegundos; o tempo de busca inclui as buscas existentes e ausentes. Não há medição de consumo de memória.

### Alturas observadas

| Entrada | AVL após inserir | Rubro-negra após inserir | AVL após remover metade | Rubro-negra após remover metade |
|---|---:|---:|---:|---:|
| Crescente | 14 | 24 | 13 | 22 |
| Decrescente | 14 | 24 | 13 | 22 |
| Aleatória | 16 | 16 | 15 | 16 |

A AVL apresentou menor altura nos casos ordenados. No caso aleatório, ambas atingiram a mesma altura após a inserção, mas a rubro-negra realizou menos rotações nessa fase: 5.865, contra 6.968 da AVL. Esses resultados evidenciam diferenças entre as estratégias de balanceamento nas entradas testadas.

Os tempos e contadores completos estão em `results/atual/comparison.csv`. Os tempos registrados correspondem a uma execução e variam com hardware, carga do sistema, compilador e ambiente. Não representam uma superioridade universal de uma estrutura; conclusões de desempenho mais amplas exigem repetições e análise da variação.

## Referências e uso de inteligência artificial

As referências teóricas utilizadas estão indicadas na [apresentação técnica](docs/Amazon_Engineering_Challenge.pdf).

### Declaração de uso de IA

Foi utilizado o ChatGPT, da OpenAI, como ferramenta de apoio em algumas etapas do trabalho, incluindo a reorganização e revisão de trechos de código, o auxílio na preparação de testes e instruções de execução, a redação e revisão da documentação.
