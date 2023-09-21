#include <iostream>
#include <map>
#include <string>

class getUserInput {
  public:

  std::string getKey(){
    std::string userKey;
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

  void displayDB(std::map<std::string, std::string> db){
    for (auto pair : db){
      std::cout << pair.first << " " << pair.second << std::endl;
    }
  }
};

class simpledb{
  public:
  getUserInput input;
  std::map<std::string, std::string> db; 

  void dbInsert(){
    std::string userKey = input.getKey();
    std::string userValue = input.getValue();

    db.insert(std::pair<std::string, std::string>(userKey, userValue));
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl;
  }

  bool dbSearch(std::map<std::string, std::string> db, std::string userValue){
    std::string value = userValue;
    std::string* ptrValue;
    for(int i = 0; i < sizeof(db)/sizeof(std::string); i++){
      //loop pelo dicionario e comparar valor, retornar true se achar;
    }
  }

  void dbRemove(std::map<std::string, std::string> db, std::string userValue){
  if(dbSearch(db, userValue)){
    std::string userKey = input.getKey();
    db.erase((userKey));
    std::cout << "a chave removida foi " << userKey;
  }

};

int main()
{
  getUserInput input;
  simpledb db;
  debugTools debug;

  db.dbInsert();
  debug.displayDB(db.db);
  return 0;
}



};
