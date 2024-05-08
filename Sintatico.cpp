// ANÁLISE SINTÁTICA
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

struct Token3 {
  string lex;
  string classificacao;
  int linha;
};

struct TabelaDeSimbolo {
  string id;
  string tipo;
  int nivel;
  int deslocamento;
};

struct ParamProcedure{
  string id;
  int nParam=0;
  string params[10];
};

class Sintatico {
public:
  Token3 tokens[200];
  int nToken = 0;
  int nEscopo = 0;
  int next = 0;
  bool failure = false;
  bool closeParenthesis = true;

  //VARIÁVEIS PARA ANÁLISE SINTÁTICA
  TabelaDeSimbolo ts[100];
  int nSimbol = 0;
  int nivel_atual = 0;        // 0 - ESCOPO GLOBAL
  int deslocamento_atual = 0; // 0 - ESCOPO GLOBAL
  bool failure_semantico = false;
  string erro = "";
  string tipo_atual = "";

  //ANALISAR PARAMETROS
  ParamProcedure pp[20];
  string procedimento = "";
  int nProcedure = 0;
  int nParamProcedure = 0;
  string idProcAtual = "";

  int AnaliseSintatica(void) {
    cout << "\n\n=====================Analisador Sintático=====================\n" << endl;
    if (tokens[next].lex == "program") {
      next++;
      if (tokens[next].classificacao == "Identificador") {
        next++;
        PreencherTabelaSimbolos(1);
        if (tokens[next].lex == ";") {
          next++;
          // DECLARAÇÃO DE VARIÁVEIS
          if (DeclaracaoVariaveis()) {
            // DECLARAÇÃO DE SUBPROGRAMAS
            if (DeclaracaoSubprograma()) {
              nivel_atual = 0;
              while (tokens[next].lex == "procedure") {
                DeclaracaoSubprograma();
                nivel_atual = 0;
              }
              // CHAMADA DE COMANDO COMPOSTO
              if (ComandoComposto()) {
                if (tokens[next].lex == "." && !failure) {
                  if((next+1)==nToken){
                    cout << "Compilação de análise sintática bem sucedida!\n"
                         << endl;
                    cout << "\n=====================Analisador Semântico=====================\n" << endl;
                    if(failure_semantico){
                      cout << erro << endl;
                    }else{
                      cout << "Compilação de análise semântica bem sucedida!\n";
                    }
                  }else if (!failure){
                    cout << "Erro sintático na linha " << tokens[next+1].linha << endl;
                    cout << "Foi encontrado código que não pertence ao programa\n";
                    failure = true;
                  }

                } else if (!failure) {
                  cout << "Erro sintático na linha " << tokens[next - 1].linha
                       << endl;
                  cout << "Falta um '.' no final" << endl;
                }
              }
            }
          }
        } else if (!failure) {
          cout << "Erro sintático na linha " << tokens[next - 1].linha << " - "
               << "Esperado ';'" << endl;
          failure = true;
        }
      } else {
        cout << "Esperado o nome do programa na linha "
             << tokens[next].linha - 1 << endl;
      }
    } else {
      cout << "Esperado a palavra program na linha " << tokens[next].linha - 1
           << endl;
    }
    return 0;
  }

  bool DeclaracaoSubprograma() {
    bool ds = false;
    if (tokens[next].lex == "procedure") {
      next++;
      if (tokens[next].classificacao == "Identificador") {
        next++;
        pp[nProcedure].id = tokens[next-1].lex;
        //ANÁLISE SEMANTICA
        // - CHAMANDO A FUNÇÃO PARA VERIFICAR SE O IDENTIFICADOR JÁ FOI DECLARADO NO NÍVEL ATUAL. CASO JÁ TENHA SIDO DECLARADO, RETORNA UM ERRO
        IdentificadoresDeclarados();
        // - CHAMANDO A FUNÇÃO PARA ADICIONAR O IDENTIFICADOR NA TABELA DE SIMBOLOS
        PreencherTabelaSimbolos(2);
        if (argumentos()) {
          if (tokens[next].lex == ";") {
            next++;
          }
          if (DeclaracaoVariaveis()) {
            if (DeclaracaoSubprograma()) {
              if (ComandoComposto()) {
                ds = true;
              }
            }
          }
        }
      } else if (!failure) {
        cout << "Esperado o nome do subprograma na linha "
             << tokens[next].linha - 1;
        failure = true;
      }
    } else {
      ds = true;
    }
    return ds;
  }

