
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

class writeToFile {
public:
  std::fstream dbFile;

  void wToFile(std::map<int, std::string> db) {  // escreve os pares no arquivo
      dbFile.open("dbLog.txt", std::ios::out);
      if (dbFile.is_open()){
        for (auto pair : db) {
          dbFile << pair.first << " " << pair.second << std::endl;
        }
      } else {
        std::cout <<"O arquivo de escrita não pode ser aberto";
      }
    dbFile.close();
  }

  void clearFile() { // responsável por limpar o arquivo
      dbFile.open("dbLog.txt", std::ios::out);
      if (dbFile.is_open()) {
        dbFile.clear();
      } else {
        std::cout << "O arquivo de escrita não pode ser aberto";
      }
    dbFile.close();
  }
};

class debugTools {
public:
  void displayDB(std::map<int, std::string> db) { // ferramenta de debug imprime os pares no console
      for (auto pair : db) {
        std::cout << pair.first << " " << pair.second << std::endl;
      }
      if (db.empty()) {
        std::cout << ("A database não foi encontrada");
      }
  }
};

class simpledb {
public:
  writeToFile write;
  std::map<int, std::string> db;

  void dbInsert(int userKey, std::string userValue) { // inserir no banco de dados
      db.insert(std::pair<int, std::string>(userKey, userValue));
      std::cout << "A chave é " << userKey << " o valor é " << userValue << std::endl;
      write.wToFile(db);
      if (db.empty()) {
        std::cout << ("A database não foi encontrada");
      }
    } 
  

  bool dbSearch(std::string userValue){ // pesquisa no banco de dados um valor
      for (auto i = db.begin(); i != db.end();){
        if (i->second == userValue) {
          std::cout << "Encontrado";
          return true;
        }
        i++;
      }
      if (db.empty()) {
        std::cout << ("A database não foi encontrada");
      }
    std::cout << "Não encontrado";
    return false;
  }
  

  void dbRemove(std::string userValue) { // remove do banco de dados o valor apontado pelo usuário
      for (auto i = db.begin(); i != db.end();) {
        if (dbSearch(userValue)) {
          std::cout << "O valor removido foi " << userValue;
          i = db.erase(i);
          write.clearFile();
          write.wToFile(db);
        }
        i++;
      }
      if (db.empty()) {
        std::cout << ("A database não foi encontrada");
      }
  }

void dbUpdate(std::string valueToRemove, std::string valueToInsert) { // substitui um valor existente no db por um novo valor
      for (auto i = db.begin(); i != db.end();) {
        if (i->second == valueToRemove) {
          const int whereToInsert = i->first;
          std::cout << "O valor removido foi " << valueToRemove << std::endl;
          i = db.erase(i);
          db.insert(std::pair<int, std::string>(whereToInsert, valueToInsert));
          write.clearFile();
          write.wToFile(db);
        }
        i++;
      }
      if (db.empty()) {
        std::cout << "A database não foi encontrada";
      }
}
};
class MyThread {
  // Implement the thread functionality here
};

struct mq_attr mq_attributes;

int main(int argc, char** argv) {

  std::cout << "Main iniciada \n";
  mq_attributes.mq_flags = 0;
  mq_attributes.mq_maxmsg = 10;
  mq_attributes.mq_msgsize = 32;

  mqd_t mqueue;

  mqueue = mq_open("/mqueue", O_CREAT | O_RDWR, 0666, &mq_attributes);
  if (mqueue == (mqd_t)-1) {
     perror("");
     std::cout << "Queue não aberta \n";
     return 1;
  }

  std::cout << "Mqueue aberta \n";

  char m_received[32];
  unsigned int m_priority = 0;

  //grupos de possiveis mensagens a serem recebidas:

  const char* i_received = "insert";
  const char* r_received = "remove";
  const char* s_received = "search";
  const char* u_received = "update";
  const char* q_received = "update";
  

  std::cout << "Propriedades definidas \n";

  while (true) {

  ssize_t size_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
  
    if (size_receive == -1) {
       perror("");
       std::cout << "Mensagem não recebida";
    }
    else if(strcmp(m_received, i_received) == 0){
      std::cout << "Mensagem " << m_received << " recebida \n";
    }
    else if(strcmp(m_received, r_received) == 0){
      std::cout << "Mensagem " << m_received << " recebida \n";
    }
    else if(strcmp(m_received, s_received) == 0){
      std::cout << "Mensagem " << m_received << " recebida \n";
    }
    else if(strcmp(m_received, u_received) == 0){
      std::cout << "Mensagem " << m_received << " recebida \n";
    }
    else if(strcmp(m_received, q_received) == 0){
      std::cout << "Mensagem " << m_received << " recebida \n";
    }
    
  

  }
  return 0;
}
