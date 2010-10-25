//* Módulo : camara.hpp                                                         
//
// _Descrição_   : Declara classe Camara.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __CAMERA_HPP
#define __CAMERA_HPP

#include "varios.hpp"

//* classe Camara #Camara#
//Esta classe guarda informações quanto posição da câmera, a tela
//de projeção. A classe também é responsável por retornar os raios para cada pixel da tela.
class Camara{
  private:
    Vetor_3D P0, dx, dy, P_atual, P_linha,
             olho, direcao, vup, xe, ye, ze;
    int linhas, colunas, l_atual, c_atual;
 //* PreparaRaios
//Esta função _Private_ 
//calcula xe, ye, ze, dx, dy e também inicializa P0, P_atual e P_linha
    void PreparaRaios( );
  public:
 //* Construtor e Destrutor
	  Camara();
	  ~Camara();
 //* Atribuição
    void AtribuiPosicao(Vetor_3D _olho);
    void AtribuiDirecao(Vetor_3D _direcao);
    void AtribuiVUP(Vetor_3D _vup);
    void Atribui( Vetor_3D olho, Vetor_3D _direcao, Vetor_3D _vup);
    void Atribui( Vetor_3D olho, Vetor_3D _direcao, Vetor_3D _vup, int _linhas, int _colunas );
 //* Pegando os Raios
    Raio PrimeiroRaio();
    Raio ProximoRaio();
};

#endif
