# Amazon Engineering Challenge — Instruções de testes

Este guia descreve como compilar e validar a versão atual das árvores AVL e rubro-negra. A suíte C++, o comparador, a demonstração e o teste automatizado do menu foram executados com sucesso no Windows em **28/10/2026**, após o ajuste de ambiente descrito abaixo.

Os testes usam as implementações de `src/avl.hpp` e `src/rubro_negra.hpp`. O arquivo `tests/test_support.hpp` adapta as interfaces para a suíte, e `std::set` é utilizado apenas como referência independente dos resultados esperados.

## 1. Preparar o terminal

Abra o **Git Bash na raiz de `projetoedb`**, onde estão `Makefile`, `testar.sh`, `src/` e `tests/`. Não execute os comandos dentro de `src/` ou `bin/`.

São necessários um compilador com suporte a C++17 e Bash. Python 3 é necessário para o teste automatizado do menu. GNU Make é opcional.

No ambiente validado, o compilador é GCC 15.2.0 do MSYS2 UCRT64, instalado em `C:\msys64\ucrt64\bin`. Antes de compilar ou executar, use:

```bash
export PATH="/c/msys64/ucrt64/bin:$PATH"
hash -r
g++ --version
python --version
```

Se a instalação estiver em outro local, ajuste o caminho. O `export` vale somente para a sessão atual: repita-o ao abrir um novo Git Bash. Esses comandos não alteram permanentemente o PATH do Windows.

### Por que ajustar o PATH?

Durante a validação, o programa compilado com o ambiente UCRT64 carregou `libstdc++-6.dll` de `C:\Program Files\Git\mingw64\bin`. O diagnóstico do GDB localizou uma falha na criação de um fluxo de saída para arquivo, antes das operações nas árvores.

Após priorizar `C:\msys64\ucrt64\bin`, os testes terminaram com código `0`, mantendo a otimização `-O2`. Não é necessário desativar a otimização nem modificar os algoritmos para reproduzir a configuração validada.

## 2. Compilar e executar os testes C++

Feche qualquer executável do projeto que esteja aberto e execute:

```bash
bash testar.sh
echo "Codigo de saida: $?"
```

O script compila `encomendas.exe`, `tests.exe`, `compare.exe` e `demo.exe` em `bin/`, usando C++17, `-O2`, `-Wall`, `-Wextra` e `-Wpedantic`. Em seguida, executa a suíte C++, o comparador e a demonstração, salvando os resultados em `results/atual/`.

O script para se algum comando falhar. Confira o código imediatamente após a execução: o esperado é **`0`**. O comando `echo` mostra o estado do comando imediatamente anterior.

Novas execuções sobrescrevem os arquivos de resultado correspondentes. Preserve uma cópia das evidências anteriores caso precise compará-las.

### Resultado esperado da suíte

Ao final da suíte, devem aparecer:

```text
TOTAL checks=27202 FALHAS=0
FIM testes revisao 2
```

Os marcadores de revisão identificam os arquivos de teste atuais. Não indicam falha nem precisam ser removidos para executar os testes.

### Resultado esperado do comparador

Devem aparecer seis linhas `PASS`: crescente, decrescente e aleatório para cada árvore. O encerramento deve incluir:

```text
Comparacao concluida: results/atual/comparison.csv
FIM comparador revisao 3
```

O CSV deve conter **12 linhas de dados**, além do cabeçalho: inserção e remoção de metade dos elementos para cada um dos seis casos. A coluna `valida` deve ser `1` em todas as linhas.

### Conferir a demonstração

```bash
cat results/atual/demo.log
```

A demonstração apresenta inserções, busca existente e inexistente, remoção, percurso e impressão das duas árvores. Todos os indicadores `valida` devem ser `1`.

## 3. Executar o teste automatizado do menu

O script `testar.sh` não executa esse teste; ele apenas informa o comando. Após a compilação, no mesmo terminal:

```bash
python tests/menu_test.py
echo "Codigo de saida: $?"
```

O resultado esperado é:

```text
PASS menu: duas arvores, entradas, duplicatas, buscas, remocoes, percurso, cores, propriedades, limpeza, comparacao e EOF.
Codigo de saida: 0
```

O comando confirmado no ambiente validado foi `python`. Se sua instalação usar outro comando para Python 3, substitua-o. Não é necessário instalar pacotes Python adicionais.

## 4. Alternativa com GNU Make

Prepare o ambiente conforme a seção 1. Se `make --version` funcionar, execute:

```bash
make all
make verify
echo "Codigo de saida: $?"
```

`make verify` executa a suíte C++, o comparador, a demonstração e o teste do menu. O código esperado é `0`.

Para executar partes separadamente:

```bash
make test
make compare
make demo
make menu-test
```

O Makefile usa `python` por padrão. Para selecionar outro comando, use, por exemplo, `make verify PYTHON=python3`.

Se precisar recompilar tudo do zero, execute `make clean` antes de `make all`. A limpeza remove somente os quatro executáveis atuais em `bin/`; não apaga código, resultados nem executáveis de diagnóstico com outros nomes.

Se Make não estiver disponível, use as seções 2 e 3. Não é necessário instalar outro ambiente apenas para executar os testes.

## 5. Abrir o programa interativo

Após a compilação, no terminal preparado:

```bash
./bin/encomendas.exe
```

O menu principal permite escolher AVL, rubro-negra ou comparação entre as duas árvores. Use esse executável para demonstrar a versão atual. Os executáveis de `validacao_windows/bin/`, caso essa pasta ainda esteja presente, pertencem ao histórico do projeto.

