#include <math.h>
#include "../hpp/varios.hpp"

Cor_rgb::Cor_rgb(int _r, int _g, int _b)
{
  r= Acerta(_r);
  g= Acerta(_g);
  b= Acerta(_b);
}

unsigned char Cor_rgb::Acerta(int x)
{
  if ( x<0 )
    return (unsigned char)0;
  if ( x>255 )
    return (unsigned char)255;
  return (unsigned char)x;
}

void Cor_rgb::Atribui(int _r, int _g, int _b)
{
  r= Acerta(_r);
  g= Acerta(_g);
  b= Acerta(_b);
}
  
void Cor_rgb::Copia(Cor_rgb origem)
{
  r= origem.R();
  g= origem.G();
  b= origem.B();
}
  
void Cor_rgb::Multiplica(float ind)
{
  r = Acerta( (int) ( (float) r * ind ) );
  g = Acerta( (int) ( (float) g * ind ) );
  b = Acerta( (int) ( (float) b * ind ) );
}

void Cor_rgb::Indexa(Cor_rgb indice)
{
  r = Acerta( (int) ( (float) r * ( (float) indice.R() / 255 ) ) );
  g = Acerta( (int) ( (float) g * ( (float) indice.G() / 255 ) ) );
  b = Acerta( (int) ( (float) b * ( (float) indice.B() / 255 ) ) );
}

void Cor_rgb::Soma(Cor_rgb outra)
{
  int t;
  t = outra.R() + (int)r;
  r = Acerta( t );
  t = outra.G() + (int)g;
  g = Acerta( t );
  t = outra.B() + (int)b;
  b = Acerta( t );
}

// implementação do Vetor_3D

void Vetor_3D::Copia(Vetor_3D origem)
{
  x = origem.x;
  y = origem.y;
  z = origem.z;
}

void Vetor_3D::Atribui(float _x, float _y, float _z)
{
  x = _x;
  y = _y;
  z = _z;
}

void Vetor_3D::Soma(Vetor_3D origem)
{
  x += origem.x;
  y += origem.y;
  z += origem.z;
}

void Vetor_3D::Subtrai(Vetor_3D origem)
{
  x -= origem.x;
  y -= origem.y;
  z -= origem.z;
}

//A função Divide(x) às vezes é usada como "Multiplica(1/x)"
void Vetor_3D::Divide(float divi)
{
  x /= divi;
  y /= divi;
  z /= divi;
}

void Vetor_3D::Multiplica(float multi)
{
  x *= multi;
  y *= multi;
  z *= multi;
}

void Vetor_3D::Negativo()
{
  x = -x;
  y = -y;
  z = -z;
}

inline float Vetor_3D::Comprimento()
{
  return ( (float)sqrt( x*x + y*y + z*z ) );
}

//divide cada componente pelo comprimento.
void Vetor_3D::Normaliza()
{
  float aux;
  aux = Comprimento();
  x = x/aux;
  y = y/aux;
  z = z/aux;
}

Vetor_3D Vetor_3D::ProdutoVetorial(Vetor_3D outro)
{
  Vetor_3D retorno( y*outro.z - z*outro.y,  z*outro.x - x*outro.z, x*outro.y - y*outro.x );
  return (retorno);
}

//valor retornado em radianos.
float Vetor_3D::ProdutoEscalar(Vetor_3D outro)
{
  return ( x*outro.x +  y*outro.y + z*outro.z );
}

// implementação do Raio
Raio::Raio()
{}

Raio::Raio(Vetor_3D _origem, Vetor_3D _destino)
{
  origem.Copia(_origem);
  destino.Copia(_destino);
  direcao.Atribui ( destino.X() - origem.X(),
                 destino.Y() - origem.Y(),
                 destino.Z() - origem.Z() );
}

//Retorna o ponto para este t aplicado na função paramétrica
Vetor_3D Raio::QualPonto(float t)
{
  Vetor_3D ponto;
  
  ponto.Atribui ( origem.X() + t*direcao.X(),
               origem.Y() + t*direcao.Y(),
               origem.Z() + t*direcao.Z() );

  return ponto;
}

void Raio::Atribui(Vetor_3D _origem, Vetor_3D _destino)
{
  origem.Copia(_origem);
  destino.Copia(_destino);
  direcao.Atribui ( destino.X() - origem.X(),
                 destino.Y() - origem.Y(),
                 destino.Z() - origem.Z() );

}