  bool DeclaracaoVariaveis() {
    bool dv = false;
    if (tokens[next].lex == "var") {
      next++;
      if (tokens[next].classificacao == "Identificador") {
        next++;
        deslocamento_atual = 1;
        IdentificadoresDeclarados();
        PreencherTabelaSimbolos(3);
        // WHILE PARA VERIFICAR SE HÁ MAIS VARIAVEIS DE UM MESMO TIPO
        while (tokens[next].lex == "," && !failure) {
          next++;
          if (tokens[next].classificacao == "Identificador") {
            next++;
            IdentificadoresDeclarados();
            PreencherTabelaSimbolos(3);
          } else if (!failure) {
            dv = false;
            cout << "Erro sintático na linha " << tokens[next - 1].linha
                 << endl;
            failure = true;
            break;
          }
        }
        if (tokens[next].lex == ":") {
          next++;
          if (tokens[next].lex == "integer" || tokens[next].lex == "real" ||
              tokens[next].lex == "boolean") {
            next++;
            PreencherTabelaSimbolos(4);
            if (tokens[next].lex == ";") {
              next++;
              while (tokens[next].classificacao == "Identificador") {
                next++;
                IdentificadoresDeclarados();
                PreencherTabelaSimbolos(3);
                while (tokens[next].lex == "," && !failure) {
                  next++;
                  if (tokens[next].classificacao == "Identificador") {
                    next++;
                    IdentificadoresDeclarados();
                    PreencherTabelaSimbolos(3);
                  } else if (!failure) {
                    dv = false;
                    cout << "Esperado o nome da variável na linha "
                         << tokens[next].linha - 1 << endl;
                    failure = true;
                    break;
                  }
                }
                if (tokens[next].lex == ":") {
                  next++;
                  if (tokens[next].lex == "integer" ||
                      tokens[next].lex == "real" ||
                      tokens[next].lex == "boolean") {
                    next++;
                    PreencherTabelaSimbolos(4);
                    if (tokens[next].lex == ";") {
                      next++;
                      dv = true;
                    } else if (!failure) {
                      cout << "Erro sintático na linha "
                           << tokens[next - 1].linha << endl;
                      cout << "Esperado ';'" << endl;
                      dv = false;
                      failure = true;
                    }
                  } else if (!failure) {
                    cout << "Erro sintático na linha " << tokens[next - 1].linha
                         << endl;
                    cout << "Esperado o tipo da variável" << endl;
                    dv = false;
                    failure = true;
                  }
                } else if (!failure) {
                  cout << "Erro sintático na linha " << tokens[next - 1].linha
                       << endl;
                  cout << "Esperado ':' ou está faltando um begin " << endl;
                  dv = false;
                  failure = true;
                }
              }
              dv = true;
            } else if (!failure) {
              cout << "Erro sintático na linha " << tokens[next - 1].linha
                   << endl;
              cout << "Esperado ';'" << endl;
              dv = false;
              failure = true;
            }
          } else if (!failure) {
            cout << "Esperado o tipo de variável na linha "
                 << tokens[next].linha - 1 << endl;
            next++;
            failure = true;
          }
        } else if (!failure) {
          cout << "Esperado o ':' na linha " << tokens[next - 1].linha << endl;
          dv = false;
          failure = true;
        }
      } else if (!failure) {
        dv = false;
        cout << "Esperado o nome da variável na linha "
             << tokens[next].linha - 1 << endl;
        failure = true;
      }
    } else {
      dv = true;
    }
    return dv;
  }

