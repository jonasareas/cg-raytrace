//* M�dulo : main.cpp                                                         
//
// _Descri��o_ : Cont�m uma c�mera um Cenario e controla o loop principal.
// _Autor_: Rodrigo P. R. de Toledo
// _Revisor_: Marcelo Gattass

 //* Includes:
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../hpp/varios.hpp"
#include "../hpp/objeto.hpp"
#include "../hpp/camara.hpp"
#include "../hpp/cenario.hpp"
#include "../hpp/luz.hpp"
#include "../hpp/io.hpp"
#include "../hpp/allegro.hpp"

//* MAIN
int main(){
    //* Vari�veis:
    int linhas,colunas;  // n�mero de linhas e colunas da imagem
    int i, j, k=0; // contadores para linhas, colunas e posi��o no vetor, respectivamente
    int percent;   // percentagem da imagem processada
    char arquivo[255]; // entrada:  arquivo.dat, sa�da: arquivo.ppm
    Cor_rgb matrix[MAX_RES*MAX_RES]; // matrix de cores

    //* Objetos:
    Camara camara;    // cria uma c�mera default
    Cenario cenario;  // cria o cen�rio
    Raio raio;        // define o raio que ser� usado

    //* C�digo:
    //* Leitura do Arquivo
    if(!LeArquivoDAT( &cenario, &camara, &linhas, &colunas, arquivo))
        return 1;
    
    LeArquivoPLY(&cenario, arquivo);
    
    //* Pega o primeiro raio  
    raio = camara.PrimeiroRaio();

    //* Realiza o loop
    printf("\nPercentual de linhas computadas:\n");
    for(i=0;i<linhas;i++)  {
        for(j=0;j<colunas;j++) {
            matrix[k++] = cenario.Intercepta( raio, 0 );
            raio = camara.ProximoRaio();
        }
        percent = (i+1)*100/linhas;
        putc(13,stdout);
        printf("%3d%%", percent);
    }
    printf("\n");
    
    while(1) {
        //* Salva o arquivo com a imagem no formato .ppm
        // SalvaPPM( linhas, colunas, 255, matrix, arquivo);
        plota(linhas, colunas, 255, matrix);

        
    }
    return 0;
}
END_OF_MAIN();
