#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
using namespace std;

#include "../hpp/varios.hpp"
#include "../hpp/objeto.hpp"
#include "../hpp/material.hpp"
#include "../hpp/camara.hpp"
#include "../hpp/cenario.hpp"
#include "../hpp/luz.hpp"
#include "../hpp/io.hpp"

bool SalvaPPM( int linhas, int colunas, int cores, Cor_rgb pix[], char arquivo[255])
{
  FILE * arqppm;
  int i,j, indice;

  strcat(arquivo,".ppm");
  if ((arqppm = fopen(arquivo, "wb")) == NULL)
  {
    printf("\nNão foi possivel abrir o arquivo %s\n",arquivo);
    return false;
  }

  //cabeçalho de um arquivo .ppm
  fprintf(arqppm,"P6\n%d %d\n%d\n", colunas, linhas, cores);

  //gravando invertendo as linhas:
  for ( i=linhas-1; i>=0; i-- )
    for ( j=0; j<colunas; j++ )
    {
      indice = i*colunas + j;
      fprintf(arqppm,"%c%c%c", pix[indice].R(), pix[indice].G(), pix[indice].B());
    }

  printf("\nO arquivo %s foi gerado com sucesso.\n",arquivo);

  fclose(arqppm);
  return true;
}

bool LeArquivoDAT( Cenario * hcenario, Camara * hcamara, int *hlinhas, int *hcolunas, char arquivo[255])
{
  FILE * arqdat;
  tok  tag;
  char linha[162],arqname[255];

  printf("\nNome do arquivo de entrada (sem a extensao .dat):");
  scanf("%s",arqname);

  strcpy(arquivo, arqname); // salva o nome do arquivo para gravação

  strcat(arqname,".dat");

  if ((arqdat = fopen(arqname, "rt")) == NULL)
  {
    printf("\nNao foi possivel abrir o arquivo de entrada %s\n", arqname);
    return false;
  }

  fgets( linha, 7, arqdat );
  if(strcmp(linha,"RT 1.0"))
  {
    printf("\nO arquivo de entrada nao tem o formato RT 1.0\n");
    return false;
  }

  FimLinha(arqdat);  //primeira linha

  //Loop para leitura das linhas do arquivo
  tag = tok_DESCONHECIDO;
  while(fgets( linha, 161, arqdat )!=NULL)
  {
    if (linha[0]=='#')
      tag = Token(&linha[1]);  //já dispenso a #
    else
      if (linha[0]!=';' && linha[0]!='!')
        LeInfo( tag, hcenario, hcamara, hlinhas, hcolunas, linha );
  }
 
  fclose(arqdat);
  printf("\nArquivo %s lido.\n", arquivo);
  return true;
}