  bool argumentos() {
    bool arg = false;
    if (tokens[next].lex == "(") {
      next++;
      if (tokens[next].classificacao == "Identificador") {
        next++;
        deslocamento_atual = 1;
        IdentificadoresDeclarados();
        PreencherTabelaSimbolos(5);
        if (tokens[next].lex == ":") {
          next++;
          if (tokens[next].lex == "integer" || tokens[next].lex == "real" ||
              tokens[next].lex == "boolean") {
            pp[nProcedure].params[pp[nProcedure].nParam] = tokens[next].lex;
            pp[nProcedure].nParam++;
            next++;
            PreencherTabelaSimbolos(6);
          }
          while (tokens[next].lex == ";" && !failure) {
            next++;
            if (tokens[next].classificacao == "Identificador") {
              next++;
              IdentificadoresDeclarados();
              PreencherTabelaSimbolos(5);
              if (tokens[next].lex == ":") {
                next++;
                if (tokens[next].lex == "integer" ||
                    tokens[next].lex == "real" ||
                    tokens[next].lex == "boolean") {
                  pp[nProcedure].params[pp[nProcedure].nParam] = tokens[next].lex;
                  pp[nProcedure].nParam++;
                  next++;
                  PreencherTabelaSimbolos(6);
                } else if (!failure) {
                  cout << "Erro sintático na linha " << tokens[next - 1].linha
                       << endl;
                  failure = true;
                }
              } else if (!failure) {
                cout << "Erro sintático na linha " << tokens[next - 1].linha
                     << endl;
                failure = true;
              }
            } else if (!failure) {
              cout << "Erro sintático na linha " << tokens[next - 1].linha
                   << endl;
              cout << "Esperado uma variável na linha " << endl;
              failure = true;
            }
          }
          if (tokens[next].lex == ")") {
            next++;
            nProcedure++;
            arg = true;
          } else if (!failure) {
            cout << "Erro sintático na linha " << tokens[next].linha << endl;
            failure = true;
          }
        } else if (!failure) {
          cout << "Erro sintático na linha " << tokens[next].linha - 1 << endl;
          failure = true;
        }
      } else if (tokens[next].lex == ")") {
        next++;
        arg = true;
      } else if (!failure) {
        cout << "Esperado ')' na linha " << tokens[next].linha << endl;
        failure = true;
      }
    } else if (!failure) {
      cout << "Esperado '(' na linha " << tokens[next].linha - 1 << endl;
      failure = true;
    }
    return arg;
  }

  bool ComandoComposto() {
    bool cc = false;
    if (tokens[next].lex == "begin") {
      next++;
      while (comando() && !failure) {
        if (tokens[next].lex == ";") {
          tipo_atual = "";
          next++;
        } else if (tokens[next].lex == "end" ||
                   (tokens[next].lex == "." && tokens[next - 1].lex == "end") || (tokens[next].classificacao == "Identificador" && tokens[next - 1].lex == "end")) {
          tipo_atual = "";
          break;
        } else if (!failure) {
          cout << "Erro sintático na linha " << tokens[next - 1].linha << endl;
          cout << "Esperado ';'" << endl;
          failure = true;
          break;
        }
      }
      if (tokens[next].lex == "end" ||
          (tokens[next].lex == "." && tokens[next - 1].lex == "end") ||
        (tokens[next].classificacao == "Identificador" && tokens[next - 1].lex == "end")) {
        if (tokens[next].lex == "end") {
          next++;
        }
        //CODIGO PARA VERIFICAR SE O PROGRAMA ESTA COMPLETO E APAGAR OS DADOS DA TABELA
        
        // for(int i=0; i<nSimbol; i++){
        //   if(nivel_atual==ts[i].nivel){
        //     nSimbol--;
        //   }
        // }
        // nivel_atual--;

        cc = true;
      } else if (!failure) {
        cout << "Erro sintático na linha " << tokens[next].linha << endl;
        failure = true;
      }
    } else {
      cc = true;
    }
    return cc;
  }

  bool comando() {
    bool cmd = false;
    if (tokens[next].classificacao == "Identificador") {
      // ANÁLISE SEMANTICA
      bool encontrei = false;
      for (int i = nSimbol - 1; i >= 0; i--) {
        if (ts[i].id == tokens[next].lex) {
          encontrei = true;
          tipo_atual = ts[i].tipo;
        }
      }
      if (!encontrei) {
        //NÃO ENCONTROU VARIÁVEL NA TABELA DE SIMBOLOS OU ENCONTROU PORÉM TIPO INCOMPATÍVEL
        if (!encontrei) {
          erro = "Erro semântico na linha " + to_string(tokens[next].linha);
          erro += "\nVariável não declarada ";
          failure_semantico = true;
        }
      }
      next++;
      if (tokens[next].lex == ":=") {
        next++;
        if (expressao()) {
          cmd = true;
        }
      } else if (tokens[next].lex == "(") {
        idProcAtual = tokens[next-1].lex;
        next++;
        if (tokens[next].lex == ")") {
          next++;
          ParametroTipo(2);
          cmd = true;
        } else if (ListaExpressoes()) {
          cmd = true;
        }
      } else if (!failure) {
        cout << "Erro na linha " << tokens[next - 1].linha << endl;
        failure = true;
        return false;
      }
    } else if (tokens[next].lex == "if") {
      next++;
      if (expressao()) {
        while (tokens[next].lex == "and") {
          next++;
          if (expressao()) {

          } else {
            cout << "Erro na linha " << tokens[next - 1].linha << endl;
            failure = true;
            break;
          }
        }
        if (tokens[next].lex == "then") {
          next++;
          if (comando()) {
            next++;
            if (tokens[next].lex == "else") {
              next++;
              if (comando()) {
                cmd = true;
              }
            } else {
              cmd = true;
            }
          }
        } else if (!failure) {
          cout << "Erro na linha " << tokens[next - 1].linha << endl;
          failure = true;
        }
      } else if (!failure) {
        cout << "Esperado uma expressão na linha " << tokens[next - 1].linha
             << endl;
        failure = true;
      }
    } else if (tokens[next].lex == "while") {
      next++;
      if (expressao()) {
        if (tokens[next].lex == "do") {
          next++;
          if (comando()) {
            cmd = true;
          }
        } else if (!failure) {
          cout << "Erro na linha " << tokens[next - 1].linha << endl;
          failure = true;
        }
      }
    } else if (ComandoComposto()) {
      cmd = true;
    }
    return cmd;
  }

