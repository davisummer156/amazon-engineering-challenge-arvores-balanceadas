"""Teste do menu atual; executar da raiz do projeto com Python 3."""
from pathlib import Path
import subprocess

raiz = Path(__file__).resolve().parents[1]
resultados = raiz / 'results' / 'atual'
resultados.mkdir(parents=True, exist_ok=True)
linhas = ['abc', '9']
for tipo in ('1', '2'):
    linhas += [tipo, '4', '3', '99', '2', '99', '1', '30 20 10 20',
               '3', '20 99', '4', '5', '6', '2', '20', '6', '7',
               '1', 'abc', '1', '1 2 erro 4', '8', '4', '6', '0']
linhas += ['3', '1', '3', '10', '0']
entrada = '\n'.join(linhas) + '\n'
(resultados / 'menu_input.txt').write_text(entrada, encoding='utf-8')
p = subprocess.run([str(raiz / 'bin' / 'encomendas.exe')], input=entrada,
                   text=True, encoding='utf-8', capture_output=True, check=True,
                   timeout=30, cwd=raiz)
(resultados / 'menu_output.txt').write_text(p.stdout, encoding='utf-8')
for texto in ['Opcao invalida.', 'ja existe, insercao ignorada',
              '20: encontrado', '99: nao encontrado', 'nada removido',
              'Em ordem: 10 20 30', '[PRETO]', '[VERMELHO]', 'Arvore vazia',
              'Nenhum valor valido informado.', 'Aviso: entrada invalida',
              'Arvore esvaziada.', 'Valor invalido.', 'Recoloracoes:']:
    assert texto in p.stdout, texto
assert 'VIOLADAS' not in p.stdout and 'ERRO' not in p.stdout
assert p.stdout.count('): OK') >= 6
for entrada_eof in ('', '1\n1\n', '2\n2\n'):
    eof = subprocess.run([str(raiz / 'bin' / 'encomendas.exe')], input=entrada_eof,
                         text=True, capture_output=True, timeout=5, cwd=raiz)
    assert eof.returncode == 0
mensagem = 'PASS menu: duas arvores, entradas, duplicatas, buscas, remocoes, percurso, cores, propriedades, limpeza, comparacao e EOF.\n'
(resultados / 'menu_test.log').write_text(mensagem, encoding='utf-8')
print(mensagem, end='')
