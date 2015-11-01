#define WIN32_LEAN_AND_MEAN
#include <c2d2/chien2d2.h>
#include <c2d2/chienaudio2.h>
#include <vector>
#include <thread>
#include <numeric>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>



#define LARGURA_TELA 800
#define ALTURA_TELA 600



int main(int ac, char **av)
{
	
	C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Amostragem");
	
	C2D2_Botao *teclado;
	teclado = C2D2_PegaTeclas();
	C2D2_Mouse *mouse;
	mouse = C2D2_PegaMouse();

	C2D2_TrocaCorLimpezaTela(0, 120, 0);
	
	unsigned int cursoresSpriteSet;
	cursoresSpriteSet = C2D2_CarregaSpriteSet("cursores.png", 26, 26);

	unsigned long proximoEnvio = C2D2_TempoSistema() + 100;


	
	while (!teclado[C2D2_ESC].pressionado)
	{
		
		if (proximoEnvio < C2D2_TempoSistema())
		{		
			proximoEnvio = C2D2_TempoSistema() + 100;
		}
		
		C2D2_LimpaTela();
		C2D2_DesenhaSprite(cursoresSpriteSet,
						   0, mouse->x, mouse->y);

		
		C2D2_Sincroniza(C2D2_FPS_PADRAO);		
	}

	C2D2_RemoveSpriteSet(cursoresSpriteSet);
	C2D2_Encerra();

	WSACleanup();

	return 0;
}