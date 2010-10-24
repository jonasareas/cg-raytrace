//* Módulo : main.cpp                                                         
//
// _Descrição_ : Contém uma câmera um Cenario e controla o loop principal.
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
    //* Variáveis:
    int linhas,colunas;  // número de linhas e colunas da imagem
    int i, j, k=0; // contadores para linhas, colunas e posição no vetor, respectivamente
    int percent;   // percentagem da imagem processada
    char arquivo[255]; // entrada:  arquivo.dat, saída: arquivo.ppm
    Cor_rgb matrix[MAX_RES*MAX_RES]; // matrix de cores

    //* Objetos:
    Camara camara;    // cria uma câmera default
    Cenario cenario;  // cria o cenário
    Raio raio;        // define o raio que será usado

    //* Código:
    //* Leitura do Arquivo
    if(!LeArquivoDAT( &cenario, &camara, &linhas, &colunas, arquivo))
        return 1;
    
    LeArquivoPLY(&cenario, arquivo);
    
    while(1) {

        //* Pega o primeiro raio  
        raio = camara.PrimeiroRaio();

        //* Realiza o loop
        printf("\nPercentual de linhas computadas:\n");
        for(i=0;i<linhas;i++)  {
            for(j=0;j<colunas;j++) {
                printf("k= %d\n", k);
                matrix[k++] = cenario.Intercepta( raio, 0 );
                raio = camara.ProximoRaio();
            }
            percent = (i+1)*100/linhas;
            putc(13,stdout);
            printf("%3d%%", percent);
        }
        printf("\n");

        //* Salva o arquivo com a imagem no formato .ppm
        // SalvaPPM( linhas, colunas, 255, matrix, arquivo);
        plota(linhas, colunas, 255, matrix);
        
        float x, y, z;
        char d[10];

        printf("Deseja reposicionar a camera? [s,n]\n");
        scanf("%s", d);
        if (d[0] == 'n') break;

        printf("Entre com a nova posicao da camera (formato=%%f %%f %%f):");
        scanf("%f %f %f", &x, &y, &z);
        Vetor_3D pos(x, y, z);

        printf("Entre com a nova direção da camera (formato=%%f %%f %%f):");
        scanf("%f %f %f", &x, &y, &z);
        Vetor_3D dir(x, y, z);

        printf("Entre com a novo olho da camera (formato=%%f %%f %%f):");
        scanf("%f %f %f", &x, &y, &z);
        Vetor_3D olho(x, y, z);

        camara.Atribui(pos, dir, olho);
        k = 0;
    }
    return 0;
}
END_OF_MAIN();
