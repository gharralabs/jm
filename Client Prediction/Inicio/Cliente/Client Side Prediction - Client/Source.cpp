#define WIN32_LEAN_AND_MEAN
#include <c2d2/chien2d2.h>
#include <c2d2/chienaudio2.h>
#include <vector>
#include <thread>
#include <numeric>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include <ComandosRede.h>
#include <ConfiguraçõesServidor.h>
#include <ConfiguraçõesJogo.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600

#define MEU_IP "0.0.0.0"

#define ESTADO_CONECTANDO 1
#define ESTADO_JOGANDO 2


struct Personagem
{
	int x, 
		y, 
		quadro = 2;
	unsigned int spritesheet;
};


bool informarMovimento(SOCKET socket,
					   Personagem& personagem, 
					   sockaddr_in& servidor)
{
	MoverComandoRequisicao req;
	req.comando = CMD_MOVER_REQ;
	req.x = personagem.x;
	req.y = personagem.y;

	sendto(socket,
		(char*)&req,
		sizeof(req),
		   NULL,
		   (SOCKADDR*) &servidor,
		   sizeof(servidor));

	MoverComandoResposta res;
	
	sockaddr_in e;
	int t = sizeof(e);
	
	recvfrom(socket,
			(char*)&res,
			sizeof(res),
			NULL,
			(SOCKADDR*)&e,
			&t);

	return res.permitido;
}


int main(int ac, char **av)
{
	Personagem personagem;
	personagem.x = JOGADOR_POSICAO_X_INICIAL;
	personagem.y = JOGADOR_POSICAO_Y_INICIAL;
	
	C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Client Side Prediction");
	
	C2D2_Botao *teclado;
	teclado = C2D2_PegaTeclas();
	C2D2_Mouse *mouse;
	mouse = C2D2_PegaMouse();

	C2D2_TrocaCorLimpezaTela(0, 120, 0);
	
	
	personagem.spritesheet = C2D2_CarregaSpriteSet("mario.png", 55, 72);

	
	int estado = ESTADO_CONECTANDO;

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET meuSocket = socket(AF_INET, 
							  SOCK_DGRAM, 
							  IPPROTO_UDP);

	sockaddr_in meuEndereco;
	inet_pton(AF_INET, MEU_IP, &meuEndereco.sin_addr.s_addr);
	meuEndereco.sin_family = AF_INET;

	int porta = 2000 + rand() % 2000;
	meuEndereco.sin_port = htons(porta);

	bind(meuSocket, 
		(SOCKADDR*)&meuEndereco, 
		sizeof(meuEndereco));

	sockaddr_in enderecoServidor;
	inet_pton(AF_INET, IP_SERVIDOR, &enderecoServidor.sin_addr.s_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

	Comando comando;
	comando.comando = CMD_ENTRAR;

	sendto( meuSocket, 
		   (char*)&comando, 
		    sizeof(comando), 
		    NULL,
		   (SOCKADDR*)&enderecoServidor, 
		    sizeof(enderecoServidor));
	
	memset(&comando, 0, sizeof(comando));
	
	int tamanhoEndereco = sizeof(enderecoServidor);

	
	while (!teclado[C2D2_ESC].pressionado)
	{
		
		if (teclado[C2D2_DIREITA].ativo || teclado[C2D2_ESQUERDA].ativo || teclado[C2D2_H].ativo)
		{
			Personagem novo = personagem;

			if (teclado[C2D2_DIREITA].ativo)
				novo.x += JOGADOR_PASSO;
			
			if (teclado[C2D2_H].ativo)
				novo.x += 20; // hack

			if (teclado[C2D2_ESQUERDA].ativo)
				novo.x -= JOGADOR_PASSO;

			bool confirmacao = informarMovimento(meuSocket,
				novo,
				enderecoServidor);

			if (confirmacao)
				personagem = novo;
		}		
		
		C2D2_LimpaTela();
		C2D2_DesenhaSprite(personagem.spritesheet,
						   personagem.quadro, 
						   personagem.x, 
						   personagem.y);


		
		C2D2_Sincroniza(C2D2_FPS_PADRAO);		
	}

	C2D2_RemoveSpriteSet(personagem.spritesheet);
	C2D2_Encerra();

	WSACleanup();

	return 0;
}