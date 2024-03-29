//* M�dulo : cenario.hpp                                                         
//
// _Descri��o_   : Declara classe Cenario.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __cenario_HPP
#define __cenario_HPP

#include <vector>

#include "varios.hpp"
#include "objeto.hpp"
#include "luz.hpp"

//* classe Cenario #Cenario#
  //* Esta classe � a classe central do RayTrace. 
// Cenario cont�m as luzes, as materiais e os objetos.
// Ela tamb�m � respons�vel pela verifica��o de todas as interse��es
//como tamb�m pelas cores a serem retornadas para cada raio tra�ado.
class Cenario{
  private:
    int total, total_materiais, total_luzes, total_boxes;
    Objeto_3D *objetos[MAXOBJ];
    Cor_rgb cor_de_fundo;
    Material *materiais[MAXOBJ];
    Luz *luzes[MAXLUZ];
	CaixaParalela *box[MAXOBJ];

  public:
 //* Construtor e Destrutor
    Cenario();
    ~Cenario();
    void    Zera();
 //* Inser��o de Objetos, materiais e Luzes
    bool     InsereObjeto( Objeto_3D *mais_um );
    bool     InsereMaterial( Material *mais_uma );
	bool     InsereBoundingBox( CaixaParalela *box);
    bool     InsereLuz( Luz *mais_uma );
 //* Recebe cor de Fundo (na leitura do arquivo de entrada)
    void    RecebeFundo( Cor_rgb _cor_de_fundo );
 //* Intercepta verifica se houve interse��o com algum objeto.
    Cor_rgb Intercepta( std::vector<CaixaParalela *> bounding_boxes, Raio r_vis, int profundidade );
 //* Verifica se houve interse��o com alguma bounding box
	std::vector<CaixaParalela *> 	interceptaBoundingBox(Raio r_vis);

  private:
 //* Cor do Ponto
 //Esta fun��o _Private_ � chamada pela Intercepta
 //caso haja interse��o com algum objeto.
    Cor_rgb CorDoPonto( std::vector<CaixaParalela *> bounding_boxes, int num_obj, Vetor_3D ponto, Raio r_vis, int profundidade );
};


#endif
