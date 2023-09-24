#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>

// não to usando "using namespace std" porque quando eu estava estudando c++ 
// eu vi alguem falar que era melhor não usar pra sempre saber de que biblioteca
// estão vindo as coisas e eu concordei 
class writeToFile{
  public:
  std::fstream dbFile;

  void wToFile(std::map<int, std::string> db){
    dbFile.open("dbLog.txt", std::ios::out);
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
  // getUserInput input;
  writeToFile write;
  std::map<int, std::string> db; 

  void dbInsert(int userKey, std::string userValue){
    // int userKey = input.getKey();
    // std::string userValue = input.getValue();

    db.insert(std::pair<int, std::string>(userKey, userValue));
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl;
    write.wToFile(db);
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
      write.clearFile();
      write.wToFile(db);
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
        write.clearFile();
        write.wToFile(db);
      }
      i++;
    }
  }
};

int main(int argc, char** argv)
{ 
  // getUserInput input;
  simpledb datab;
  debugTools debug;
  writeToFile write;

  if(std::strcmp(argv[1], "simpledb") == 0){
    //insert
        if(std::strcmp(argv[2], "insert") == 0){
          datab.dbInsert(std::atoi(argv[3]), argv[4]);
        }
    //remove
        else if(std::strcmp(argv[2], "remove") == 0){
          datab.dbRemove(argv[3]);
        }
    //search
        else if(std::strcmp(argv[2], "search") == 0){
          datab.dbSearch(argv[3]);
        }
    //update  
        else if(std::strcmp(argv[2], "update") == 0){
          datab.dbUpdate(argv[3], argv[4]);
          debug.displayDB(datab.db);
        }
    //quit
        else if(std::strcmp(argv[2], "quit") == 0){
              exit(0);
        }
    }
          return 0;

  }
    

  




