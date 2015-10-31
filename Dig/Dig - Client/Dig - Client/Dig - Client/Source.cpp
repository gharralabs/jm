#include <c2d2/chien2d2.h>
#include <c2d2/chienaudio2.h>
#include "Grid.h"
#include <vector>

#define AGUARDANDO_JOGADORES 0
#define LARGURA_TELA 800
#define ALTURA_TELA 600


int main(int ac, char **av)
{
	
	C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Minha Janela Chien 2D");
	
	C2D2_Botao *teclado;
	teclado = C2D2_PegaTeclas();
	C2D2_Mouse *mouse;
	mouse = C2D2_PegaMouse();

	C2D2_TrocaCorLimpezaTela(0, 120, 0);

	unsigned int megamanSpriteSet;
	megamanSpriteSet  = C2D2_CarregaSpriteSet("Megaman_sprite.jpg", 200, 250);
	unsigned long proximaAtualizacaoFrame = C2D2_TempoSistema() + 500;
	int frameAtual = 4;

	// AUDIO
	CA2_Inicia();
	int bombaSom = CA2_CarregaEfeito("bomba.wav");

	int estado = AGUARDANDO_JOGADORES;

	
	Grid* grid;
	grid = new Grid(LARGURA_TELA, ALTURA_TELA, 3, 3, 5);

	while (!teclado[C2D2_ESC].pressionado)
	{
	
		//if (proximaAtualizacaoFrame < C2D2_TempoSistema())
		//{
		//	frameAtual = (frameAtual == 4) ? 5 : 4;
		//	proximaAtualizacaoFrame = C2D2_TempoSistema() + 500;
		//	// AUDIO
		//	CA2_TocaEfeito(bombaSom, 0);
		//}
		
		


		C2D2_LimpaTela();
		/*C2D2_DesenhaSprite(megamanSpriteSet,
						   frameAtual, mouse->x, mouse->y);*/
		
		C2D2_Sincroniza(C2D2_FPS_PADRAO);		
	}

	delete grid;

	C2D2_RemoveSpriteSet(megamanSpriteSet);

	// AUDIO
	CA2_Encerra();
	C2D2_Encerra();


	return 0;
}