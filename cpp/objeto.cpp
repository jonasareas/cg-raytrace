#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "../hpp/objeto.hpp"

Esfera::Esfera(int _indice_textura, float _raio, Vetor_3D _centro) : Objeto_3D ( _indice_textura )
{
    raio = _raio;
    centro.Copia( _centro );
}

//Foley capítulo 15.10, fórmula 15.17
float Esfera::Intercepta( Raio r_vis )
{
    float distx, disty, distz, a, b, c, delta, raiz1, raiz2;

    // valores intermediários
    distx = r_vis.X0() - centro.X();
    disty = r_vis.Y0() - centro.Y();
    distz = r_vis.Z0() - centro.Z();

    // montando a equação do 2º grau at2 + bt + c = 0
    a = SQR(r_vis.Dx()) + SQR(r_vis.Dy()) + SQR(r_vis.Dz());
    b = 2 * ( r_vis.Dx() * distx + r_vis.Dy() * disty + r_vis.Dz() * distz );
    c = SQR(distx) + SQR(disty) + SQR(distz) - SQR(raio);

    // Calculando Delta
    delta = SQR(b) - 4*a*c;


    // Retornando t (se não existir t retornar valor negativo que será ignorado)
    if ( delta < 1 )
        return(-1);

    // raiz1 = ( -b - (float)sqrt(delta) ) / (2*a);
    // raiz2 = ( -b + (float)sqrt(delta) ) / (2*a);
    delta = (float)sqrt(delta);
    a*=2;
    raiz1 = ( -b - delta ) / a;
    raiz2 = ( -b + delta ) / a;

    return(MINI(raiz1,raiz2));
    // ATENÇÃO!!!
    // Como escolho a interseção de menor valor, podem ocorrer 
    // exceções que não são bem tratadas. Por exemplo, uma
    // esfera que contenha a camara e a tela de projeção.
    // neste caso, será retornada a interseção t<0, mesmo havendo
    // um t>1, que deveria ser retornado.

}

Vetor_3D Esfera::Normal( Vetor_3D ponto )
{
    ponto.Subtrai(centro);
    ponto.Normaliza();
    return ponto;
}

float PlanoX::Intercepta( Raio r_vis )
{
    float t,y,z;

    //calcula-se o t da interseção do raio com o plano
    t = (a - r_vis.X0()) / r_vis.Dx();

    if (t<0)
        return(-1);

    //se o t for maior que zero, verifica-se o ponto de interseção está
    //entre os limites que definem o segmento de plano
    y = r_vis.Y0() + t * r_vis.Dy();
    if (INSIDE(y,bmin,bmax))
    {
        z = r_vis.Z0() + t*r_vis.Dz();
        if (INSIDE(z,cmin,cmax))
            return(t);
    }
    return(-1);
}

float PlanoY::Intercepta( Raio r_vis )
{
    float t,x,z;

    t = (a - r_vis.Y0()) / r_vis.Dy();

    if (t<0)
        return(-1);

    x = r_vis.X0() + t * r_vis.Dx();
    if (INSIDE(x,bmin,bmax))
    {
        z = r_vis.Z0() + t*r_vis.Dz();
        if (INSIDE(z,cmin,cmax))
            return(t);
    }
    return(-1);
}

float PlanoZ::Intercepta( Raio r_vis )
{
    float t,x,y;

    t = (a - r_vis.Z0()) / r_vis.Dz();

    if (t<0)
        return(-1);

    x = r_vis.X0() + t * r_vis.Dx();
    if (INSIDE(x,bmin,bmax))
    {
        y = r_vis.Y0() + t*r_vis.Dy();
        if (INSIDE(y,cmin,cmax))
            return(t);
    }
    return(-1);
}

