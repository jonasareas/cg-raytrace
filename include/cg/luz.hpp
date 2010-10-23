//* Módulo : luz.hpp                                                         
//
// _Descrição_   : Declara classe Luz.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __LUZ_HPP
#define __LUZ_HPP

#include <cg/varios.hpp>

//* classe Luz #Luz#
  //* Esta classe guarda informações quanto a posição e intensidade de uma luz.
  //* Atenção! Não foi necessário um arquivo .cpp para este módulo.
class Luz{
  private:
    Vetor_3D posicao;
    Cor_rgb  intensidade;
  public:
 //* Construtor e Destrutor
    Luz( Vetor_3D _posicao, Cor_rgb _intensidade ) : 
         posicao(_posicao), intensidade(_intensidade) {};
    ~Luz() {};
 //* Funções inline que retornam informações da Luz
    inline Vetor_3D Posicao() { return(posicao); };
    inline Cor_rgb  Intensidade() { return(intensidade); };
};

#endif
