#include <stdio.h>
#include "../hpp/material.hpp"

Material::Material(Cor_rgb _ambiente,Cor_rgb _cor_difusa,
					Cor_rgb _indice_especular,
					int _n_especular, float _coeficiente_reflexao,
					float _indice_refracao, float _indice_opacidade )
{
  ambiente.Copia( _ambiente );
  cor_difusa.Copia( _cor_difusa );
  indice_especular.Copia( _indice_especular );

  n_especular = _n_especular;

	coeficiente_reflexao = _coeficiente_reflexao;
  indice_refracao = _indice_refracao;
  indice_opacidade = _indice_opacidade;
}
