//A ideia é o no ter um array aos vizinhos e
//um conjunto com os db_index dos elementos 
#include <iostream>
#define ALPHABET_SIZE

class Node {
public:
  Node* pChilds[26];
  std::vector indexes;
}


  class Trie {
  protected:
    Node* pRoot;

    int valor(char c) {
      if ((int c) >= 97)
	return (int c) - 97;
      else //letra maisucula, de 65 a 90
	return (int c) - 65;
    }
    
  public:
    void addword(char* palavra, int len, int index) {
      Node* pNode;
      pNode = pRoot;
      for (int i = 0; i < len; i++){
	//se o nó da i-ésima letra não existe, cria
	if !(pNode->pChilds[valor(*(palavra + i))])
	  pNode->pChilds[valor(*(palavra + i))] = new Node();
	pNode = pNode->pChilds[valor(*(palavra + i))];
      }
    }
    
    //Node** find1(char* palavra, int len,){}

    bool find2(char* palavra, int len){
      Node* pNode;
      pNode = pRoot;
      for (int i = 0; i<len; i++){
	if !(pNode->pChilds[valor(*(palavra + i))])
	  return false;
	else
	  pNode = pNode->pChilds[valor(*(palavra + i))]
      }
      return true;
    }
    
    void query() {

    }
  }