  bool expressao() {
    bool exp = false;
    if (ExpressaoSimples()) {
      if (tokens[next].classificacao == "Operador Relacional") {
        next++;
        if(tokens[next].classificacao=="Lógico"){
          erro = "Erro semântico na linha " + to_string(tokens[next].linha);
          erro += "\nOperador lógico não aceito em uma comparação relacional";
          failure_semantico = true;
        }
        for(int l=nSimbol-1;l>=0;l--){
          if(ts[l].id==tokens[next].lex){
            if(ts[l].tipo=="boolean"){
              erro = "Erro semântico na linha " + to_string(tokens[next].linha);
              erro += "\nOperador lógico não aceito em uma comparação relacional";
              failure_semantico = true;
            }
          }
        }
        for(int l=nSimbol-1;l>=0;l--){
          if(ts[l].id==tokens[next-2].lex){
            if(ts[l].tipo=="boolean"){
              erro = "Erro semântico na linha " + to_string(tokens[next].linha);
              erro += "\nOperador lógico não aceito em uma comparação relacional";
              failure_semantico = true;
            }
          }
        }
        if (ExpressaoSimples()) {
          exp = true;
        }
      } else {
        exp = true;
      }
    } else if(tokens[next].classificacao == "Lógico"){
      if(tipo_atual == "real" || tipo_atual == "integer"){
        erro = "Erro semântico na linha " + to_string(tokens[next].linha);
        erro += "\nOperador lógico não pode ser utilizado com tipo real ou inteiro";
        failure_semantico = true;
      }
      next++;
      exp = true;
    }else if (!failure) {
      cout << "Erro na linha " << tokens[next - 2].linha << endl;
      failure = true;
    }
    return exp;
  }

  bool ExpressaoSimples() {
    bool es = false;
    if (termo()) {
      if (tokens[next].classificacao == "Operador Aditivo") {
        next++;
        if (termo()) {
          es = true;
        } else if (!failure) {
          cout << "Erro na linha " << tokens[next - 1].linha << endl;
          failure = true;
        }
      } else {
        es = true;
      }
    }
    return es;
  }

  bool termo() {
    bool t = false;
    if (fator()) {
      if (tokens[next].classificacao == "Operador Multiplicativo") {
        next++;
        if (ExpressaoSimples()) {
          t = true;
        }
      } else if (tokens[next].classificacao == "Operador Aditivo") {
        if (ExpressaoSimples()) {
          t = true;
        }
      } else {
        t = true;
      }
    } else if (tokens[next].classificacao == "Operador Aditivo") {
      t = true;
    }
    return t;
  }

