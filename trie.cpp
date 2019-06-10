//A ideia eh o no ter um array aos vizinhos e
//um conjunto com os db_index dos elementos 
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "trie.hpp"



int main () {
  Trie teste;
  std::ifstream names("files");
  std::ifstream file;
  std::string s;
  std::string s1;
  std::string word;
  int index;

  std::cout << "Carregando...";
  
  while (getline(names, s)) {
    file.open("/home/gambitura/EDA/Wikipedia_Search_Engine/wiki_files/" + s +"(conv)");
    //std::cout << "lendo arquivo " << s << "\n";
    while (getline(file, s1)) {
      if (s1.substr(0,4) == "<doc") {
	index = getIndex(s1);
      } else {
	if (s1.substr(0,5) != "</doc" && s1.substr(0,5) != "ENDOF"){
	  for (std::string::iterator it = s1.begin(); it != s1.end(); ++it) {
	    if (*it == ' ' ||*it == '-' ||*it == '.'||*it == ',' || *it == ';'|| *it == ':'||*it == '?'|| *it == '!') {
	      //std::cout << word << "\n";
	      teste.addword(word, index);
	      word.clear();
	    } else {
	      word += *it;
	    }
	  }
	}
      }
    }
    file.close();
  }
  word.clear();
  std::cout << "pronto! \n";
  std::cout << "O que deseja procurar? (depois, vou tentar serializar)\n";
  getline(std::cin, word);
  std::vector<int> vec;
  vec = teste.query(word);
  std::cout << "mo " << word << "\n\n";
  for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    std::cout << *it << " ";
  std::cout << "\n\n";
  //std::fstream file2;
  //file2.open("serial_test");
  //teste.serialize(file2);
  
  return 0;
} 
