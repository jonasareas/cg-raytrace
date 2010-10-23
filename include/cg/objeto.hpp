//* Módulo : objeto.hpp
//
// _Descrição_ _.
// Declara as classes *Objeto_3D*, *Esfera*, *Triangulo*, CaixaParalela.
// Declara as classes *Plano*, *PlanoX*, *PlanoY*, PlanoZ.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __OBJ_HPP
#define __OBJ_HPP

#include <cg/varios.hpp>
#include <cg/material.hpp>

//* classe Objeto_3D #Objeto_3D#
  //* Esta classe é mãe das classes Esfera, CaixaParalela e Triângulo.
class Objeto_3D {
  protected:
    int indice_textura;
  public:
 //* Construtor e Destrutor
    Objeto_3D(int _indice_textura) : indice_textura( _indice_textura ) {};
	  virtual ~Objeto_3D() {};
 //* Funções puras virtuais para serem implementadas pelos herdeiros
    virtual float Intercepta( Raio r_vis ) = 0;
    virtual Vetor_3D Normal( Vetor_3D ponto ) = 0;
 //* Função inline que retorna qual é a textura
    inline int Indice_textura() { return indice_textura; };
};

//* classe Esfera #Esfera#
class Esfera : public Objeto_3D {
  private:
    float raio;
    Vetor_3D centro;
  public:
 //* Construtor e Destrutor
    Esfera( int _indice_textura, float _raio, Vetor_3D _centro );
    ~Esfera() {};
 //* interseção e Normal
    float Intercepta( Raio r_vis );
    Vetor_3D Normal( Vetor_3D ponto );
};

//* classe Triangulo #Triangulo#
class Triangulo : public Objeto_3D {
  private:
    Vetor_3D vert1, vert2, vert3;
    Vetor_3D aresta12, aresta23, aresta31;
    Vetor_3D normal;
  public:
 //* Construtor e Destrutor
    Triangulo( int _indice_textura, Vetor_3D vertices[] );
    ~Triangulo() {};
 //* interseção e Normal
    float Intercepta( Raio r_vis );
    Vetor_3D Normal( Vetor_3D ponto );
};

//* classe Plano #Plano#
  //* Define um plano. Esta classe é mãe das classes PlanoX, PlanoY e PlanoZ.
class Plano {
  protected:
    float a, bmin, bmax, cmin, cmax;

  public:
 //* Construtor e Destrutor
    Plano( float _a, float _bmin, float _bmax, float _cmin, float _cmax ) :
      a(_a), bmin(_bmin), bmax(_bmax), cmin(_cmin), cmax(_cmax) {};
    ~Plano() {};
 //* interseção. Função pura virtual para ser implementada pelos herdeiros
    virtual float Intercepta( Raio r_vis ) = 0;
};

//* classe PlanoX #PlanoX#
  //* Este plano é perpendicular ao eixo X
class PlanoX : public Plano {
  public:
 //* Construtor e Destrutor
    PlanoX( float _x, float _ymin, float _ymax, float _zmin, float _zmax ) :
        Plano( _x,  _ymin,  _ymax,  _zmin,  _zmax ) {};
    ~PlanoX() {};
 //* interseção
    float Intercepta( Raio r_vis );
};

//* classe PlanoY #PlanoY#
  //* Este plano é perpendicular ao eixo Y
class PlanoY : public Plano {
  public:
 //* Construtor e Destrutor
    PlanoY( float _y, float _xmin, float _xmax, float _zmin, float _zmax ) :
        Plano(  _y,  _xmin,  _xmax,  _zmin,  _zmax ) {};
    ~PlanoY() {};
 //* interseção
    float Intercepta( Raio r_vis );
};

//* classe PlanoZ #PlanoZ#
  //* Este plano é perpendicular ao eixo Z
class PlanoZ : public Plano {
  public:
 //* Construtor e Destrutor
    PlanoZ( float _z, float _xmin, float _xmax, float _ymin, float _ymax ) :
        Plano(  _z,  _xmin,  _xmax,  _ymin,  _ymax ) {};
    ~PlanoZ() {};
 //* interseção
    float Intercepta( Raio r_vis );
};

//* classe CaixaParalela #CaixaParalela#
  //* A CaixaParalela tem 6 planos, contidos dois a dois nos planos:xy, xz, yz.
class CaixaParalela : public Objeto_3D {
  private:
    Vetor_3D extremo_inferior, extremo_superior; //pontos que limitam a caixa
    PlanoX *px1, *px2;
    PlanoY *py1, *py2;
    PlanoZ *pz1, *pz2;
  public:

 //* Construtor e Destrutor
    CaixaParalela( int _indice_textura, Vetor_3D _extremo_inferior,
                   Vetor_3D _extremo_superior );
    ~CaixaParalela();
 //* interseção e Normal
    float Intercepta( Raio r_vis );
    Vetor_3D Normal( Vetor_3D ponto );
};


#endif
