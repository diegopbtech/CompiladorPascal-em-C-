#include <iomanip>
#include <iostream>
#include <fstream>
#include "Lexico.cpp"
#include "Sintatico.cpp" 
#include "Semantico.cpp"

using namespace std;

int main(void){
  Lexico lexema;
  Sintatico sint;
  Semantico semantica;
    ifstream file("Test1.txt");
  
  if (!file.is_open()) {
    cerr << "Erro ao abrir o código fonte.\n";
    return 1;
  }
  
  lexema.AnaliseLexica(file);
  file.close();

  if(!lexema.error){
    sint.nToken = lexema.nToken;
    for(int i = 0; i < lexema.nToken; i++){
      sint.tokens[i].lex = lexema.tokens[i].lex;
      sint.tokens[i].classificacao = lexema.tokens[i].classificacao;
      sint.tokens[i].linha = lexema.tokens[i].linha;
      if(sint.tokens[i].classificacao == "Palavra-chave"){
        for (char &c : sint.tokens[i].lex) {
            c = tolower(c);
        }
      }
    }
    sint.AnaliseSintatica();

    if(!sint.failure){
      semantica.nSimbol = sint.nSimbol;
        for(int i = 0; i < sint.nSimbol; i++){
          semantica.ts[i].id = sint.ts[i].id;
          semantica.ts[i].tipo = sint.ts[i].tipo;
          semantica.ts[i].nivel = sint.ts[i].nivel;
          semantica.ts[i].deslocamento = sint.ts[i].deslocamento;
        }
      semantica.ExibirTabela();
    }
    
  }
  
  return 0;
}