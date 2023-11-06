#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <mqueue.h>
#include <cerrno>

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
  try{
    for (auto pair : db){
      std::cout << pair.first << " " << pair.second << std::endl; 
    }
    void* checkDb = &db;
    if(checkDb == NULL){
      throw 104;
    }
  }
  catch(int exCode){
    std::cout << "Error: " << exCode << "A database não foi encontrada";
  }
  }
};

class simpledb{
  public:
  writeToFile write; 
  std::map<int, std::string> db; 

  void dbInsert(int userKey, std::string userValue){ //inserir no banco de dados
  try{
    db.insert(std::pair<int, std::string>(userKey, userValue)); 
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl; 
    write.wToFile(db);
    void* checkDb = &db;
    if(checkDb == NULL){
      throw 104;
    }
  }
  catch(int exCode){
    std::cout << "Error: " << exCode << "A database não foi encontrada";
  }
  }

  bool dbSearch(std::string userValue){ //pesquisa no banco de dados um valor 
  try{
    for(auto i = db.begin(); i != db.end();){ 
      if(i -> second == userValue){ 
        std::cout << "encontrado"; 
        return true;
      }
      i++;
    }
     void* checkDb = &db;
     if(checkDb == NULL){
      throw 104;
    }
  }
  catch(int exCode){
    std::cout << "Error: " << exCode << "A database não foi encontrada";
  }
    std::cout << "não encontrado"; 
    return false;
  }
  void dbRemove(std::string userValue){ //remove do banco de dados o valor apontado pelo usuario
  try{
    for(auto i = db.begin(); i != db.end();){ 
      if(dbSearch(userValue) == true){ 
      std::cout << "o valor removido foi" << userValue;
      i = db.erase(i);
      write.clearFile();
      write.wToFile(db);
      }
      i++; 
    }
     void* checkDb = &db;
     if(checkDb == NULL){
      throw 104;
    }
  }
  catch(int exCode){
    std::cout << "Error: " << exCode << "A database não foi encontrada";
  }

  }

  void dbUpdate(std::string valueToRemove, std::string valueToInsert){ //substitui um valor existente no db por um novo valor
  try{
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
    void* checkDb = &db;
     if(checkDb == NULL){
      throw 104;
    }
  }
  catch(int exCode){
    std::cout << "Error: " << exCode << "A database não foi encontrada";
  }
  }
};

class thread{
  //run thread according to the received message, must be a switch
  //send a message back telling the output/result
};

struct mq_attr mq_attributes;



int main(int argc, char** argv)
{ 
  mq_attributes.mq_flags = 0;       
  mq_attributes.mq_maxmsg = 10;      
  mq_attributes.mq_msgsize = 32;  

  mqd_t mqueue;


  mqueue = mq_open("/mqueue", O_CREAT | O_RDWR, 0666, mq_attributes);
  if (mqueue == (mqd_t)-1) {
        perror("");
        return 1;  // Handle the error, don't continue with a bad file descriptor
    }


  char m_received[32];
  const char* i_received = "insert";
  unsigned int m_priority = 0;


  while(true){    
    if(ssize_t size_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority) == -1){
    perror("");
    }
  
    if(strcmp(m_received, i_received) == true){
      perror("");
      std::cout << "received message" << m_received;
    }
    //receive message
    //call thread
  }
  return 0;

}
    

  




