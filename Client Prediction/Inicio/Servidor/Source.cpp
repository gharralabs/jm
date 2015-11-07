#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ComandosRede.h>
#include <ConfiguraçõesServidor.h>
#include <ConfiguraçõesJogo.h>

struct Jogador
{
	int x, y;
	sockaddr_in endereco;
};

int main()
{
	int r;

	WSAData wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
	sockaddr_in meuEndereco;
	
	inet_pton(AF_INET, 
			  IP_SERVIDOR, 
			  &meuEndereco.sin_addr);

	meuEndereco.sin_family = AF_INET;
	meuEndereco.sin_port = htons(PORTA_SERVIDOR);


	SOCKET socketPrincipal = socket(AF_INET, 
		SOCK_DGRAM, 
		IPPROTO_UDP);

	bind(socketPrincipal, 
		(SOCKADDR*)&meuEndereco,
		sizeof(sockaddr_in));
	

	sockaddr_in enderecoCliente;
	int tamanho = sizeof(sockaddr_in);

	std::vector<Jogador> jogadores;

	char msg[MAX_PACKET_SIZE];

	do
	{
		recvfrom(socketPrincipal, 
				 msg, 
				 MAX_PACKET_SIZE, 
				 NULL,
				 (SOCKADDR*)&enderecoCliente,
				 &tamanho);


		int comando;
		comando = *((int*) &msg);

		switch (comando)
		{
			case CMD_ENTRAR:
				Jogador jogador;
				jogador.x = JOGADOR_POSICAO_X_INICIAL;
				jogador.y = JOGADOR_POSICAO_Y_INICIAL;
				jogador.endereco = enderecoCliente;
				jogadores.push_back(jogador);
				break;

			case CMD_MOVER_REQ:

				MoverComandoRequisicao req = *((MoverComandoRequisicao*) &msg);
				
				for (int i = 0; i < jogadores.size(); i++)
				{
					if (jogadores[i].endereco.sin_addr.s_addr == enderecoCliente.sin_addr.s_addr &&
						jogadores[i].endereco.sin_port == enderecoCliente.sin_port)
					{
						MoverComandoResposta res;
						res.comando = CMD_MOVER_RES;

						// validação antes de atualizar o gamestate oficial
						if ( (req.x == jogadores[i].x + JOGADOR_PASSO ||
							  req.x == jogadores[i].x - JOGADOR_PASSO ||
							  req.x == jogadores[i].x) &&
							 (req.y == jogadores[i].y + JOGADOR_PASSO ||
							  req.y == jogadores[i].y - JOGADOR_PASSO ||
							  req.y == jogadores[i].y))
						{

							res.permitido = true;
							jogadores[i].x = req.x;
							jogadores[i].y = req.y;
						}
						else
							res.permitido = false;
						
						std::this_thread::sleep_for(std::chrono::milliseconds(SIMULAÇÃO_LATÊNCIA));
						

						sendto(socketPrincipal,
							(char*)&res,
							sizeof(res),
							NULL,
							(SOCKADDR*)&jogadores[i].endereco,
							sizeof(sockaddr_in));

						break;
					}
				}

				break;
		}

	} while (true);


	closesocket(socketPrincipal);
	
	WSACleanup();


	return 0;

}