  bool fator() {
    bool fat = false;
    if (tokens[next].classificacao == "Identificador") {
      // ANÁLISE SEMANTICA
      bool encontrei = false;
      for (int i = nSimbol - 1; i >= 0; i--) {
        if (ts[i].id == tokens[next].lex) {
          if(tipo_atual=="integer" || tipo_atual=="real"){
            if(tipo_atual=="integer"){
              if(tipo_atual==ts[i].tipo){
                encontrei = true;
              }else{
                break;
              }
            }else if(tipo_atual=="real" && ts[i].tipo!="boolean"){
              encontrei = true;
            }
          }else if (tipo_atual==""){
            encontrei = true;
          }
          break;
        }
      }
      //NÃO ENCONTROU VARIÁVEL NA TABELA DE SIMBOLOS OU ENCONTROU PORÉM TIPO INCOMPATÍVEL
      if (!encontrei) {
        erro = "Erro semântico na linha " + to_string(tokens[next].linha);
        erro += "\nVariável não declarada ou tipo incompatível";
        failure_semantico = true;
      }
      next++;
      if (tokens[next].lex != "(") {
        fat = true;
      } else if (tokens[next].lex == "(") {
        idProcAtual = tokens[next].lex;
        next++;
        if (tokens[next].lex == ")") {
          next++;
          fat = true;
        } else if (ListaExpressoes()) {
          fat = true;
        }
      }
    } else if (tokens[next].classificacao == "Numero Inteiro" ||
               tokens[next].classificacao == "Numero Real") {
      //VERIFICANDO SE O TIPO É COMPATÍVEL COM O NÚMERO
      bool tcom = false;
      if ((tipo_atual=="real") || (tipo_atual=="integer" && tokens[next].classificacao=="Numero Inteiro") || tipo_atual=="") {
        if(tipo_atual=="integer"){
          if(tokens[next].classificacao == "Numero Inteiro"){
            tcom = true;
          }
        }else{
          tcom = true;
        }
      }
      if(!tcom){
        erro = "Erro semântico na linha " + to_string(tokens[next].linha);
        erro += "\nAtribuição com tipos de dados incompatíveis";
        failure_semantico = true;
      }
      next++;
      fat = true;
    } else if (tokens[next].lex == "(") {
      closeParenthesis = false;
      next++;
      if (expressao()) {
        if (tokens[next - 1].lex == ")" || tokens[next].lex == ")") {
          next++;
          fat = true;
          closeParenthesis = true;
        } else if (!failure) {
          cout << "Erro na linha " << tokens[next - 1].linha << endl;
          failure = true;
        }
      } else if (!failure) {
        cout << "Erro na linha " << tokens[next - 1].linha << endl;
        failure = true;
      }
    }
    return fat;
  }

  bool ListaExpressoes() {
    bool le = false;
    if (tokens[next].classificacao == "Identificador"){
      IdentificadorTabelaSimbolo();
      ParametroTipo(1);
      nParamProcedure++;
      next++;
      while (tokens[next].lex == "," && !failure) {
        next++;
        if (tokens[next].classificacao == "Identificador") {
          IdentificadorTabelaSimbolo();
          ParametroTipo(1);
          nParamProcedure++;
          next++;
        } else {
          cout << "Erro ao chamar o procedure na linha " << tokens[next - 1].linha << endl;
          failure = true;
          break;
        }
      }
      if (tokens[next].lex == ")") {
        le = true;
        next++;
        ParametroTipo(2);
      } else {
        cout << "Erro ao chamar o procedure na linha " << tokens[next - 1].linha << endl;
        failure = true;
      }
    } else {
      cout << "Erro ao chamar o precedure na linha " << tokens[next - 1].linha - 1 << endl;
      failure = true;
    }
    return le;
  }

  //ANÁLISE SINTÁTICA

  //ADICIONANDO OS IDENTIFICADORES NA TABELA DE SIMBOLOS
  void PreencherTabelaSimbolos(int i){
    //1 = QUANDO FOR O NOME DO PROGRAMA
    if(i==1){
      ts[nSimbol].id = tokens[next - 1].lex;
      ts[nSimbol].tipo = "Program";
      ts[nSimbol].nivel = nivel_atual;
      ts[nSimbol].deslocamento = deslocamento_atual;
      nSimbol++;
      deslocamento_atual++;
    }
    //2 = QUANDO FOR O NOME DE UM PROCEDIMENTO
    if(i==2){
      ts[nSimbol].id = tokens[next - 1].lex;
      ts[nSimbol].tipo = "PROC";
      ts[nSimbol].nivel = nivel_atual;
      ts[nSimbol].deslocamento = deslocamento_atual;
      nSimbol++;
      deslocamento_atual++;
      nivel_atual++;
    }
    //3 = QUANDO FOR O NOME DE UMA VARIÁVEL
    if(i==3){
      ts[nSimbol].id = tokens[next - 1].lex;
      ts[nSimbol].tipo = "AA";
      ts[nSimbol].nivel = nivel_atual;
      ts[nSimbol].deslocamento = deslocamento_atual;
      nSimbol++;
      deslocamento_atual++;
    }
    //4 = QUANDO FOR PARA ADICIONAR O TIPO DAS VARIÁVEIS ADICIONADAS
    if(i==4){
      int s = nSimbol - 1;
      while (ts[s].tipo == "AA") {
        ts[s].tipo = tokens[next - 1].lex;
        s--;
      }
    }
    //5 = QUANDO FOR O NOME DE UM PARÂMETRO PASSADO EM UMA FUNÇÃO
    if(i==5){
      ts[nSimbol].id = tokens[next - 1].lex;
      ts[nSimbol].nivel = nivel_atual;
      ts[nSimbol].deslocamento = deslocamento_atual;
      deslocamento_atual++;
    }
    //6 - QUANDO FOR PARA ADICIONAR O TIPO DAS VARIÁVEIS PASSADAS EM UMA FUNÇÃO
    if(i==6){
      ts[nSimbol].tipo = tokens[next - 1].lex;
      nSimbol++;
    }
  }

