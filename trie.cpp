#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include "trie.hpp"



int main () {
  //Aqui, a trie é construida e serializada.
  Trie teste;
  std::ifstream names("files");
  std::ifstream file;
  std::string s;
  std::string s1;
  std::string word;
  int index;
  int docs = 0;

  std::cout << "Carregando...";
  auto t0 = std::chrono::high_resolution_clock::now();
  while (getline(names, s)) {
    file.open("/home/gambitura/EDA/Wikipedia_Search_Engine/wiki_files/" + s +"(conv)");
    //std::cout << "lendo arquivo " << s << "\n";
    while (getline(file, s1)) {
      if (s1.substr(0,4) == "<doc") {
	docs++;
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
  std::chrono::duration<double> dt = std::chrono::high_resolution_clock::now() - t0;
  std::cout << "pronto! \n";
  std::cout << "Foram indexados " << docs << " artigos em " << dt.count() <<" segundos\n\n";
  std::cout << "Agora, serializando ...\n";
  t0 = std::chrono::high_resolution_clock::now();
  std::fstream file2;
  file2.open("serial_trie");
  teste.serialize(file);
  dt = std::chrono::high_resolution_clock::now() - t0;
  std::cout << "Isso durou " << dt.count() << " segundos.\n";
  return 0;
} 
