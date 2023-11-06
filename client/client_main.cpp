#include <iostream>
#include <stdexcept>
#include <thread>
#include <cstring>
#include <string>
#include <mqueue.h>

struct mq_attr mq_attributes; //criando a struct que vai possuir as propriedades da mensagem


 int main(int argc, char** argv)
{
s: 
  const char* m_insert = "insert";
  const char* m_remove = "remove";
  const char* m_search = "search";
  const char* m_update = "update";
  const char* m_quit = "quit";

  //criação da queue
  mqd_t mqueue;
  //

  //propriedades da mensagem
  mq_attributes.mq_flags = 0;       
  mq_attributes.mq_maxmsg = 10;      
  mq_attributes.mq_msgsize = 32; 
  //

  //definição dos atributos da queue
  //tem permissão de escrita e leitura 
  mqueue = mq_open("/mqueue", O_CREAT | O_RDWR, 0666, &mq_attributes);
  //

if(std::strcmp(argv[1], "simpledb") == 0){
    //insert
        if(std::strcmp(argv[2], "insert") == 0){
          
          if (mq_send(mqueue, m_insert, strlen(m_insert), 1) == -1){
            perror("");
            std::cout << "ta dando erro na queue";
          }
          std::cout << "Mensagem " << m_insert << " lançada ao sistema"; //ta enviando ta retornando 0
        }
    //remove
        else if(std::strcmp(argv[2], "remove") == 0){
          mq_send(mqueue, m_remove, strlen(m_remove + 1), 1);
          std::cout << "Mensagem " << m_remove << " lançada ao sistema"; 
        }
    //search
        else if(std::strcmp(argv[2], "search") == 0){
          mq_send(mqueue, m_search, strlen(m_search + 1), 1);
          std::cout << "Mensagem " << m_search << " lançada ao sistema";
        }
    //update  
        else if(std::strcmp(argv[2], "update") == 0){
          mq_send(mqueue, m_update, strlen(m_update + 1), 1);
          std::cout << "Mensagem " << m_update << " lançada ao sistema";
        }
    //quit
        else if(std::strcmp(argv[2], "quit") == 0){
          mq_send(mqueue, m_quit, strlen(m_quit + 1), 1);
          mq_close(mqueue);
          mq_unlink("mqueue");
          std::cout << "Mensagem " << m_quit << " lançada ao sistema";
          exit(0);
        }
        else{
          try{
            throw 102;
          }
          catch (int exCode){
            std::cout << "Error: " << exCode << "Segundo argumento incorreto, verifique o README.txt para lista de possiveis argumentos";
          }
        }     
    }
    else if(std::strcmp(argv[1], "simpledb") != 0){
      try{
        throw 103;
      }
      catch(int exCode){
        std::cout << "Error: " << exCode << " Comandos devem ter simpledb como primeiro argumento";
      }
    }
          return 0;
}