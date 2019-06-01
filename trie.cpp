//A ideia eh o no ter um array aos vizinhos e
//um conjunto com os db_index dos elementos 
#include <iostream>
#include <vector>
#include <fstream>

class Node {
public:
  Node* pChilds[36]; //testar com 26.
  std::vector<int> indexes; //talvez substituir por linked list.
  Node() {
    for (int i=0; i<36; i++)
      pChilds[i] = nullptr;
  }
};


class Trie {
protected:
  Node* pRoot;

  int valor(char c) {
    /**************************
    -1 se nao esta no alfabeto:

    'A', 'a', 'À', 'Á', 'Â', 'Ã', 'Ä', 'Å',  'à', 'á', 'â', 'ã', 'ä', 'å' -> 0; a
    'ç', 'Ç', 'c', 'C' -> 2; c
    'd', 'D', 'Ð', 'ð' -> 3; d
    'e', 'è', 'é', 'ê', 'ë', 'E', 'É', 'È', 'É', 'Ê', 'Ë', 'Æ', 'æ' -> 4; e    
    'i', 'ì', 'í', 'î', 'ï', 'I', 'Ì', 'Í', 'Î', 'Ï' -> 8; i
    'n', 'ñ', 'N', 'Ñ' -> 13; n
    'o', 'ò', 'ó', 'ô', 'õ', 'ö', 'O', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö' -> 14; o
    's', 'š', 'S', 'Š' -> 18; s
    'u', 'ù', 'ú', 'û', 'ü', 'U', 'Ú', 'Ù', 'Û', 'Ü' -> 20; u
    'y', 'ý', 'ÿ', 'Y', 'Ý', 'Ÿ' -> 24;y
    'z', 'ž', 'Z', 'Ž' -> 25;z
    Os outros caracteres do alfabeto terão valores normais.
    Os demais especiais serão -1.

    Para informações sobre os ints que aparecem: 
    https://en.wikipedia.org/wiki/ISO/IEC_8859-15.
    **************************/
    int x = c;
    if ((x<=122) && (x >= 97)) // a:97; z:122
      return (x - 97);
    else if ((x <=90) && (x >= 65)) //A:65;  Z:90
      return (x - 65);
    else if ((x <= 57) && (x >= 48)) //dígitos de 0 a 9
      return (x-12);
    else if ((x >= 192 && x <= 197) || (x >= 224 && x <= 229)) //diferentes A's
      return 0;
    else if (x==199 || x==201) //ç
      return 2;
    else if (x==208 || x==240) //ð
      return 3;
    else if ((x >= 200 && x <= 203) || (x >= 232 && x <= 235) ||
	     (x == 198) || (x == 230)) //diferentes E's
      return 4;
    else if ((x >= 204 && x <= 207) || (x >= 236 && x <= 239)) //diferentes I's
      return 8;
    else if (x==209 || x==241) //ñ
      return 13;
    else if ((x >= 210 && x <= 214) || (x >= 242 && x <= 246)) //diferentes O's
      return 14;
    else if (x==352 || x==353) //š
      return 18;
    else if ((x >= 217 && x <= 220) || (x >= 249 && x <= 252)) //diferentes U's
      return 20;
    else if ((x == 221) || (x == 253) || (x == 376) || (x==255)) //diferentes Y's
      return 23;
    else if (x == 381 || x == 382) //ž
      return 24;
    else // demais símbolos
      return -1;
  }
    
public:
    
  void addword(std::wstring word, int index) {
    Node* pNode;
    pNode = pRoot;
    int x;
    for (std::wstring::iterator it = word.begin(); it != word.end(); ++it) {
      //se o no da i-esima letra nao existe, cria
      x = valor(*it);
      if (x != -1) {
	if (!(pNode->pChilds[x]))
	  pNode->pChilds[x] = new Node();
	pNode = pNode->pChilds[x];
      }
      /*
      if (!(pNode->pChilds[valor(*(palavra + i))])) {
	//testar : pNode->pChilds[valor(*(palavra + i))] = new Node();
	Node* pNo = new Node();
	pNode->pChilds[valor(*(palavra + i))] = pNo;
      }
      pNode = pNode->pChilds[valor(*(palavra + i))];
      */
    }

    //ao fim do for; pNode sera ponteiro ao no
    //da palavra a ser adicionada.
    if (!(pNode->indexes).empty())
      if ((pNode->indexes).back() == index)
	return;
    (pNode->indexes).push_back(index);
  }
    
  //Node** find1(char* palavra, int len,){}

  bool find2(char* palavra, int len){
    Node* pNode;
    pNode = pRoot;
    for (int i = 0; i<len; i++) {
      if (!(pNode->pChilds[valor(*(palavra + i))]))
	return false;
      else
	pNodxe = pNode->pChilds[valor(*(palavra + i))];
    }
    return true;
  }
    
  /*void query() {

    }*/
}; 

int main () {
  //Trie teste;
  int index;
  std::string s;
  std::string word;
  std::fstream arquivo ("englishText_0_10000");
  
  //PEGANDO UM ARQUIVO:
  while (getline(arquivo,s)) {
    for (std::string::iterator it = s.begin(); it != s.end(); ++it){
      if (*it == ' ') {
	//adiciono a palavra na trie.
	std::cout << word << '\n';
	word.clear();
      } else {
	//aqui coloca restrição quanto aos caracteres a entrar.
	word += *it;
      }
    }
    //como as linhas terminam sem espaço;
    //refaz o caso do espaço ao final.
    std::cout << word << '\n';
    word.clear();
    std::cout << "\n\n\n PROXIMA LINHA \n\n\n";
  }
  
  std::cout << "iai\n";
  //char* x = "olá";
  return 0;
} 
