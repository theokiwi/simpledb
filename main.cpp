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
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl;
  }
};

int main()
{
  getUserInput input;
  simpledb db;

  db.dbInsert();
  return 0;
}




