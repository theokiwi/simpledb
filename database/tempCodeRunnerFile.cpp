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
        else{
          std::cout<<"resposta invalida";
        }     
    }
    else if(std::strcmp(argv[1], "simpledb") != 0){
          std::cout<<"resposta invalida, comandos começam com simpledb";
    }