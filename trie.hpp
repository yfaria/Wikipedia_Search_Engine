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

std::string getTitle (int index) {
  //Dado um índice, acha o título do documento com esse título.
  //Faz a busca no arquivo dele.
  std::fstream file;
  std::stringstream ss;
  ss << "wiki_files/englishText_";
  ss << index - (index%10000) << "_" << index - (index%10000) + 10000  << "(conv)";
  file.open(ss.str());
  //std::cout << ss.str() << "\n";
  std::string s;
  int ind;
  std::string title;
  while (getline(file,s)) { //na condicão do while
    //std::cout << s << "\n";
    ind = getIndex(s); //s sempre estará numa tag.
    int linhas = getLen(s);
    if (ind != index) {
      for (int i = 0; i < linhas; i++)
	getline(file,s); 
    } else { // Achou a tag, que está armazenada em s.
      std::string::iterator it = s.begin();
      while (*it != ' ') //precisa andar dois espaços para chegar em title.
	it++; // O primeiro separa doc de id.
      it++;
      while (*it != ' ') // o segundo id de title.
	it++;
      std::advance(it, 8); // para it apontar para primeira letra do titulo.
      while (*it != '"') {
	title.push_back(*it);
	it++;
      }
      return title;
    }
  }
  return "";
}
  

class Node {
public:
  Node* pChilds[36];
  std::vector<int> indexes;
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
      return (x-22);
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
      if ((y >= -96 && y <= -91) || (y >= -128 && y <= -123)) //diferentes A's
	return 0;
      else if (y == -89 || y == -121) //ç
	return 2;
      else if (y == -112 || y == -80) //ð
	return 3;
      else if ((y >= -88 && y <= -85) || (y >= -120 && y <= -117)|| //diferentes E's
	       (y == -90) || (y == -122)) //æ e Æ
	return 4;
      else if ((y >= -84 && y <= -81) || (y >= -116 && y <= -113)) //diferentes I's
	return 8;
      else if (y == -79 || y == -111) //ñ
	return 13;
      else if ((y >= -78 && y <= -74) || (y >= -110 && y <= -106)||
	       (y==-72) || (y == -104))//diferentes O's
	return 14;
      else if ((y >= -71  && y <= -68) || (y >= -103 && y <= -100)) //diferentes U's
	return 20;
      else if ((y == -67) || (y == -65) || (y == -99) || (y==-72)) //diferentes Y's
	return 24;
      else // demais símbolos
	return -1;
    } else { //x = -59 ou qualquer coisa fora que apareça ao acaso
      if (y == -95 || y == -96) //š
	return 18;
      else if (y == -66 || y == -67) //ž
	return 25;
      else if (y == -72) //Ÿ
	return 24;
      else if (y == 108 || y==109 || y==107 || y==115 || y == 44 || y==0) //Å
	return 0;
      else if (y == 102) //Ø
	return 14;
      else
	return -1;
    }  
  }
    
