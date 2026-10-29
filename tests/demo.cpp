#include <iostream>
#include "test_support.hpp"
template<class T>void run(const char* name){T t;std::cout<<"=== "<<name<<" ===\n";for(int k:{30,20,10,5}){std::cout<<"inserir "<<k<<" -> "<<t.insert(k)<<'\n';t.exibir();std::cout<<"valida="<<t.valid()<<"\n";}
 std::cout<<"buscar 20 -> "<<t.contains(20)<<"\nbuscar 99 -> "<<t.contains(99)<<"\nremover 20 -> "<<t.erase(20)<<'\n';t.exibir();std::cout<<"em ordem: ";for(int k:t.inorder())std::cout<<k<<' ';std::cout<<"\nvalida="<<t.valid()<<"\n";}
int main(){run<AVL>("AVL");run<RB>("RB");}
