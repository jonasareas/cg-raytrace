//* M�dulo : io.hpp                                                         
//
// _Descri��o_  _.
// Respons�vel pelo *parsing* de leitura do arquivo de entrada.
// Respons�vel pela grava��o do arquivo de sa�da (.ppm).
// _Autor_: Rodrigo P. R. de Toledo

#include "varios.hpp"
#include "objeto.hpp"
#include "camara.hpp"
#include "cenario.hpp"
#include "luz.hpp"

//* GRAVA��O
  //* A fun��o SalvaPPM � usada para a gera��o do arquivo de sa�da.
bool SalvaPPM( int linhas, int colunas, int cores, Cor_rgb pix[], char arquivo[255]);

//* LEITURA
  //* Estas fun��es s�o usadas para a leitura do arquivo de entrada. 
//A leitura � feita linha a linha isoladamente.
//Existe sempre um token corrente que ser� trocado ao se encontrar outro
//(um dos tokens � "desconhecido")_. 
//OBS: N�o � poss�vel dividir os n�meros de um �nico elemento em duas linhas
//e nem colocar informa��o na mesma linha do token.

 //* LeArquivo
  //* Esta fun��o � a �nica que interage diretamente com o arquivo de entrada! 
//Cada linha � lida para uma string e as demais fun��es
//passam a interagir com essa string. 
//Esta fun��o descobre se a linha � um in�cio de um novo conjunto
//de elementos (#token) ou se � uma linha com as informa��es a serem lidas.
bool LeArquivo( Cenario * todos, Camara * cam, int *lines, int *cols, char arquivo[255]);

 //* Token
  //* Reconhece qual � o token.
tok Token(char linha[82]);

 //* LeInfo
  //* L� as informa��es que est�o na string de acordo com o token corrente
void LeInfo( tok i, Cenario *hcenario, Camara *hcamara, int *hh, int *hw, char *linha );

 //* FimLinha
  //* Dispensa o fim da linha
void FimLinha(FILE * arqdat);

 //* Dispensa
  //* Dispensa da linha as informa��es que j� foram lidas. 
  //* Na verdade esta fun��o retorna a partir de que ponto a linha deve
  //* continuar a ser interpretada.
int Dispensa(char *linha, int quantos);

 //* Fun��es para agrupar a leitura de (x y z) e de (r g b).
Vetor_3D LeVetor(char *linha);
Cor_rgb LeCor(char *linha);