vector<Triangulo *> read(char file_name[255]) {
    char line[80];
    int vertices;
    int faces;
	float Xmax = -1000000;
	float Ymax = -1000000;
	float Zmax = -1000000;
	float Xmin = 1000000;
	float Ymin = 1000000;
	float Zmin = 1000000;
    
    strcpy(file_name, "dat/input.ply");

    vector<Vetor_3D> vectors;
    vector<Triangulo *> triangles;

    FILE * file = fopen(file_name, "r");
    if (!file) 
        return triangles;
    
    fgets(line, 80, file);
    if (strncmp(line, "ply", 3)) 
        return triangles;
    
    while (1) {
        fgets(line, 80, file);
        
        if (strncmp(line, "element vertex", 14) == 0) {
            sscanf(line, "element vertex %d", &vertices);
        }
        
        if (strncmp(line, "element face", 12) == 0) {
            sscanf(line, "element face %d", &faces);
        }

        if (strncmp(line, "end_header", 10) == 0) {
            break;
        }
    }
    
    for (int i =0; i < vertices; i++) {
        float x, y, z;
        fgets(line, 80, file);
        sscanf(line, "%f %f %f", &x, &y, &z);
        vectors.push_back(Vetor_3D(x, y, z));
    }
    
    printf("vectors size = %d\n", vectors.size());

	// ViewPort

	// Calcula extremiadades da malha de triangulos
	for (unsigned int i = 0; i < vectors.size(); i++) {
		Xmax = MAXI(vectors[i].X(), Xmax);
		Xmin = MINI(vectors[i].X(), Xmin);	
		Ymax = MAXI(vectors[i].Y(), Ymax);
		Ymin = MINI(vectors[i].Y(), Ymin);	
		Zmax = MAXI(vectors[i].Z(), Zmax);
		Zmin = MINI(vectors[i].Z(), Zmin);	
		printf("%f %f %f\n", vectors[i].X(), vectors[i].Y(), vectors[i].Z());
	}

	// Executa o viewport
	for (unsigned int i = 0; i < vectors.size(); i++) {
		vectors[i].Atribui(
			(vectors[i].X() * (MAX_RES / (2*(Xmax - Xmin))) + ((MAX_RES / (2*(Xmax - Xmin))) * -Xmin + MAX_RES/4)),
			(vectors[i].Y() * (MAX_RES / (2*(Ymax - Ymin))) + ((MAX_RES / (2*(Ymax - Ymin))) * -Ymin + MAX_RES/4)),
			(vectors[i].Z() * (MAX_RES / (2*(Zmax - Zmin))) + ((MAX_RES / (2*(Zmax - Zmin))) * -Zmin + MAX_RES/4)));	
		printf("%f %f %f\n", vectors[i].X(), vectors[i].Y(), vectors[i].Z());
	}

    for (int i = 0; i < faces; i++) {
        int n, m, l;

        fgets(line, 80, file);
        if(!sscanf(line, "3 %d %d %d", &n, &m, &l)) {
            printf("Didnt match!\n");
        }

        Vetor_3D v[3];
        v[0].Copia(vectors[n]);
        v[1].Copia(vectors[m]);
        v[2].Copia(vectors[l]);

        triangles.push_back(new Triangulo(12, v));
    }
    
    printf("triangles size = %d\n", triangles.size());
    
    return triangles;
}

bool LeArquivoPLY( Cenario * hcenario, char arquivo[255]) {
    vector<Triangulo *> triangles = read(arquivo);

    for (unsigned int i = 0; i < triangles.size(); i++) {
        hcenario->InsereObjeto(triangles[i]);
    }

    return true;
}

//Dispensa fim da linha
void FimLinha(FILE * arqdat)
{
  char ch;
  while((ch=fgetc(arqdat))!='\n');
}

//Reconhece o token
tok Token(char linha[82])
{

  linha[10]=0;
  if(!strcmp(linha,"BACKGROUND")) return tok_BACKGROUND;

  linha[8]=0;
  if(!strcmp(linha,"POSITION")) return tok_POSITION;

  if(!strcmp(linha,"TRIANGLE")) return tok_TRIANGLE;

  linha[7]=0;
  if(!strcmp(linha,"SURFACE")) return tok_SURFACE;

  linha[6]=0;
  if(!strcmp(linha,"SPHERE")) return tok_SPHERE;

  linha[5]=0;
  if(!strcmp(linha,"LIGHT")) return tok_LIGHT;

  linha[4]=0;
  if(!strcmp(linha,"SIZE")) return tok_SIZE;

  linha[3]=0;
  if(!strcmp(linha,"BOX")) return tok_BOX;

  return tok_DESCONHECIDO;
}

