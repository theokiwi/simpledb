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

class simpledb{
  public:
  getUserInput input;
  std::map<std::string, std::string> db; 

  void dbInsert(){
    std::string userValue = input.getValue();
    std::string userKey = input.getKey();

    db.insert(std::pair<std::string, std::string>(userKey, userValue));
    
    for(auto pair : db){
      std::cout << "a chave é " <<pair.first << " o valor é " << pair.second;
    }
  }
};

int main()
{
  getUserInput input;
  simpledb db;

  db.dbInsert();
  return 0;
}