## 6. Evidências e resultados confirmados

Os arquivos atuais ficam em `results/atual/`:

| Arquivo | Conteúdo ou resultado confirmado |
|---|---|
| `tests.log` | 27.202 verificações, zero falhas e marcador de encerramento |
| `operations.csv` | 27.100 registros, todos com `pass=1` |
| `comparison.csv` | 12 registros, todos com `valida=1` |
| `comparison.log` | Seis casos aprovados e encerramento do comparador |
| `input_crescente.txt` | Sequência crescente utilizada na comparação |
| `input_decrescente.txt` | Sequência decrescente utilizada na comparação |
| `input_aleatorio.txt` | Sequência embaralhada utilizada na comparação |
| `demo.log` | Demonstração das duas árvores com propriedades válidas |
| `menu_input.txt` | Entrada principal do teste do menu |
| `menu_output.txt` | Saída correspondente do programa |
| `menu_test.log` | Confirmação de aprovação do teste do menu |

As 27.202 verificações incluem verificações adicionais que não geram linhas de operação; por isso o total difere das 27.100 linhas de `operations.csv`.

Na validação de 28/10/2026, tanto `bash testar.sh` quanto `python tests/menu_test.py` terminaram com código `0`. Os logs e CSVs enviados foram conferidos após essa execução. Os códigos de saída foram observados no terminal; não são gravados automaticamente nos logs pelo script.

Não utilize `results/operations.csv`, `results/atual.zip`, a pasta `validacao_windows/` ou logs antigos com `debug` no nome como evidência da execução atual. Esses itens, se presentes, são históricos.

## 7. Cobertura e metodologia

A suíte cobre árvores vazias, um único elemento, duplicatas, valores ausentes, ordens de inserção, remoção de nós com zero, um e dois filhos, remoções sucessivas da raiz, extremos inteiros, operações intercaladas e os quatro casos de rotação AVL. A quantidade de filhos é conferida antes dos testes específicos de remoção. Os resultados são comparados com um conjunto de referência e as propriedades das árvores são verificadas.

No comparador, para cada ordem de entrada e cada árvore:

1. Inserir 10.000 valores distintos de 1 a 10.000.
2. Registrar métricas e verificar as propriedades após a inserção.
3. Buscar 10.000 valores existentes e 10.000 inexistentes.
4. Remover os primeiros 5.000 valores da sequência de entrada.
5. Registrar métricas e verificar as propriedades após as remoções.

As duas árvores recebem a mesma sequência. A entrada aleatória utiliza `std::mt19937` com semente `42` e `std::shuffle`. Os arquivos de entrada registram a sequência efetivamente utilizada; considere-os como referência ao comparar ambientes com bibliotecas padrão diferentes.

| Entrada | Altura AVL após inserir | Altura rubro-negra após inserir | Altura AVL após remover metade | Altura rubro-negra após remover metade |
|---|---:|---:|---:|---:|
| Crescente | 14 | 24 | 13 | 22 |
| Decrescente | 14 | 24 | 13 | 22 |
| Aleatória | 16 | 16 | 15 | 16 |

As alturas são medidas em nós: árvore vazia tem altura 0; um nó tem altura 1. Rotações e recolorações são acumuladas. Para obter apenas os ajustes da remoção, subtraia os valores da linha de inserção dos valores da linha final do mesmo caso.

Os tempos do CSV são totais dos lotes em milissegundos. O tempo de busca inclui valores existentes e inexistentes. Os tempos variam entre execuções, computadores e ambientes e não precisam coincidir para o teste passar. A medição atual é de uma execução, sem estimativa de variação estatística. Não há medição de consumo de memória.

## 8. Se ocorrer uma falha

Se houver `Segmentation fault` ou código `139`, confirme primeiro o preparo do ambiente da seção 1 e repita a compilação e os testes. Não considere a execução aprovada apenas porque algumas linhas `PASS` apareceram antes da falha.

Se a falha persistir no comparador, com GDB instalado, gere um diagnóstico:

```bash
g++ -std=c++17 -O2 -g -Wall -Wextra -Wpedantic tests/compare.cpp -o bin/compare.exe
gdb -q -batch -ex run -ex "bt full" --args ./bin/compare.exe > diagnostico_gdb.txt 2>&1
```

Esse comando adiciona informações de depuração e mantém `-O2`. O arquivo `diagnostico_gdb.txt` serve para investigar a falha, não para comprovar aprovação. O código de saída do GDB não substitui a conferência da execução normal do programa.

Se `python` não for encontrado, verifique a instalação e o comando disponível para Python 3. Se `make` não for encontrado, utilize `testar.sh`. Em erros de compilação, corrija a falha antes de executar binários: um executável antigo pode permanecer na pasta mesmo após uma compilação malsucedida.

## 9. Limites e entrega

Os testes confirmam os cenários descritos; não são uma prova de ausência de todos os defeitos nem uma certificação de ausência de vazamentos de memória.

Os executáveis Windows atuais dependem de bibliotecas do ambiente de compilação. A execução foi validada no ambiente UCRT64 preparado acima; a distribuição para computadores sem esse ambiente ainda precisa ser preparada e verificada.

Preserve código, executáveis e resultados correspondentes à mesma versão. A entrega acadêmica exige arquivos diretamente acessíveis no repositório público, incluindo o executável Windows atualizado, e apresentação em PDF com o link do repositório. O ZIP de preparação não substitui essa entrega. Este guia complementa o `README.md`; a apresentação está em `docs/Amazon_Engineering_Challenge.pdf` e contém o link do repositório público.
