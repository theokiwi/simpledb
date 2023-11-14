#include <iostream>
#include <stdexcept>
#include <thread>
#include <cstring>
#include <string>
#include <mqueue.h>
#include <cerrno>
#include <semaphore>
#include <chrono>

struct mq_attr mq_attributes; //criando a struct que vai possuir as propriedades da mensagem que envia

struct mq_attr mq_r_attributes; //criando a struct que vai possuir as propriedades da mensagem que recebe

 int main(int argc, char** argv)
{
  const char* m_insert = "insert";
  const char* m_remove = "remove";
  const char* m_search = "search";
  const char* m_update = "update";
  const char* m_quit = "quit";

  int userKey;
  std::string userValue;

  //criação da queue
  mqd_t mqueue;
  mqd_t mqueue_response;
  //

  //propriedades da mensagem queue que envia
  mq_attributes.mq_flags = 0;       
  mq_attributes.mq_maxmsg = 10;      
  mq_attributes.mq_msgsize = 1024; 
  //

  //propriedades da mensagem queue que recebe
  mq_r_attributes.mq_flags = 0;
  mq_r_attributes.mq_maxmsg = 10;
  mq_r_attributes.mq_msgsize = 1024;

  //propriedades da mensagem recebida
  char m_received[1024];
  unsigned int m_priority;

  //definição dos atributos da queue
  //tem permissão de escrita e leitura 
  mqueue = mq_open("/mqueue", O_CREAT | O_RDWR, 0666, &mq_attributes);
  mqueue_response = mq_open("/mqueue_response", O_CREAT | O_RDWR , 0666, &mq_r_attributes);
  //

  //ATENÇÃO PRIMEIRO KEY E DEPOIS VALUE

if(std::strcmp(argv[1], "simpledb") == 0){
    //insert
        if(std::strcmp(argv[2], "insert") == 0){ 
        const char* userKey = argv[3];
        const char* userValue = argv[4];
           if(mq_send(mqueue, m_insert, strlen(m_insert), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'tipo de mensagem da queue'";
           }
           if (mq_send(mqueue, userKey, strlen(userKey), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userKey'";
           }
           if (mq_send(mqueue, userValue, strlen(userValue), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userValue'";
           }
           std::cout << "VIA CLIENTE: Mensagem " << m_insert << " lançada ao sistema \n";

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t ukReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: KeyValue " << m_received << " inserido no banco de dados \n";

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t uvReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: UserValue " << m_received << " inserido no banco de dados \n";

          //blocking mq_receive
        }
    //remove 
        else if(std::strcmp(argv[2], "remove") == 0){ //precisa de string userValue
        const char* userValue = argv[3];
          if(mq_send(mqueue, m_remove, strlen(m_remove), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro no enviar da queue\n";
          }
          if (mq_send(mqueue, userValue, strlen(userValue), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userValue'\n";
          }
          std::cout << "VIA CLIENTE: Mensagem " << m_remove << " lançada ao sistema\n"; 

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t rmReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: UserValue" << m_received << " removido do banco de dados \n";

        }
    //search
        else if(std::strcmp(argv[2], "search") == 0){ //precisa de string userValue
        const char* userValue = argv[3];
          if(mq_send(mqueue, m_search, strlen(m_search), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro no enviar da queue";
          }
          if (mq_send(mqueue, userValue, strlen(userValue), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userValue'\n";
          }
          std::cout << "VIA CLIENTE: Mensagem " << m_search << " lançada ao sistema\n";

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t shReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: o resultado da pesquisa no banco de dados é " << m_received;

        }
    //update  
        else if(std::strcmp(argv[2], "update") == 0){ //precisa de userValueToRemove e userValueToInsert
        const char* uv_remove = argv[3];
        const char* uv_new = argv[4];
          if(mq_send(mqueue, m_update, strlen(m_update), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro no enviar da queue";
          }
          if(mq_send(mqueue, uv_remove, strlen(uv_remove), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userValue' a ser removido\n";
          }
          if(mq_send(mqueue, uv_new, strlen(uv_new), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro ao enviar o 'userValue' a ser inserido\n";
          }
          std::cout << "VIA CLIENTE: Mensagem " << m_update << " lançada ao sistema\n";

          std::this_thread::sleep_for(std::chrono::seconds(1));


          ssize_t oldReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: KeyValue " << m_received << " removido do banco de dados \n";

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t newReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: UserValue " << m_received << " inserido no banco de dados \n";

        }
    //quit
        else if(std::strcmp(argv[2], "quit") == 0){
          if(mq_send(mqueue, m_quit, strlen(m_quit), 1) == -1){
            perror("");
            std::cout << "Está ocorrendo um erro no enviar da queue\n";
          }
          std::cout << "VIA CLIENTE: Mensagem " << m_quit << " lançada ao sistema\n";

          std::this_thread::sleep_for(std::chrono::seconds(1));

          ssize_t qtReceive = mq_receive(mqueue_response, m_received, sizeof(m_received), &m_priority);
          std::cout << "VIA DB: Mensagem " << m_received << " lançada ao sistema \n";

        }
        else{
          std::cout << "Error: Segundo argumento incorreto, verifique o README.txt para lista de possiveis argumentos\n";
        }     
  }
  else if(std::strcmp(argv[1], "quit") == 0){
    exit(0);
  }
  else if(std::strcmp(argv[1], "simpledb") != 0 && std::strcmp(argv[1], "quit") != 0){
    std::cout << "Error: Comandos devem ter simpledb como primeiro argumento\n";
  }
          return 0;
}
