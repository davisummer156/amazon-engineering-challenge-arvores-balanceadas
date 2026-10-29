#include <iostream>
#include <fstream>
#include <set>
#include <random>
#include <numeric>
#include "test_support.hpp"
long long checks=0;
void require(bool b,const std::string& msg){++checks;if(!b)throw std::runtime_error(msg);}
template<class T> void scenario(std::ostream& logFile,const std::string& name,const std::vector<std::pair<char,int>>& ops){T t;std::set<int> expected;
 for(auto [op,k]:ops){bool e,a;if(op=='I'){e=expected.insert(k).second;a=t.insert(k);}else if(op=='R'){e=expected.erase(k)>0;a=t.erase(k);}else{e=expected.count(k)>0;a=t.contains(k);}
 bool ok=e==a&&t.valid()&&t.size()==expected.size()&&t.inorder()==std::vector<int>(expected.begin(),expected.end());
 logFile<<name<<','<<op<<','<<k<<','<<e<<','<<a<<','<<ok<<'\n';require(ok,name+" chave="+std::to_string(k));}
 std::cout<<"PASS "<<name<<" operacoes="<<ops.size()<<"\n";
}
template<class T>void suite(std::ostream& logFile,const std::string& type){
 scenario<T>(logFile,type+"_vazia_unico",{{'B',7},{'R',7},{'I',7},{'I',7},{'B',7},{'B',8},{'R',7},{'R',7}});
 for(int mode=0;mode<3;++mode){std::vector<int> v(128);std::iota(v.begin(),v.end(),0);if(mode==1)std::reverse(v.begin(),v.end());if(mode==2){std::mt19937 rng(39);std::shuffle(v.begin(),v.end(),rng);}
 std::vector<std::pair<char,int>> ops;for(int k:v)ops.push_back({'I',k});for(int k:v)ops.push_back({'B',k});for(int k:v)ops.push_back({'I',k});for(int k:v)ops.push_back({'R',k});scenario<T>(logFile,type+"_ordem_"+std::to_string(mode),ops);}
 // As pré-condições são verificadas em cada estrutura, não presumidas pela chave.
 for(int degree=0;degree<=2;++degree){T t;for(int k:{20,10,30,5,15,25,35,2}){t.insert(k);require(t.valid(),"setup filhos");}
 int victim=0;bool found=false;auto expected=t.inorder();for(int k:expected)if(t.children(k)==degree){victim=k;found=true;break;}
 require(found,"configuracao ausente");require(t.children(victim)==degree,"precondicao");require(t.erase(victim),"remocao");expected.erase(std::find(expected.begin(),expected.end(),victim));require(t.valid()&&t.inorder()==expected&&t.size()==expected.size(),"pos remocao");
 std::cout<<"PASS "<<type<<"_filhos="<<degree<<" chave="<<victim<<" esperado=removido obtido=removido invariantes=OK\n";}
 {T t;for(int k:{10,5,15,2,7})t.insert(k);while(t.size()){int k=t.rootKey();require(t.erase(k)&&t.valid(),"raiz");std::cout<<"PASS "<<type<<"_raiz chave="<<k<<"\n";}}
 std::vector<std::pair<char,int>> ops;std::mt19937 rng(2026);for(int i=0;i<12000;++i){int op=int(rng()%3),k=int(rng()%501)-250;ops.push_back({"IRB"[op],k});}scenario<T>(logFile,type+"_intercalado_seed2026",ops);
 scenario<T>(logFile,type+"_limites",{{'I',INT32_MIN},{'I',INT32_MAX},{'I',0},{'R',INT32_MIN},{'R',INT32_MAX},{'R',0}});
}
int main() {
    std::cout << std::unitbuf;
    try {
        std::cout << "INICIO testes revisao 2\n";
        {
            std::ofstream logFile("results/atual/operations.csv");
            if (!logFile) throw std::runtime_error("Nao foi possivel abrir results/atual/operations.csv");
            logFile << "cenario,operacao,chave,esperado,obtido,pass\n";
            suite<AVL>(logFile, "AVL");
            suite<RB>(logFile, "RB");
            for (const auto& valores : std::vector<std::vector<int>>{
                     {30,20,10}, {10,20,30}, {30,10,20}, {10,30,20}}) {
                AVL arvore;
                for (int chave : valores) {
                    arvore.insert(chave);
                    require(arvore.valid(), "rotacao AVL");
                }
                require(arvore.rootKey() == 20, "raiz AVL");
                require(arvore.total_rotacoes == (valores[1] == 20 ? 1 : 2), "contagem AVL");
                std::cout << "PASS AVL ajuste " << valores[0] << ' ' << valores[1] << ' '
                          << valores[2] << " raiz=20 rotacoes=" << arvore.total_rotacoes << '\n';
            }
            std::cout << "ENCERRAMENTO fechando CSV\n";
            logFile.close();
            if (logFile.fail()) throw std::runtime_error("Falha ao fechar operations.csv");
        }
        std::cout << "ENCERRAMENTO CSV fechado e objeto liberado\n";
        std::cout << "TOTAL checks=" << checks << " FALHAS=0\n";
        std::cout << "FIM testes revisao 2\n";
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "FAIL " << erro.what() << '\n';
        return 1;
    }
}
