#include <math.h>
#include "../hpp/camara.hpp"

Camara::Camara( )
{
  linhas = 200;
  colunas = 200;
  olho.Atribui( 0.0, 0.0, 0.0);
  direcao.Atribui( 0.0, 0.0, -1.0 );
  vup.Atribui(0.0, 1.0, 0.0 );

  PreparaRaios();
}

void Camara::Atribui( Vetor_3D _olho, Vetor_3D _direcao, Vetor_3D _vup, int _linhas, int _colunas )
{
  linhas = _linhas;
  colunas = _colunas;
  olho.Copia( _olho );
  direcao.Copia( _direcao );
  vup.Copia( _vup );

  PreparaRaios();
}

void Camara::PreparaRaios( void )
{
  // Determina xe, ye, ze
  ze.Copia( olho );
  ze.Subtrai( direcao );
  ze.Normaliza();
  xe = vup.ProdutoVetorial( ze );
  xe.Normaliza();
  ye = ze.ProdutoVetorial( xe );
  
  // Acha PU
  P0.Copia( olho );
  P0.Subtrai( ze );
  P0.Subtrai( ye );
  P0.Subtrai( xe );
  
  // dx/2 e dy/2
  dx.Copia ( xe );
  dx.Divide( (float) colunas );
  dy.Copia ( ye );
  dy.Divide( (float) linhas );

  // Acha P0
  P0.Soma( dx );
  P0.Soma( dy );

  // Acha dx e dy
  dx.Divide ( 0.5 );
  dy.Divide ( 0.5 );

  P_atual.Copia( P0 );
  P_linha.Copia( P0 );
  l_atual = c_atual = 0;
}

Camara::~Camara()
{}

Raio Camara::PrimeiroRaio()
{
  Raio retorno( olho, P0 );
  l_atual = c_atual = 0;
  return (retorno);
}

Raio Camara::ProximoRaio()
{
  Raio retorno;
  c_atual++;

  //Anda na linha
  if ( c_atual < colunas )
    P_atual.Soma( dx );
  //Anda na coluna  
  else
  {
    c_atual = 0;
    l_atual++;
    if ( l_atual < linhas )
    {
      P_linha.Soma( dy );
      P_atual.Copia( P_linha );
    }
    else
    //Se chegou no fim, então recomeça
    {
      l_atual = 0;
      P_linha.Copia( P0 );
      P_atual.Copia( P0 );
    }
  }
  retorno.Atribui( olho, P_atual );
  return (retorno);
}
