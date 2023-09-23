#include <iostream>
#include <map>
#include <string>
#include <fstream>

// não to usando "using namespace std" porque quando eu estava estudando c++ 
// eu vi alguem falar que era melhor não usar pra sempre saber de que biblioteca
// estão vindo as coisas e eu concordei 
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
    for(auto i = db.begin(); i != db.end();){
      if(i -> second == userValue){
      std::cout << "o valor removido foi" << userValue;
      i = db.erase(i);
      }
      i++;
    }
  }

  void dbUpdate(std::string valueToRemove, std::string valueToInsert){
    for(auto i = db.begin(); i != db.end();){
      if(i -> second == valueToRemove){
        const int* whereToInsert = &(i -> first); 
        //tive que apontar pro ponteiro que aponta pro valor
        //coloquei como constante porque o console pediu ? 
        std::cout << "o valor removido foi" << valueToRemove << std::endl;
        i = db.erase(i);
        db.insert(std::pair<int, std::string>(*whereToInsert, valueToInsert));
      }
      i++;
    }
  }
};

class writeToFile{
  public:
  std::fstream dbFile;

  void wToFile(std::map<int, std::string> db){
    dbFile.open("dbLog.txt"), std::ios::out;
    if(dbFile.is_open()){
      for (auto pair : db){
      dbFile << pair.first << " " << pair.second << std::endl;
      }
    }
    dbFile.close();
  }

  void clearFile(){
    dbFile.open("dbLog.txt", std::ios::out);
      if(dbFile.is_open()){
        dbFile.clear();
      }
    dbFile.close();

    }
  };
class ReceiveCommands{

};

int main()
{ 
  getUserInput input;
  simpledb datab;
  debugTools debug;
  writeToFile write;
  
  datab.dbInsert();
  write.wToFile(datab.db);
  write.clearFile();
  return 0;
}

//TODO LIST
//Escrita em arquivo
//inserir comandos com switch 