  //VERIFICANDO SE UM IDENTIFICADOR COM MESMO NOME JÁ FOI DECLARADO ANTERIOMENTE NO MESMO ESCOPO
  void IdentificadoresDeclarados(){
    //VERIFICANDO SE O IDENTIFICADOR ENCONTRADO JÁ FOI DECLARADO ANTERIORMENTE
    for (int i = nSimbol - 1; i >= 0; i--) {
      if ((ts[i].id == tokens[next-1].lex)&&ts[i].nivel == nivel_atual) {
        erro = "Erro semântico na linha " + to_string(tokens[next-1].linha);
        erro += "\nIdentificador '" + tokens[next-1].lex + "' já declarado neste escopo!";
        failure_semantico = true;
        break;
      }
    }
  }

  //VERIFICAR SE A VARIÁVEL FOI DECLARADO NO ESCOPO
  void IdentificadorTabelaSimbolo(){
    bool encontrei = false;
    for (int i = nSimbol - 1; i >= 0; i--) {
      if (ts[i].id == tokens[next].lex) {
        encontrei = true;
      }
    }
    if (!encontrei) {
      //NÃO ENCONTROU VARIÁVEL NA TABELA DE SIMBOLOS OU ENCONTROU PORÉM TIPO INCOMPATÍVEL
      if (!encontrei) {
        erro = "Erro semântico na linha " + to_string(tokens[next].linha);
        erro += "\nVariável não declarada";
        failure_semantico = true;
      }
    }
  }

  //VERIFICANDO SE O PARÂMETRO PASSADO É DO TIPO CORRETO
  void ParametroTipo(int p){
    //VERIFICANDO SE O TIPO DO PARÂMETRO PASSADO É COMPATÍVEL COM A FUNÇÃO
    if(p==1){
      bool pt = false;
      for(int i=nProcedure;i>=0;i--){
        if(pp[i].id == idProcAtual){
          for(int l=nSimbol-1;l>=0;l--){
            if(tokens[next].lex == ts[l].id){
              if(ts[l].tipo == pp[i].params[nParamProcedure]){
                pt = true;
                break;
              }
            }
          }
          break;
        }
      }
      if(!pt && !failure_semantico){
        erro = "Erro semântico na linha " + to_string(tokens[next-1].linha);
        erro += "\nO parâmetro '" + tokens[next].lex + "' não é do tipo esperado";
        failure_semantico = true;
      }
    }
    //VERIFICANDO SE O NÚMERO DE PARÂMETROS PASSADOS É COMPATÍVEL COM A FUNÇÃO
    if(p==2){
      for(int k=nProcedure-1;k>=0;k--){
        if(pp[k].id == idProcAtual){
          if(nParamProcedure!=pp[k].nParam){
            erro = "Erro semântico na linha " + to_string(tokens[next - 1].linha);
            erro += "\nNúmero de parâmetros incompatível";
            failure_semantico = true;
            break;
          }
        }
      }
      idProcAtual = "";
      nParamProcedure = 0;
    }
  }

  void exibirTabelaProcedimentos(){
    cout << "Tabela de parametros dos simbolos" << endl;
    cout << "-----------------------------------" << endl;
    cout << "ID" << setw(20) << "TIPO" << setw(20) << endl;
    for(int i = 0; i<nProcedure;i++){
      cout << pp[i].id << endl;
      cout << "Numero de parametros: " << pp[i].nParam << endl;
      for(int j=0;j<pp[i].nParam;j++){
        cout << pp[i].params[j] << endl;
      }
    }
  }

  // ESSE FECHA-CHAVE É O DA CLASSE
};
