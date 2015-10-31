#include <iostream>
#include <WinSock2.h>
#define PORTA_SERVIDOR 10
#define TAMANHO_MENSAGEM 16

int main()
{
	int r;

	WSAData wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_addr.s_addr = inet_addr("0.0.0.0");
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = PORTA_SERVIDOR;


	SOCKET socketPrincipal = socket(AF_INET, 
		SOCK_DGRAM, 
		IPPROTO_UDP);

	bind(socketPrincipal, 
		(SOCKADDR*) &enderecoServidor, 
		sizeof(sockaddr_in));

	char* buffer = new char[TAMANHO_MENSAGEM];
	sockaddr_in enderecoCliente;
	int tamanhoEnderecoCliente = sizeof(sockaddr_in);
	do
	{
		r = recvfrom(socketPrincipal,
				buffer,
				TAMANHO_MENSAGEM,
				NULL,
				(SOCKADDR*)&enderecoCliente,
				&tamanhoEnderecoCliente);


	} while (true);



	delete[] buffer;
	closesocket(socketPrincipal);
	
	WSACleanup();


	return 0;

}