void LeInfo( tok tag, Cenario *hcenario, Camara *hcamara, int *hlinhas, int *hcolunas, char *linha )
{
  int a,b,c;
  float e, f, g;
  Cor_rgb cor1,cor2,cor3;
  Vetor_3D vet1, vet2, vet3, vets[3];
  Material *mat1;
  Luz *luz1;
  Esfera *esf1;
  CaixaParalela *cxp1;
  Triangulo *tri1;
  int v=0;
  
  switch(tag)
  {
    case tok_SIZE:
      sscanf(linha,"%d %d",hlinhas,hcolunas);
      //*hlinhas = MINI(*hlinhas,MAX_RES);      //Para não estourar a minha
      //*hcolunas = MINI(*hcolunas,MAX_RES);    //matriz[MAX_RES*MAX_RES]
	*hlinhas = MAX_RES;
	*hcolunas =  MAX_RES;
      return;
    case tok_BACKGROUND:
      cor1.Copia(LeCor(linha));
      hcenario->RecebeFundo(cor1);
      return;
    case tok_SURFACE:
      cor1.Copia(LeCor(linha));     //Cor da iluminação ambiente
      v = Dispensa(linha,3);
      cor2.Copia(LeCor(&linha[v])); //Cor da difusa
      v += Dispensa(&linha[v],3);
      cor3.Copia(LeCor(&linha[v])); //Cor da especular
      v += Dispensa(&linha[v],3);
      sscanf(&linha[v],"%d. %f %f %f",&a,&e,&f,&g);
      mat1 = new Material( cor1, cor2, cor3, a, e, f, g );
      hcenario->InsereMaterial( mat1 );
      return;
    case tok_LIGHT:
      vet1.Copia(LeVetor(linha));   //Posição
      v = Dispensa(linha,3);
      sscanf(&linha[v],"%d %d %d",&a,&b,&c);
      //Esta cor não pode ser lida como as demais por não ter '.'
      cor1.Atribui(a, b, c);           //Cor
      luz1 = new Luz( vet1, cor1 );
      hcenario->InsereLuz( luz1 );
      return;
    case tok_SPHERE:
      sscanf(linha,"%d",&a);
      v = Dispensa(linha,1);
      sscanf(&linha[v],"%f",&e);      //Raio
      v += Dispensa(&linha[v],1);
      vet1.Copia(LeVetor(&linha[v])); //Centro  
      esf1 = new Esfera( a, e, vet1 );
      hcenario->InsereObjeto( esf1 );
      return;
    case tok_BOX:
      sscanf(linha,"%d",&a);
      v = Dispensa(linha,1);
      vet1.Copia(LeVetor(&linha[v])); //canto inferior esquerdo
      v += Dispensa(&linha[v],3);
      vet2.Copia(LeVetor(&linha[v])); //canto superior direito
      cxp1 = new CaixaParalela( a, vet1, vet2 );
      hcenario->InsereObjeto( cxp1 );
      return; 
    case tok_TRIANGLE:
      sscanf(linha,"%d",&a);
      v = Dispensa(linha,1);
      vets[0].Copia(LeVetor(&linha[v]));//primeiro vértice
      v += Dispensa(&linha[v],3);
      vets[1].Copia(LeVetor(&linha[v]));//segundo vértice
      v += Dispensa(&linha[v],3);
      vets[2].Copia(LeVetor(&linha[v]));//terceiro vértice
      tri1 = new Triangulo( a, vets );
      hcenario->InsereObjeto( tri1 );
      return;
    case tok_POSITION:
      vet1.Copia(LeVetor(linha));       //eye
      v = Dispensa(linha,3);
      vet2.Copia(LeVetor(&linha[v]));   //ref
      v += Dispensa(&linha[v],3);
      vet3.Copia(LeVetor(&linha[v]));   //up
      hcamara->Atribui( vet1, vet2, vet3, *hlinhas, *hcolunas );
      return;
    case tok_DESCONHECIDO:
      return;
  }
}
  
//quantos = quantos números serão dispensados
int Dispensa(char *linha, int quantos)
{
  int i=0;
  while (quantos>0)
  {
    i++;
    if (linha[i]==' ')
    {
      quantos--;
      while (linha[++i]==' ');
    }
  }
  return i;
}

//As duas funções abaixo são apenas facilitadoras na leitura
Cor_rgb LeCor(char *linha)
{
  Cor_rgb cor;
  int a,b,c;

  sscanf(linha,"%d. %d. %d.",&a,&b,&c);
  cor.Atribui(a,b,c);
  return cor;
}

Vetor_3D LeVetor(char *linha)
{
  Vetor_3D vet;
  float a,b,c;

  sscanf(linha,"%f %f %f",&a,&b,&c);
  vet.Atribui(a,b,c);
  return vet;
}


