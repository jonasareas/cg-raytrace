//* Módulo : material.hpp
//
// _Desc_   : Declara classe Material.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __MATERIAL_HPP
#define __MATERIAL_HPP

#include "varios.hpp"

//* classe Material #Material#
// Material também poderia ser chamado de textura. 
// Nesta implementação do RayTrace não há refração, portanto as variáveis
// indice_refracao e indice_opacidade não foram disponibilizadas
// nas funções inline.
class Material{
  private:
    Cor_rgb ambiente, cor_difusa, indice_especular;
    int n_especular;
    float coeficiente_reflexao, indice_refracao, indice_opacidade;
  public:
 //* Construtor e Destrutor
    Material(Cor_rgb _ambiente, Cor_rgb _cor_difusa, Cor_rgb _indice_especular,
             int _n_especular, float _coeficiente_reflexao,
             float _indice_refracao, float _indice_opacidade );
    virtual ~Material() {};
 //* Funções inline que retornam informações do Material
    inline Cor_rgb Ambiente() {return ambiente;};
    inline Cor_rgb Cor_difusa() {return cor_difusa;};
    inline Cor_rgb Indice_especular() {return indice_especular;};
    inline int N_especular() {return n_especular;};
    inline float Coeficiente_reflexao() {return coeficiente_reflexao;};
};

#endif