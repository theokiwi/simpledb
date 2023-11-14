
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <mqueue.h>
#include <cerrno>
#include <semaphore>

class writeToFile {
public:
  std::fstream dbFile;

  void wToFile(std::map<std::string, std::string> db) {  // escreve os pares no arquivo
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
  void displayDB(std::map<std::string, std::string> db) { // ferramenta de debug imprime os pares no console
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
  std::map<std::string, std::string> db;

  void dbInsert(std::string userKey, std::string userValue) { // inserir no banco de dados
      db.insert(std::pair<std::string, std::string>(userKey, userValue));
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
          const std::string whereToInsert = i->first;
          std::cout << "O valor removido foi " << valueToRemove << std::endl;
          i = db.erase(i);
          db.insert(std::pair<std::string, std::string>(whereToInsert, valueToInsert));
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

struct mq_attr mq_attributes; //struct que guarda as propriedades da mensagem

struct mq_attr mq_r_attributes;

int main(int argc, char** argv) {
  simpledb sdb;

  //criação das variaveis da mqueue
  mqd_t mqueue;
  mqd_t mqueue_response;

  
  std::cout << "Main iniciada \n";

  //definição das propriedades da struct que recebe resposta
  mq_attributes.mq_flags = 0;
  mq_attributes.mq_maxmsg = 10;
  mq_attributes.mq_msgsize = 1024;

  //definição das propriedades da struct que envia confirmação
  mq_r_attributes.mq_flags = 0;
  mq_r_attributes.mq_maxmsg = 10;
  mq_r_attributes.mq_msgsize = 1024;

  //abertura das mqueue
  mqueue = mq_open("/mqueue", O_CREAT | O_RDWR, 0666, &mq_attributes);
  if (mqueue == (mqd_t)-1) {
     perror("");
     std::cout << "Queue que recebe mensagem não aberta \n";
     return 1;
  }

  std::cout << "Mqueue que recebe mensagem aberta \n";

  mqueue_response = mq_open("/mqueue_response", O_CREAT | O_RDWR, 0666, &mq_r_attributes);
  if (mqueue_response == (mqd_t)-1) {
     perror("");
     std::cout << "Queue que envia confirmação não aberta \n";
     return 1;
  }

  std::cout << "Mqueue que envia confirmação aberta\n";

  //propriedades do receive da mqueue
  char m_received[1024];
  unsigned int m_priority = 0;

  //grupos de possiveis mensagens a serem recebidas:
  const char* i_received = "insert";
  const char* r_received = "remove";
  const char* s_received = "search";
  const char* u_received = "update";
  const char* q_received = "quit";
  

  std::cout << "Propriedades definidas \n";

  while (true) {
  //receptor de mensagem
  ssize_t f_argument = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
  
  if (f_argument == -1) {
    perror("");
    std::cout << "Mensagem não recebida";
  } 
  else if(strcmp(m_received, i_received) == 0){
    std::cout << "Mensagem " << m_received << " recebida \n";

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uk_receive] = '\0';
    std::string userKey = m_received;
    std::cout << "keyValue " << m_received << " recebido \n";

    if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta de KeyValue\n";
    }
    else{
      std::cout << "Mensagem keyValue enviada ao client\n";
    }

    ssize_t uv_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uv_receive] = '\0';
    std::string userValue = m_received;
    std::cout << "userValue " << m_received << " recebido \n";
    
    if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta de userValue\n";
    }
    else{
      std::cout << "Mensagem userValue enviada ao client\n";
    }

    // auto toInsert = [&sdb](std::string userKey, std::string userValue){
    //   sdb.dbInsert(userKey, userValue);
    // }; //o problema é que a função anonima não sabe que o sdb existe. ver exemplos de codigo e reestruturar o codigo

    // std::thread t_insert(toInsert);
    // t_insert.join();
  }
  else if(strcmp(m_received, r_received) == 0){
    std::cout << "Mensagem " << m_received << " recebida \n";

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uk_receive] = '\0';
    std::cout << "userValue " << m_received << " recebido \n";

    if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
    perror("");
    std::cout << "Erro ao enviar resposta de valor removido\n";
    }
    else{
      std::cout << "Mensagem valor removido enviada ao client\n";
    }
  }
  else if(strcmp(m_received, s_received) == 0){
    std::cout << "Mensagem " << m_received << " recebida \n";

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uk_receive] = '\0';
    std::cout << "userValue " << m_received << " recebido \n";

    const char* m_send = "teste";

    if(mq_send(mqueue_response, m_send, strlen(m_send), 1) == -1){
    perror("");
    std::cout << "Erro ao enviar resposta de valor pesquisado\n";
    }
    else{
      std::cout << "Mensagem valor pesquisado enviado ao client\n";
    }
  }
  else if(strcmp(m_received, u_received) == 0){
    std::cout << "Mensagem " << m_received << " recebida \n";

    ssize_t uv_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uv_receive] = '\0';
    std::cout << "UserValue a ser removido " << m_received << " recebido \n";

     if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta do antigo userValue removido\n";
    }
    else{
      std::cout << "Antigo userValue removido enviado ao cliente\n";
    }

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uk_receive] = '\0';
    std::cout << "UserValue a ser inserido " << m_received << " recebido \n";

     if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta do novo userValue inserida\n";
    }
    else{
      std::cout << "Novo userValue inserido enviado ao cliente\n";
    }
  }
  else if(strcmp(m_received, q_received) == 0){
    ssize_t qt_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[qt_receive] = '\0';
    std::cout << "Comando " << m_received << " recebido \n";
    
    if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar a informação de quit\n";
    }
    else{
      std::cout << "Informação de quit enviada\n";
    }
    break;
    exit(0);
  } 

  }

  return 0;
}
