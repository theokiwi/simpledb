# simpledb
(PT-BR/ENG)

Projeto feito no vscode com coderunner, as extensões padrões de C++ e o compiler G++. No SO linux mint.
Project built on vscode with coderunner, C++ default extensions and G++ compiler. On linux mint SO. 

Final Build PrimeiraEntrega/Final Build FirstAssignment

Uma database não sql simples para a disciplina "Sistemas Operacionais" na Puc Minas. Feita em C++.
A simple non sql database for the course "Sistemas Operacionais" on PUC Minas. Built on C++.

Instruções/Instructions

./main simpledb insert <key(int), value(string)>
./main simpledb remove <value(string)>
./main simpledb search <value(string)>
./main simpledb update <value(String, nemValue(string))>
./main simpledb quit

Para o professor/(Message to my teacher): 

O meu código ficou um pouco diferente do solicitado na tarefa porque eu fiz algumas alterações que eu considerei mais funcionais. Por exemplo, estou fazendo a pesquisa por valor e não por chave porque eu assumi que o usuário não teria como saber qual int a coisa que ele está procurando recebeu dentro do programa. Além disso eu estou fazendo as operações dentro do dicionário e usando ele como banco de dados e não usando o arquivo de texto pra fazer as operações ele funciona apenas como um log, porque eu também achei que seria mais funcional. Não sei se tem problema, mas a minha percepção foi de que o código funcionaria melhor assim e faria pouca diferença para o usuário porque ele continuaria funcionando. Fora isso o readme tá em inglês e português porque queria usar ele para portifolio. 

