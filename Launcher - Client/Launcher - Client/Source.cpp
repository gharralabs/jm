#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define PORTA_SERVIDOR 9000

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in enderecoServidor;
	
	inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

	SOCKET meuSocket;
	meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	connect(meuSocket, 
		   (SOCKADDR*)&enderecoServidor, 
		   sizeof(sockaddr_in));

	int minhaVersao = 1;
	
	send(meuSocket, 
		(char*)&minhaVersao, 
		sizeof(int), 
		NULL);


	int bytesRecebidos = 0; 
	int totalBytesRecebidos = 0;
	int totalBytesAReceber = 0;


	do
	{
		bytesRecebidos = recv(meuSocket,
							  (char*)&totalBytesAReceber + totalBytesRecebidos,
							  sizeof(int) - totalBytesRecebidos,
							  NULL);
		
		if (bytesRecebidos == SOCKET_ERROR ||
			bytesRecebidos == 0)
		{
			std::cout << "Ocorreu um erro ao receber o tamanho do arquivo" << std::endl;
			exit(1);
		}

		totalBytesRecebidos += bytesRecebidos;

	} while (totalBytesRecebidos != sizeof(int));


	closesocket(meuSocket);

	return 0;
}