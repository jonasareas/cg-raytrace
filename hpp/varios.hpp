//* M�dulo : varios.hpp
//
// _Descri��o_ _.
// Declara as classes *Cor_rgb*, *Vetor_3D* e *Raio* _. 
// Declara *Macros*, *Constantes* e *Enumerados* _. 
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __COR_HPP
#define __COR_HPP

//* constantes #Constantes#
  //* O ZERO � apenas um n�mero bem perto de 0.
#define ZERO 0.0001
  //* Estas constantes estipulam limites.
  //* #MAXLUZ# 
  //* #MAXOBJ# 
  //* #MAX_RES# 
  //* #MAX_PROFUNDIDADE#
#define MAX_PROFUNDIDADE 4
#define MAX_RES 600
#define MAXOBJ 5000
#define MAXLUZ 10

//* macros #Macros#
//Macros usadas para simplificar a programa��o.
#define SQR(X) (X*X)
#define MAXI(x,y) (x>y?x:y)
#define MINI(x,y) (x<y?x:y)
#define PROXIMO(a,b) ((a-b<ZERO)&&(a-b>-ZERO))
#define INSIDE(a,b,c) (((b>a)&&(a>c))||((b<a)&&(a<c)))
// inside: "a est� entre b e c?"   (booleano)

//--------------------
//* enumerados #Enumerados#
  //* O _tok_ enumera as op��es de token do arquivo de entrada.
typedef enum {
  tok_DESCONHECIDO = 0,
  tok_SIZE,
  tok_BACKGROUND,
  tok_SURFACE,
  tok_LIGHT,
  tok_SPHERE,
  tok_BOX,
  tok_TRIANGLE,
  tok_POSITION
}tok;

//--------------------
//* classe Cor_rgb #Cor_rgb#
  //* Contem r g b em tr�s unsigned char e faz opera��es sobre cor.
class Cor_rgb{
  private:
    unsigned char r,g,b;
 //* Fun��o _Private_ que trunca para x ficar entre 0 e 255
    unsigned char Acerta(int x);
  public:

 //* Construtores e Destrutor
    Cor_rgb() : r(0), g(0), b(0) {};
    Cor_rgb(int _r, int _g, int _b);
    ~Cor_rgb() {};
 //* Atribui��es dos valores de r g b
    void Atribui(int _r, int _g, int _b);
    void Copia(Cor_rgb origem);
 //* Opera��es sobre cor
    void Multiplica(float ind);
    void Indexa(Cor_rgb indice);
    void Soma(Cor_rgb outra);
 //* Fun��es inline que retornam as cores
    inline unsigned char R()  {  return(r); };
    inline unsigned char G()  {  return(g); };
    inline unsigned char B()  {  return(b); };
};

//* classe Vetor_3D #Vetor_3D#
  //* Vetor tridimensional com valores x y z guardados em float
class Vetor_3D{
  private:
    float x, y, z;
  public:

 //* Construtores e Destrutor
    Vetor_3D() : x(0.0), y(0.0), z(0.0) {};
    Vetor_3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    ~Vetor_3D() {};
 //* Atribui��es dos valores de x y z
    void Atribui(float _x, float _y, float _z);
    void Copia(Vetor_3D origem);
 //* Opera��es sobre vetor
    void Soma(Vetor_3D origem);
    void Subtrai(Vetor_3D origem);
    void Divide(float divi);
    void Multiplica(float multi);
    void Negativo();
    void Normaliza();
 //* Fun��es que retornam alguma informa��o sobre o vetor
    float Comprimento();
    float ProdutoEscalar(Vetor_3D outro);
    Vetor_3D ProdutoVetorial(Vetor_3D outro);
 //* Fun��es inline que retornam os valores de x y z
    inline float X()  {  return(x); };
    inline float Y()  {  return(y); };
    inline float Z()  {  return(z); };
};

//* classe Raio #Raio#
  //* Contem 3 vetores tridimensionais
class Raio{
  private:
    Vetor_3D origem, destino, direcao;
  public:
 //* Construtores e Destrutor
    Raio();
    Raio(Vetor_3D _origem, Vetor_3D _destino);
    ~Raio() {};
 //* Atribui��o dos vetores origem e destino
    void  Atribui(Vetor_3D _origem, Vetor_3D _destino);
 //* Fun��es inline que retornam informa��es do Raio
    inline Vetor_3D Origem()  {return origem;};
    inline Vetor_3D Destino() {return destino;};
    inline Vetor_3D Direcao() {return direcao;};
    inline float    Dx()      {return direcao.X();};
    inline float    Dy()      {return direcao.Y();};
    inline float    Dz()      {return direcao.Z();};
    inline float    X0()      {return origem.X();};
    inline float    Y0()      {return origem.Y();};
    inline float    Z0()      {return origem.Z();};
 //* QualPonto retorna o ponto x y z para um t da fun��o param�trica
    Vetor_3D QualPonto(float t);
};

#endif