CaixaParalela::CaixaParalela( int _indice_textura, Vetor_3D _extremo_inferior, Vetor_3D _extremo_superior ) : Objeto_3D ( _indice_textura )
{
    float xmin, ymin, zmin, xmax, ymax, zmax;

    extremo_inferior.Copia(_extremo_inferior);
    extremo_superior.Copia(_extremo_superior);
    xmin = extremo_inferior.X();
    ymin = extremo_inferior.Y();
    zmin = extremo_inferior.Z();
    xmax = extremo_superior.X();
    ymax = extremo_superior.Y();
    zmax = extremo_superior.Z();

    px1 = new PlanoX( xmin,   ymin, ymax, zmin, zmax );
    px2 = new PlanoX( xmax,   ymin, ymax, zmin, zmax );

    py1 = new PlanoY( ymin,   xmin, xmax, zmin, zmax );
    py2 = new PlanoY( ymax,   xmin, xmax, zmin, zmax );

    pz1 = new PlanoZ( zmin,   xmin, xmax, ymin, ymax );
    pz2 = new PlanoZ( zmax,   xmin, xmax, ymin, ymax );

}

CaixaParalela::~CaixaParalela()
{
    delete( px1 );
    delete( px2 );
    delete( py1 );
    delete( py2 );
    delete( pz1 );
    delete( pz2 );
}

// Chama a função Intercepta de cada um dos seis planos.
// Pega o menor t (maior que zero) entre todos.
float CaixaParalela::Intercepta( Raio r_vis )
{
    float t,t_menor=-1;

    t = px1->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    t = px2->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    t = py1->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    t = py2->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    t = pz1->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    t = pz2->Intercepta( r_vis );
    if ( t>0 && ( t<t_menor || t_menor==-1 ) )
        t_menor = t;

    return(t_menor);
    //Vide observações na função Intercepta da Esfera!
}

Vetor_3D CaixaParalela::Normal( Vetor_3D ponto )
{
    Vetor_3D normal;

    //Os valores são comparados por aproximação por perda de precisão
    //nos cálculos usando float
    if ( PROXIMO( ponto.X(), extremo_superior.X() ) )
    { normal.Atribui (1,0,0); return normal; };
    if ( PROXIMO( ponto.X(), extremo_inferior.X() ) )
    { normal.Atribui (-1,0,0); return normal; };

    if ( PROXIMO( ponto.Y(), extremo_superior.Y() ) )
    { normal.Atribui (0,1,0); return normal; };
    if ( PROXIMO( ponto.Y(), extremo_inferior.Y() ) )
    { normal.Atribui (0,-1,0); return normal; };

    if ( PROXIMO( ponto.Z(), extremo_superior.Z() ) )
    { normal.Atribui (0,0,1); return normal; };
    if ( PROXIMO( ponto.Z(), extremo_inferior.Z() ) )
    { normal.Atribui (0,0,-1); return normal; };

    //apenas uma flag, caso algo de errado...
    printf("\nAlgo errado no cálculo da Normal da Caixa Paralela\n");
    return( normal );
}


Triangulo::Triangulo( int _indice_textura, Vetor_3D vertices[] ) : Objeto_3D ( _indice_textura )
{
    vert1.Copia(vertices[0]);
    vert2.Copia(vertices[1]);
    vert3.Copia(vertices[2]);

    //  float a1,b1,c1;
    aresta12.Copia(vert2);
    aresta12.Subtrai(vert1);
    aresta23.Copia(vert3);
    aresta23.Subtrai(vert2);
    aresta31.Copia(vert1);
    aresta31.Subtrai(vert3);
    Vetor_3D temp;
    temp.Copia(vert3);
    temp.Subtrai(vert1);
    normal = aresta12.ProdutoVetorial(temp);

}

float Triangulo::Intercepta( Raio r_vis )
{
    Vetor_3D temp;
    float t;

    //Cálculo da ponto em que o raio intercepta o plano
    //Foley, Appendix, A.3.5.
    {
        temp.Copia( vert1 );
        temp.Subtrai( r_vis.Origem() );
        t = temp.ProdutoEscalar( normal );
        temp.Copia( normal );
        t/= temp.ProdutoEscalar( r_vis.Direcao() );

        temp = r_vis.QualPonto(t);
        if (t<0)
            return -1;
    }

    //Verificação se o ponto pertence ao triângulo
    {
        float v1,v2,v3;
        Vetor_3D dif_temp;
        Vetor_3D d1, d2, d3;

        //Produto vetorial entre a aresta e a reta que sai do vértice até o ponto
        dif_temp.Copia( temp );
        dif_temp.Subtrai( vert1 );
        d1 = aresta12.ProdutoVetorial( dif_temp );

        dif_temp.Copia( temp );
        dif_temp.Subtrai( vert2 );
        d2 = aresta23.ProdutoVetorial( dif_temp );

        dif_temp.Copia( temp );
        dif_temp.Subtrai( vert3 );
        d3 = aresta31.ProdutoVetorial( dif_temp );

        //Produto Escalar apenas para verificar o sinal (+-)
        v1 = d1.ProdutoEscalar( d2 );
        v2 = d1.ProdutoEscalar( d3 );
        v3 = d2.ProdutoEscalar( d3 );

        //Se todos os vetores resultantes estiverem para o mesmo lado...
        if ( ( v1>0 && v2>0 && v3>0 ) || ( v1<0 && v2<0 && v3<0 ) )
            return (t);
        else
            return (-1); 

    }
}

