//* Módulo : io.hpp                                                         
//
// _Descrição_  _.
// Responsável pelo *parsing* de leitura do arquivo de entrada.
// Responsável pela gravação do arquivo de saída (.ppm).
// _Autor_: Rodrigo P. R. de Toledo

#include "varios.hpp"
#include "objeto.hpp"
#include "camara.hpp"
#include "cenario.hpp"
#include "luz.hpp"

//* GRAVAÇÃO
  //* A função SalvaPPM é usada para a geração do arquivo de saída.
bool SalvaPPM( int linhas, int colunas, int cores, Cor_rgb pix[], char arquivo[255]);

//* LEITURA
  //* Estas funções são usadas para a leitura do arquivo de entrada. 
//A leitura é feita linha a linha isoladamente.
//Existe sempre um token corrente que será trocado ao se encontrar outro
//(um dos tokens é "desconhecido")_. 
//OBS: Não é possível dividir os números de um único elemento em duas linhas
//e nem colocar informação na mesma linha do token.

 //* LeArquivo
  //* Esta função é a única que interage diretamente com o arquivo de entrada! 
//Cada linha é lida para uma string e as demais funções
//passam a interagir com essa string. 
//Esta função descobre se a linha é um início de um novo conjunto
//de elementos (#token) ou se é uma linha com as informações a serem lidas.
bool LeArquivo( Cenario * todos, Camara * cam, int *lines, int *cols, char arquivo[255]);

 //* Token
  //* Reconhece qual é o token.
tok Token(char linha[82]);

 //* LeInfo
  //* Lê as informações que estão na string de acordo com o token corrente
void LeInfo( tok i, Cenario *hcenario, Camara *hcamara, int *hh, int *hw, char *linha );

 //* FimLinha
  //* Dispensa o fim da linha
void FimLinha(FILE * arqdat);

 //* Dispensa
  //* Dispensa da linha as informações que já foram lidas. 
  //* Na verdade esta função retorna a partir de que ponto a linha deve
  //* continuar a ser interpretada.
int Dispensa(char *linha, int quantos);

 //* Funções para agrupar a leitura de (x y z) e de (r g b).
Vetor_3D LeVetor(char *linha);
Cor_rgb LeCor(char *linha);
