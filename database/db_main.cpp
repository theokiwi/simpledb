
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <mqueue.h>
#include <cerrno>
#include <future>
#include <fstream>
#include <mutex>
#include <vector>
#include <algorithm>


//não consegui implementar o problema dos produtores e consumidores então por enquanto estou simplesmente usando um mutex

std::mutex mq_mutex; 

class writeToFile {
public:
  std::fstream dbFile;

  void wToFile(std::map<std::string, std::string> db) {  // escreve os pares no arquivo
      dbFile.open("dbLog.txt", std::ios::out);
      if (dbFile.is_open()){
        for (auto pair : db) {
          mq_mutex.lock();
          dbFile << pair.first << " " << pair.second << std::endl;
          mq_mutex.unlock();
        }
      } else {
        std::cout <<"O arquivo de escrita não pode ser aberto\n";
      }
    dbFile.close();
  }

  void clearFile() { // responsável por limpar o arquivo
      dbFile.open("dbLog.txt", std::ios::out);
      if (dbFile.is_open()) {
        mq_mutex.lock();
        dbFile.clear();
        mq_mutex.unlock();
      } else {
        std::cout << "O arquivo de escrita não pode ser aberto\n";
      }
    dbFile.close();
  }
};
//as funções acima tem sincronização dentro de si por isso não é travado um mutex quando elas são chamadas, elas fazem isso sozinhas

//ferramente de debug exibe toda a base de dados
class debugTools {
public:
  void displayDB(std::map<std::string, std::string> db) { // ferramenta de debug imprime os pares no console
      for (auto pair : db) {
        std::cout << pair.first << " " << pair.second << std::endl;
      }
  }
};


class simpledb {
public:
  writeToFile write;
  debugTools debug;
  std::map<std::string, std::string> db;
  int mxsize = 2;
  std::vector<std::string> valueVector;

  
  //pesquisa na base de dados
  bool dbSearch(std::string userValue) { //não precisa de sincronização porque não escreve só le
    for (auto i = db.begin(); i != db.end(); i++) {
        if (i-> second == userValue) {
            std::cout << "Encontrado";

            return true;
        }
    }
    std::cout << "Não encontrado\n";
    return false;
}

  

 void dbRemove(std::string userValue) { // remove do banco de dados o valor apontado pelo usuário
      for (auto i = db.begin(); i != db.end();) {
        if (dbSearch(userValue)) {
            mq_mutex.lock();
            i = db.erase(i);
            mq_mutex.unlock();

            write.clearFile();
            write.wToFile(db);
            debug.displayDB(db);

            auto whereIs = std::find(valueVector.begin(), valueVector.end(), userValue);
            if (whereIs != valueVector.end()) {
                int index = std::distance(valueVector.begin(), whereIs);
                valueVector.erase(valueVector.begin() + index);
            }

            std::cout << "\n";
            std::cout << "O valor removido foi " << userValue << "\n";
        } else {
            ++i;
        }
    }
  }

  void fifo(std::vector<std::string> valueVector){
    std::string whichValue = valueVector[1];
    dbRemove(whichValue);
    std::cout << "O valor " << whichValue << " foi removido via FIFO porque a database estava cheia\n";
  } 

  void aging(std::map<std::string, std::string> db, std::vector<std::string> valueVector){

  }

  void lru(std::map<std::string, std::string> db, std::vector<std::string> valueVector){

  }

  //insere ne base de dados
  void dbInsert(std::string userKey, std::string userValue) { // inserir no banco de dados
    valueVector.push_back(userValue);
      if (db.size() >= 1){
        std::cout << "A database atingiu o tamanho máximo, escolha um metodo de remoção de 1 a 3 (1 = fifo, 2 = Aging, 3 = LRU)";
        int method;
        std::cin >> method;
        switch(method){
          case 1:
          std::cout << "Iniciando remoção com metódo FIFO";
          fifo(valueVector);
          break;
          case 2:
          std::cout << "Iniciando remoção com método AGING";
          break;
          case 3:
          std::cout << "Iniciando remoção com metódo LRU";
          break;
        }
      }
      mq_mutex.lock(); 
      db.insert(std::pair<std::string, std::string>(userKey, userValue));
      mq_mutex.unlock();
      write.wToFile(db);
      std::cout << "A chave é " << userKey << " o valor é " << userValue << std::endl;
      debug.displayDB(db);
    } 


void dbUpdate(std::string valueToRemove, std::string valueToInsert) { // substitui um valor existente no db por um novo valor
        for (auto i = db.begin(); i != db.end();) {
        if (i->second == valueToRemove) {
          mq_mutex.lock();
          const std::string whereToInsert = i->first;
          std::cout << "O valor removido foi " << valueToRemove << std::endl;
          i = db.erase(i);
          db.insert(std::pair<std::string, std::string>(whereToInsert, valueToInsert));
          mq_mutex.unlock();
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

    std::thread t_insert([userKey, userValue, &sdb](){
      sdb.dbInsert(userKey, userValue);
    });
    t_insert.join();
    
    if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta de userValue\n";
    }
    else{
      std::cout << "Mensagem userValue enviada ao client\n";
    }
  }
  else if(strcmp(m_received, r_received) == 0){
    std::cout << "Mensagem " << m_received << " recebida \n";

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    m_received[uk_receive] = '\0';
    std::string userValue = m_received;
    std::cout << "userValue " << m_received << " recebido \n";

    std::thread t_remove([userValue, &sdb](){
      sdb.dbRemove(userValue);
    });
    t_remove.join();

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
    std::string userValue = m_received;
    std::cout << "userValue " << m_received << " recebido \n";

    std::promise<bool> wasFound;
    std::future<bool> foundResult = wasFound.get_future();

    std::thread t_search([userValue, &wasFound,&sdb](){
      bool result = sdb.dbSearch(userValue);
      wasFound.set_value(result);
      
    });
    t_search.join();

    bool resultFromThread = foundResult.get();
    const char* m_send;

    if (resultFromThread == true){
      m_send = "encontrado";
    }
    else{
      m_send = "não encontrado";
    }

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
    std::string userRm = m_received;
    std::cout << "UserValue a ser removido " << m_received << " recebido \n";

     if(mq_send(mqueue_response, m_received, strlen(m_received), 1) == -1){
      perror("");
      std::cout << "Erro ao enviar resposta do antigo userValue removido\n";
    }
    else{
      std::cout << "Antigo userValue removido enviado ao cliente\n";
    }

    ssize_t uk_receive = mq_receive(mqueue, m_received, sizeof(m_received), &m_priority);
    std::string userIn = m_received;
    m_received[uk_receive] = '\0';
    std::cout << "UserValue a ser inserido " << m_received << " recebido \n";


    std::thread t_update([userRm, userIn ,&sdb](){
      sdb.dbUpdate(userRm, userIn);
      
    });
    t_update.join();

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
    exit(0);
  } 

  }

  return 0;
}
