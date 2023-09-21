#include <iostream>
#include <map>
#include <string>

class getUserInput {
  public:

  int getKey(){
    int userKey;
    std::cin >> userKey;

    return userKey;
  }

  std::string getValue(){
      std::string userValue;
      std::cin >> userValue;

      return userValue;
  } 
  
};

class debugTools{
  public:

  void displayDB(std::map<int, std::string> db){
    for (auto pair : db){
      std::cout << pair.first << " " << pair.second << std::endl;
    }
  }
};

class simpledb{
  public:
  getUserInput input;
  std::map<int, std::string> db; 

  void dbInsert(){
    int userKey = input.getKey();
    std::string userValue = input.getValue();

    db.insert(std::pair<int, std::string>(userKey, userValue));
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl;
  }

  bool dbSearch(std::string userValue){
     std::string value = userValue;
    for(auto i = db.begin(); i != db.end();){
      if(i -> second == userValue){
        std::cout << "encontrado";
        return true;
      }
      i++;
    }
    std::cout << "não encontrado";
    return false;
  }
//o db remove usou ponteiros porque se eu so removesse o i ele eliminar um item do for
//e ia quebrar o for e me dava um erro falando que eu não tinha permissão
//eu tive que "i = db.erase(I)" ao inves de so db.erase porque o erase retorna pro i um ponteiro
//apontando pro proximo ponto
  void dbRemove(std::string userValue){
   std::string value = userValue;
    for(auto i = db.begin(); i != db.end();){
      if(i -> second == userValue){
      std::cout << "o valor removido foi" << userValue;
      i = db.erase(i);
      }
      i++;
    }
  }
};
class ReceiveCommands{

};

int main()
{ 
  getUserInput input;
  simpledb datab;
  debugTools debug;
  
  datab.dbInsert();
  debug.displayDB(datab.db);
  datab.dbSearch("batata");
  return 0;
}

//TODO LIST
//inserir comandos com switch 

