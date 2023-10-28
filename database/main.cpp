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
    dbFile.open("dbLog.txt", std::ios::out); //inicializa o arquivo de texto
    if(dbFile.is_open()){
      for (auto pair : db){
      dbFile << pair.first << " " << pair.second << std::endl; //se o arquivo de texto estiver aberto insere os pares para cada pair
      }
    }
    else{
      throw 101;
    }
    dbFile.close(); //fecha o arquivo depois de completar a escrita
  }

  void clearFile(){ //responsavel por limpar o arquivo 
    dbFile.open("dbLog.txt", std::ios::out); //inicializa o arquivo de texto
      if(dbFile.is_open()){
        dbFile.clear(); // se o arquivo de texto tiver aberto da clear em tudo la dentro
      }
      else{
        throw 101;
      }
    dbFile.close(); //fecha o arquivo depois de completar a remoção de escrita

    }
 };

class debugTools{
  public:

  void displayDB(std::map<int, std::string> db){
    for (auto pair : db){
      std::cout << pair.first << " " << pair.second << std::endl; //ferramente de debug printa os pares no console
    }
  }
};

class simpledb{
  public:
  writeToFile write; //instancia a classe de escrita em arquivo 
  std::map<int, std::string> db; 

  void dbInsert(int userKey, std::string userValue){ //responsavel por inserir no banco de dados
    db.insert(std::pair<int, std::string>(userKey, userValue)); //insere no banco de dados
    std::cout << "a chave é " << userKey << " o valor é " << userValue << std::endl; //printa o que foi inserido
    write.wToFile(db); //escreve no arquivo o que foi inserido
  }

  bool dbSearch(std::string userValue){ //pesquisa no banco de dados um valor 
    for(auto i = db.begin(); i != db.end();){ //percorre o map até o fim
      if(i -> second == userValue){ //acessa o valor por meio de um ponteiro e compara com o valor apresentado pelo ususario
        std::cout << "encontrado"; 
        return true;//se o valor for encontrado printa encontrado e retorna true
      }
      i++;
    }
    std::cout << "não encontrado"; 
    return false; //se o valor não for encontrado pritna não encontrado e retorna false
  }
//o db remove usou ponteiros porque se eu so removesse o i ele eliminar um item do for
//e ia quebrar o for e me dava um erro falando que eu não tinha permissão
//eu tive que "i = db.erase(I)" ao inves de so db.erase porque o erase retorna pro i um ponteiro
//apontando pro proximo ponto
  void dbRemove(std::string userValue){ //remove do banco de dados o valor apontado pelo usuario
    for(auto i = db.begin(); i != db.end();){ //percorre o map até o fim
      if(dbSearch(userValue) == true){ //verifica se o valor existe no banco de dados
      std::cout << "o valor removido foi" << userValue;
      i = db.erase(i);
      write.clearFile();
      write.wToFile(db); //se o valor existir remove o valor, limpa o arquivo de texto e reescreve com os itens atuais
      }
      i++; //incrementa, se não existir o valor ele só pula pra cá e incrementa de uma vez
    }
  }

  void dbUpdate(std::string valueToRemove, std::string valueToInsert){ //substitui um valor existente no db por um novo valor
    for(auto i = db.begin(); i != db.end();){ //percorre o map até o fim
      if(i -> second == valueToRemove){ //se o valor for igual o valor buscado
        const int* whereToInsert = &(i -> first);  //um int que calcula a posição usando um ponteiro, tive que apontar pro ponteiro que aponta pro valor
        std::cout << "o valor removido foi" << valueToRemove << std::endl;
        i = db.erase(i);
        db.insert(std::pair<int, std::string>(*whereToInsert, valueToInsert)); //substitui o valor
        write.clearFile();
        write.wToFile(db); //limpa o arquivo e preenche com os novos dados do banco de dados
      }
      i++; //incrementa, se não encontrar o valor ele só pula pra cá e incrementa de uma vez
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
      throw 102;
      try{

      }
      catch(int exCode){
        std::cout << "Error: " << exCode << " Comandos devem ter simpledb como primeiro argumento";
      }
    }
          return 0;

  }
    

  




