#!/usr/bin/env bash
set -eu
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
mkdir -p bin results/atual
printf '%s\n' 'Compilando o programa e os testes atuais...'
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic src/main.cpp -o bin/encomendas.exe
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic tests/tests.cpp -o bin/tests.exe
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic tests/compare.cpp -o bin/compare.exe
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic tests/demo.cpp -o bin/demo.exe
./bin/tests.exe > results/atual/tests.log
cat results/atual/tests.log
./bin/compare.exe > results/atual/comparison.log
cat results/atual/comparison.log
./bin/demo.exe > results/atual/demo.log
printf '%s\n' 'Testes C++ e demonstracao concluidos. Resultados: results/atual/'
printf '%s\n' 'Teste adicional do menu (Python 3): python tests/menu_test.py'
