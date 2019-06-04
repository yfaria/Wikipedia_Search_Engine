//A ideia eh o no ter um array aos vizinhos e
//um conjunto com os db_index dos elementos 
#include <iostream>
#include <vector>
#include <fstream>
#include "trie.hpp"

int getIndex (std::string tag) {
  //Pega a string tag que é da forma
  //<doc id="x" title="s1" nonfiltered="z" processed="y" dbindex="a" len="w">
  //retorna o a.
  std::string::iterator it = tag.end();
  while (*it != ' ')
    --it;
  --it;
  --it;
  int x = 0;
  int i = 1;
  while (*it != '"'){
    x += (*it - 48) * i; // o 48 é que os dígitos vão de 48 a 57 em ASCII
    i *= 10;
    --it;
  }
  return x;
}

int getLen (std::string tag) {
  //Pega a string tag que é da forma
  //<doc id="x" title="s1" nonfiltered="z" processed="y" dbindex="a" len="w">
  //retorna o w.
  std::string::iterator it = tag.end();
  while (*it != '"')
    --it;
  int x = 0;
  int i = 1;
  while (*it != '"'){
    x += (*it - 48) * i; // o 48 é que os dígitos vão de 48 a 57 em ASCII
    i *= 10;
    --it;
  }
  return x;
}

int main () {
  Trie teste;
  std::fstream names ("files");
  std::fstream file;
  std::string s;
  std::string s1;
  std::string word;
  int index;
  while (std::getline(names, s)) {
    file.open("/home/gambitura/EDA/Wikipedia_Search_Engine/wiki_files/" + s);
    std::cout << "lendo arquivo " << s << '\n'; 
    while (std::getline(file, s1)) {
      if (s1.substr(0,4) == "<doc") {
	index = getIndex(s1);
      } else {
	for (std::string::iterator it = s1.begin(); it != s1.end(); ++it) {
	  if (*it == ' ') {
	    teste.addword(word, index);
	    word.clear();
	  } else {
	    word += *it;
	  }
	}
      }
    }
  }
  
  std::cout << "iai\n";
  return 0;
} 
