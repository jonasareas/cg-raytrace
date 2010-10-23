//* M�dulo : luz.hpp                                                         
//
// _Descri��o_   : Declara classe Luz.
// _Autor_: Rodrigo P. R. de Toledo

#ifndef __LUZ_HPP
#define __LUZ_HPP

#include <cg/varios.hpp>

//* classe Luz #Luz#
  //* Esta classe guarda informa��es quanto a posi��o e intensidade de uma luz.
  //* Aten��o! N�o foi necess�rio um arquivo .cpp para este m�dulo.
class Luz{
  private:
    Vetor_3D posicao;
    Cor_rgb  intensidade;
  public:
 //* Construtor e Destrutor
    Luz( Vetor_3D _posicao, Cor_rgb _intensidade ) : 
         posicao(_posicao), intensidade(_intensidade) {};
    ~Luz() {};
 //* Fun��es inline que retornam informa��es da Luz
    inline Vetor_3D Posicao() { return(posicao); };
    inline Cor_rgb  Intensidade() { return(intensidade); };
};

#endif
