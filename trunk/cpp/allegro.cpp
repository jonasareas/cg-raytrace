#include "../hpp/allegro.hpp"
#include <stdio.h>

void plota (int linhas, int colunas, int cores, Cor_rgb pix[]){
	if (!inicia()){
		finaliza();
		printf("Não foi possível iniciar o allegro!");
		return;
	}
	
	anim(linhas, colunas, cores, pix);

	finaliza();
};

int inicia (void)
{
	allegro_init();

	install_keyboard();

	set_color_depth(CORES);
	
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, RESOL_X, RESOL_Y, 0, 0) < 0){
		return FALSE;
	}
	if (CORES == 32){
		set_alpha_blender();
	}
	
	return TRUE;
};

void finaliza (void){
	allegro_exit();
};

void anim (int linhas, int colunas, int cores, Cor_rgb pix[]){ 
	int i,j, indice;
	for ( i=linhas-1; i>=0; i-- )
		for ( j=colunas-1; j>=0; j-- )
		{
			indice = i*colunas + j;
			putpixel(screen, j, linhas-i, makecol(pix[indice].R(), pix[indice].G(), pix[indice].B()));
		}
	while (!key[KEY_ESC]){}
};

