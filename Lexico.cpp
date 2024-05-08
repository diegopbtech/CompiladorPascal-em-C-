#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

struct Token {
  string lex;
  string classificacao;
  int linha;
};

class Lexico {

public:
  Token tokens[200];
  int nToken = 0;
  bool error = false;

  int AnaliseLexica(ifstream &file) {
    string mensagem;
    int estagio = 0;
    string linha;
    string comando = "";
    int linhaAtual = 0;

    while (getline(file, linha) && !error) {
      linhaAtual++;
      for (int i = 0; i < linha.length(); ++i) {
        char c = linha[i];
        // COMENTARIOS
        if (c == '{') {
          estagio = 34;
          comando = "{";
        } else if (estagio == 34) {
          if (c != '}') {
            comando += c;
          } else {
            comando += '}';

            comando = "";
            estagio = 0;
          }
        }
        // PROGRAM
        else if ((c == 'p' || c == 'P') && estagio == 0) {
          estagio = 1;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 1) {
          estagio = 2;
          comando += c;
        } else if ((c == 'o' || c == 'O') && estagio == 2) {
          estagio = 3;
          comando += c;
        } else if ((c == 'g' || c == 'G') && estagio == 3) {
          estagio = 4;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 4) {
          estagio = 5;
          comando += c;
        } else if ((c == 'a' || c == 'A') && estagio == 5) {
          estagio = 6;
          comando += c;
        } else if ((c == 'm' || c == 'M') && estagio == 6) {
          estagio = 7;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == ',') && estagio == 7) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // PROCEDURE
        else if ((c == 'c' || c == 'C') && estagio == 3) {
          estagio = 61;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 61) {
          estagio = 62;
          comando += c;
        } else if ((c == 'd' || c == 'D') && estagio == 62) {
          estagio = 63;
          comando += c;
        } else if ((c == 'u' || c == 'U') && estagio == 63) {
          estagio = 64;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 64) {
          estagio = 65;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 65) {
          estagio = 66;
          comando += c;
        } else if ((c == ' ' || c == '(' || c == ',' || c == ';') &&
                   estagio == 66) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // VAR
        else if ((c == 'v' || c == 'V') && estagio == 0) {
          estagio = 8;
          comando = c;
        } else if ((c == 'a' || c == 'A') && estagio == 8) {
          estagio = 9;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 9) {
          estagio = 10;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '.' || c == '(') &&
                   estagio == 10) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // INTEGER
        else if ((c == 'i' || c == 'I') && estagio == 0) {
          estagio = 11;
          comando = c;
        } else if ((c == 'n' || c == 'N') && estagio == 11) {
          estagio = 12;
          comando += c;
        } else if ((c == 't' || c == 'T') && estagio == 12) {
          estagio = 13;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 13) {
          estagio = 14;
          comando += c;
        } else if ((c == 'g' || c == 'G') && estagio == 14) {
          estagio = 15;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 15) {
          estagio = 16;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 16) {
          estagio = 17;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == ')' || c == ',') &&
                   estagio == 17) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // IF
        else if ((c == 'f' || c == 'F') && estagio == 11) {
          estagio = 18;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(') && estagio == 18) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // THEN
        else if ((c == 't' || c == 'T') && estagio == 0) {
          estagio = 51;
          comando = c;
        } else if ((c == 'h' || c == 'H') && estagio == 51) {
          estagio = 52;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 52) {
          estagio = 53;
          comando += c;
        } else if ((c == 'n' || c == 'N') && estagio == 53) {
          estagio = 54;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == ',') &&
                   estagio == 54) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // TRUE
        else if ((c == 'r' || c == 'R') && estagio == 51) {
          estagio = 84;
          comando += c;
        } else if ((c == 'u' || c == 'U') && estagio == 84) {
          estagio = 85;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 85) {
          estagio = 86;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == ',' || c == ')') && estagio == 86) {
          estagio = 0;
          tokens[nToken].classificacao = "Lógico";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // TO
        else if ((c == 'o' || c == 'O') && estagio == 51) {
          estagio = 74;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == ',') &&
                   estagio == 74) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // AND
        else if ((c == 'a' || c == 'A') && estagio == 0) {
          estagio = 55;
          comando = c;
        } else if ((c == 'n' || c == 'N') && estagio == 55) {
          estagio = 56;
          comando += c;
        } else if ((c == 'd' || c == 'D') && estagio == 56) {
          estagio = 57;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '.' || c == '(') &&
                   estagio == 57) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // REAL
        else if ((c == 'r' || c == 'R') && estagio == 0) {
          estagio = 19;
          comando = c;
        } else if ((c == 'e' || c == 'E') && estagio == 19) {
          estagio = 20;
          comando += c;
        } else if ((c == 'a' || c == 'A') && estagio == 20) {
          estagio = 21;
          comando += c;
        } else if ((c == 'l' || c == 'L') && estagio == 21) {
          estagio = 22;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == ',' || c == ')' || c == '(') &&
                   estagio == 22) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // BEGIN
        else if ((c == 'b' || c == 'B') && estagio == 0) {
          estagio = 23;
          comando = c;
        } else if ((c == 'e' || c == 'E') && estagio == 23) {
          estagio = 24;
          comando += c;
        } else if ((c == 'g' || c == 'G') && estagio == 24) {
          estagio = 25;
          comando += c;
        } else if ((c == 'i' || c == 'I') && estagio == 25) {
          estagio = 26;
          comando += c;
        } else if ((c == 'n' || c == 'N') && estagio == 26) {
          estagio = 27;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == '.' || c == ',') &&
                   estagio == 27) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // BOOLEAN
        else if ((c == 'o' || c == 'O') && estagio == 23) {
          estagio = 78;
          comando += c;
        } else if ((c == 'o' || c == 'O') && estagio == 78) {
          estagio = 79;
          comando += c;
        } else if ((c == 'l' || c == 'L') && estagio == 79) {
          estagio = 80;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 80) {
          estagio = 81;
          comando += c;
        } else if ((c == 'a' || c == 'A') && estagio == 81) {
          estagio = 82;
          comando += c;
        } else if ((c == 'n' || c == 'N') && estagio == 82) {
          estagio = 83;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == '.' || c == ',') &&
                   estagio == 83) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // END
        else if ((c == 'e' || c == 'E') && estagio == 0) {
          estagio = 28;
          comando = c;
        } else if ((c == 'n' || c == 'N') && estagio == 28) {
          estagio = 29;
          comando += c;
        } else if ((c == 'd' || c == 'D') && estagio == 29) {
          estagio = 30;
          comando += c;
        } else if ((c == '.' || c == ' ') && estagio == 30) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          nToken++;
          if (c != ' ') {
            i--;
          }
          comando = "";
        }
        // ELSE
        else if ((c == 'l' || c == 'L') && estagio == 28) {
          estagio = 31;
          comando += c;
        } else if ((c == 's' || c == 'S') && estagio == 31) {
          estagio = 32;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 32) {
          estagio = 33;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(') && estagio == 33) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // WHILE
        else if ((c == 'w' || c == 'W') && estagio == 0) {
          estagio = 67;
          comando = c;
        } else if ((c == 'h' || c == 'H') && estagio == 67) {
          estagio = 68;
          comando += c;
        } else if ((c == 'i' || c == 'I') && estagio == 68) {
          estagio = 69;
          comando += c;
        } else if ((c == 'l' || c == 'L') && estagio == 69) {
          estagio = 70;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 70) {
          estagio = 71;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(') && estagio == 71) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // DO
        else if ((c == 'd' || c == 'D') && estagio == 0) {
          estagio = 72;
          comando = c;
        } else if ((c == 'o' || c == 'O') && estagio == 72) {
          estagio = 73;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(') && estagio == 73) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // FOR
        else if ((c == 'f' || c == 'F') && estagio == 0) {
          estagio = 75;
          comando = c;
        } else if ((c == 'o' || c == 'O') && estagio == 75) {
          estagio = 76;
          comando += c;
        } else if ((c == 'r' || c == 'R') && estagio == 76) {
          estagio = 77;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(') && estagio == 77) {
          estagio = 0;
          tokens[nToken].classificacao = "Palavra-chave";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // FALSE
        else if ((c == 'a' || c == 'A') && estagio == 75) {
          estagio = 87;
          comando += c;
        } else if ((c == 'l' || c == 'L') && estagio == 87) {
          estagio = 88;
          comando += c;
        } else if ((c == 's' || c == 'S') && estagio == 88) {
          estagio = 89;
          comando += c;
        } else if ((c == 'e' || c == 'E') && estagio == 89) {
          estagio = 90;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == '(' || c == ')') && estagio == 90) {
          estagio = 0;
          tokens[nToken].classificacao = "Lógico";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // DELIMITADOR , (VIRGULA)
        else if (c == ',' && estagio == 0) {
          comando = c;
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // DELIMITADOR ;
        else if (c == ';' && estagio == 0) {
          comando = c;
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // DELIMITADOR :
        else if (c == ':' && estagio == 0) {
          estagio = 42;
          comando = c;
        } else if (c != '=' && estagio == 42) {
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          estagio = 0;
          nToken++;
          if (c != ' ') {
            i--;
          }
        }
        // DELIMITADOR (
        else if (c == '(' && estagio == 0) {
          estagio = 0;
          comando = c;
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // DELIMITADOR )
        else if (c == ')' && estagio == 0) {
          estagio = 0;
          comando = c;
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // DELIMITADOR .
        else if (c == '.' && estagio == 0) {
          estagio = 0;
          comando = c;
          tokens[nToken].classificacao = "Delimitador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // ATRIBUIÇÃO :=
        else if (c == '=' && estagio == 42) {
          comando += c;
          tokens[nToken].classificacao = "Atribuição";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          estagio = 0;
          nToken++;
        }
        // OPERADOR ADITIVO +
        else if (c == '+' && estagio == 0) {
          comando = c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Aditivo";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR ADITIVO -
        else if (c == '-' && estagio == 0) {
          comando = c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Aditivo";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR MULTIPLICATIVO *
        else if (c == '*' && estagio == 0) {
          comando = c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Multiplicativo";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR MULTIPLICATIVO /
        else if (c == '/' && estagio == 0) {
          comando = c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Multiplicativo";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL ==
        else if (c == '=' && estagio == 0) {
          comando = c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL <
        else if (c == '<' && estagio == 0) {
          estagio = 48;
          comando = c;
        } else if (c != '>' && c != '=' && estagio == 48) {
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL <=
        else if (c == '=' && estagio == 48) {
          comando += c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL <>
        else if (c == '>' && estagio == 48) {
          comando += c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL >
        else if (c == '>' && estagio == 0) {
          estagio = 49;
          comando = c;
        } else if (c != '=' && estagio == 49) {
          comando += c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // OPERADOR RELACIONAL >=
        else if (c == '=' && estagio == 49) {
          comando += c;
          estagio = 0;
          tokens[nToken].classificacao = "Operador Relacional";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
        }
        // NUMEROS

        // NÚMEROS INTEIROS [0..9]
        else if ((c >= '0' && c <= '9') && estagio == 0) {
          estagio = 45;
          comando = c;
        } else if (((c == ' ' || c == ';' || c == ')' || c == ',') ||
                    (c >= '0' && c <= '9') || (c >= '0' && c <= '9') ||
                    (c >= 'A' && c <= 'z')) &&
                   estagio == 45) {
          if (c >= '0' && c <= '9') {
            comando += c;
            estagio = 45;
          } else {
            estagio = 0;
            tokens[nToken].classificacao = "Numero Inteiro";
            tokens[nToken].linha = linhaAtual;
            tokens[nToken].lex = comando;
            comando = "";
            nToken++;
            if (c != ' ') {
              i--;
            }
          }
          // NÚMEROS REAIS [0..9].[0..9]
        } else if (c == '.' && estagio == 45) {
          estagio = 46;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == ')' ||
                    (c >= '0' && c <= '9') || (c >= 'A' && c <= 'z')) &&
                   estagio == 46) {
          if (c >= '0' && c <= '9') {
            comando += c;
            estagio = 46;
          } else {
            estagio = 0;
            tokens[nToken].classificacao = "Numero Real";
            tokens[nToken].linha = linhaAtual;
            tokens[nToken].lex = comando;
            comando = "";
            nToken++;
            if (c != ' ') {
              i--;
            }
          }
        }
        // IDENTIFICADORES
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
          int primeiroCaracter = 0;
          if (comando != "" && primeiroCaracter == 0) {
            for (char n : comando) {
              if (n >= '0' && n <= '9') {
                error = true;
                mensagem = "Simbolos nao pertencentes a linguagem";
                break;
              }
            }
            primeiroCaracter++;
          }
          estagio = 47;
          comando += c;
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                   (c >= '0' && c <= '9')) {
          estagio = 47;
          comando += c;
        } else if ((c == ' ' || c == ';' || c == ':' || c == ',' || c == ')') &&
                   estagio == 47) {
          estagio = 0;
          tokens[nToken].classificacao = "Identificador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;

          if (c != ' ') {
            i--;
          }
        } else if ((c == ',' || c == ' ' || c == '(' || c == ';' || c == ':' ||
                    c == '+' || c == '-' || c == '/' || c == '*' || c == ')') &&
                   comando != "") {
          estagio = 0;
          tokens[nToken].classificacao = "Identificador";
          tokens[nToken].linha = linhaAtual;
          tokens[nToken].lex = comando;
          comando = "";
          nToken++;
          if (c != ' ') {
            i--;
          }
        } else if (c != ' ' && c != '"') {
          error = true;
          mensagem = "Simbolos nao pertencentes a linguagem";
          break;
        }
      }
      int i = 0;
      for (char cc : comando) {
        if (i == 0) {
          if (cc == '{') {
            i = 1;
          }
        } else {
          if (cc == '}') {
            i = 0;
          }
        }
      }
      if (i == 1) {
        error = true;
        mensagem = "Comentario nao foi fechado";
        break;
      }

      if (estagio == 27 || estagio == 33 || estagio == 30 || estagio == 18 ||
          estagio == 22 || estagio == 17 || estagio == 10 || estagio == 54 ||
          estagio == 57 || estagio == 71 || estagio == 73 || estagio == 74 ||
          estagio == 77 || estagio == 66 || estagio == 83) {
        estagio = 0;
        tokens[nToken].classificacao = "Palavra-chave";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }
      if (estagio == 47) {
        estagio = 0;
        tokens[nToken].classificacao = "Identificador";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }
      if (comando != "") {
        estagio = 0;
        tokens[nToken].classificacao = "Identificador";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }
      if (estagio == 45) {
        estagio = 0;
        tokens[nToken].classificacao = "Numero Inteiro";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }
      if (estagio == 46) {
        estagio = 0;
        tokens[nToken].classificacao = "Numero Real";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }
      if (estagio == 86 || estagio == 90) {
        estagio = 0;
        tokens[nToken].classificacao = "Lógico";
        tokens[nToken].linha = linhaAtual;
        tokens[nToken].lex = comando;
        nToken++;
        comando = "";
      }

      estagio = 0;
      comando = "";
    }

    if (!error) {
      cout << "=====================Analisador Lexico====================="
           << endl;
      cout << setw(20) << "     Token       |" << setw(20)
           << "   Classificacao  |"
           << "  Linha  " << endl;
      cout << "-----------------------------------------------------------"
           << endl;
      for (int i = 0; i < nToken; i++) {
        cout << setw(17) << tokens[i].lex << "  |" << setw(16)
             << tokens[i].classificacao << "   |   " << tokens[i].linha << endl;
        cout << "-----------------------------------------------------------"
             << endl;
      }

      ofstream arquivo("tokens.tsv");
      for (int i = 0; i < nToken; i++) {
        arquivo << tokens[i].lex << "\\" << tokens[i].classificacao << "\\"
                << tokens[i].linha << endl;
      }
      arquivo.close();
    } else {
      cout << "Erro na linha " << linhaAtual << " - " << mensagem << endl;
    }

    return 0;
  }
};