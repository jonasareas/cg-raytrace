#include <math.h>
#include <vector>
#include "../hpp/cenario.hpp"

Cenario::Cenario()
{
  total = 0;
  total_luzes = 0;
  total_materiais = 0;
  total_boxes = 0;
  cor_de_fundo.Atribui(60,60,60);
}

Cenario::~Cenario()
{
  Zera();
}

void Cenario::Zera()
{
  int i;
  for (i=0;i<total;delete(objetos[i]),i++);
  for (i=0;i<total_luzes;delete(luzes[i]),i++);
  for (i=0;i<total_materiais;delete(materiais[i]),i++);
  total=total_luzes=total_materiais=0;
}

void Cenario::RecebeFundo( Cor_rgb _cor_de_fundo )
{
  cor_de_fundo = _cor_de_fundo;
}

bool Cenario::InsereObjeto( Objeto_3D *mais_um )
{
  if( total+1 == MAXOBJ )
    return false;
  
  objetos[total] = mais_um;

  total++;

  return true;
}

bool Cenario::InsereMaterial( Material *mais_um )
{
  if( total_materiais+1 == MAXOBJ )
    return false;
  
  materiais[total_materiais] = mais_um;

  total_materiais++;

  return true;
}

bool Cenario::InsereBoundingBox( CaixaParalela *mais_uma )
{
  if( total_boxes+1 == MAXOBJ )
    return false;
  
  box[total_boxes] = mais_uma;

  total_boxes++;

  return true;
}

bool Cenario::InsereLuz( Luz *mais_uma )
{
  if( total_luzes+1 == MAXLUZ )
    return false;
  
  luzes[total_luzes] = mais_uma;

  total_luzes++;

  return true;
}

//------------------------------------------//
// Correspondente ao TRACE da transpar�ncia //
//------------------------------------------//
Cor_rgb Cenario::Intercepta( std::vector<CaixaParalela *> bounding_boxes, Raio r_vis, int profundidade )
{
  float t=-1, t_atual, t_min;
  int i=0, objeto_proximo=-1;
  Vetor_3D ponto;

  //Verifico se � o primeiro raio, se for s� me interessa o que
  //estiver depois da tela de proje��o
  //(que tem valor 1 na fun��o param�trica do raio).
  if ( profundidade == 0 )
    t_min = 1;
  else
    t_min = (float)ZERO;//0.0001 para n�o iterceptar com a pr�pria origem do raio

  //Percorro todos os objetos perguntando se houve interse��o e se
  //a interse��o � mais perto do que a mais perto que eu tiver at� o momento.
  //Quando n�o h� interse��o o objeto retorna -1.

  for(i=0;i<total;i++)
  {
	for (unsigned int k = 0; k < bounding_boxes.size(); k++) {
		if (objetos[i]->BoundingBox() == bounding_boxes[k]) {
			t_atual = objetos[i]->Intercepta( r_vis );
			if ( (t_atual > t_min) && ( objeto_proximo==-1  ||  t_atual < t ) )
			{
			  t = t_atual;
			  objeto_proximo = i;
			}
		}
	 }
  }

  //Houve interse��o? (sem interse��o: objeto_proximo = -1)
  if (objeto_proximo>=0)
  {
    ponto = r_vis.QualPonto(t);
    return( CorDoPonto( bounding_boxes, objeto_proximo, ponto, r_vis, profundidade ) );
  }
  else
    return( cor_de_fundo );

}

//------------------------------------------//
// Correspondente ao SHADE da transpar�ncia // 
//------------------------------------------//
Cor_rgb Cenario::CorDoPonto(std::vector<CaixaParalela *> bounding_boxes, int num_obj, Vetor_3D ponto, Raio r_vis, int profundidade)
{
  int i, j=0, bool_sombra, ind_textura;
  float nl, t, rv, coef_reflexao, nv;

  Cor_rgb final, intermediaria, ilumina;
  Vetor_3D normal_ponto, direcao_luz, reflexao_luz, direcao_vista, reflexao_vista;
  Raio sombra, reflexao;

  //== c�lculos iniciais ==//
  normal_ponto  =  objetos[num_obj]->Normal(ponto);
  ind_textura   =  objetos[num_obj]->Indice_textura();
  // calcula a dire��o da vista normalizada para usar no c�lculo da difusa
  direcao_vista.Copia( r_vis.Origem() );
  direcao_vista.Subtrai( r_vis.Destino() );
  direcao_vista.Normaliza();
  //=======================//


  //== loop para todas as luzes (c�lculo da difusa e especular) ==/
  for(i=0;i<total_luzes;i++)
  {
    bool_sombra = 0;
    direcao_luz.Copia(luzes[i]->Posicao());

    // Verifica se h� sombra para a luz i

    sombra.Atribui(ponto, direcao_luz);
    for(j=0;j<total;j++)
      if ( j != num_obj )
      {
        t = objetos[j]->Intercepta( sombra );
        if ( INSIDE(t,ZERO,1) ) //0.00001 para evitar sombra de objetos sobrepostos
        {
          bool_sombra = 1;
          break;
        }      
      }

    // Se n�o houver sombra
    if ( !bool_sombra )
    {
      //calcula a difusa

      direcao_luz.Subtrai(ponto);
      direcao_luz.Normaliza();
      nl = direcao_luz.ProdutoEscalar( normal_ponto );
      intermediaria.Copia(luzes[i]->Intensidade());
      intermediaria.Multiplica(nl);
      ilumina.Copia(materiais[ind_textura]->Cor_difusa());
      ilumina.Indexa(intermediaria);
      final.Soma(ilumina);
      
      //calcula a especular

      reflexao_luz.Copia(normal_ponto);  //
      reflexao_luz.Divide(1/(nl*2));     //Foley 16.16
      reflexao_luz.Subtrai(direcao_luz); // 
      rv = reflexao_luz.ProdutoEscalar(direcao_vista);  //Foley 16.17
      if ( rv > 0.1 )
      {
        rv = (float)pow(rv,materiais[ind_textura]->N_especular());
        ilumina.Copia(materiais[ind_textura]->Indice_especular());
        ilumina.Multiplica(rv);
        final.Soma(ilumina);
      }
    }
  }

  //calcula a ambiente

    final.Soma(materiais[ind_textura]->Ambiente());

  //calcula a reflex�o

  coef_reflexao = materiais[ind_textura]->Coeficiente_reflexao();
  if ( coef_reflexao > 0  && profundidade < MAX_PROFUNDIDADE )
  {
     nv = direcao_vista.ProdutoEscalar( normal_ponto );
     reflexao_vista.Copia(normal_ponto);    //
     reflexao_vista.Divide(1/(nv*2));       //Foley 16.16
     reflexao_vista.Subtrai(direcao_vista); // 
     reflexao_vista.Soma(ponto);
     reflexao.Atribui(ponto,reflexao_vista);
     intermediaria = Intercepta( bounding_boxes, reflexao, profundidade + 1 );
     intermediaria.Multiplica( coef_reflexao );
     final.Soma(intermediaria);
  }

  return final;
}

std::vector<CaixaParalela *> Cenario::interceptaBoundingBox(Raio r_vis) {

	std::vector<CaixaParalela *> bounding_boxes;

	for (int i =0; i < total_boxes; i++) {
		if (box[i]->Intercepta(r_vis) != -1)
			bounding_boxes.push_back(box[i]);
	}
	return bounding_boxes;
}
