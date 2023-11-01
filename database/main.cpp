#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <thread>

// lançar exceção com comando
class writeToFile{
  public:
  std::fstream dbFile;

  void wToFile(std::map<int, std::string> db){  //escreve os pares no arquivo
  try{
    dbFile.open("dbLog.txt", std::ios::out); 
    if(dbFile.is_open()){
      for (auto pair : db){
      dbFile << pair.first << " " << pair.second << std::endl; 
      }
    }
    else{
      throw 101;
    }
  }
  catch (int exCode){
    std::cout << "Error: " << exCode << "O arquivo de escrita não pode ser aberto";
  }
    dbFile.close(); 
  }

  void clearFile(){ //responsavel por limpar o arquivo 
  try{
    dbFile.open("dbLog.txt", std::ios::out); 
      if(dbFile.is_open()){
        dbFile.clear();
      }
      else{
        throw 101;
      }
  }
  catch (int exCode){
        std::cout << "Error: " << exCode << "O arquivo de escrita não pode ser aberto";
  }
    dbFile.close(); 

    }
 };

class debugTools{
  public:

  void displayDB(std::map<int, std::string> db){ //ferramente de debug printa os pares no console
    for (auto pair : db){
      std::cout << pair.first << " " << pair.second << std::endl; 
    }
  }
};

class simpledb{
  public:
  writeToFile write; 
  std::map<int, std::string> db; 

  void dbInsert(int userKey, std::string userValue){ //inserir no banco de dados
    db.insert(std::pair<int, std::string>(userKey, userValue)); 
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl; 
    write.wToFile(db);
  }

  bool dbSearch(std::string userValue){ //pesquisa no banco de dados um valor 
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
  void dbRemove(std::string userValue){ //remove do banco de dados o valor apontado pelo usuario
    for(auto i = db.begin(); i != db.end();){ 
      if(dbSearch(userValue) == true){ 
      std::cout << "o valor removido foi" << userValue;
      i = db.erase(i);
      write.clearFile();
      write.wToFile(db);
      }
      i++; 
    }
  }

  void dbUpdate(std::string valueToRemove, std::string valueToInsert){ //substitui um valor existente no db por um novo valor
    for(auto i = db.begin(); i != db.end();){ 
      if(i -> second == valueToRemove){ 
        const int* whereToInsert = &(i -> first); 
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
  simpledb datab;
  debugTools debug;
  writeToFile write;

  if(std::strcmp(argv[1], "simpledb") == 0){
    //insert
        if(std::strcmp(argv[2], "insert") == 0){
          try{
            datab.dbInsert(std::atoi(argv[3]), argv[4]);
          }
          catch (int exCode){
            std::cout << "O log não pode ser inicializado, favor inserir a função novamente";
          }
        }
    //remove
        else if(std::strcmp(argv[2], "remove") == 0){
          try{
              datab.dbRemove(argv[3]);
          }
          catch(int exCode){
              std::cout << "O log não pode ser inicializado, favor inserir a função novamente";
          }
        }
    //search
        else if(std::strcmp(argv[2], "search") == 0){
          datab.dbSearch(argv[3]);
        }
    //update  
        else if(std::strcmp(argv[2], "update") == 0){
          try{
              datab.dbUpdate(argv[3], argv[4]);
              debug.displayDB(datab.db);
          }
          catch(int exCode){
              std::cout << "Error: " << exCode << " O log não pode ser inicializado, favor inserir a função novamente";
          }
          
        }
    //quit
        else if(std::strcmp(argv[2], "quit") == 0){
              exit(0);
        }
        else{
          std::cout<<"resposta invalida";
        }     
    }
    else if(std::strcmp(argv[1], "simpledb") != 0){
      try{
        throw 102;
      }
      catch(int exCode){
        std::cout << "Error: " << exCode << " Comandos devem ter simpledb como primeiro argumento";
      }
    }
          return 0;

  }
    

  




