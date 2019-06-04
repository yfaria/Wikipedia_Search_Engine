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
     Provavelmente tem jeito melhor de fazer isso, mas é o que temos pra hoje.
     Não parece ter solução tão trivial pra trabalhar com caracteres que usam
     mais de um byte e não são UTF8 (a maior parte das soluções simples exigem
     algum recurso externo).
     O mais próximo foi com o uso de wstring e o wfstream; mas deu errado pois,
     por razões desconhecidas, quando ele encara algo fora do alfabeto ascii,
     ele para de ler; empaca. A sugestão mais simples foi trata-los
     com força bruta; que é o feito

    -1 se nao esta no alfabeto:
    'À', 'Á', 'Â', 'Ã', 'Ä', 'Å',  'à', 'á', 'â', 'ã', 'ä', 'å' -> 0; a
    'ç', 'Ç' -> 2; c
    'Ð', 'ð' -> 3; d
    'è', 'é', 'ê', 'ë', 'É', 'È', 'É', 'Ê', 'Ë', 'Æ', 'æ' -> 4; e    
    'ì', 'í', 'î', 'ï', 'Ì', 'Í', 'Î', 'Ï' -> 8; i
    'ñ', 'Ñ' -> 13; n
    'ò', 'ó', 'ô', 'õ', 'ö', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö' -> 14; o
    'š', 'Š' -> 18; s
    'ù', 'ú', 'û', 'ü', 'Ú', 'Ù', 'Û', 'Ü' -> 20; u
    'ý', 'ÿ', 'Ý', 'Ÿ' -> 24;y
    'ž', 'Ž' -> 25;z
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
    else if (x == -59 || x == -61) //o próx símbolo talvez seja lido
      return x;
    else //demais símbolos.
      return -1;
  }
  
  int valor(int x, char c2) {
    /**************************
    Os que vem do ISO 8859-1 são da forma x = -61 e c2 = n;
    Do ISO 8859-15 são x = -59 e c2 = n;
    **************************/
    int y = c2;
    
    if (x == -61) {
      if ((x >= -96 && x <= -91) || (x >= -128 && x <= -123)) //diferentes A's
	return 0;
      else if (x == -89 || x == -121) //ç
	return 2;
      else if (x == -112 || x == -80) //ð
	return 3;
      else if ((x >= -88 && x <= 85) || (x >= -120 && x <= -117)|| //diferentes E's
	       (x == -90) || (x == -122)) //æ e Æ
	return 4;
      else if ((x >= -84 && x <= -81) || (x >= -116 && x <= -113)) //diferentes I's
	return 8;
      else if (x == -79 || x == -111) //ñ
	return 13;
      else if ((x >= -78 && x <= -74) || (x >= -110 && x <= -106)) //diferentes O's
	return 14;
      else if ((x >= -71  && x <= -68) || (x >= -103 && x <= -100)) //diferentes U's
	return 20;
      else if ((x == -67) || (x == -65) || (x == -99) || (x==-72)) //diferentes Y's
	return 23;
      else // demais símbolos
	return -1;
    } else { //x = -59 ou qualquer coisa fora que apareça ao acaso
      if (x == -95 || x == -96) //š
	return 18;
      else if (x == -66 || x == -67) //ž
	return 24;
      else if (x == -72) //Ÿ
	return 23;
      else
	return -1;
    }  
  }
    
public:
  
  void addword(std::string word, int index) {
    Node* pNode;
    pNode = pRoot;
    int x, y;
    for (std::string::iterator it = word.begin(); it != word.end(); ++it) {
      //se o no da i-esima letra nao existe, cria
      x = valor(*it);
      if (x >= 0) {
	if (!(pNode->pChilds[x]))
	  pNode->pChilds[x] = new Node();
	pNode = pNode->pChilds[x];
      }
      else if (x != -1) {
	++it;
	y = valor(x, *it);
	if (!(pNode->pChilds[y]))
	  pNode->pChilds[y] = new Node();
	pNode = pNode->pChilds[y];
      }	
    }
    //ao fim do for; pNode sera ponteiro ao no
    //da palavra a ser adicionada.
    
    //Antes de colocar o índice, precisamos verificar
    //se o index já está no nó ou se o nó é o root.
    if (!(pNode->indexes).empty() || pNode == pRoot)
      if ((pNode->indexes).back() == index || pNode == pRoot)
	return;
    (pNode->indexes).push_back(index);
  }
    
  std::vector<int> query (std::string word) {
    //A query é bem parecida com a inserção, só que em vez de criar um novo nó
    //quando ele não é achado; é para retornar um vetor vazio.
    Node* pNode = pRoot;
    int x, y;
    for (std::string::iterator it = word.begin(); it != word.end(); ++it) {
      x = valor(*it);
      if (x >= 0) {
	if (!(pNode->pChilds[x])) {
	  std::cout << "Não há essa palavra nos artigos \n";
	  std::vector<int> z;
	  return z;
	}
	pNode = pNode->pChilds[x];
      } else if (x != -1) {
	++it;
	y = valor(x, *it); // só pode ser >=0 ou -1
	if (y >= 0) {
	  if (!(pNode->pChilds[y])) {
	    std::cout << "Não há essa palavra nos artigos \n";
	    std::vector<int> z;
	    return z;
	  }
	  pNode = pNode->pChilds[y]; 
	} else {
	  std::cout << "Não há essa palavra nos artigos \n";
	  std::vector<int> z;
	  return z;
	}
      } else { //x == -1
	std::cout << "Não há essa palavra nos artigos \n";
	  std::vector<int> z;
	  return z;
      }
    }
    return pNode->indexes;
  }
  
}; 
