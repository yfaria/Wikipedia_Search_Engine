#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <chrono>
#include "trie.hpp"

int main () {
  std::ios::sync_with_stdio(false);
  std::cout << "Carregando ... ";
  Trie trie;
  std::fstream serial ("serial_trie");
  trie.deserialize(serial);
  serial.close();
  std::cout << "pronto\n\n";
  
  while (true) {
    std::string s;
    std::cout << "O que deseja buscar?\n";
    getline(std::cin, s); 
    std::vector<int> pags;
    auto t0 = std::chrono::high_resolution_clock::now();
    pags = trie.query(s);
    std::chrono::duration<double> dt = std::chrono::high_resolution_clock::now() - t0;
    std::cout << "... Um total de " << pags.size() << " páginas (cerca de "
	      << dt.count() << " segundos)\n\n";
    //pags contém os índices das páginas que tem a busca.
    std::string user_r;
    int j = 0;
    
    while (user_r != "n") {
      
      for (int i = 0; i < 20; i++){
	if (i + 20*j < pags.size())
	  std::cout << "[" << i+1 << "] " << getTitle(pags[i + 20*j]) << "\n";
      }
      
      std::cout << "Deseja abrir algum artigo ?\n"
		<< "[número do artigo na tela para abrir; 'm' para abrir outros 20"
		<< " resultados, 'v' para voltar para os 20 resultados anteriores"
		<< " e 'n' para fazer uma nova busca]\n";
      getline(std::cin, user_r);
      if (user_r == "m" || user_r == "M") {
	j++;
      } else if (user_r == "v" || user_r == "V") {
	if (j>0)
	  j--;
      } else {
	int ent = 0;
	for (std::string::iterator it = user_r.begin(); it != user_r.end(); it++) {
	  if (*it < '0' || *it > '9') { //se a entrada não for opção acima
	    user_r = "n"; //e não for número
	    break; //refaz a busca.
	  }
	  ent = (*it - '0') + 10*ent;
	}
	  if (ent > 20 || ent < 0) {
	    user_r = "n";
	  } else {
	    trie.display(pags[ent - 1 + 20*j]);
	    user_r = "n";
	  }
      }
    }
  }
}