Vetor_3D Triangulo::Normal( Vetor_3D ponto )
{
    return (normal);
    Vetor_3D temp;

    temp.Atribui( -normal.X(), -normal.Y(), -normal.Z() );
    return temp; 
}

Torus::Torus(int _indice_textura, float _raio, float _raioTubo, Vetor_3D _centro, Vetor_3D _normal) : Objeto_3D ( _indice_textura )
{
    raio = _raio;
    raioTubo = _raioTubo;
    centro.Copia( _centro );
    normal.Copia( _normal );
}

float Torus::Intercepta( Raio r_vis )
{

    // TODO

    return -1;

}

Vetor_3D Torus::Normal( Vetor_3D ponto )
{
    ponto.Subtrai(centro);
    ponto.Normaliza();
    return ponto;
}

Vetor_3D Cilinder::Normal( Vetor_3D ponto )
{
    ponto.Subtrai(centro);
    ponto.Normaliza();
    return ponto;
}

Cilinder::Cilinder(int _indice_textura, float _raio, Vetor_3D _centro, float _tamanho, Vetor_3D _direcao) : Objeto_3D ( _indice_textura )
{
    raio = _raio;
    centro.Copia( _centro );
    tamanho = _tamanho;
    direcao.Copia( _direcao );
}

bool Cilinder::isPointOnCylinder(double raiz, Raio r_vis) {

    Vetor_3D AP;
    float t;

    AP.Atribui( r_vis.X0() - r_vis.Dx(), r_vis.Y0() - r_vis.Dy(), r_vis.Z0() - r_vis.Dz());
    t = AP.ProdutoEscalar(direcao);

    if(t > tamanho || t < 0)
        return false;

    return true;
}

float Cilinder::Intercepta( Raio r_vis )
{

    Vetor_3D AO, AOxAB, VxAB;
    float a, b, c, delta, raiz1, raiz2;

    AO.Atribui( r_vis.X0() - centro.X(), r_vis.Y0() - centro.Y(), r_vis.Z0() - centro.Z());
    AOxAB = AO.ProdutoVetorial(direcao);
    VxAB = r_vis.Direcao().ProdutoVetorial(direcao);

    // montando a equação do 2º grau at2 + bt + c = 0
    a = VxAB.ProdutoEscalar(VxAB);
    b = 2 * VxAB.ProdutoEscalar(AOxAB);
    c = AOxAB.ProdutoEscalar(AOxAB) - raio*raio;

    printf("%f e %f e %f", a, b, c);

    // Calculando Delta
    delta = SQR(b) - 4*a*c;

    // Retornando t (se não existir t retornar valor negativo que será ignorado)
    if ( delta < 1 )
        return(-1);

    delta = (float)sqrt(delta);
    a*=2;
    raiz1 = ( -b - delta ) / a;
    raiz2 = ( -b + delta ) / a;

    if(raiz1 <= 0 && raiz2 <= 0)
    {
        return -1;
    }
    else if(raiz1 >= 0 && raiz2 >= 0)
    {
        puts("CCCCCCCCCCCC");
        if(isPointOnCylinder(raiz1, r_vis))
        {
            puts("BBBBBBBBBBBBB");
            if(isPointOnCylinder(raiz2, r_vis))
            {
                puts("AAAAAAAAAAAAAAAA");
                return (MINI(raiz1,raiz2));
            }
            else
            {
                return raiz1;
            }
        }
        else if(isPointOnCylinder(raiz2, r_vis))
        {
            return raiz2;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        puts("DDDDDDDDDDDDDDDDDDD");
        return (MAXI(raiz1, raiz2));
    }

}

