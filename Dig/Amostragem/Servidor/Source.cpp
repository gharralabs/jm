#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ComandosRede.h>
#include <ConfiguraçõesServidor.h>


#define ESTADO_AGUARDANDO_JOGADORES 1
#define ESTADO_JOGANDO 2


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
	
	int estado = ESTADO_AGUARDANDO_JOGADORES;
	
	Comando comando;
	MoverMouseComando mouseComando;

	sockaddr_in enderecoCliente;
	int tamanhoEnderecoCliente = sizeof(sockaddr_in);

	std::vector<sockaddr_in> jogadores;

	do
	{
		switch (estado)
		{
		case ESTADO_AGUARDANDO_JOGADORES:

			r = recvfrom(socketPrincipal,
				(char*)&comando,
				sizeof(comando),
				NULL,
				(SOCKADDR*)&enderecoCliente,
				&tamanhoEnderecoCliente);

			switch (comando.comando)
			{
				case CMD_ENTRAR:
					jogadores.push_back(enderecoCliente);

					if (jogadores.size() == 2)
					{
						Comando inicioComando;
						inicioComando.comando = CMD_INICIAR;
						for (int i = 0; i < jogadores.size(); i++)
						{
							sendto(socketPrincipal,
								(char*)&inicioComando,
								sizeof(inicioComando),
								NULL,
								(SOCKADDR*)&jogadores[i],
								sizeof(jogadores[i]));
						
							estado = ESTADO_JOGANDO;
						}
					}
				break;
			}
			break;

		case ESTADO_JOGANDO:
			
			r = recvfrom(socketPrincipal,
				(char*)&mouseComando,
				sizeof(mouseComando),
				NULL,
				(SOCKADDR*)&enderecoCliente,
				&tamanhoEnderecoCliente);

			if (enderecoCliente.sin_addr.s_addr == jogadores[0].sin_addr.s_addr &&
				enderecoCliente.sin_port == jogadores[0].sin_port )
			{
				sendto(socketPrincipal,
					(char*)&mouseComando,
					sizeof(mouseComando),
					NULL,
					(SOCKADDR*)&jogadores[1],
					sizeof(jogadores[1]));
			}
			else
			{
				sendto(socketPrincipal,
					(char*)&mouseComando,
					sizeof(mouseComando),
					NULL,
					(SOCKADDR*)&jogadores[0],
					sizeof(jogadores[0]));
			}


			break;
		}
		
	} while (true);


	closesocket(socketPrincipal);
	
	WSACleanup();


	return 0;

}