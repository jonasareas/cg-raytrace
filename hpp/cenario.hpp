//* Módulo : cenario.hpp                                                         
//
// _Descrição_   : Declara classe Cenario.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __cenario_HPP
#define __cenario_HPP

#include <vector>

#include "varios.hpp"
#include "objeto.hpp"
#include "luz.hpp"

//* classe Cenario #Cenario#
  //* Esta classe é a classe central do RayTrace. 
// Cenario contém as luzes, as materiais e os objetos.
// Ela também é responsável pela verificação de todas as interseções
//como também pelas cores a serem retornadas para cada raio traçado.
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
 //* Inserção de Objetos, materiais e Luzes
    bool     InsereObjeto( Objeto_3D *mais_um );
    bool     InsereMaterial( Material *mais_uma );
	bool     InsereBoundingBox( CaixaParalela *box);
    bool     InsereLuz( Luz *mais_uma );
 //* Recebe cor de Fundo (na leitura do arquivo de entrada)
    void    RecebeFundo( Cor_rgb _cor_de_fundo );
 //* Intercepta verifica se houve interseção com algum objeto.
    Cor_rgb Intercepta( std::vector<CaixaParalela *> bounding_boxes, Raio r_vis, int profundidade );
 //* Verifica se houve interseção com alguma bounding box
	std::vector<CaixaParalela *> 	interceptaBoundingBox(Raio r_vis);

  private:
 //* Cor do Ponto
 //Esta função _Private_ é chamada pela Intercepta
 //caso haja interseção com algum objeto.
    Cor_rgb CorDoPonto( std::vector<CaixaParalela *> bounding_boxes, int num_obj, Vetor_3D ponto, Raio r_vis, int profundidade );
};


#endif