public:
  Trie () {
    pRoot = new Node();
  }


  std::vector<int> intersection(std::vector<int> docs1, std::vector<int> docs2) {
    //Nossa árvore foi constuída de modo que indices dos vetores estejam
    //ordenados; requisito desse algoritmo.
    std::vector<int>::iterator it1 = docs1.begin();
    std::vector<int>::iterator it2 = docs2.begin();
    std::vector<int> vec;
    while (it1 != docs1.end() && it2 != docs2.end()) {
      if (*it1 > *it2) {
	it2++;
      } else if (*it2 > *it1) {
	it1++;
      } else {
	vec.push_back(*it1);
	it1++;
	it2++;
      }
    }
    return vec;
  }
  
  void addword(std::string word, int index) {
    Node* pNode;
    pNode = pRoot;
    int x;
    for (std::string::iterator it = word.begin(); it != word.end(); ++it) {
       //se o no da i-esima letra nao existe, cria
      x = valor(*it);
      if (x >= 0) {
	if (pNode->pChilds[x] == nullptr)
	  pNode->pChilds[x] = new Node();
	pNode = pNode->pChilds[x];
      } else if (x != -1) { 
	//std::cout << "simbolo\n";
	//std::cout << word << '\n';
	++it;
	int y = *it;
	//std::cout << x << " e " << y << "\n\n\n";
	y = valor(x, *it);
	//std::cout << y << "\n\n\n\n\n\n";
	if (y > 0) {
	  if (pNode->pChilds[y] == nullptr)
	    pNode->pChilds[y] = new Node();
	  pNode = pNode->pChilds[y];
	}
      }	
    }
    //ao fim do for; pNode sera ponteiro ao no
    //da palavra a ser adicionada.
    
    //Antes de colocar o índice, precisamos verificar
    //se o index já está no nó ou se o nó é o root.
    if (pNode == pRoot)
      return;
    
    if (!(pNode->indexes).empty())
      if ((pNode->indexes).back() == index){
	//std::cout << word << " já colocado\n";
	return;
      }

    (pNode->indexes).push_back(index);
    (pNode->indexes).reserve((pNode->indexes).size());
  }
    
  std::vector<int> query_oneword (std::string word) {
    //A query é bem parecida com a inserção, só que em vez de criar um novo nó
    //quando ele não é achado; é para retornar um vetor vazio.

    //Esse query só serve para uma palavra 
    Node* pNode = pRoot;
    int x;
    for (std::string::iterator it = word.begin(); it != word.end(); ++it) {
      x = valor(*it);
      if (x >= 0) {
	if (!(pNode->pChilds[x])) {
	  std::cout << "Não encontrado nos artigos \n";
	  std::vector<int> z;
	  return z;
	}
	pNode = pNode->pChilds[x];
      } else if (x != -1) {
	int y;
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

  std::vector<int> query (std::string s) {
    std::string word;
    std::vector<int> vec;
    bool multi_w = false;
    for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
      if (*it == ' ' ||*it == '-' ||*it == '.'||*it == ',' || *it == ';'|| *it == ':'||*it == '?'|| *it == '!') {
	multi_w = true; //se a string tem várias palavras;
	if (vec.empty())//achamos os índices das palavras
	  vec = query_oneword(word); // e fazemos as interseções
	else
	  vec = intersection(vec, query_oneword(word));
	word.clear();
      } else {
	word += *it;
      }
    }
    if (!(multi_w)) //se iterou em toda a string e era somente uma palavra
      vec = query_oneword(word); //busca essa palavra.
    else
      vec = intersection(vec, query_oneword(word));
    return vec;
  }

  void serial_rec (Node* pNode, std::fstream& file , char fonte) {
    if (pNode) {
      file << fonte << "(";
      for (std::vector<int>::iterator it = pNode->indexes.begin(); it != pNode->indexes.end(); it++) 
	file << *it << ",";
      file << ")";
      for (int i = 0; i < 36; i++){
	//char c = "abcdefghijklmnopqrstuvwxyz0123456789"[i];
	serial_rec(pNode->pChilds[i], file, "abcdefghijklmnopqrstuvwxyz0123456789"[i]);
      }
      file << "$";
    }
  }
  
  void serialize (std::fstream& file) {
    Node* pNode;
    for (int i = 0; i < 36; i++){
      pNode = pRoot->pChilds[i];
      char c = "abcdefghijklmnopqrstuvwxyz0123456789"[i];
      serial_rec(pNode, file, c);
    }
  }

  void deserialize (std::fstream& file) {
    //Nossa serialização retorna um arquivo de uma linha só.
    Node* pNode;
    std::string arq;
    std::string word;
    //int x = 0;
    int index = 0;
    getline(file, arq);
    for (std::string::iterator it = arq.begin(); it != arq.end(); it++) {
      if (*it != '(' && *it != '$') {
	word += *it;
      } else if (*it == '(') {
	it++;
	while (*it != ')') {
	  if (*it != ',') {
	    index = (*it - 48) + 10*index;
	  } else { //chega na vírgula.
	    addword(word, index);
	    index = 0;
	  }
	  it++;
	}
      } else { //*it == '$'
	word.pop_back();
      }
    }
  }

  void display (int index) {
    std::fstream file;
    std::stringstream ss;
    ss << "wiki_files/englishText_";
    ss << index - (index%10000) << "_" << index - (index%10000) + 10000  << "(conv)";
    file.open(ss.str());
    //std::cout << ss.str() << "\n";
    std::string s;
    int ind;
    while (getline(file,s)) { //na condicão do while
      ind = getIndex(s); //s sempre estará numa tag.
      int linhas = getLen(s);
      if (ind != index) {
	for (int i = 0; i < linhas; i++) //ao final desse for
	  getline(file,s); //o getline vai estar no </doc>
      } else { //aí o while volta pra uma tag doc.
	for (int i = 0; i < (linhas-2); i++) { //Como as duas ultimas linhas
	  getline(file,s); //são ENDOFARTICLE e </doc>, não precisa ler.
	  std::cout << s << "\n";
	}
	file.close();
	return;
      }
    }
  }
}; 
